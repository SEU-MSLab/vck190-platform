/*
 * JESD.h
 *
 *  revised on: 2022Äê3ÔÂ22ÈÕ
 *  	Author: Zhe Li
 *  	Email:  904016301@qq.com
 */

#include "xgpio.h"
#include "xparameters.h"
//#include "cc.h"

XGpio AD_SDIO;
XGpio AD_CS_SCK;

XGpio DA_SDIO_SDO;
XGpio DA_CS_SCLK;

void delay(int delay_num);
//Init function
int DA_GPIO_Init();
int AD_GPIO_Init();
//DA SPI
void DA_Write(u16_t Addr,u8_t Data);
u8_t DA_Read(u16_t Addr);
//AD SPI
void AD_Write(u16_t Addr,u8_t Data);
u8_t AD_Read(u16_t Addr);

