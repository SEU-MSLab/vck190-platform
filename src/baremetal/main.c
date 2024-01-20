
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "SPI_support.h"
#include "JESD.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "DMA_support.h"
#include "sys_intr.h"
#include "timer_intr.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "netif/xadapter.h"
#include "tcp_transmission.h"





int main()
{
	init_platform();
	int Status;

	//init JESD204B and ADC/DAC
	Status = hardware_init();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed hardware initialization\r\n");
	}

	xil_printf("Start DMA transfer...\r\n");

	//init DMA
	Status = DMA_Init(&AxiDma,DMA_DEV_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed DMA initialization! \r\n");
		return XST_FAILURE;
	}


	//init timer for lwip
	Timer_init();

	//init interrupt system
	Init_Intr_System(); // initial DMA interrupt system
	DMA_SetupIntrSystem();//setup dma interrpt system
	Timer_Setup_Intr_System();


	/* Send packets cyclically */
	Status = DMA_SendPacket();
	if (Status != XST_SUCCESS) {

		xil_printf("Failed send packet\r\n");
		return XST_FAILURE;
	}

	/* Receive a packet */
	Status = DMA_ReceivePacket();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed receive packet\r\n");
		return XST_FAILURE;
	}

	//init LWIP
	Status = lwip_sys_init();
	if(Status != XST_SUCCESS) {
		xil_printf("Failed lwip initialization\r\n");
	}



	while (1)
	{
		/* call tcp timer every 250ms */
		if(TcpTmrFlag)
		{
			if(request_pcb->state == CLOSED || (request_pcb->state == SYN_SENT && request_pcb->nrtx == TCP_SYNMAXRTX))
			{
				request_pcb = tcp_new();
				if (!request_pcb) {
					xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
					return -1;
				}

//				ip_set_option(request_pcb, SOF_REUSEADDR);

				err = tcp_connect(request_pcb, &ipaddress, port, tcp_connected_callback);
				if (err != ERR_OK) {
					xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
					return err;
				}
			}
			tcp_tmr();
			TcpTmrFlag = 0;
		}
		/*receive input packet and control command from emac*/
		xemacif_input(netif);//将MAC队列里的packets传输到你的LwIP/IP stack里

		/*receive control command from uart1*/
		UartPs_recv(&UartPs, 4);

		/* if connected to the server and received start command,
		 * start receiving data from PL through AXI DMA,
		 * then transmit the data to the PC using TCP
		 * */
		if(tcp_client_connected && tcp_trans_start)
			send_received_data();

		/*if received reset command, reset some PL's logic*/
		if(tcp_trans_reset && packet_trans_done)
		{
			/*reset some PL's logic*/
			PL_reset(&Gpio_reset);
			/*re-initial PL's AXI DMA*/
			DMA_Intr_Init(&AxiDma, AXIDMA_DEV_ID);
			DMA_Intr_Enable(&Intc, &AxiDma);
			tcp_trans_reset = 0;
			first_trans_start = 0;
			packet_trans_done = 0;
			packet_index = 0;
			xil_printf("PL reset done!\r\n");
		}
	}


	cleanup_platform();
	return 0;
}

int hardware_init()
{
	int Status;

	xil_printf("Start hardware initialization...\n\r");
	Xil_DCacheDisable();
	// init_gpio

	Status = DA_GPIO_Init();
	if(Status!=XST_SUCCESS)
		return Status;
	Status = AD_GPIO_Init();
	if(Status!=XST_SUCCESS)
		return Status;

	xil_printf("AD9173 chip ID is 0x%02x\n",DA_Read(0x0003));
	xil_printf("AD6688 chip ID is 0x%02x\n",AD_Read(0x0003));
	/*************************JESD************************************/

	JESD_init();
	JESD_phy_clocked();//CPLL is clocked
	//TX CGS
	JESD_TX_SYNC();

	//check for TX Link quality
	DA_check_JESD();
	//check for RX Link quality
	AD_check_JESD();


	u32 i;
	for(i=0;i<24576;i++)
	{
		Xil_Out64(0x20000000+8*i,i+1);
	}

	for(i=0;i<24576;i++)
	{
		Xil_Out64(0x20030000+8*i,i+2);
	}
	for(i=0;i<24576;i++)
	{
		Xil_Out64(0x20060000+8*i,i+3);
	}
	for(i=0;i<24576;i++)
	{
		Xil_Out64(0x20090000+8*i,i+4);
	}

	return XST_SUCCESS;
}


