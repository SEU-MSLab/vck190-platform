/*
 * AD9173.h
 *
 *  Created on: May 25, 2022
 *      Author: zhe
 */

#ifndef SRC_AD9173_H_
#define SRC_AD9173_H_

#include "sleep.h"
#include "SPI_support.h"
#include "xil_printf.h"
#include <stdio.h>
void AD9173_Init_cycle();
void AD9173_Init(u8 val,u8 del);



#endif /* SRC_AD9173_H_ */
