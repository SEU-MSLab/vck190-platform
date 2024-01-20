/*
 * JESD.h
 *
 *  revised on: 2022Äê3ÔÂ22ÈÕ
 *  	Author: Zhe Li
 *  	Email:  904016301@qq.com
 */

#include "xgpio.h"
#include "xparameters.h"
#include "sleep.h"

void delay(int delay_num);
// Init function
int DACLK_GPIO_Init();
int AD_GPIO_Init();
// DA SPI
void DA_Write(u16 Addr,u8 Data);
u8 DA_Read(u16 Addr);
// CLK SPI
void CLK_Write(u16 Addr,u8 Data);
u8 CLK_Read(u16 Addr);
// AD SPI
void AD_Write(u16 Addr,u8 Data);
u8 AD_Read(u16 Addr);
void AD_Write_4wire(u16 Addr, u8 Data);
u8 AD_Read_4wire(u16 Addr);

