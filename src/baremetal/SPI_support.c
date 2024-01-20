#include "SPI_support.h"


XGpio AD_SDI_SDO;
XGpio AD_CS_SCLK;

XGpio DACLK_SDIO_SDI;
XGpio DACLK_CS_SCLK;
XGpio CLK_CS;

void delay(int delay_num){
	for(int i = 0;i<delay_num;i++){
	}
}

//AD DA Init
int DACLK_GPIO_Init(){
	int Status;
	Status = XGpio_Initialize(&DACLK_CS_SCLK,XPAR_SPI_CTRL_DACLK_CS_SCLK_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;
	Status = XGpio_Initialize(&DACLK_SDIO_SDI,XPAR_SPI_CTRL_DACLK_SDIO_SDI_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;
	Status = XGpio_Initialize(&CLK_CS,XPAR_SPI_CTRL_CLK_CS_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;

	// The second param to choose GPIO1 or GPIO2 channel
	// The third param set to 0 are output and bits set to 1 are input.
	XGpio_SetDataDirection(&DACLK_CS_SCLK,1,0);	// CS output
	XGpio_SetDataDirection(&DACLK_CS_SCLK,2,0);  	// SCLK output
	XGpio_SetDataDirection(&DACLK_SDIO_SDI,2,1); 	// For AD9173 is output, for FPGA is input
	// Don't need to spcecify direction of SDIO port now
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,1);//CS = 1
	return XST_SUCCESS;
}

int AD_GPIO_Init(){
	int Status;
	Status = XGpio_Initialize(&AD_CS_SCLK,XPAR_SPI_CTRL_AD_CS_SCLK_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;
	Status = XGpio_Initialize(&AD_SDI_SDO,XPAR_SPI_CTRL_AD_SDI_SDO_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;
	XGpio_SetDataDirection(&AD_CS_SCLK,1,0);
	XGpio_SetDataDirection(&AD_CS_SCLK,2,0);
	return XST_SUCCESS;
}
//DA SPI function
void DA_Write(u16 Addr,u8 Data){
	u16 inst;
	inst = Addr|0x0000;

	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,1);//CS = 1
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}

	//Write Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		if((Data&0x80)==0x80)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		Data = Data<<1;
		delay(10000);
	}
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,1);//CS = 1
	delay(10000);
}
//DA Read
u8 DA_Read(u16 Addr){
	u8 data = 0;
	u16 inst;
	inst = Addr|0x8000; // 将所有地址变成0x8xxx，因为15bit为1表示读操作

	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,0);//channel 1 output(0)
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,1);//CS = 1
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0); //先将SCLK拉低，决定了是在SCLK上升沿传数
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}


//Read Data, 4-wire mode
//	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,1);//channel 1 input(1)
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		data = data<<1;
		data |= (u8)XGpio_DiscreteRead(&DACLK_SDIO_SDI,2);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		delay(10000);
	}
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,1,1);//CS = 1
	delay(10000);
	return data;
}
//AD SPI
void AD_Write(u16 Addr,u8 Data){
	u16 inst;
	inst = Addr|0x0000;

	XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,1);//CS = 1
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,1);
		else
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,0);
		delay(10000);
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}

	//Write Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,0);
		if((Data&0x80)==0x80)
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,1);
		else
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,0);
		delay(10000);
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);
		Data = Data<<1;
		delay(10000);
	}
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,1);//CS = 1
	delay(10000);
}
u8 AD_Read(u16 Addr){
	u8 data = 0;
	u16 inst;
	inst = Addr|0x8000;

	XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,1);//CS = 1
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,1);
		else
			XGpio_DiscreteWrite(&AD_SDI_SDO,2,0);
		delay(10000);
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}

	//Read Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,0);
		data = data<<1;
		data |= (u8)XGpio_DiscreteRead(&AD_SDI_SDO,1);
		delay(10000);
		XGpio_DiscreteWrite(&AD_CS_SCLK,2,1);
		delay(10000);
	}
	XGpio_DiscreteWrite(&AD_CS_SCLK,1,1);//CS = 1
	delay(10000);
	return data;
}

void CLK_Write(u16 Addr,u8 Data){
	u16 inst;
	inst = Addr|0x0000;

	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,0);//channel 1 out(0)
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&CLK_CS,1,1);//CS = 1
	XGpio_DiscreteWrite(&CLK_CS,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}

	//Write Data
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		if((Data&0x80)==0x80)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		Data = Data<<1;
		delay(10000);
	}
	XGpio_DiscreteWrite(&CLK_CS,1,1);//CS = 1
	delay(10000);
};

// AD9173 eval board can't read register from HMC7044
u8 CLK_Read(u16 Addr){
	u8 data = 0;
	u16 inst;
	inst = Addr|0x8000;

	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,0);//channel 1 output(0)
	XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);//SCK = 1
	XGpio_DiscreteWrite(&CLK_CS,1,1);//CS = 1
	XGpio_DiscreteWrite(&CLK_CS,1,0);//CS = 0

	//Write Addr
	for(int i = 0;i<16;i++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0); //先将SCLK拉低，决定了是在SCLK上升沿传数
		if((inst&0x8000)==0x8000)
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,1);
		else
			XGpio_DiscreteWrite(&DACLK_SDIO_SDI,1,0);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		inst = inst<<1;
		delay(10000);
	}


	//Read Data
	XGpio_SetDataDirection(&DACLK_SDIO_SDI,1,1);//channel 1 input(1)
	for(int j = 0;j<8;j++){
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,0);
		data = data<<1;
		data |= (u8)XGpio_DiscreteRead(&DACLK_SDIO_SDI,2);
		delay(10000);
		XGpio_DiscreteWrite(&DACLK_CS_SCLK,2,1);
		delay(10000);
	}
	XGpio_DiscreteWrite(&CLK_CS,1,1);//CS = 1
	delay(10000);
	return data;
};
