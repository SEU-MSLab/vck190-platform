/*
 * uartps_intr.c
 *
 *  Created on: 2017Äê3ÔÂ14ÈÕ
 *      Author: 201607062058
 */

#include"uartps_intr.h"

/**************************************************************************/
/**
*
* This function is the handler which performs processing to handle data events
* from the device.  It is called from an interrupt context. so the amount of
* processing should be minimal.
*
* This handler provides an example of how to handle data for the device and
* is application specific.
*
* @param	CallBackRef contains a callback reference from the driver,
*		in this case it is the instance pointer for the XUartPs driver.
* @param	Event contains the specific kind of event that has occurred.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
***************************************************************************/
void Uart_Handler(void *CallBackRef, u32 Event, unsigned int EventData)
{
	xil_printf("interrupt happen!\r\n");
	/* All of the data has been sent */
	if (Event == XUARTPS_EVENT_SENT_DATA) {
		TotalSentCount = EventData;
		//xil_printf("interrupt happen! 0\r\n");
	}

	/* All of the data has been received */
	if (Event == XUARTPS_EVENT_RECV_DATA) {
		TotalReceivedCount = EventData;
		//xil_printf("interrupt happen! 1\r\n");
	}

	/*
	 * Data was received, but not the expected number of bytes, a
	 * timeout just indicates the data stopped for 8 character times
	 */
	if (Event == XUARTPS_EVENT_RECV_TOUT) {
		TotalReceivedCount = EventData;
		//xil_printf("interrupt happen! 2\r\n");
	}

	/*
	 * Data was received with an error, keep the data but determine
	 * what kind of errors occurred
	 */
	if (Event == XUARTPS_EVENT_RECV_ERROR) {
		TotalReceivedCount = EventData;
		Uart_TotalErrorCount++;
		//xil_printf("interrupt happen! 3\r\n");
	}

	/*
	 * Data was received with an parity or frame or break error, keep the data
	 * but determine what kind of errors occurred. Specific to Zynq Ultrascale+
	 * MP.
	 */
	if (Event == XUARTPS_EVENT_PARE_FRAME_BRKE) {
		TotalReceivedCount = EventData;
		Uart_TotalErrorCount++;
		//xil_printf("interrupt happen! 4\r\n");
	}

	/*
	 * Data was received with an overrun error, keep the data but determine
	 * what kind of errors occurred. Specific to Zynq Ultrascale+ MP.
	 */
	if (Event == XUARTPS_EVENT_RECV_ORERR) {
		TotalReceivedCount = EventData;
		Uart_TotalErrorCount++;
		//xil_printf("interrupt happen! 5\r\n");
	}
}



void UartPs_Setup_Intr_System(XScuGic *GicInstancePtr, XUartPs *UartInstPtr, u16 UartIntrId)
{
	u32 IntrMask;
	/*
	 * Setup the handlers for the UART that will be called from the
	 * interrupt context when data has been sent and received, specify
	 * a pointer to the UART driver instance as the callback reference
	 * so the handlers are able to access the instance data
	 */
	XUartPs_SetHandler(UartInstPtr, (XUartPs_Handler)Uart_Handler, UartInstPtr);

	/*
	 * Enable the interrupt of the UART so interrupts will occur, setup
	 * a local loopback so data that is sent will be received.
	 */
	IntrMask =

		  XUARTPS_IXR_RXOVR;


	XUartPs_SetInterruptMask(UartInstPtr, IntrMask);

	XScuGic_Connect(GicInstancePtr, UartIntrId,
                        (Xil_ExceptionHandler)XUartPs_InterruptHandler,//set up the timer interrupt
                        (void *)UartInstPtr);

    XScuGic_Enable(GicInstancePtr, UartIntrId);//enable the interrupt for the Timer at GIC
        //XScuTimer_EnableInterrupt(TimerInstancePtr);//enable interrupt on the timer
 }


int UartPs_init(XUartPs *UartInstPtr, u16 DeviceId)
{
	int Status;
	XUartPs_Config *Config;


	/*
	 * Initialize the UART driver so that it's ready to use
	 * Look up the configuration in the config table, then initialize it.
	 */
	Config = XUartPs_LookupConfig(DeviceId);
	if (NULL == Config) {
		xil_printf("no config find \r\n");
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(UartInstPtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("UartPs init failed\r\n");
		return XST_FAILURE;
	}

	/* Check hardware build */
//	Status = XUartPs_SelfTest(UartInstPtr);
//	if (Status != XST_SUCCESS) {
//		xil_printf("UartPs selftest failed\r\n");
//		return XST_FAILURE;
//	}

//	XUartPs_DisableUart(UartInstPtr);


//
//	XUartPs_SetOperMode(UartInstPtr, XUARTPS_OPER_MODE_NORMAL);
//
//	XUartPs_SetBaudRate(UartInstPtr, 115200);
//
//	XUartPs_SetFifoThreshold(UartInstPtr, 4);
//
//	/*
//	 * Set the receiver timeout. If it is not set, and the last few bytes
//	 * of data do not trigger the over-water or full interrupt, the bytes
//	 * will not be received. By default it is disabled.
//	 *
//	 * The setting of 8 will timeout after 8 x 4 = 32 character times.
//	 * Increase the time out value if baud rate is high, decrease it if
//	 * baud rate is low.
//	 */
//	XUartPs_SetRecvTimeout(UartInstPtr, 0);
//
////	XUartPs_EnableUart(UartInstPtr);
//
//	TotalReceivedCount = 0;
//	TotalSentCount = 0;
//	Uart_TotalErrorCount = 0;
//
//	UartInstPtr->ReceiveBuffer.RequestedBytes = 4;
//	UartInstPtr->ReceiveBuffer.RemainingBytes = 4;
//	UartInstPtr->ReceiveBuffer.NextBytePtr = Uart_RecvBuffer;

	received_num = 0;
	receive_index = 0;

	return XST_SUCCESS;
}

void UartPs_recv(XUartPs *UartInstPtr, u32 NumBytes)
{
	received_num = XUartPs_Recv(UartInstPtr, Uart_RecvBuffer, NumBytes);

	if(received_num > 0)
	{
		//xil_printf("%d\r\n", recieved_num);
		memcpy((u8 *)&Uart_Recvdata + receive_index, Uart_RecvBuffer, received_num);

		receive_index = receive_index + received_num;

		if(receive_index == 4)
		{
			receive_index = 0;
			Uart_Recvdata = ntohl(Uart_Recvdata);
			if(Uart_Recvdata == UART_START_CMD)
			{
				if(tcp_trans_start)
					xil_printf("UART: already start!\r\n");
				else
				{
					tcp_trans_reset = 0;
					XGpio_DiscreteWrite(&Gpio, 1, 1);
					tcp_trans_start = 1;
					xil_printf("UART: start!\r\n");
				}
				xil_printf("AA55BBA0\r\n");
				uart_return_info = ntohl(UART_SUCCESS);
				XUartPs_Send(UartInstPtr, (u8 *)&uart_return_info, 4);
			}
			else if(Uart_Recvdata == UART_STOP_CMD)
			{
				if(!tcp_trans_start)
					xil_printf("UART: already stop!\r\n");
				else
				{
					tcp_trans_reset = 0;
					XGpio_DiscreteWrite(&Gpio, 1, 0);
					tcp_trans_start = 0;
					xil_printf("UART: stop!\r\n");
				}
				xil_printf("AA55BBA0\r\n");
				uart_return_info = ntohl(UART_SUCCESS);
				XUartPs_Send(UartInstPtr, (u8 *)&uart_return_info, 4);
			}
			else if(Uart_Recvdata == UART_RESET_CMD)
			{
				if(tcp_trans_reset)
					xil_printf("UART: already reset!\r\n");
				else
				{
					tcp_trans_reset = 1;
					XGpio_DiscreteWrite(&Gpio, 1, 0);
					tcp_trans_start = 0;
					xil_printf("UART: reset!\r\n");
				}
				xil_printf("AA55BBA0\r\n");
				uart_return_info = ntohl(UART_SUCCESS);
				XUartPs_Send(UartInstPtr, (u8 *)&uart_return_info, 4);
			}
			else
			{
				xil_printf("UART: unsupported uart command!\r\n");
				xil_printf("AA55CCA0\r\n");
				uart_return_info = ntohl(UART_FAILURE);
				XUartPs_Send(UartInstPtr, (u8 *)&uart_return_info, 4);
			}
		    //xil_printf("%08x\r\n", Uart_Recvdata);
		}
	}
}

