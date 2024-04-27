/*
 * DPD.h
 *
 *  Created on: 2022Äê8ÔÂ28ÈÕ
 *      Author: zhe
 */

#ifndef SRC_DPD_H_
#define SRC_DPD_H_

#include "xil_io.h"
#include "DMA_support.h"
#include "xgpio.h"
#include "sleep.h"

#define MPBRAM_DEVICE_ID		XPAR_BRAM_0_DEVICE_ID
#define MPBRAM_BASEADDR 		XPAR_BRAM_0_BASEADDR
#define PRVTDNNBRAM_DEVICE_ID	XPAR_BRAM_1_DEVICE_ID
#define PRVTDNNBRAM_BASEADDR	XPAR_BRAM_1_BASEADDR

// range in vivado address editor, 64kB
#define MP_BUFFER_BASE 		(CMD_BUFFER_BASE + 0x00010000)
#define MP_BUFFER_HIGH 		(MP_BUFFER_BASE + 0x0000FFFF)
#define PRVTDNN_BUFFER_BASE (MP_BUFFER_HIGH + 0x00000001)
#define PRVTDNN_BUFFER_HIGH (PRVTDNN_BUFFER_BASE + 0x0000FFFF)

// For MP model
#define LUT_depth 	4096
#define LUT_num 	4
#define LUT_size 	(LUT_depth * 4)

// For PRVTDNN model
#define weightsNum  300
#define biasesNum 	28

extern u32* MPPacket;
extern u32* PRVTDNNPacket;

int DPDInit();
void Write_MP_coeff();
void Write_PRVTDNN_coeff();
void MPBRAM_Out32(UINTPTR Addr, u32 Value);
void PRVTDNNBRAM_Out32(UINTPTR addr, u32 Value);

#endif /* SRC_DPD_H_ */
