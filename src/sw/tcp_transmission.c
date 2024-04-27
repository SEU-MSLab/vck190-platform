/*
 * tcp_transmission.c
 *
 *  Created on: 2016年9月7日
 *      Author: Michael Wang
 */



#include "tcp_transmission.h"



volatile unsigned tcp_client_connected;
volatile u32 buffer_index;
volatile u32 total_length;

volatile u32 DAC_length;
volatile u32 ADC_length;
volatile u32 DPD_length;
volatile u32 ADC_sent_length;

volatile enum TCP_CMD enum_recv_cmd = TCP_STOP_CMD;
volatile enum BUFFER_TYPE enum_recv_buffer = CMD_BUFFER;

struct tcp_pcb *connected_pcb;
struct tcp_pcb *request_pcb;

ip_addr_t ipaddress;
u16_t port;

u32 *tcp_rx_buffer ;
u32 *tcp_cmd_buffer;
u32 *tcp_dpd_buffer;

// This function only use to send ADC data
void send_received_data()
{
	err_t err = 0;
	int Status;
	struct tcp_pcb *tpcb = connected_pcb;
	u32 once_length;
	int tcp_flag = TCP_WRITE_FLAG_COPY & (~TCP_WRITE_FLAG_MORE);

	if (!connected_pcb)
		return;

	/* if tcp send buffer has enough space to hold the data we want to transmit from PL, then start tcp transmission*/
	if (tcp_sndbuf(tpcb) > ADC_length )//&& tcp_sndqueuelen(tpcb) == 0
	{
		/*transmit received data through TCP*/
		err = tcp_write(tpcb, (u8 *)RxBufferPtr + ADC_sent_length, ADC_length, tcp_flag);
		if (err != ERR_OK) {
			xil_printf("Error on tcp_write: %d\r\n", err);
			connected_pcb = NULL;
			return;
		}
		err = tcp_output(tpcb);
		if (err != ERR_OK) {
			xil_printf("Error on tcp_output: %d\r\n",err);
			return;
		}
		enum_recv_cmd = TCP_STOP_CMD;
		ADC_sent_length = 0;
		xil_printf("ADC data transfer finished! \r\n");

		/*initial the other axidma transmission when the current transmission is done*/
		Status = DMA_ReceivePacket();
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi dma failed! %d \r\n", Status);
			return;
		}

	}
	else
	{
//		Xil_DCacheInvalidateRange((UINTPTR)RxBufferPtr, ADC_length);
		once_length = tcp_sndbuf(tpcb);
		err += tcp_write(tpcb, (u8 *)RxBufferPtr + ADC_sent_length, once_length, tcp_flag);
		ADC_length -= once_length;
		ADC_sent_length += once_length;
		err += tcp_output(tpcb);
		if (err != ERR_OK) {
			xil_printf("Error on tcp_wirte or tcp_output: %d\r\n",err);
			return;
		}
		xil_printf("The remaining length is %d \r\n", ADC_length);

	}



}

/*this fuction is called when a complete ADC packet is sent done*/
static err_t
tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	err_t err;
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
	struct pbuf *q;
	q = p;
	u32 remain_length;
	u32 *tcp_recv_buffer;

	/* close socket if the peer has sent the FIN packet  */
    if (p == NULL) {
        tcp_close(tpcb);
        xil_printf("tcp connection closed\r\n");
        return ERR_OK;
    }

    // switch the buffer to specific address
    switch(enum_recv_buffer){
        case DA_BUFFER  :
        	tcp_recv_buffer = tcp_rx_buffer;
        	break;
        case MP_BUFFER :
        case PRVTDNN_BUFFER:
        	tcp_recv_buffer = tcp_dpd_buffer;
        	break;
        default :
        	tcp_recv_buffer = tcp_cmd_buffer;
    }

	/*if received ip fragment packets*/
	if(q->tot_len > q->len)
	{
		remain_length = q->tot_len;
		while(remain_length > 0)
		{
			memcpy((u8 *)tcp_recv_buffer + buffer_index, q->payload, q->len);

			buffer_index += q->len;
			remain_length -= q->len;
			/*go to next pbuf pointer*/
			q = q->next;
		}
	}
	/*if received no ip fragment packets*/
	else
	{
		// For command, command tot_len always equal to len
		// For DAC data, the last pbuf's tot_len always equal to len
		memcpy((u8 *)(tcp_recv_buffer) + buffer_index, q->payload, q->len);
		buffer_index += q->len;
	}

	// Don't need to change endian
//	*tcp_rx_buffer = ntohl(*tcp_rx_buffer);


	if(enum_recv_cmd == TCP_DACDATA_CMD || enum_recv_cmd == TCP_MP_CMD || enum_recv_cmd == TCP_PRVTDNN_CMD)
	{
		total_length -= q->tot_len;
		if(total_length == 0)
		{
			xil_printf("The length is equal to command, Transfer complete!.\r\n");
			buffer_index = 0;
			if(enum_recv_cmd == TCP_MP_CMD) Write_MP_coeff();
			else if(enum_recv_cmd == TCP_PRVTDNN_CMD) Write_PRVTDNN_coeff(DPD_length);
			Xil_DCacheFlush();
//			Xil_DCacheFlushRange((UINTPTR)tcp_recv_buffer, TX_BD_SPACE_HIGH - TX_BD_SPACE_BASE + 1);
			enum_recv_cmd = TCP_STOP_CMD;
		}
		else
		{
			xil_printf("The remaining length is %d \r\n", total_length);
		}
	}
	else
	{
		buffer_index = 0;
		enum_recv_cmd = *tcp_recv_buffer;
		xil_printf("TCP: received command is %04x \r\n", *tcp_recv_buffer);
		switch(enum_recv_cmd){
		case TCP_START_CMD :
			ADC_length = *(tcp_recv_buffer + 1);
			xil_printf("TCP: start Transmitting ADC data， length is %d \r\n", ADC_length);
			break;
		case TCP_STOP_CMD :
			// nothing to do now
			xil_printf("TCP: STOP!\r\n");
			break;
		case TCP_RESET_CMD:
			// nothing to do now
			xil_printf("TCP: reset!\r\n");
			break;
		case TCP_DACDATA_CMD:
			DAC_length = *(tcp_recv_buffer + 1);
			total_length = DAC_length;
			enum_recv_buffer = DA_BUFFER;
			xil_printf("TCP: ready to received DAC data, length is %d\r\n", DAC_length);
			break;
		case TCP_MP_CMD :
			DPD_length = *(tcp_recv_buffer + 1);
        	total_length = DPD_length;
			enum_recv_buffer = MP_BUFFER;
			xil_printf("TCP: ready to received MP data, length is %d\r\n", DPD_length);
			tcp_dpd_buffer = MPPacket;
			break;
		case TCP_PRVTDNN_CMD:
			DPD_length = *(tcp_recv_buffer + 1);
			total_length = DPD_length;
			enum_recv_buffer = PRVTDNN_BUFFER;
			xil_printf("TCP: ready to received PRVTDNN data, length is %d\r\n", DPD_length);
			tcp_dpd_buffer = PRVTDNNPacket;
			break;
		default :
			xil_printf("TCP: unsupported TCP command!\r\n");
		}
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
	if(err != ERR_OK) {
		tcp_client_close(tpcb);
		xil_printf("Connection error \n\r");
		return err;
	}

	enum_recv_cmd = TCP_STOP_CMD;
	enum_recv_buffer = CMD_BUFFER;
	buffer_index = 0;



	xil_printf("Connected to MATLAB server\r\n");

	/* store state */
	connected_pcb = tpcb;

	/* set callback values & functions */
	tcp_arg(connected_pcb, NULL);
	tcp_sent(connected_pcb, tcp_sent_callback);
	tcp_recv(connected_pcb, tcp_recv_callback);
	tcp_err(connected_pcb, tcp_client_err);

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

	tcp_cmd_buffer = CMDPacket;
	tcp_rx_buffer = TxPacket;
	// TODO: DPDPacket设置为命令控制
	xil_printf("CMD data address start from 0x%08x \r\n", tcp_cmd_buffer);
	xil_printf("DAC data address start from 0x%08x \r\n", tcp_rx_buffer);
	xil_printf("ADC data address start from 0x%08x \r\n", RxBufferPtr);
	xil_printf("MP data address start from 0x%08x \r\n", MPPacket);
	xil_printf("PRVTDNN data address start from 0x%08x \r\n", PRVTDNNPacket);


	/* create new TCP PCB structure */
	request_pcb = tcp_new();
	if (!request_pcb) {
		xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
		return -1;
	}

	/* connect to tcp server */
#ifdef TESTBENCH
	IP4_ADDR(&ipaddress,  192, 168, 1, 2);
#else
	IP4_ADDR(&ipaddress,  192, 168, 3, 3);		/* tcp server address */
#endif

	port = PC_TCP_SERVER_PORT;					/* tcp server port */

    tcp_client_connected = 0;


    connected_pcb = NULL;

    //ip_set_option(request_pcb, SOF_REUSEADDR);

	err = tcp_connect(request_pcb, &ipaddress, port, tcp_connected_callback);
	if (err != ERR_OK) {
		xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
		return err;
	}

	return 0;
}

int lwip_sys_init(struct netif *netif)
{
	ip_addr_t ipaddr, netmask, gw;

	unsigned char mac_ethernet_address[] = {0x00, 0x0a, 0x35, 0x07, 0x70, 0x37};

#ifdef TESTBENCH
	IP4_ADDR(&ipaddr, 	192,168,1,12);
	IP4_ADDR(&gw,		192,168,1,1);
#else
	IP4_ADDR(&ipaddr, 	192,168,3,10);
	IP4_ADDR(&gw,		192,168,3,1);
#endif
	IP4_ADDR(&netmask, 	255,255,255,0);

	lwip_init();

	// Use GEM0
	if(!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, XPAR_XEMACPS_0_BASEADDR)) {
		xil_printf("ERROR adding N/W interface\r\n");
		return XST_FAILURE;
	}

	netif_set_default(netif);
	Timer_start();
	netif_set_up(netif);
	tcp_send_init();

	return XST_SUCCESS;

}

/** Close a tcp session */
void tcp_client_close(struct tcp_pcb *pcb)
{
	err_t err;

	if (pcb != NULL) {
		tcp_sent(pcb, NULL);
		tcp_err(pcb, NULL);
		err = tcp_close(pcb);
		if (err != ERR_OK) {
			/* Free memory with abort */
			tcp_abort(pcb);
		}
	}
}

/** Error callback, tcp session aborted */
void tcp_client_err(void *arg, err_t err)
{
	LWIP_UNUSED_ARG(err);

	tcp_client_close(connected_pcb);
	connected_pcb = NULL;
	xil_printf("TCP connection aborted\n\r");
}
