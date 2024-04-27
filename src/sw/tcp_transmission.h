/*
 * tcp_transmission.h
 *
 *  Created on: 2017Äê3ÔÂ13ÈÕ
 *      Author: 201607062058
 */

#ifndef TCP_TRANSMISSION_H_
#define TCP_TRANSMISSION_H_

#include <stdio.h>
#include <string.h>
#include "xil_types.h"
#include "DMA_support.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "sleep.h"
#include "timer_intr.h"
#include "netif/xadapter.h"
#include "DPD.h"
#include "sys_opt.h"





#define PC_TCP_SERVER_PORT     		5001

#define ADC_PACKET_LENGTH       (16 * 1023)
#define TCP_PACKET_SIZE       	ADC_PACKET_LENGTH

enum TCP_CMD
{
	TCP_START_CMD = 1, TCP_STOP_CMD, TCP_RESET_CMD, TCP_DACDATA_CMD, TCP_MP_CMD, TCP_PRVTDNN_CMD,
};

enum BUFFER_TYPE
{
	CMD_BUFFER = 1, DA_BUFFER, MP_BUFFER, PRVTDNN_BUFFER,
};

extern volatile unsigned tcp_client_connected;

extern struct tcp_pcb *connected_pcb;
extern struct tcp_pcb *request_pcb;

extern ip_addr_t ipaddress;
extern u16_t port;

extern u32 *tcp_rx_buffer;
extern volatile enum TCP_CMD enum_recv_cmd;
extern volatile enum BUFFER_TYPE enum_recv_buffer;



int lwip_sys_init();
int tcp_send_init();
void send_received_data();
err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
void tcp_client_close(struct tcp_pcb *pcb);
void tcp_client_err(void *arg, err_t err);

#endif /* TCP_TRANSMISSION_H_ */
