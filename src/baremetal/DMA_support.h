/*
 * DMA_support.h
 *
 *  Created on: 2022年3月27日
 *      Author: zhe
 */

#ifndef SRC_DMA_SUPPORT_H_
#define SRC_DMA_SUPPORT_H_

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"
#include "sys_intr.h"


#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID
#define DDR_BASE_ADDR	XPAR_AXI_NOC_0_C0_DDR_LOW0_FPD_CCI_NOC_0_BASEADDR
#define MEM_BASE_ADDR	(DDR_BASE_ADDR + 0x1000000)

#define RX_INTR_ID		XPAR_FABRIC_AXIDMA_0_S2MM_INTROUT_VEC_ID
#define TX_INTR_ID		XPAR_FABRIC_AXIDMA_0_MM2S_INTROUT_VEC_ID

#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR)
#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x0000FFFF)
#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00010000)
#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x0001FFFF)
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)
//#define RX_BUFFER1_BASE		(MEM_BASE_ADDR + 0x00500000)
//#define RX_BUFFER1_HIGH		(MEM_BASE_ADDR + 0x005FFFFF)

/* Packet configuration */
#define NUMBER_OF_BDS_PER_PKT		2
#define NUMBER_OF_PKTS_TO_TRANSFER 	1
#define NUMBER_OF_BDS_TO_TRANSFER	(NUMBER_OF_PKTS_TO_TRANSFER * NUMBER_OF_BDS_PER_PKT)

/* For Interrupt */
#define COALESCING_COUNT		NUMBER_OF_PKTS_TO_TRANSFER
#define DELAY_TIMER_COUNT		100

/* Timeout loop counter for reset */
#define RESET_TIMEOUT_COUNTER	10000

/*
 * Buffer and Buffer Descriptor related constant definition
 * 单位是字节
 */
#define MAX_BD_LEN		2048

u32 *TxPacket = (u32 *) TX_BUFFER_BASE;
u32 *RxPacket = (u32 *) RX_BUFFER_BASE;

volatile int packet_trans_done;  //Tell lwip that ADC packet is ready to transfer to PC
volatile int Error;


XAxiDma AxiDma;
UINTPTR RxBufferPtr;


int DMA_SetupIntrSystem();
void DMA_TxCallBack(XAxiDma_BdRing * TxRingPtr);
void DMA_TxIntrHandler(void *Callback);
void DMA_RxCallBack(XAxiDma_BdRing * RxRingPtr);
void DMA_RxIntrHandler(void *Callback);
int DMA_Init(XAxiDma *DMAPtr,u32 DeviceId);
int DMA_TxSetup(XAxiDma * AxiDmaInstPtr);
int DMA_RxSetup(XAxiDma * AxiDmaInstPtr);
int DMA_SendPacket(XAxiDma * AxiDmaInstPtr);
int DMA_ReceivePacket(XAxiDma * AxiDmaInstPtr);


#endif /* SRC_DMA_SUPPORT_H_ */
