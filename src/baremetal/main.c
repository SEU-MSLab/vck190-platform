
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
#include "AD9173.h"
#include "AD6688.h"
#include "clock.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "tcp_transmission.h"
//#include "DFX.h"


// Go to sys_opt.h to setup some define

int hardware_init();


int main()
{
	int Status;
	err_t err;
	struct netif server_netif;
	struct netif *netif = &server_netif;

//	sd_init();
//	read_pdi();
//	dow_pdi();


	// init JESD204B and ADC/DAC
	Status = hardware_init();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed hardware initialization\r\n");
	}

	xil_printf("Start DMA transfer...\r\n");


	// Init XScuGic
	Status = Init_Intr_System();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed XScuGic initialization! \r\n");
		return XST_FAILURE;
	}

	//init DMA
	Status = DMA_Init();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed DMA initialization! \r\n");
		return XST_FAILURE;
	}


	// Init timer for lwip
	Status = Timer_init();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed Timer initialization! \r\n");
		return XST_FAILURE;
	}

	/* Send packets cyclically */
	 Status = DMA_SendPacket();
	if (Status != XST_SUCCESS) {

		xil_printf("Failed send packet\r\n");
		return XST_FAILURE;
	}

	Status = DPDInit();
	if(Status != XST_SUCCESS) {
		xil_printf("Failed DPD initialization\r\n");
		return XST_FAILURE;
	}

	//init LWIP
	Status = lwip_sys_init(netif);
	if(Status != XST_SUCCESS) {
		xil_printf("Failed lwip initialization\r\n");
		return XST_FAILURE;
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
		xemacif_input(netif);

		/* if connected to the server and received start command,
		 * start receiving data from PL through AXI DMA,
		 * then transmit the data to the PC using TCP
		 * */
		if(tcp_client_connected && enum_recv_cmd == TCP_START_CMD)
		{
			send_received_data();
		}
	}


	cleanup_platform();
	return 0;
}

int hardware_init()
{
	int Status;

	xil_printf("Start hardware initialization...\n\r");
//	Xil_DCacheDisable(); // Should not disable cache

	Status = DACLK_GPIO_Init();
	if(Status!=XST_SUCCESS)
	{
		xil_printf("DACLK GPIO init failed!");
		return Status;
	}

	Status = AD_GPIO_Init();
	if(Status!=XST_SUCCESS)
	{
		xil_printf("AD GPIO init failed!");
		return Status;
	}

	HMC7044_Init();
	AD9173_Init(0x0c, 0x00);
	AD6688_Init();


	xil_printf("AD9173 chip ID is 0x%02x\n",DA_Read(0x003));
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

	return XST_SUCCESS;
}


