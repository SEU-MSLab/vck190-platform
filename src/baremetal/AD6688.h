/*
 * AD6688.h
 *
 *  Created on: May 25, 2022
 *      Author: zhe
 */

#ifndef SRC_AD6688_H_
#define SRC_AD6688_H_

#include "sleep.h"
#include "SPI_support.h"
#include "xil_printf.h"
#include <stdio.h>
void AD6688_Init();
void AD6688_FIR();
void FIR_Real48Tap();
void FIR_Real96Tap();

#endif /* SRC_AD6688_H_ */
