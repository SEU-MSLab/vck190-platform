/*
 * DMA_support.h
 *
 *  Created on: 2022Äê3ÔÂ27ÈÕ
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
#define DDR_BASE_ADDR	XPAR_AXI_NOC_C0_DDR_LOW0_FPD_CCI_NOC_0_BASEADDR
#define MEM_BASE_ADDR	(DDR_BASE_ADDR + 0x1000000)

#define RX_INTR_ID		XPAR_FABRIC_AXIDMA_0_S2MM_INTROUT_VEC_ID
#define TX_INTR_ID		XPAR_FABRIC_AXIDMA_0_MM2S_INTROUT_VEC_ID

#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR)
#define RX_BD_SPACE_HIGH	(RX_BD_SPACE_BASE + 0x00000040)
//#define RX_BD_SPACE_HIGH	(RX_BD_SPACE_BASE + 0x00000080)
//#define RX_BD_SPACE_HIGH	(RX_BD_SPACE_BASE + 0x0000FFFF)
//#define TX_BD_SPACE_BASE	(RX_BD_SPACE_HIGH + 0x00000001)
#define TX_BD_SPACE_BASE	0x01010000
#define TX_BD_SPACE_HIGH	(TX_BD_SPACE_BASE + 0x0000FFFF)
#define TX_BUFFER_BASE		(TX_BD_SPACE_HIGH + 0x00000001)
#define	TX_BUFFER_HIGH 		(TX_BUFFER_BASE + 0x003FFFFF)
#define RX_BUFFER_BASE		(TX_BUFFER_HIGH + 0x00000001)
#define RX_BUFFER_HIGH		(RX_BUFFER_BASE + 0x003FFFFF)
#define CMD_BUFFER_BASE 	(RX_BUFFER_HIGH + 0x00000001)
#define CMD_BUFFER_HIGH 	(CMD_BUFFER_BASE + 0x000000FF)


/* Packet configuration */
#define NUMBER_OF_BDS_PER_PKT		1
#define NUMBER_OF_PKTS_TO_TRANSFER 	1
#define NUMBER_OF_BDS_TO_TRANSFER	(NUMBER_OF_PKTS_TO_TRANSFER * NUMBER_OF_BDS_PER_PKT)

/* For Interrupt */
#define COALESCING_COUNT		NUMBER_OF_PKTS_TO_TRANSFER
#define DELAY_TIMER_COUNT		100
// Disable IRQDelay
//#define DELAY_TIMER_COUNT		0

/* Timeout loop counter for reset */
#define RESET_TIMEOUT_COUNTER	10000

/*
 * Buffer and Buffer Descriptor related constant definition
 * Unit is Bytes
 */
//#define MAX_TX_BD_LEN 98304
#define MAX_TX_BD_LEN 4194304
#define MAX_RX_BD_LEN 4194304
//#define MAX_RX_BD_LEN 2097152
//#define MAX_RX_BD_LEN 1048576
//#define MAX_TX_BD_LEN 1048576

extern u32 *TxPacket;
extern u32 *RxPacket;
extern u32 *CMDPacket;

extern volatile int packet_trans_done;  //Tell lwip that ADC packet is ready to transfer to PC
extern volatile int Error;

extern XAxiDma AxiDma;
extern u16 *RxBufferPtr;


int DMA_SetupIntrSystem();
void DMA_TxIntrHandler(void *Callback);
void DMA_RxIntrHandler(void *Callback);
int DMA_Init();
int DMA_TxSetup(XAxiDma * AxiDmaInstPtr);
int DMA_RxSetup(XAxiDma * AxiDmaInstPtr);
int DMA_SendPacket();
int DMA_ReceivePacket();
void Write_Data(short *Packet);


#endif /* SRC_DMA_SUPPORT_H_ */
