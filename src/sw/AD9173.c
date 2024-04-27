/*
 * AD9173.c
 *
 *  Created on: May 25, 2022
 *      Author: zhe
 */


#include"AD9173.h"



void AD9173_Init_cycle()
{
   u8 DelayMax=0,DelayMin=200;
   u8 DelayTmp;
   u8 val,del;
   int i;
   for(i=0;i<20;i++)
   {
	   printf("This is %d times power up\n\r",i);
	   AD9173_Init(0,0);
	   DelayTmp =  DA_Read(0x302) ;
	   DelayTmp &= 0x3f;
	   if((DelayTmp && 0xFE )== 0x00)
	   {
		   DelayTmp += 8;   // if delay is 0 or 1,add 8
	   }
	   if(DelayMax < DelayTmp)
		   DelayMax = DelayTmp;
	   if(DelayMin > DelayTmp)
		   DelayMin = DelayTmp;
	   printf("Max delay is %d, Min delay is %d\n\r",DelayMax,DelayMin);
   }
   val =  DelayMax-DelayMin+4;
   del = (DelayMin-2)%16;
   AD9173_Init(val,del);
}


void AD9173_Init(u8 val,u8 del)
{
	int i=10;
	u8 DArd;
	DA_Write(0x000,0x81);	   // Soft reset
	DA_Write(0x000,0x3c);	   // 4-wire mode
	DA_Write(0x090, 0x03);	   // Power down DAC0 and DAC1
	DA_Write(0x203, 0x03);	   // Power down SYNC0 and SYNC1
	DA_Write(0x091, 0x00);	   // Power up clock receiver
	DA_Write(0x206, 0x01);	   // Take PHYs out of reset.
	DA_Write(0x705, 0x01);	   // Enable boot loader.
	usleep(10000);             // wait for 10ms
    while(i>0)
    {
    	DArd = DA_Read(0x705);
    	if((DArd & 0x02) == 0x02)
    		break;
    	i--;
    }
    if(i<=0)
    {
    	print("DAC POWER UP FAILED!!!\n\r");
    	return;
    }
    else
    	print("DAC POWER UP OK\r");
    DA_Write(0x090, 0x00);// Power on DACs and bias circuitry.
	/*
	 * DAC PLL Configuration
	 */
	DA_Write(0x095, 0x00);             // Set to 0x00 to use internal DAC PLL.
	DA_Write(0x790, 0x00);	           // Use the PLL
	DA_Write(0x791, 0x00);	           // Use the PLL
	DA_Write(0x796, 0xE5);	           // DAC PLL required write.
	DA_Write(0x7A0, 0xBC);	           // DAC PLL required write.
	DA_Write(0x794, 0x08);	           // Set DAC PLL charge pump current
	DA_Write(0x797, 0x10);             // DAC PLL required write.
	DA_Write(0x797, 0x20);	           // DAC PLL required write.
	DA_Write(0x798, 0x10);	           // DAC PLL required write.
	DA_Write(0x7A2, 0x7F);	           // DAC PLL required write.
	usleep(200000);                    // wait for 100ms
	//fdac= 8*N*Fref/M/1=11796.48
    DA_Write(0x799, 0xc3);		   // CLK_OUT=/4, N=3;
	DA_Write(0x793, 0x18);		   // M=1; for Fref=491.52MHz
	DA_Write(0x094, 0x00);		   // DAC clock = PLL VCO freq
	DA_Write(0x792, 0x02);	           // Reset VCO.
	DA_Write(0x792, 0x00);
	usleep(200000);                    // wait for 100ms
	i=10;
    while(i>0)
    {
    	DArd = DA_Read(0x7B5);
    	if((DArd & 0x01) == 0x01)
    		break;
    	i--;
    }
    if(i<=0)
    {
    	print("DAC PLL LOCK FAILED!!!\n\r");
    	return;
    }
    else
    	print("DAC PLL LOCK OK\r");

    /*
     * DAC DLL Configuration
     */
    DA_Write(0x0C0, 0x00);		 // Power-up delay line.
    DA_Write(0x0DB, 0x00);		 //
    DA_Write(0x0DB, 0x01);		 // Update DLL settings to circuitry
    DA_Write(0x0DB, 0x00);
    DA_Write(0x0C1, 0x68);
    DA_Write(0x0C1, 0x69);
    DA_Write(0x0C7, 0x01);
    i = 10;
    while(i>0)
    {
    	DArd = DA_Read(0x0C3);
    	if((DArd & 0x01) == 0x01)
    		break;
    	i--;
    }
    if(i<=0)
    {
    	print("DAC DLL LOCK FAILED!!!\n\r");
    	return;
    }
    else
    	print("DAC DLL LOCK OK\r");
    /*
     * Calibration
     */
    DA_Write(0x050, 0x2A);		// Optimized calibration setting register write.
    DA_Write(0x061, 0x68);		// Required calibration control register write.
    DA_Write(0x051, 0x82);		// Optimized calibration setting register write
    DA_Write(0x051, 0x83);		// Required calibration control register write.
    DA_Write(0x081, 0x03);		// Required calibration control register write.
    /*
     * JESD204B Mode Setup
     */
    DA_Write(0x100, 0x00);	 	// Power up digital datapath clocks when internal clocks are stable.
    DA_Write(0x110, 0x28);		// JESD204B MODE = 8, BIT5->DUAL LINK L = 4 M = 2,F = 1,S = 1
    DA_Write(0x111, 0xC1);	 	// interpolation, MAIN DATAPATH 12x,Channel path 1x
    DArd = DA_Read(0x110);
    if(DArd != 0x28)
    {
    	print("Invalid interpolation config!!!\n\r");
    	return ;
    }
    else
	print("Valid interpolation config\r");
    DA_Write(0x084, 0x40);	 // sys dc-coupled bit0->pd
    DA_Write(0x312, 0x00);	 // Set SYNCOUTx error duration, depending on the selected mode.
    DA_Write(0x300, 0x0b);	 //->LINK0
    DA_Write(0x475, 0x09);	 // Soft reset the JESD204B quad-byte deframer.
    DA_Write(0x453, 0x03);	 // Disable scrambling option, L-1 = 3
    DA_Write(0x458, 0x2F);	 // SUBCLASSV=1, NP=16-1
    DA_Write(0x475, 0x01);	 // Bring the JESD204B quad-byte deframer out of reset
    DA_Write(0x300, 0x0f);	 //->LINK1
	DA_Write(0x475, 0x09);	 // Soft reset the JESD204B quad-byte deframer.
	DA_Write(0x453, 0x03);	 // Disable scrambling option, L-1 = 3
	DA_Write(0x458, 0x2F);	 // SUBCLASSV=1, NP=16-1
	DA_Write(0x475, 0x01);	 // Bring the JESD204B quad-byte deframer out of reset

    DA_Write(0x008, 0xff);
    DA_Write(0x112, 0x08);

//    DA_Write(0x114, 0x8E);
//    DA_Write(0x115, 0xE3);
//    DA_Write(0x116, 0x38);
//    DA_Write(0x117, 0x8E);
//    DA_Write(0x118, 0xB3);
//    DA_Write(0x119, 0x15);	//DDC FTW 5---1000MHz

//    DA_Write(0x114, 0x55);
//    DA_Write(0x115, 0x55);
//    DA_Write(0x116, 0x55);
//    DA_Write(0x117, 0x55);
//    DA_Write(0x118, 0x55);
//    DA_Write(0x119, 0x35);	//DDC FTW 5---2457.6MHz

//	DA_Write(0x114, 0x00);
//	DA_Write(0x115, 0x00);
//	DA_Write(0x116, 0x00);
//	DA_Write(0x117, 0x00);
//	DA_Write(0x118, 0x00);
//	DA_Write(0x119, 0x20);	//DDC FTW 5---1474.56MHz

//	DA_Write(0x114, 0x00);
//	DA_Write(0x115, 0x00);
//	DA_Write(0x116, 0x00);
//	DA_Write(0x117, 0x00);
//	DA_Write(0x118, 0x00);
//	DA_Write(0x119, 0x30);	//DDC FTW 5---2211.84MHz

//    DA_Write(0x114, 0x8E);
//    DA_Write(0x115, 0xE3);
//    DA_Write(0x116, 0x38);
//    DA_Write(0x117, 0x8E);
//    DA_Write(0x118, 0xFF);
//    DA_Write(0x119, 0x32);	//DDC FTW 5---2350MHz

//    DA_Write(0x114, 0x00);
//    DA_Write(0x115, 0x00);
//    DA_Write(0x116, 0x00);
//    DA_Write(0x117, 0x00);
//    DA_Write(0x118, 0x98);
//    DA_Write(0x119, 0x3A);	//DDC FTW 5---2700MHz

    DA_Write(0x114, 0x72);
    DA_Write(0x115, 0x1C);
    DA_Write(0x116, 0xC7);
    DA_Write(0x117, 0x71);
    DA_Write(0x118, 0xF4);
    DA_Write(0x119, 0x4B);	//DDC FTW 5---3500MHz

//    DA_Write(0x114, 0x00);
//    DA_Write(0x115, 0x00);
//    DA_Write(0x116, 0x00);
//    DA_Write(0x117, 0x00);
//    DA_Write(0x118, 0x00);
//    DA_Write(0x119, 0x50);	//DDC FTW 5---3686.4MHz


    DA_Write(0x11C, 0x00);
    DA_Write(0x11D, 0x00);
    DA_Write(0x113, 0x01);

    DA_Write(0x14B, 0x00);	// Turn off PRBS

    /*
     * JESD204B SERDES Required Interface Setup
     */
    DA_Write(0x240, 0xAA);	 // EQ settings.
    DA_Write(0x241, 0xAA);
    DA_Write(0x242, 0x55);
    DA_Write(0x243, 0x55);
    DA_Write(0x244, 0x1F);
    DA_Write(0x245, 0x1F);
    DA_Write(0x246, 0x1F);
    DA_Write(0x247, 0x1F);
    DA_Write(0x248, 0x1F);
    DA_Write(0x249, 0x1F);
    DA_Write(0x24A, 0x1F);
    DA_Write(0x24B, 0x1F);	 	// EQ settings.
    DA_Write(0x201, 0x00); 		//Power down unused PHYs. Bit x corresponds to SERDINx¡À pin power-down
    DA_Write(0x203, 0x00);		// Enable SYNC0, SYNC1
    DA_Write(0x253, 0x01);	 	// Set SYNCOUT0 to be LVDS output.
    DA_Write(0x254, 0x01);	 	// Set SYNCOUT1 to be LVDS output.
    DA_Write(0x210, 0x16);	 	// SERDES required register write.
    DA_Write(0x216, 0x05);	 	// SERDES required register write.
    DA_Write(0x212, 0xFF);	 	// SERDES required register write.
    DA_Write(0x212, 0x00);	 	// SERDES required register write.
    DA_Write(0x210, 0x87);	 	// SERDES required register write.
    DA_Write(0x216, 0x11);		// SERDES required register write.
    DA_Write(0x213, 0x01);		// SERDES required register write.
    DA_Write(0x213, 0x00);		// SERDES required register write.
    DA_Write(0x200, 0x00);		// Power up the SERDES circuitry blocks.
    usleep(150000);             // wait for 100ms
    DA_Write(0x210, 0x86);		// SERDES required register write.
    DA_Write(0x216, 0x40);
    DA_Write(0x213, 0x01);
    DA_Write(0x213, 0x00);
    DA_Write(0x210, 0x86);
    DA_Write(0x216, 0x00);
    DA_Write(0x213, 0x01);
    DA_Write(0x213, 0x00);
    DA_Write(0x210, 0x87);
    DA_Write(0x216, 0x01);
    DA_Write(0x213, 0x01);
    DA_Write(0x213, 0x00);
    DA_Write(0x280, 0x05);
    DA_Write(0x280, 0x01);		// SERDES required register write.

    DA_Write(0x05A, 0xFF);		// Full-scale current = 16 + 1020 * 25 / 1024 = 40.9mA
//    DA_Write(0x05A, 0x00);

    i = 10;
    while(i>0)
    {
    	DArd = DA_Read(0x281);
    	if((DArd & 0x01) == 0x01)
    		break;
    	i--;
    }
    if(i<=0)
    {
    	print("DAC SERDSES PLL LOCK FAILED!!!\n\r");
    	return;
    }
    else
    	print("DAC SERDSES PLL LOCK OK\r");
    /*
     * Transport Layer Setup
     */
    // for VCK190 and eval board
    DA_Write(0x308,0x37);// Logical lane and physical lane mapping
    DA_Write(0x309,0x25);
    DA_Write(0x30a,0x02);
    DA_Write(0x30b,0x19);

    DA_Write(0x306,0x0c);
	DA_Write(0x307,0x0c);
    DA_Write(0x304,0x00);
    DA_Write(0x305,0x01);

//    DA_Write(0x304,del);
//    DA_Write(0x306,val);

    DA_Write(0x03B,0xF1);
    DA_Write(0x03A,0x02);
    DA_Write(0x300,0x0b);
    DA_Write(0x085,0x13);
    DA_Write(0x1DE,0x03);
    DA_Write(0x008,0xC0);
    DA_Write(0x596,0x0C);
    DA_Write(0x334,0xf0); // invert logical lane4-7
	print("AD9173 INITIAL OK\n\r");
}
