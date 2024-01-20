/*
 * DMA_support.c
 *
 *  Created on: 2022年3月27日
 *      Author: zhe
 */

#include "DMA_support.h"

/*****************************************************************************/
/*
*
* This function setups the interrupt system so interrupts can occur for the
* DMA, it assumes XScuGic component exists in the hardware system.
*
* @param	IntcInstancePtr is a pointer to the instance of the XScuGic.
* @param	AxiDmaPtr is a pointer to the instance of the DMA engine
* @param	TxIntrId is the TX channel Interrupt ID.
* @param	RxIntrId is the RX channel Interrupt ID.
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not successful
*
* @note		None.
*
******************************************************************************/
int DMA_SetupIntrSystem()
{
	XAxiDma_BdRing *TxRingPtr = XAxiDma_GetTxRing(&AxiDma);
	XAxiDma_BdRing *RxRingPtr = XAxiDma_GetRxRing(&AxiDma);
	int Status;


	XScuGic_SetPriorityTriggerType(&Intc, TX_INTR_ID, 0xA0, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, RX_INTR_ID, 0xA0, 0x3);
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XScuGic_Connect(&Intc, TX_INTR_ID,
				(Xil_InterruptHandler)DMA_TxIntrHandler,
				TxRingPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	Status = XScuGic_Connect(&Intc, RX_INTR_ID,
				(Xil_InterruptHandler)DMA_RxIntrHandler,
				RxRingPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	XScuGic_Enable(&Intc, TX_INTR_ID);
	XScuGic_Enable(&Intc, RX_INTR_ID);

	return XST_SUCCESS;
}


/*****************************************************************************/
/*
*
* This is the DMA TX Interrupt handler function.
*
* It gets the interrupt status from the hardware, acknowledges it, and if any
* error happens, it resets the hardware. Otherwise, if a completion interrupt
* presents, then it calls the callback function.
*
* @param	Callback is a pointer to TX channel of the DMA engine.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void DMA_TxIntrHandler(void *Callback)
{
	XAxiDma_BdRing *TxRingPtr = (XAxiDma_BdRing *) Callback;
	u32 IrqStatus;
	int TimeOut;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_BdRingGetIrq(TxRingPtr);

	/* Acknowledge pending interrupts */
	XAxiDma_BdRingAckIrq(TxRingPtr, IrqStatus);

	/* If no interrupt is asserted, we do not do anything */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		XAxiDma_BdRingDumpRegs(TxRingPtr);

		Error = 1;
		xil_printf("A DMA TX error occurred!!!");

		/*
		 * Reset should never fail for transmit channel
		 */
		XAxiDma_Reset(&AxiDma);

		TimeOut = RESET_TIMEOUT_COUNTER;

		while (TimeOut) {
			if (XAxiDma_ResetIsDone(&AxiDma)) {
				break;
			}

			TimeOut -= 1;
		}

		return;
	}

	/*
	 * If Transmit done interrupt is asserted, call TX call back function
	 * to handle the processed BDs and raise the according flag
	 */
	if ((IrqStatus & (XAXIDMA_IRQ_DELAY_MASK | XAXIDMA_IRQ_IOC_MASK))) {
		int BdCount;
		XAxiDma_Bd *BdPtr;

		/* Get all processed BDs from hardware */
		BdCount = XAxiDma_BdRingFromHw(TxRingPtr, XAXIDMA_ALL_BDS, &BdPtr);
		/* Don't need to do anything in cyclic mode */
	}
}


/*****************************************************************************/
/*
*
* This is the DMA RX interrupt handler function
*
* It gets the interrupt status from the hardware, acknowledges it, and if any
* error happens, it resets the hardware. Otherwise, if a completion interrupt
* presents, then it calls the callback function.
*
* @param	Callback is a pointer to RX channel of the DMA engine.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void DMA_RxIntrHandler(void *Callback)
{
	XAxiDma_BdRing *RxRingPtr = (XAxiDma_BdRing *) Callback;
	u32 IrqStatus;
	int TimeOut;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_BdRingGetIrq(RxRingPtr);

	/* Acknowledge pending interrupts */
	XAxiDma_BdRingAckIrq(RxRingPtr, IrqStatus);

	/* If no interrupt is asserted, we do not do anything */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		XAxiDma_BdRingDumpRegs(RxRingPtr);

		Error = 1;
		xil_printf("A DMA RX error occurred!!!");


		XAxiDma_Reset(&AxiDma);

		TimeOut = RESET_TIMEOUT_COUNTER;

		while (TimeOut) {
			if(XAxiDma_ResetIsDone(&AxiDma)) {
				break;
			}

			TimeOut -= 1;
		}

		if(!XAxiDma_ResetIsDone(&AxiDma)){
			xil_printf("DMA reset is failed")
		}

		return;
	}


	//Because it's SG mode, so a XAXIDMA_IRQ_DELAY_MASK is needed
	if ((IrqStatus & (XAXIDMA_IRQ_DELAY_MASK | XAXIDMA_IRQ_IOC_MASK))) {
		int BdCount;
		XAxiDma_Bd *BdPtr;
		XAxiDma_Bd *BdCurPtr;
		u32 BdSts;
		int Index;

		/* Get finished BDs from hardware but do nothing*/
		BdCount = XAxiDma_BdRingFromHw(RxRingPtr, XAXIDMA_ALL_BDS, &BdPtr);
		BdCurPtr = BdPtr;
		for (Index = 0; Index < BdCount; Index++) {

			/*
			 * Check the flags set by the hardware for status
			 * If error happens, processing stops, because the DMA engine
			 * is halted after this BD.
			 */
			BdSts = XAxiDma_BdGetSts(BdCurPtr);
			if ((BdSts & XAXIDMA_BD_STS_ALL_ERR_MASK) ||
				(!(BdSts & XAXIDMA_BD_STS_COMPLETE_MASK))) {
				Error = 1;
				break;
			}

			/* Find the next processed BD */
			BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
		}

		/* Free all processed BDs for future transmission */
		Status = XAxiDma_BdRingFree(RxRingPtr, BdCount, BdPtr);
		if (Status != XST_SUCCESS) {
			Error = 1;
			xil_printf("BD free failed!\r\n");
		}

		if(packet_trans_done)
			xil_printf("last lwip transmission has not finished!\r\n");
		else
		{
			/*set the axidma done flag*/
			packet_trans_done = 1;

		}
	}



}


int DMA_Init(XAxiDma *DMAPtr,u32 DeviceId)
{
	int Status;
	XAxiDma_Config *Config=NULL;

	Config = XAxiDma_LookupConfig(DeviceId);
	if (!Config) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	/* Initialize DMA engine */
	Status = XAxiDma_CfgInitialize(DMAPtr, Config);

	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(!XAxiDma_HasSg(DMAPtr)){
		xil_printf("Device doesn't configured as SG mode! \r\n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;

	/* Initialize flags before start transfer test  */
	Error = 0;
	packet_trans_done = 0;


	/* Set up TX/RX channels to be ready to transmit and receive packets */
	Status = DMA_TxSetup(&AxiDma);

	if (Status != XST_SUCCESS) {

		xil_printf("Failed TX setup\r\n");
		return XST_FAILURE;
	}

	Status = DMA_RxSetup(&AxiDma);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed RX setup\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}


/*****************************************************************************/
/*
*
* This function sets up the TX channel of a DMA engine to be ready for packet
* transmission.
*
* @param	AxiDmaInstPtr is the pointer to the instance of the DMA engine.
*
* @return	- XST_SUCCESS if the setup is successful.
*		- XST_FAILURE otherwise.
*
* @note		None.
*
******************************************************************************/
int DMA_TxSetup(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *TxRingPtr = XAxiDma_GetTxRing(AxiDmaInstPtr);
	XAxiDma_Bd BdTemplate;
	int Status;

	/* Disable all TX interrupts before TxBD space setup */
	XAxiDma_BdRingIntDisable(TxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	Status = XAxiDma_BdRingCreate(TxRingPtr, TX_BD_SPACE_BASE,
				     TX_BD_SPACE_BASE,
				     XAXIDMA_BD_MINIMUM_ALIGNMENT, NUMBER_OF_BDS_TO_TRANSFER);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed create BD ring\r\n");
		return XST_FAILURE;
	}


	XAxiDma_BdClear(&BdTemplate);
	Status = XAxiDma_BdRingClone(TxRingPtr, &BdTemplate);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed clone BDs\r\n");
		return XST_FAILURE;
	}

	Status = XAxiDma_BdRingSetCoalesce(TxRingPtr, COALESCING_COUNT,
			DELAY_TIMER_COUNT);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed set coalescing"
		" %d/%d\r\n",COALESCING_COUNT, DELAY_TIMER_COUNT);
		return XST_FAILURE;
	}

	/* Enable Cyclic DMA mode */
	XAxiDma_BdRingEnableCyclicDMA(TxRingPtr);
	XAxiDma_SelectCyclicMode(AxiDmaInstPtr, XAXIDMA_DMA_TO_DEVICE, 1);

	/* Enable all TX interrupts */
	XAxiDma_BdRingIntEnable(TxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	/* Start the TX channel */
	Status = XAxiDma_BdRingStart(TxRingPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed bd start\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/*****************************************************************************/
/*
*
* This function sets up RX channel of the DMA engine to be ready for packet
* reception
*
* @param	AxiDmaInstPtr is the pointer to the instance of the DMA engine.
*
* @return	- XST_SUCCESS if the setup is successful.
*		- XST_FAILURE if fails.
*
* @note		None.
*
******************************************************************************/
int DMA_RxSetup(XAxiDma * AxiDmaInstPtr)
{
	XAxiDma_BdRing *RxRingPtr;
	int Status;
	XAxiDma_Bd BdTemplate;
	XAxiDma_Bd *BdPtr;
	XAxiDma_Bd *BdCurPtr;
	int BdCount;
	int FreeBdCount;

	int Index;

	RxRingPtr = XAxiDma_GetRxRing(AxiDmaInstPtr);

	/* Disable all RX interrupts before RxBD space setup */
	XAxiDma_BdRingIntDisable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	/* Setup Rx BD space */
	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,
				RX_BD_SPACE_HIGH - RX_BD_SPACE_BASE + 1);

	Status = XAxiDma_BdRingCreate(RxRingPtr, RX_BD_SPACE_BASE,
					RX_BD_SPACE_BASE,
					XAXIDMA_BD_MINIMUM_ALIGNMENT, BdCount);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx bd create failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	/*
	 * Setup a BD template for the Rx channel. Then copy it to every RX BD.
	 */
	XAxiDma_BdClear(&BdTemplate);
	Status = XAxiDma_BdRingClone(RxRingPtr, &BdTemplate);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx bd clone failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	/* Attach buffers to RxBD ring so we are ready to receive packets */
	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);

	Status = XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx bd alloc failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	BdCurPtr = BdPtr;
	//TODO:考虑是否要将RxBufferPtr变成u16 *类型
	RxBufferPtr = RX_BUFFER_BASE;
//	RxBufferPtr[1] = RX_BUFFER1_BASE;

	for (Index = 0; Index < FreeBdCount; Index++) {

		Status = XAxiDma_BdSetBufAddr(BdCurPtr, RxBufferPtr);
		if (Status != XST_SUCCESS) {
			xil_printf("Rx set buffer addr %x on BD %x failed %d\r\n",
			(unsigned int)RxBufferPtr,
			(UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		Status = XAxiDma_BdSetLength(BdCurPtr, MAX_BD_LEN,
					RxRingPtr->MaxTransferLen);
		if (Status != XST_SUCCESS) {
			xil_printf("Rx set length %d on BD %x failed %d\r\n",
			    MAX_BD_LEN, (UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		/* Receive BDs do not need to set anything for the control
		 * The hardware will set the SOF/EOF bits per stream status
		 */
		XAxiDma_BdSetCtrl(BdCurPtr, 0);

		XAxiDma_BdSetId(BdCurPtr, RxBufferPtr);

		RxBufferPtr += MAX_BD_LEN;
		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
	}

	/*
	 * Set the coalescing threshold
	 *
	 * If you would like to have multiple interrupts to happen, change
	 * the COALESCING_COUNT to be a smaller value
	 */
	Status = XAxiDma_BdRingSetCoalesce(RxRingPtr, COALESCING_COUNT,
			DELAY_TIMER_COUNT);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx set coalesce failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = XAxiDma_BdRingToHw(RxRingPtr, FreeBdCount, BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx ToHw failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	/* Enable all RX interrupts */
	XAxiDma_BdRingIntEnable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);
	/* Enable Cyclic DMA mode */
//	XAxiDma_BdRingEnableCyclicDMA(RxRingPtr);
//	XAxiDma_SelectCyclicMode(AxiDmaInstPtr, XAXIDMA_DEVICE_TO_DMA, 1);

	/* Start transfer data now! */
	Status = XAxiDma_BdRingStart(RxRingPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx start BD ring failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}



int DMA_SendPacket()
{
	XAxiDma_BdRing *TxRingPtr = XAxiDma_GetTxRing(&AxiDma);
	u8 *Packet;
	u8 Value;
	XAxiDma_Bd *BdPtr, *BdCurPtr;
	int Status;
	int Index, Pkts;
	UINTPTR BufferAddr;

	/*
	 * Each packet is limited to TxRingPtr->MaxTransferLen
	 * This will not be the case if hardware has store and forward built in
	 */
	if (MAX_BD_LEN * NUMBER_OF_BDS_PER_PKT >
			TxRingPtr->MaxTransferLen) {

		xil_printf("Invalid total per packet transfer length for the "
		    "packet %d/%d\r\n",
		    MAX_BD_LEN * NUMBER_OF_BDS_PER_PKT,
		    TxRingPtr->MaxTransferLen);

		return XST_INVALID_PARAM;
	}

	Packet = (u8 *) TxPacket;

	Value = 0xC;

	for(Index = 0; Index < MAX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER;
								Index ++) {
		Packet[Index] = Value;

		Value = (Value + 1) & 0xFF;
	}

	/* Flush the buffers before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)Packet, MAX_BD_LEN *
							NUMBER_OF_BDS_TO_TRANSFER);
	Xil_DCacheFlushRange((UINTPTR)RX_BUFFER_BASE, MAX_BD_LEN *
							NUMBER_OF_BDS_TO_TRANSFER);

	Status = XAxiDma_BdRingAlloc(TxRingPtr, NUMBER_OF_BDS_TO_TRANSFER,
								&BdPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed bd alloc\r\n");
		return XST_FAILURE;
	}

	BufferAddr = (UINTPTR)Packet;
	BdCurPtr = BdPtr;

	/*
	 * Set up the BD using the information of the packet to transmit
	 * Each transfer has NUMBER_OF_BDS_PER_PKT BDs
	 */
	for(Index = 0; Index < NUMBER_OF_PKTS_TO_TRANSFER; Index++) {

		for(Pkts = 0; Pkts < NUMBER_OF_BDS_PER_PKT; Pkts++) {
			u32 CrBits = 0;

			Status = XAxiDma_BdSetBufAddr(BdCurPtr, BufferAddr);
			if (Status != XST_SUCCESS) {
				xil_printf("Tx set buffer addr %x on BD %x failed %d\r\n",
				(unsigned int)BufferAddr,
				(UINTPTR)BdCurPtr, Status);

				return XST_FAILURE;
			}

			Status = XAxiDma_BdSetLength(BdCurPtr, MAX_BD_LEN,
						TxRingPtr->MaxTransferLen);
			if (Status != XST_SUCCESS) {
				xil_printf("Tx set length %d on BD %x failed %d\r\n",
				MAX_BD_LEN, (UINTPTR)BdCurPtr, Status);

				return XST_FAILURE;
			}

			if (Pkts == 0) {
				/* The first BD has SOF set
				 */
				CrBits |= XAXIDMA_BD_CTRL_TXSOF_MASK;

			}

			if(Pkts == (NUMBER_OF_BDS_PER_PKT - 1)) {
				/* The last BD should have EOF and IOC set
				 */
				CrBits |= XAXIDMA_BD_CTRL_TXEOF_MASK;
			}

			XAxiDma_BdSetCtrl(BdCurPtr, CrBits);
			XAxiDma_BdSetId(BdCurPtr, BufferAddr);

			BufferAddr += MAX_BD_LEN;
			BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(TxRingPtr, BdCurPtr);
		}
	}

	/* Give the BD to hardware and the transaction start! */
	Status = XAxiDma_BdRingToHw(TxRingPtr, NUMBER_OF_BDS_TO_TRANSFER,
						BdPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed to hw, length %d\r\n",
			(int)XAxiDma_BdGetLength(BdPtr,
					TxRingPtr->MaxTransferLen));

		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int DMA_ReceivePacket()
{

	int Status;
	int FreeBdCount;

	XAxiDma_BdRing *RxRingPtr = XAxiDma_GetRxRing(&AxiDma);

	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);

	Status = XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx bd alloc failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	/* launch next transfer */
	Status = XAxiDma_BdRingToHw(RxRingPtr, NUMBER_OF_BDS_TO_TRANSFER,
						BdPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed to hw, length %d\r\n",
			(int)XAxiDma_BdGetLength(BdPtr,
					TxRingPtr->MaxTransferLen));

		return XST_FAILURE;
	}


	return Status;
}

