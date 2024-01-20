/*
 * uartps_intr.h
 *
 *  Created on: 2017Äê3ÔÂ14ÈÕ
 *      Author: 201607062058
 */

#ifndef UARTPS_INTR_H_
#define UARTPS_INTR_H_

#include "xparameters.h"
#include "xuartps.h"
#include "xscugic.h"
#include "xil_cache.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "sleep.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "lwip/tcp_impl.h"
#include "tcp_transmission.h"
#include "gpiopl_intr.h"


#define UART_DEVICE_ID		XPAR_XUARTPS_0_DEVICE_ID
#define UART_INT_VEC_ID		XPAR_XUARTPS_0_INTR

#define UART_START_CMD       		0xAA55EEA0
#define UART_STOP_CMD         		0xAA55EEB1
#define UART_RESET_CMD        		0xAA55EEC1
#define UART_SUCCESS         		0xAA55BBA0
#define UART_FAILURE        		0xAA55CCA0

volatile int TotalReceivedCount;
volatile int TotalSentCount;
volatile int Uart_TotalErrorCount;

XUartPs UartPs;		/* Instance of the UART Device */

u8 Uart_RecvBuffer[4];
u32 Uart_Recvdata;
u32 received_num;
u32 receive_index;

u32 uart_return_info;

int UartPs_init(XUartPs *UartInstPtr, u16 DeviceId);
void UartPs_Setup_Intr_System(XScuGic *GicInstancePtr, XUartPs *UartInstPtr, u16 UartIntrId);
void Uart_Handler(void *CallBackRef, u32 Event, unsigned int EventData);
void UartPs_recv(XUartPs *UartInstPtr, u32 NumBytes);


#endif /* UARTPS_INTR_H_ */
