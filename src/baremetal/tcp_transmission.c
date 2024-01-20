/*
 * tcp_transmission.c
 *
 *  Created on: 2016Äê9ÔÂ7ÈÕ
 *      Author: Michael Wang
 */


#include <stdio.h>
#include <string.h>
#include "xgpio.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "lwip/tcp_impl.h"
#include "xaxidma.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "sleep.h"
#include "tcp_transmission.h"
#include "gpiopl_intr.h"



void send_received_data()
{
	err_t err;
	int Status;
	struct tcp_pcb *tpcb = connected_pcb;

	/*initial the first axidma transmission, only excuse once*/
	if(!first_trans_start)
	{
		//
		Status = DMA_ReceivePacket();
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi dma failed! 0 %d\r\n", Status);
			return;
		}
		/*set the flag, so this part of code will not excuse again*/
		first_trans_start = 1;
		//xil_printf("first trans\r\n");
	}

	/*if the last axidma transmission is done, the interrupt triggered, then start TCP transmission*/
	if(packet_trans_done )//&& tcp_trans_done
	{

		if (!connected_pcb)
			return;

		/* if tcp send buffer has enough space to hold the data we want to transmit from PL, then start tcp transmission*/
		if (tcp_sndbuf(tpcb) > TCP_PACKET_SIZE )//&& tcp_sndqueuelen(tpcb) == 0
		{
			//tcp_trans_done = 0;
			header_p = (packet_header *)RxBufferPtr[packet_index & 1];
			header_p->ID0 = HEADER_ID0;
			header_p->ID1 = HEADER_ID1;
			header_p->frame_cnt = packet_index;
			header_p->length = ADC_PACKET_LENGTH;
			Xil_DCacheInvalidateRange((u32)RxBufferPtr[packet_index & 1] + HEADER_SIZE, ADC_PACKET_LENGTH);
			/*transmit received data through TCP*/
			err = tcp_write(tpcb, RxBufferPtr[packet_index & 1], TCP_PACKET_SIZE, TCP_WRITE_FLAG_COPY & (~TCP_WRITE_FLAG_MORE));
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_write: %d\r\n", err);
				connected_pcb = NULL;
				return;
			}
			err = tcp_output(tpcb);
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_output: %d\r\n",err);
				return;
			}

			packet_index++;
			/*clear the axidma done flag*/
			packet_trans_done = 0;

			/*initial the other axidma transmission when the current transmission is done*/
			Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[packet_index & 1] + HEADER_SIZE,
						(u32)(ADC_PACKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
			if (Status != XST_SUCCESS)
			{
				xil_printf("axi dma %d failed! %d \r\n", packet_index, Status);
				return;
			}

		}
	}

	/*initial the first axidma transmission, only excuse once*/
//	if(!first_trans_start)
//	{
//		Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[0] + HEADER_SIZE,
//				(u32)(ADC_PACKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
//		if (Status != XST_SUCCESS)
//		{
//			xil_printf("axi dma failed! 0 %d\r\n", Status);
//			return;
//		}
//		/*set the flag, so this part of code will not excuse again*/
//		first_trans_start = 1;
//		xil_printf("first trans\r\n");
//	}


}

/*this fuction is called when a complete ADC packet is sent done*/
static err_t
tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	err_t err;
	tcp_trans_done = 1;
	err = tcp_output(tpcb);
	if (err != ERR_OK) {
		xil_printf("txperf: Error on tcp_output: %d\r\n",err);
		return -1;
	}

	return ERR_OK;
}

/*This fuction is called when a tcp packet is received*/
err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	//err_t error;
	struct pbuf *q;
	u32 remain_length;

	q = p;
	/* close socket if the peer has sent the FIN packet  */
    if (p == NULL) {
        tcp_close(tpcb);
        xil_printf("tcp connection closed\r\n");
        return ERR_OK;
    }

	/*if received ip fragment packets*/
	if(q->tot_len > q->len)
	{
		remain_length = q->tot_len;
		while(remain_length > 0)
		{
			memcpy(tcp_rx_buffer + file_length, q->payload, q->len);

			file_length += q->len;
			remain_length -= q->len;
			/*go to next pbuf pointer*/
			q = q->next;
		}
	}
	/*if received no ip fragment packets*/
	else
	{
		memcpy(tcp_rx_buffer, q->payload, q->len);
	}

	/*change the endian of received command*/
	*tcp_rx_buffer = ntohl(*tcp_rx_buffer);

	if(*tcp_rx_buffer == TCP_START_CMD)
	{
		if(tcp_trans_start)
			xil_printf("TCP: already start!\r\n");
		else
		{
			tcp_trans_reset = 0;
			XGpio_DiscreteWrite(&Gpio, 1, 1);
			tcp_trans_start = 1;
			xil_printf("TCP: start!\r\n");
		}
	}
	else if(*tcp_rx_buffer == TCP_STOP_CMD)
	{
		if(!tcp_trans_start)
			xil_printf("TCP: already stop!\r\n");
		else
		{
			tcp_trans_reset = 0;
			XGpio_DiscreteWrite(&Gpio, 1, 0);
			tcp_trans_start = 0;
			xil_printf("TCP: stop!\r\n");
		}
	}
	else if(*tcp_rx_buffer == TCP_RESET_CMD)
	{
		if(tcp_trans_reset)
			xil_printf("TCP: already reset!\r\n");
		else
		{
			tcp_trans_reset = 1;
			XGpio_DiscreteWrite(&Gpio, 1, 0);
			tcp_trans_start = 0;
			xil_printf("TCP: reset!\r\n");
		}
	}
	else if(*tcp_rx_buffer == TCP_DACDATA_CMD)
	{
		if(tcp_trans_reset)
			xil_printf("TCP: already reset!\r\n");
		else
		{
			xil_printf("TCP: ready to received DAC data\r\n");
		}
	}
	else
	{
		xil_printf("TCP: unsupported TCP command!\r\n");
		//Here the data copy to BD buffer address
	}



	//xil_printf("tcp data come in!%d, %d, %08x\r\n", p->tot_len, p->len, *file);

	/* tell lwip we've received the tcp packet */
	tcp_recved(tpcb, p->tot_len);
	pbuf_free(p);

    return ERR_OK;
}

err_t
tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	xil_printf("txperf: Connected to iperf server\r\n");

	/* store state */
	connected_pcb = tpcb;

	/* set callback values & functions */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, tcp_sent_callback);
	tcp_recv(tpcb, tcp_recv_callback);

	/* disable nagle algorithm to ensure
	 * the last small segment of a ADC packet will be sent out immediately
	 * with no delay
	 * */
	tcp_nagle_disable(tpcb);

	if(!tcp_nagle_disabled(tpcb))
		xil_printf("tcp nagle disable failed!\r\n");

	tcp_client_connected = 1;

	/* initiate data transfer */
	return ERR_OK;
}


int tcp_send_init()
{

	err_t err;

	tcp_rx_buffer = (u32 *)TCP_RXBUFFER_BASE_ADDR;


	/* create new TCP PCB structure */
	request_pcb = tcp_new();
	if (!request_pcb) {
		xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
		return -1;
	}

	/* connect to tcp server */
	IP4_ADDR(&ipaddress,  192, 168,   2, 26);		/* tcp server address */

	port = PC_TCP_SERVER_PORT;					/* tcp server port */

    tcp_client_connected = 0;
    first_trans_start = 0;
    packet_trans_done = 0;
    packet_index = 0;
    tcp_trans_start = 0;
    tcp_trans_reset = 0;
    tcp_trans_done = 1;
    file_length = 0;

    connected_pcb = NULL;

    //ip_set_option(request_pcb, SOF_REUSEADDR);

	err = tcp_connect(request_pcb, &ipaddress, port, tcp_connected_callback);
	if (err != ERR_OK) {
		xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
		return err;
	}

	return 0;
}



