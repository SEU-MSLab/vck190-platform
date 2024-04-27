/*
 * DMA_support.c
 *
 *  Created on: 2022年3月27日
 *      Author: zhe
 */

#include "DMA_support.h"


u32 *TxPacket = (u32 *) TX_BUFFER_BASE;
u32 *RxPacket = (u32 *) RX_BUFFER_BASE;
u32 *CMDPacket = (u32 *) CMD_BUFFER_BASE;


volatile int Error;

XAxiDma AxiDma;
u16 *RxBufferPtr;

u16 SinWave[256] = {0x0000,0x0031,0x0062,0x0093,0x00C4,0x00F5,0x0125,0x0156,
		0x0186,0x01B6,0x01E6,0x0215,0x0245,0x0273,0x02A2,0x02D0,0x02FD,
		0x032A,0x0357,0x0383,0x03AF,0x03DA,0x0404,0x042E,0x0457,0x0480,
		0x04A7,0x04CE,0x04F5,0x051A,0x053F,0x0563,0x0586,0x05A8,0x05CA,
		0x05EA,0x060A,0x0629,0x0646,0x0663,0x067F,0x069A,0x06B3,0x06CC,
		0x06E4,0x06FA,0x0710,0x0724,0x0738,0x074A,0x075B,0x076B,0x077A,
		0x0788,0x0794,0x079F,0x07AA,0x07B3,0x07BA,0x07C1,0x07C6,0x07CB,
		0x07CE,0x07CF,0x07D0,0x07CF,0x07CE,0x07CB,0x07C6,0x07C1,0x07BA,
		0x07B3,0x07AA,0x079F,0x0794,0x0788,0x077A,0x076B,0x075B,0x074A,
		0x0738,0x0724,0x0710,0x06FA,0x06E4,0x06CC,0x06B3,0x069A,0x067F,
		0x0663,0x0646,0x0629,0x060A,0x05EA,0x05CA,0x05A8,0x0586,0x0563,
		0x053F,0x051A,0x04F5,0x04CE,0x04A7,0x0480,0x0457,0x042E,0x0404,
		0x03DA,0x03AF,0x0383,0x0357,0x032A,0x02FD,0x02D0,0x02A2,0x0273,
		0x0245,0x0215,0x01E6,0x01B6,0x0186,0x0156,0x0125,0x00F5,0x00C4,
		0x0093,0x0062,0x0031,0x0000,0xFFCF,0xFF9E,0xFF6D,0xFF3C,0xFF0B,
		0xFEDB,0xFEAA,0xFE7A,0xFE4A,0xFE1A,0xFDEB,0xFDBB,0xFD8D,0xFD5E,
		0xFD30,0xFD03,0xFCD6,0xFCA9,0xFC7D,0xFC51,0xFC26,0xFBFC,0xFBD2,
		0xFBA9,0xFB80,0xFB59,0xFB32,0xFB0B,0xFAE6,0xFAC1,0xFA9D,0xFA7A,
		0xFA58,0xFA36,0xFA16,0xF9F6,0xF9D7,0xF9BA,0xF99D,0xF981,0xF966,
		0xF94D,0xF934,0xF91C,0xF906,0xF8F0,0xF8DC,0xF8C8,0xF8B6,0xF8A5,
		0xF895,0xF886,0xF878,0xF86C,0xF861,0xF856,0xF84D,0xF846,0xF83F,
		0xF83A,0xF835,0xF832,0xF831,0xF830,0xF831,0xF832,0xF835,0xF83A,
		0xF83F,0xF846,0xF84D,0xF856,0xF861,0xF86C,0xF878,0xF886,0xF895,
		0xF8A5,0xF8B6,0xF8C8,0xF8DC,0xF8F0,0xF906,0xF91C,0xF934,0xF94D,
		0xF966,0xF981,0xF99D,0xF9BA,0xF9D7,0xF9F6,0xFA16,0xFA36,0xFA58,
		0xFA7A,0xFA9D,0xFAC1,0xFAE6,0xFB0B,0xFB32,0xFB59,0xFB80,0xFBA9,
		0xFBD2,0xFBFC,0xFC26,0xFC51,0xFC7D,0xFCA9,0xFCD6,0xFD03,0xFD30,
		0xFD5E,0xFD8D,0xFDBB,0xFDEB,0xFE1A,0xFE4A,0xFE7A,0xFEAA,0xFEDB,
		0xFF0B,0xFF3C,0xFF6D,0xFF9E,0xFFCF,};


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


// Callback is a pointer to TX channel of the DMA engine
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
//		BdCount = XAxiDma_BdRingFromHw(TxRingPtr, XAXIDMA_ALL_BDS, &BdPtr);
		/* Don't need to do anything in cyclic mode */
	}
}


// Callback is a pointer to RX channel of the DMA engine.
void DMA_RxIntrHandler(void *Callback)
{
	xil_printf("RX Intr Handler start! \r\n");
	XAxiDma_BdRing *RxRingPtr = (XAxiDma_BdRing *) Callback;
	u32 IrqStatus;
	int TimeOut;
	int Status;
	int FreeBdCount;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_BdRingGetIrq(RxRingPtr);

	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);
	xil_printf("%d BDs left free in RX hander \r\n", FreeBdCount);

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
			xil_printf("DMA reset is failed");
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
		if(BdCount == 0)
			// everytime it will return this line, because the cache invalidate
			xil_printf("RX intr No RX BD fetch from HW! \r\n");
		else
			xil_printf("%d BD fetch from HW. \r\n", BdCount);
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
				xil_printf("RX BD back with error!");
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
	}



}


int DMA_Init()
{
	int Status;
	XAxiDma_Config *Config=NULL;
	XAxiDma *DMAPtr = &AxiDma;
	u32 DeviceId = DMA_DEV_ID;

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

	/* Initialize flags before start transfer test  */
	Error = 0;


	/* Set up TX/RX channels to be ready to transmit and receive packets */
	Status = DMA_TxSetup(&AxiDma);

	if (Status != XST_SUCCESS) {

		xil_printf("Failed TX setup\r\n");
		return XST_FAILURE;
	}



	Status = DMA_SetupIntrSystem();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed DMA Interrupt init\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}


// AxiDmaInstPtr is the pointer to the instance of the DMA engine.
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


// AxiDmaInstPtr is the pointer to the instance of the DMA engine.
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
	// use as much BD as possible, 65536 / 64 = 1024 BDs
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
	xil_printf("%d BDs add to Pre-process stage \r\n", FreeBdCount);

	// To Pre-process
	Status = XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx bd alloc failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	BdCurPtr = BdPtr;

	RxBufferPtr = (u16 *)RX_BUFFER_BASE;

	for (Index = 0; Index < FreeBdCount; Index++) {

		Status = XAxiDma_BdSetBufAddr(BdCurPtr, (UINTPTR)RxBufferPtr);
		if (Status != XST_SUCCESS) {
			xil_printf("Rx set buffer addr %x on BD %x failed %d\r\n",
			(unsigned int)RxBufferPtr,
			(UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		Status = XAxiDma_BdSetLength(BdCurPtr, MAX_RX_BD_LEN,
					RxRingPtr->MaxTransferLen);
		if (Status != XST_SUCCESS) {
			xil_printf("Rx set length %d on BD %x failed %d\r\n",
			    MAX_RX_BD_LEN, (UINTPTR)BdCurPtr, Status);

			return XST_FAILURE;
		}

		/* Receive BDs do not need to set anything for the control
		 * The hardware will set the SOF/EOF bits per stream status
		 */
		XAxiDma_BdSetCtrl(BdCurPtr, 0);

		XAxiDma_BdSetId(BdCurPtr, RxBufferPtr);

		RxBufferPtr += MAX_RX_BD_LEN / 2;

		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
	}

	// For tcp
	RxBufferPtr = (u16 *)RX_BUFFER_BASE;

	/*
	 * Set the coalescing threshold
	 *
	 * If you would like to have multiple interrupts to happen, change
	 * the COALESCING_COUNT to be a smaller value
	 */
	Status = XAxiDma_BdRingSetCoalesce(RxRingPtr, COALESCING_COUNT,0);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx set coalesce failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	// Update RX BD space memory to DDR
	Xil_DCacheInvalidateRange((UINTPTR)BdPtr, RX_BD_SPACE_HIGH - RX_BD_SPACE_BASE + 1);

	Status = XAxiDma_BdRingToHw(RxRingPtr, FreeBdCount, BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx To Hw failed with %d\r\n", Status);
		return XST_FAILURE;
	}

	/* Enable all RX interrupts */
	XAxiDma_BdRingIntEnable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);

	Xil_DCacheFlush();

	xil_printf("RX ready! \r\n");

	/* Start transfer data now! */
	Status = XAxiDma_BdRingStart(RxRingPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Rx start BD ring failed with %d\r\n", Status);
		return XST_FAILURE;
	}
	xil_printf("RX finished! \r\n");

	return XST_SUCCESS;
}



int DMA_SendPacket()
{
	XAxiDma_BdRing *TxRingPtr = XAxiDma_GetTxRing(&AxiDma);
	u16 *Packet;

	XAxiDma_Bd *BdPtr, *BdCurPtr;
	int Status;
	int Index, Pkts;
	UINTPTR BufferAddr;

	/*
	 * Each packet is limited to TxRingPtr->MaxTransferLen
	 * This will not be the case if hardware has store and forward built in
	 */
	if (MAX_TX_BD_LEN * NUMBER_OF_BDS_PER_PKT >
			TxRingPtr->MaxTransferLen) {

		xil_printf("Invalid total per packet transfer length for the "
		    "packet %d/%d\r\n",
		    MAX_TX_BD_LEN * NUMBER_OF_BDS_PER_PKT,
		    TxRingPtr->MaxTransferLen);

		return XST_INVALID_PARAM;
	}

	Packet = (u16 *) TxPacket;

	Write_Data(Packet);

	/* Flush the buffers before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)Packet, MAX_TX_BD_LEN *
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

			Status = XAxiDma_BdSetLength(BdCurPtr, MAX_TX_BD_LEN,
						TxRingPtr->MaxTransferLen);
			if (Status != XST_SUCCESS) {
				xil_printf("Tx set length %d on BD %x failed %d\r\n",
				MAX_TX_BD_LEN, (UINTPTR)BdCurPtr, Status);

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
			XAxiDma_BdSetId(BdCurPtr, BufferAddr); // Use BufferAddr as ID

			// TODO: 这里是否也需要是MAX_TX_BD_LEN/2？
			BufferAddr += MAX_TX_BD_LEN;
			BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(TxRingPtr, BdCurPtr);
		}
	}

	// Update TX BD space memory to DDR
	Xil_DCacheFlushRange((UINTPTR)BdPtr, TX_BD_SPACE_HIGH - TX_BD_SPACE_BASE + 1);

	/* Give the BD to hardware and the transaction start! */
	Status = XAxiDma_BdRingToHw(TxRingPtr, NUMBER_OF_BDS_TO_TRANSFER,
						BdPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to hw, length %d\r\n",
			(int)XAxiDma_BdGetLength(BdPtr,
					TxRingPtr->MaxTransferLen));

		return XST_FAILURE;
	}


	Status = DMA_RxSetup(&AxiDma);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed RX setup\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int DMA_ReceivePacket()
{

	int Status = XST_SUCCESS;
	XAxiDma_BdRing *RxRingPtr;
	int BdCount;
	XAxiDma_Bd *BdPtr;
	XAxiDma_Bd *BdCurPtr;
	u32 BdSts;
	int Index;


//	RxRingPtr = XAxiDma_GetRxRing(&AxiDma);
//	Status = XAxiDma_BdRingStart(RxRingPtr);
//	if (Status != XST_SUCCESS) {
//		xil_printf("Rx start BD ring failed with %d\r\n", Status);
//		return XST_FAILURE;
//	}
////
//	BdCount = XAxiDma_BdRingFromHw(RxRingPtr, XAXIDMA_ALL_BDS, &BdPtr);
//	BdCurPtr = BdPtr;
//	if(BdCount == 0)
//	{
//		// TODO: 这个也会在发送ADC数据时打印
//		xil_printf("DMA receive packet No RX BD fetch from HW! \r\n");
//	}
//	for (Index = 0; Index < BdCount; Index++) {
//
//		/*
//		 * Check the flags set by the hardware for status
//		 * If error happens, processing stops, because the DMA engine
//		 * is halted after this BD.
//		 */
//		BdSts = XAxiDma_BdGetSts(BdCurPtr);
//		if ((BdSts & XAXIDMA_BD_STS_ALL_ERR_MASK) ||
//			(!(BdSts & XAXIDMA_BD_STS_COMPLETE_MASK))) {
//			Error = 1;
//			xil_printf("RX BD back with error!");
//			break;
//		}
//
//		/* Find the next processed BD */
//		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
//	}
//
//	/* Free all processed BDs for future transmission */
//	Status = XAxiDma_BdRingFree(RxRingPtr, BdCount, BdPtr);
//	if (Status != XST_SUCCESS) {
//		Error = 1;
//		xil_printf("BD free failed!\r\n");
//	}


	Status = DMA_RxSetup(&AxiDma);


	return Status;
}

void Write_Data(short *Packet){
	int Index;
	u16 value;

	// Write MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER Byte

	// DAC 3
	value = 0;
	for(Index = 0; Index < MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER / 2;
								Index = Index + 16) {
		// Packet is u16, so one time write 2 bytes
//		Packet[Index] = Value;
//		Value = (Value + 1) & 0xFFFF;
		// Index is dac3_sample0
		// Index + 1 is dac3_sample1
		// Index + 2 is dac3_sample2
		// Index + 3 is dac3_sample3
		Packet[Index] = SinWave[value++ % 256];
		Packet[Index+1] = SinWave[value++ % 256];
		Packet[Index+2] = SinWave[value++ % 256];
		Packet[Index+3] = SinWave[value++ % 256];
	}


	// DAC 2
	value = 64;
	for(Index = 4; Index <  MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER / 2; Index = Index + 16)
	{
		Packet[Index] = SinWave[value++ % 256];
		Packet[Index+1] = SinWave[value++ % 256];
		Packet[Index+2] = SinWave[value++ % 256];
		Packet[Index+3] = SinWave[value++ % 256];
	}

	// DAC 1
	value = 0;
	for(Index = 8; Index <  MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER / 2; Index = Index + 16)
	{
		Packet[Index] = SinWave[value++ % 256];
		Packet[Index+1] = SinWave[value++ % 256];
		Packet[Index+2] = SinWave[value++ % 256];
		Packet[Index+3] = SinWave[value++ % 256];
	}

	// DAC 0
	value = 64;
	for(Index = 12; Index <  MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER / 2; Index = Index + 16)
	{
		Packet[Index] = SinWave[value++ % 256];
		Packet[Index+1] = SinWave[value++ % 256];
		Packet[Index+2] = SinWave[value++ % 256];
		Packet[Index+3] = SinWave[value++ % 256];
	}

	// adjust scale
	for(Index = 0; Index <  MAX_TX_BD_LEN * NUMBER_OF_BDS_TO_TRANSFER / 2; Index = Index + 1)
	{
		Packet[Index] = Packet[Index] >> 1;
	}


}
