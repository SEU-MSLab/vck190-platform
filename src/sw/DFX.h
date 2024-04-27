/*
 * DFX.h
 *
 *  Created on: 2022Äê9ÔÂ28ÈÕ
 *      Author: zhe
 */

#ifndef SRC_DFX_H_
#define SRC_DFX_H_

#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"
#include "xilfpga.h"

#define BITSTREAM_SIZE	0x10000000U /* Bin or bit or PDI image size */
#define PDI_LOAD        XFPGA_PDI_LOAD


int sd_init();
int read_pdi();
int dow_pdi();

#endif /* SRC_DFX_H_ */
