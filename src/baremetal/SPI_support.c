#include "SPI_support.h"
//#include "sleep.h"

void delay(int delay_num){
	for(int i = 0;i<delay_num;i++){

	}
}

//AD DA Init
int DA_GPIO_Init(){
	int Status;
	Status = XGpio_Initialize(&DA_CS_SCLK,XPAR_DA_CS_SCLK_GPIO_BASEADDR);
	if(Status!=XST_SUCCESS)
		return Status;
	Status = XGpio_Initialize(&DA_SDIO_SDO,XPAR_DA_SDIO_SDO_GPIO_BASEADDR);
	if(Status!=XST_SUCCESS)
		return Status;
	// The second param to choose GPIO1 or GPIO2 channel
	// The third param set to 0 are output and bits set to 1 are input.
	XGpio_SetDataDirection(&DA_CS_SCLK,1,0);
	XGpio_SetDataDirection(&DA_CS_SCLK,2,0);
	XGpio_SetDataDirection(&DA_SDIO_SDO,2,1);
	// Don't need to spcecify direction of SDIO port now
	return XST_SUCCESS;
}

int AD_GPIO_Init(){
	int Status;
	Status = XGpio_Initialize(&AD_CS_SCLK,XPAR_AD_CS_SCLK_GPIO_BASEADDR);
	if(Status!=XST_SUCCESS)
		return Status;
	Status = XGpio_Initialize(&AD_SDIO,XPAR_AD_SDIO_GPIO_BASEADDR);
	if(Status!=XST_SUCCESS)
		return Status;
	XGpio_SetDataDirection(&AD_CS_SCLK,1,0);
	XGpio_SetDataDirection(&AD_CS_SCLK,2,0);
	return XST_SUCCESS;
}
//DA SPI function
void DA_Write(u16_t Addr,u8_t Data){
	u16_t inst;
	inst = Addr|0x0000;

	XGpio_SetDataDirection(&DA_SDIO_SDO,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&DA_SCK_CS,1,1);//SCK = 1
	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1
	XGpio_DiscreteWrite(&DA_SCK_CS,2,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DA_SCK_CS,1,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,1);
		else
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&DA_SCK_CS,1,1);
		inst = inst<<1;
		delay(1000);
	}

	//Write Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DA_SCK_CS,1,0);
		if((Data&0x80)==0x80)
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,1);
		else
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&DA_SCK_CS,1,1);
		Data = Data<<1;
		delay(1000);
	}
	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1
}
//DA Read
u8_t DA_Read(u16_t Addr){
	u8_t data = 0;
	u16_t inst;
	inst = Addr|0x8000;

	XGpio_SetDataDirection(&DA_SDIO_SDO,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&DA_SCK_CS,1,1);//SCK = 1
	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1
	XGpio_DiscreteWrite(&DA_SCK_CS,2,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DA_SCK_CS,1,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,1);
		else
			XGpio_DiscreteWrite(&DA_SDIO_SDO,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&DA_SCK_CS,1,1);
		inst = inst<<1;
		delay(1000);
	}

	//Read Data
//	for(int j = 0;j<8;j++){
//		XGpio_DiscreteWrite(&DA_SCK_CS,1,0);
//		data = data<<1;
//		data |= (u8_t)XGpio_DiscreteRead(&DA_SDIO_SDO,2);
//		delay(1000);
//		XGpio_DiscreteWrite(&DA_SCK_CS,1,1);
//		delay(1000);
//	}
//	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1


	//Read Data
	XGpio_SetDataDirection(&DA_SDIO_SDO,1,1);//channel 1 in(1)
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DA_SCK_CS,1,0);
		data = data<<1;
		data |= (u8_t)XGpio_DiscreteRead(&DA_SDIO_SDO,1);
		delay(1000);
		XGpio_DiscreteWrite(&DA_SCK_CS,1,1);
		delay(1000);
	}
	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1
	return data;
}
//AD SPI
void AD_Write(u16_t Addr,u8_t Data){
	u16_t inst;
	inst = Addr|0x0000;

	XGpio_SetDataDirection(&AD_SDIO_SCK,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&AD_CS,1,1);//CS = 1
	XGpio_DiscreteWrite(&AD_CS,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,1);
		else
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);
		inst = inst<<1;
		delay(1000);
	}

	//Write Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,0);
		if((Data&0x80)==0x80)
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,1);
		else
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);
		Data = Data<<1;
		delay(1000);
	}
	XGpio_DiscreteWrite(&AD_CS,1,1);//CS = 1
}
u8_t AD_Read(u16_t Addr){
	u8_t data = 0;
	u16_t inst;
	inst = Addr|0x8000;

	XGpio_SetDataDirection(&AD_SDIO_SCK,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&AD_CS,1,1);//CS = 1
	XGpio_DiscreteWrite(&AD_CS,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,1);
		else
			XGpio_DiscreteWrite(&AD_SDIO_SCK,1,0);
		delay(1000);
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);
		inst = inst<<1;
		delay(1000);
	}

	XGpio_SetDataDirection(&AD_SDIO_SCK,1,1);
	//Read Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,0);
		data = data<<1;
		data |= (u8_t)XGpio_DiscreteRead(&AD_SDIO_SCK,1);
		delay(1000);
		XGpio_DiscreteWrite(&AD_SDIO_SCK,2,1);
		delay(1000);
	}
	XGpio_DiscreteWrite(&DA_SCK_CS,2,1);//CS = 1
	return data;
}
