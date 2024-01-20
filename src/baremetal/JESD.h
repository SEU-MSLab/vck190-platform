/*
 * JESD.h
 *
 *  Created on: 2019年5月4日
 *      Author: THEPHOENIX
 *
 *  revised on: 2022年3月22日
 *  	Author: Zhe Li
 *  	Email:  904016301@qq.com
 */

//#include "cc.h" // For lwIP
#include "xil_io.h"
#include "xparameters.h"
#include "xstatus.h"
#include "SPI_support.h"
#include "xgpio.h"
//Link
#define JESD0 0
#define JESD1 1
#define JESD2 2
//BaseAddr
#define JESD0_BaseAddr XPAR_JESD204C_0_BASEADDR //Transmit0
#define JESD1_BaseAddr XPAR_JESD204C_1_BASEADDR //Transmit1
#define JESD2_BaseAddr XPAR_JESD204C_2_BASEADDR //Receiver

//physical layer
#define JESD_PHYRX0      XPAR_GT_QUAD_BASE_0_BASEADDR
#define JESD_PHYRX1      XPAR_GT_QUAD_BASE_1_BASEADDR
#define JESD_PHYTX0      XPAR_GT_QUAD_BASE_2_BASEADDR
#define JESD_PHYTX1      XPAR_GT_QUAD_BASE_3_BASEADDR

//register space
#define JESD_VERSION 	0x000
#define JESD_RESET		0x020
#define JESD_STATUS		0x060
#define JESD_8B10BCFG	0x03c
// For RX only
#define JESD_RXERR		0x058
#define JESD_RXDEBUG	0x05c
// For TX only
#define JESD_TXILA0		0x070
#define JESD_TXILA1		0X074
#define JESD_TXILA2		0x078
#define JESD_TXILA3		0X07C
#define JESD_TXILA4		0x080



//value definition
#define STATUS_SYNC			0x1000  //16'd4096
#define STATUS_SYSREF 		0x0002
#define STATUS_RX_STARTED 	0x4000	//16'd16394
// F (Octets per Frame per Lane)
// K (Frames per Multiframe)
// CS (Control bits per Sample)
// N' (Totals bits per Sample)
// N (Convertor Resolution)
#define CFG_8B10BTX			0x03021F00  // F= 1, K = 32, disable link error counter, disable scrambling
#define CFG_TXILA1			0x000F0F01 	// CS = 0, N = 16, NP = 16, M = 2
#define CFG_TXILA2   		0x00010000	// CF = 0,HD=1,S = 1

#define CFG_8B10BRX			0x03021F00	// F = 1, K = 32, disable link error counter, disable scrambling

// TX ID register
#define Lane0_ID_REG 0x404
#define Lane1_ID_REG 0x484
#define Lane2_ID_REG 0x504
#define Lane3_ID_REG 0x584


//Set transmit Link parameters (IP don't have)
void JESD_init();
//phy CPLL clocked
void JESD_phy_clocked();
//JESD TX SYNC
void JESD_TX_SYNC();
//JESD TX check Link
void DA_check_JESD();
//JESD RX check Link
void AD_check_JESD();

//JESD_read
u32 JESD_read(u32 Addr,int channel);//channel 0 1 2
void JESD_write(u32 Addr,int channel,u32 data);
