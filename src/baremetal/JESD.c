#include "JESD.h"
#include "sleep.h"

XGpio PLL_LOCKED;

void JESD_init(){
	int reset0,reset1,reset2;
	// %02x means print in hex，02 means if not 2 bits，add 0 in front；
	xil_printf("JESD0 Version is 0x%08x\n",JESD_read(JESD_VERSION,0));
	xil_printf("JESD1 Version is 0x%08x\n",JESD_read(JESD_VERSION,1));
	xil_printf("JESD2 Version is 0x%08x\n",JESD_read(JESD_VERSION,2));
	xil_printf("==========JESD Init========\n");
	xil_printf("Link0 ILA Multiframes is 0x%02x\n",JESD_read(0x03c,0)>>24); // only show MSB 8 bits
	xil_printf("Link1 ILA Multiframes is 0x%02x\n",JESD_read(0x03c,1)>>24);
	xil_printf("Link2 RX Lanes enbale is 0x%02x\n",JESD_read(0x040,2));

	// enable Lane
	JESD_write(0x040, 0, 0x0F);
	JESD_write(0x040, 1, 0x0F);
	JESD_write(0x040, 2, 0xFF);

	//configure lanes parameters
	// TX Link 0
	JESD_write(JESD_8B10BCFG, 0, CFG_8B10BTX);
	JESD_write(JESD_TXILA1, 0, CFG_TXILA1);
	JESD_write(JESD_TXILA2, 0, CFG_TXILA2);

	// TX Link 1
	JESD_write(JESD_8B10BCFG, 1, CFG_8B10BTX);
	JESD_write(JESD_TXILA1, 1, CFG_TXILA1);
	JESD_write(JESD_TXILA2, 1, CFG_TXILA2);

	// RX Link
	JESD_write(JESD_8B10BCFG, 2, CFG_8B10BRX);


	// configure transmit lanes ID  multiple IP cores should be configuration
	JESD_write(Lane0_ID_REG,0,0x00030000);
	JESD_write(Lane1_ID_REG,0,0x00030001);
	JESD_write(Lane2_ID_REG,0,0x00030002);
	JESD_write(Lane3_ID_REG,0,0x00030003);
	JESD_write(Lane0_ID_REG,1,0x00030004);
	JESD_write(Lane1_ID_REG,1,0x00030005);
	JESD_write(Lane2_ID_REG,1,0x00030006);
	JESD_write(Lane3_ID_REG,1,0x00030007);

	//reset all links include transmit and receiver
	JESD_write(JESD_RESET,0,0x00000001);
	JESD_write(JESD_RESET,1,0x00000001);
	JESD_write(JESD_RESET,2,0x00000001);// receive core
	usleep(1000);

	JESD_write(JESD_RESET,0,0x00000000);
	JESD_write(JESD_RESET,1,0x00000000);
	JESD_write(JESD_RESET,2,0x00000000);
	usleep(1000);
	//detect reset is done?
	while(1){
		reset0 = JESD_read(JESD_RESET,0);
		reset1 = JESD_read(JESD_RESET,1);
		reset2 = JESD_read(JESD_RESET,2);
		reset0 &= 0x1;// Only get bit 0
		reset1 &= 0x1;
		reset2 &= 0x1;
		if(!reset0&&!reset1&&!reset2)
			break;
	}
	xil_printf("JESD Transmitter Link Init and All Links reset done!\n");
	xil_printf("==========end==========\n");
}


// Versal中不再能用axi4-lite来判断PLL是否锁定，转而用AXI GPIO
void JESD_phy_clocked(){
	u32 PLL_Status;
	int Status;
	Status = XGpio_Initialize(&PLL_LOCKED, XPAR_PLLLOCK_GPIO_DEVICE_ID);
	if(Status != XST_SUCCESS)
		xil_printf("Phy locked GPIO not initiated!");

	PLL_Status = (u8)XGpio_DiscreteRead(&PLL_LOCKED,1);
	xil_printf("PLL Status is 0x%08x\n",PLL_Status);
	if((PLL_Status&0x01) == 1)
		xil_printf("all the CPLLs are locked\n");
	else
		xil_printf("all the CPLLS are not locked\n");
}

void JESD_TX_SYNC(){
	u32 SYNC_Status0,SYNC_Status1;
	SYNC_Status0 = JESD_read(JESD_STATUS,0);
	if(SYNC_Status0 & STATUS_SYSREF)
		xil_printf("Link0 SYSREF event has been captured\n");
	else
		xil_printf("Link0 SYSREF event has NOT been captured\n");
	if(SYNC_Status0 & STATUS_SYNC)
		xil_printf("Link0 SYNC achieved\n");
	else
		xil_printf("Link0 SYNC NOT achieved\n");

	SYNC_Status1 = JESD_read(JESD_STATUS,1);
	if(SYNC_Status1 & STATUS_SYSREF)
		xil_printf("Link1 SYSREF event has been captured\n");
	else
		xil_printf("Link1 SYSREF event has NOT been captured\n");
	if(SYNC_Status1 & STATUS_SYNC)
		xil_printf("Link1 SYNC achieved\n");
	else
		xil_printf("Link1 SYNC NOT achieved\n");
}

void DA_check_JESD(){
	xil_printf("DYN_LINK_LAT0:0x%02x\n",DA_Read(0x302));
	xil_printf("DYN_LINK_LAT1:0x%02x\n",DA_Read(0x303));

	xil_printf("\nDA Link 0 ERROR MONITORING\n");
	DA_Write(0x300,0x0B);//enable double Link (need or not) link0
	xil_printf("Bad DisParity:0x%02x\n",DA_Read(0x46D));
	xil_printf("Not in Table:0x%02x\n",DA_Read(0x46E));
	xil_printf("Unexpected K character:0x%02x\n",DA_Read(0x46F));
	xil_printf("Code CGS Status:0x%02x\n",DA_Read(0x470));
	xil_printf("Frame SYNC:0x%02x\n",DA_Read(0x471));
	xil_printf("Good CheckSum:0x%02x\n",DA_Read(0x472));
	xil_printf("Init Lane SYNC:0x%02x\n",DA_Read(0x473));

	xil_printf("\nDA Link 1 ERROR MONITORING\n");
	DA_Write(0x300,0x0F);//enable double Link (need or not) link1
	xil_printf("Bad DisParity:0x%02x\n",DA_Read(0x46D));
	xil_printf("Not in Table:0x%02x\n",DA_Read(0x46E));
	xil_printf("Unexpected K character:0x%02x\n",DA_Read(0x46F));
	xil_printf("Code CGS Status:0x%02x\n",DA_Read(0x470));
	xil_printf("Frame SYNC:0x%02x\n",DA_Read(0x471));
	xil_printf("Good CheckSum:0x%02x\n",DA_Read(0x472));
	xil_printf("Init Lane SYNC:0x%02x\n",DA_Read(0x473));

	xil_printf("\r\n====PRINT DA CHECKSUM for LINK0====\r\n");
	DA_Write(0x300,0x0B);
	xil_printf("CHECKSUM_REG0 is %2x\r\n",DA_Read(0x40D));
	xil_printf("CHECKSUM_REG1 is %2x\r\n",DA_Read(0x415));
	xil_printf("CHECKSUM_REG2 is %2x\r\n",DA_Read(0x41D));
	xil_printf("CHECKSUM_REG3 is %2x\r\n",DA_Read(0x425));
	xil_printf("CHECKSUM_REG4 is %2x\r\n",DA_Read(0x42D));
	xil_printf("CHECKSUM_REG5 is %2x\r\n",DA_Read(0x435));
	xil_printf("CHECKSUM_REG6 is %2x\r\n",DA_Read(0x43D));
	xil_printf("CHECKSUM_REG7 is %2x\r\n",DA_Read(0x445));
	xil_printf("CP_CHECKSUM_REG0 is %2x\r\n",DA_Read(0x40D+1));
	xil_printf("CP_CHECKSUM_REG1 is %2x\r\n",DA_Read(0x415+1));
	xil_printf("CP_CHECKSUM_REG2 is %2x\r\n",DA_Read(0x41D+1));
	xil_printf("CP_CHECKSUM_REG3 is %2x\r\n",DA_Read(0x425+1));
	xil_printf("CP_CHECKSUM_REG4 is %2x\r\n",DA_Read(0x42D+1));
	xil_printf("CP_CHECKSUM_REG5 is %2x\r\n",DA_Read(0x435+1));
	xil_printf("CP_CHECKSUM_REG6 is %2x\r\n",DA_Read(0x43D+1));
	xil_printf("CP_CHECKSUM_REG7 is %2x\r\n",DA_Read(0x445+1));
	xil_printf("LID0_REG is %2x\r\n",DA_Read(0x402));
	xil_printf("LID1_REG is %2x\r\n",DA_Read(0x412));
	xil_printf("LID2_REG is %2x\r\n",DA_Read(0x41A));
	xil_printf("LID3_REG is %2x\r\n",DA_Read(0x422));
	xil_printf("LID4_REG is %2x\r\n",DA_Read(0x42A));
	xil_printf("LID5_REG is %2x\r\n",DA_Read(0x432));
	xil_printf("LID6_REG is %2x\r\n",DA_Read(0x43A));
	xil_printf("LID7_REG is %2x\r\n",DA_Read(0x442));
	xil_printf("Link information received on Lanes\r\n");
	xil_printf("DID_REG is %2x\r\n",DA_Read(0x400));
	xil_printf("BID_REG is %2x\r\n",DA_Read(0x401));
	xil_printf("SCR_L_REG is %2x\r\n",DA_Read(0x403));
	xil_printf("F_REG is %2x\r\n",DA_Read(0x404));
	xil_printf("K_REG is %2x\r\n",DA_Read(0x405));
	xil_printf("M_REG is %2x\r\n",DA_Read(0x406));
	xil_printf("CS_N_REG is %2x\r\n",DA_Read(0x407));
	xil_printf("NP_REG is %2x\r\n",DA_Read(0x408));
	xil_printf("S_REG is %2x\r\n",DA_Read(0x409));
	xil_printf("HD_CF_REG is %2x\r\n",DA_Read(0x40A));
	xil_printf("RES1_REG is %2x\r\n",DA_Read(0x40B));
	xil_printf("RES2_REG is %2x\r\n",DA_Read(0x40C));

	xil_printf("\r\n====PRINT DA CHECKSUM for LINK1====\r\n");
	DA_Write(0x300,0x0F);
	xil_printf("CHECKSUM_REG0 is %2x\r\n",DA_Read(0x40D));
	xil_printf("CHECKSUM_REG1 is %2x\r\n",DA_Read(0x415));
	xil_printf("CHECKSUM_REG2 is %2x\r\n",DA_Read(0x41D));
	xil_printf("CHECKSUM_REG3 is %2x\r\n",DA_Read(0x425));
	xil_printf("CHECKSUM_REG4 is %2x\r\n",DA_Read(0x42D));
	xil_printf("CHECKSUM_REG5 is %2x\r\n",DA_Read(0x435));
	xil_printf("CHECKSUM_REG6 is %2x\r\n",DA_Read(0x43D));
	xil_printf("CHECKSUM_REG7 is %2x\r\n",DA_Read(0x445));
	xil_printf("CP_CHECKSUM_REG0 is %2x\r\n",DA_Read(0x40D+1));
	xil_printf("CP_CHECKSUM_REG1 is %2x\r\n",DA_Read(0x415+1));
	xil_printf("CP_CHECKSUM_REG2 is %2x\r\n",DA_Read(0x41D+1));
	xil_printf("CP_CHECKSUM_REG3 is %2x\r\n",DA_Read(0x425+1));
	xil_printf("CP_CHECKSUM_REG4 is %2x\r\n",DA_Read(0x42D+1));
	xil_printf("CP_CHECKSUM_REG5 is %2x\r\n",DA_Read(0x435+1));
	xil_printf("CP_CHECKSUM_REG6 is %2x\r\n",DA_Read(0x43D+1));
	xil_printf("CP_CHECKSUM_REG7 is %2x\r\n",DA_Read(0x445+1));
	xil_printf("LID0_REG is %2x\r\n",DA_Read(0x402));
	xil_printf("LID1_REG is %2x\r\n",DA_Read(0x412));
	xil_printf("LID2_REG is %2x\r\n",DA_Read(0x41A));
	xil_printf("LID3_REG is %2x\r\n",DA_Read(0x422));
	xil_printf("LID4_REG is %2x\r\n",DA_Read(0x42A));
	xil_printf("LID5_REG is %2x\r\n",DA_Read(0x432));
	xil_printf("LID6_REG is %2x\r\n",DA_Read(0x43A));
	xil_printf("LID7_REG is %2x\r\n",DA_Read(0x442));
	xil_printf("Link information received on Lanes\r\n");
	xil_printf("DID_REG is %2x\r\n",DA_Read(0x400));
	xil_printf("BID_REG is %2x\r\n",DA_Read(0x401));
	xil_printf("SCR_L_REG is %2x\r\n",DA_Read(0x403));
	xil_printf("F_REG is %2x\r\n",DA_Read(0x404));
	xil_printf("K_REG is %2x\r\n",DA_Read(0x405));
	xil_printf("M_REG is %2x\r\n",DA_Read(0x406));
	xil_printf("CS_N_REG is %2x\r\n",DA_Read(0x407));
	xil_printf("NP_REG is %2x\r\n",DA_Read(0x408));
	xil_printf("S_REG is %2x\r\n",DA_Read(0x409));
	xil_printf("HD_CF_REG is %2x\r\n",DA_Read(0x40A));
	xil_printf("RES1_REG is %2x\r\n",DA_Read(0x40B));
	xil_printf("RES2_REG is %2x\r\n",DA_Read(0x40C));
}
/*
 *@ILA config Data3
 *28:24 L(lanes per Link)    -1
 *20:16 LID(Lane ID)  important to map data
 *11:8  BID(Bank ID)
 *7:0   DID(Device ID)
 *
 *@SYNC Status
 *1 SYSREF captured
 *12 SYNC captured
 *
 *@Debug Status
 *bit3 = 1    start of data detect
 *bit2 = 1    state of ILA detect
 *bit1 = 1    lane has Code Group Sync
 *bit0 = 1    lane is currently receiving K28'5s
 */
void AD_check_JESD(){
	u32 SyncStatus;
	SyncStatus = JESD_read(JESD_STATUS,2);
	xil_printf("\nRX Link SyncStatus(0x060):%08x\n",SyncStatus);

	xil_printf("Recieve Err Status(0x058) is %08x\n", JESD_read(JESD_RXERR, 2));
	xil_printf("LANE0 ILA Config Data 0(0x430) is %08x\r\n", JESD_read(0x400+0x30, 2));
	xil_printf("LANE0 ILA Config Data 1(0x434) is %08x\r\n", JESD_read(0x400+0x34, 2));
	xil_printf("LANE0 ILA Config Data 2(0x438) is %08x\r\n", JESD_read(0x400+0x38, 2));
	xil_printf("LANE0 ILA Config Data 3(0x43c) is %08x\r\n", JESD_read(0x400+0x3c, 2));
	xil_printf("LANE1 ILA Config Data 3(0x4Bc) is %08x\r\n", JESD_read(0x480+0x3c, 2));
	xil_printf("LANE2 ILA Config Data 3(0x53c) is %08x\r\n", JESD_read(0x500+0x3c, 2));
	xil_printf("LANE3 ILA Config Data 3(0x5Bc) is %08x\r\n", JESD_read(0x580+0x3c, 2));
	xil_printf("LANE4 ILA Config Data 3(0x63c) is %08x\r\n", JESD_read(0x600+0x3c, 2));
	xil_printf("LANE5 ILA Config Data 3(0x6Bc) is %08x\r\n", JESD_read(0x680+0x3c, 2));
	xil_printf("LANE6 ILA Config Data 3(0x73c) is %08x\r\n", JESD_read(0x700+0x3c, 2));
	xil_printf("LANE7 ILA Config Data 3(0x7Bc) is %08x\r\n", JESD_read(0x780+0x3c, 2));
	xil_printf("LANE0 ILA Config Data 4(0x440) is %08x\r\n", JESD_read(0x400+0x40, 2));
	xil_printf("LANE0 ILA Config Data 5(0x444) is %08x\r\n", JESD_read(0x400+0x44, 2));
	xil_printf("JESD RX Core Debug status(0x05c) is %08x\r\n",JESD_read(JESD_RXDEBUG, 2));

	xil_printf("Clock status is 0x%02x\r\n",AD_Read(0x011B)); // return 1 if detected/locked
	xil_printf("SYSREF Status 1 (hold and setup) is 0x%02x\r\n",AD_Read(0x0128));
	xil_printf("SYSREF Status 2 (SYSREF divider phase) is 0x%02x\r\n",AD_Read(0x0129));
	xil_printf("SYSREF Status 3 (SYSREF counter) is 0x%02x\r\n",AD_Read(0x012A));
}

u32 JESD_read(u32 Addr,int channel){
	u32 data;
	switch(channel){
		case JESD0:
			data = Xil_In32(Addr+JESD0_BaseAddr);
			break;
		case JESD1:
			data = Xil_In32(Addr+JESD1_BaseAddr);
			break;
		case JESD2:
			data = Xil_In32(Addr+JESD2_BaseAddr);
			break;
		default:
			xil_printf("No JESD channel find, return error!");
			break;
	}
	return data;
}

void JESD_write(u32 Addr,int channel,u32 data){
	switch(channel){
		case JESD0:
			Xil_Out32(Addr+JESD0_BaseAddr,data);
			break;
		case JESD1:
			Xil_Out32(Addr+JESD1_BaseAddr,data);
			break;
		case JESD2:
			Xil_Out32(Addr+JESD2_BaseAddr,data);
			break;
		default:
			xil_printf("No JESD channel find, return error!");
			break;
	}
}
