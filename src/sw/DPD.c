/*
 * DPD.c
 *
 *  Created on: 2022年8月28日
 *      Author: zhe
 */

#include"DPD.h"

u32 *MPPacket = (u32 *)MP_BUFFER_BASE;
u32 *PRVTDNNPacket = (u32 *)PRVTDNN_BUFFER_BASE;
XGpio PRVTDNNLoad;


int DPDInit(){
	int Status;
	Status = XGpio_Initialize(&PRVTDNNLoad,XPAR_PRVTDNNLOAD_GPIO_DEVICE_ID);
	if(Status!=XST_SUCCESS)
		return Status;

	XGpio_SetDataDirection(&PRVTDNNLoad,1,0);
	XGpio_DiscreteWrite(&PRVTDNNLoad,1,0);
	return XST_SUCCESS;

}

void Write_MP_coeff(){

	int addr;
	int i = 0;

	//一次写入32位的数据
	for(addr = 0; addr < LUT_size * LUT_num; addr = addr + 4)
	{
		MPBRAM_Out32(addr, MPPacket[i]);
		i++;
	}
	xil_printf("Writing MP coeff to 0x%08x fininshed! \r\n", MPBRAM_BASEADDR);
}

void Write_PRVTDNN_coeff(int coeffNum){
	int addr;
	int i = 0;



	for(addr = 0; addr < coeffNum; addr = addr + 4)
	{
		PRVTDNNBRAM_Out32(addr, PRVTDNNPacket[i]);
		i++;
	}
	xil_printf("Writing PRVTDNN coeff to 0x%08x fininshed! \r\n", PRVTDNNBRAM_BASEADDR);

	usleep(1000);
	XGpio_DiscreteWrite(&PRVTDNNLoad,1,1);
	usleep(100);
	XGpio_DiscreteWrite(&PRVTDNNLoad,1,0);

}




void MPBRAM_Out32(UINTPTR addr, u32 Value){
	// Addr must be 4-bytes aligned!
	Xil_Out32(addr + MPBRAM_BASEADDR, Value);
}


void PRVTDNNBRAM_Out32(UINTPTR addr, u32 Value){
	Xil_Out32(addr + PRVTDNNBRAM_BASEADDR, Value);
}
