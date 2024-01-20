/*
 * tcp_transmission.h
 *
 *  Created on: 2017年3月13日
 *      Author: 201607062058
 */

#ifndef TCP_TRANSMISSION_H_
#define TCP_TRANSMISSION_H_

#include <stdio.h>
#include "xil_types.h"
#include "Xscugic.h"
#include "Xil_exception.h"
#include "DMA_support.h"


#define TCP_START_CMD       		0xAA55FFA0
#define TCP_STOP_CMD         		0xAA55FFB1
#define TCP_RESET_CMD        		0xAA55FFC1
#define TCP_DACDATA_CMD				0xAA55FFD1

//TODO: putty设置本机端口为5001
#define PC_TCP_SERVER_PORT     		5001

#define HEADER_ID0      			0xAA55AA55
#define HEADER_ID1      			0xAA55AA55

#define HEADER_SIZE             (16)
#define ADC_PACKET_LENGTH       (16 * 1023)
#define TCP_PACKET_SIZE         (ADC_PACKET_LENGTH + HEADER_SIZE)


volatile int tcp_trans_start;
volatile int tcp_trans_reset;
unsigned first_trans_start;
volatile u32 packet_index;
volatile unsigned tcp_client_connected;

struct tcp_pcb *connected_pcb;
struct tcp_pcb *request_pcb;
volatile int tcp_trans_done;
volatile u32 file_length;

struct ip_addr ipaddress;
u16_t port;

//
u32 *tcp_rx_buffer = TxPacket;

typedef struct packet_header
{
	u32 ID0;
	u32 ID1;
	u32 frame_cnt;
	u32 length;

}packet_header;

packet_header *header_p;

int tcp_send_init();
void send_received_data();
err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);


#endif /* TCP_TRANSMISSION_H_ */
