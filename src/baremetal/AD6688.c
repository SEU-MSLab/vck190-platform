/*
 * AD6688.c
 *
 *  Created on: May 25, 2022
 *      Author: zhe
 */


#include"AD6688.h"

//const double BPFIL[48] = {
//  -0.001051800434772, -0.00652685201338, 0.002704540863072, -0.01319325302398,
//  -0.005124628166445, 0.005541093080989, 0.005443035116169,-0.0004441180469376,
//  -1.697793010725e-05, 0.001487290231408,-0.004964939857881,-0.007962344215539,
//   0.002518254963499,  0.01024292540182, 0.002790693220642,-0.003563335628626,
//  0.0004045336757748,-0.001996965620654, -0.01116759395713,-0.003739202792407,
//    0.01454570376719,  0.01208389491294,-0.005354556272182,-0.006362954556237,
//  0.0002499281605446,-0.009545172635255, -0.01272194351426,  0.01283240561528,
//    0.02711532566596, 0.001375467479201, -0.01897623043578,  -0.0056971117241,
//  -0.002637934913105, -0.01882565235836, 0.001632816012047,   0.0449027569518,
//    0.02684156512034, -0.03344410521105, -0.03475533260603, 0.002499005945475,
//   -0.01303957228939, -0.02182731369801,  0.06912221507473,   0.1175815150553,
//    -0.0431847341095,  -0.2126712601498, -0.08029153252957,   0.2019398372003};


// 900M BW filter
//const double BPFIL[24] = {
//		-0.0020266939, 0.0006073162, 0.0010477530, 0.0005625075,
//		0.0060236463, -0.0023046500, -0.0042344511,	-0.0011888556,
//		-0.0127565690, 0.0075034657, 0.0123108134, 0.0014757519,
//		0.0227834320, -0.0195192928, -0.0290992977,	0.0013636025,
//		-0.0371565927, 0.0479927604, 0.0676572004, -0.0167501573,
//		0.0685316945, -0.1572365868, -0.2777877615, 0.3226817010};

// 300M BW filter
//const double BPFIL[24] = {
//
//		0.0009527, -0.0019501, -0.0011696, 0.0001475, -0.0010193, 0.0032311,
//		0.0051876, -0.0090510, -0.0105896, 0.0143079, 0.0136384, -0.0141309,
//		-0.0093960, 0.0033811, -0.0063833, 0.0205134, 0.0345994, -0.0552726,
//		-0.0711192, 0.0934263, 0.1072546, -0.1245233, -0.1325956, 0.1391015};

// 600M BW filter for fs/4
//const double BPFIL[24] = {
//		0.0003642, -0.0005549, -0.0008282, 0.0005415, -0.0007149, 0.0022036,
//		0.0041410, -0.0044656, -0.0022356, -0.0024856, -0.0095653, 0.0148803,
//		0.0152284, -0.0077173, 0.0087021, -0.0285669, -0.0441448, 0.0450943,
//		0.0218506, 0.0271224, 0.0969973, -0.1743751, -0.2400686, 0.2785437, };

// 600M BW filter for 3.5G
const double BPFIL[24] = {
		-0.0004450, -0.0001977, 0.0011242, 0.0006592, -0.0000690, 0.0031555,
		0.0028467, -0.0046841, -0.0040944, 0.0005948, -0.0113687, -0.0154534,
		0.0092662, 0.0133145, -0.0038116, 0.0285960, 0.0563649, -0.0059930,
		-0.0340695, 0.0213687, -0.0678493, -0.2435595, -0.0753607, 0.3297114,
};



void AD6688_Init()
{
	u8 ADrd;
	u32 i;
	/*
	 * Analog Devices SPI Registers
	 */
	AD_Write(0x000A,0x01);  // The first version
	AD_Write(0x0000,0x81);  //reset SPI
	usleep(10000);          //wait for 10ms
	AD_Write(0x0001,0x02);  //reset datapath
	usleep(10000);
	AD_Write(0x0571,0x15);
	AD_Write(0x0002,0x00);  //Normal mode

	AD_Write(0x0008,0x03); 	// cfg ADC core AB at same time
	/*
	 * clock/SYSREF/chip Power-Down
	 */
	AD_Write(0x003F,0x80);  //PD pin disable
	AD_Write(0x0040,0x80);  //PD is ignored, FD_A && FD_B
	AD_Write(0x0108,0x00);  //CLK DIV = 1, Input clock is 2949.12MHz
	AD_Write(0x1908,0x00);	// AC coupling
	AD_Write(0x18E3,0x00);  // Turn off VCM buffer
	AD_Write(0x18A6,0x00);	// Internal VREF
	AD_Write(0x1910,0x08);	// minimum Vp-p, 1.13V Vp-p
	AD_Write(0x1A4C,0x09);	// Input buffer P current 500uA
	AD_Write(0x1A4D,0x09);	// Input buffer N current 500uA

	AD_Write(0x0109,0x00);  // Clock divider phase 0
	AD_Write(0x010A,0x00);	// Clock divider phase is not changed by SYSREF (disabled).
	usleep(5000);
	i=10;
    while(i>0)
    {
    	ADrd = AD_Read(0x011B);
    	if((ADrd & 0x01) == 0x01)
    		break;
    	i--;
    }
    if(i<=0)
    {
    	// If unplug J201, will print NOT DETECT!
    	print("ADC CLK NOT DETECT!!!\n\r");
    	return;
    }
    else
    	print("ADC CLK DETECT\r");
    AD_Write(0x0120,0x00);		// Disable SYSREF



    /*
     * Chip Operate Mode
     */
    AD_Write(0x0200, 0x02);	 // Two DDC mode
    AD_Write(0x0201, 0x08);  // DCM by 3
    /*
     * DDC Function
     */
    AD_Write(0x0300,0x00);
    // TB1 doesn't have 6 dB gain
    // fs/4 sampling mode
//    AD_Write(0x0310, 0x27);		// Mixer Real input, fs/4 IF, HB1(dec=3,complex output)
//    AD_Write(0x0311, 0x70);	 	// DDC0 input select Channel A to I and Q
//    AD_Write(0x0330, 0x27);		// Mixer Real input, fs/4 IF, HB1(dec=3,complex output)
//    AD_Write(0x0331, 0x75);		// DDC1 input select Channel B to I and Q

    // variable IF mode
    AD_Write(0x0300, 0x00);
    AD_Write(0x0310, 0x07);
    AD_Write(0x0311, 0x70);
    AD_Write(0x0330, 0x07);
    AD_Write(0x0331, 0x75);

    // frequency at 3.5G
    AD_Write(0x0316, 0xC7);
    AD_Write(0x0317, 0x71);
    AD_Write(0x0318, 0x1C);
    AD_Write(0x0319, 0xC7);
    AD_Write(0x031a, 0xD1);
    AD_Write(0x031b, 0x2F);
    AD_Write(0x0336, 0xC7);
    AD_Write(0x0337, 0x71);
    AD_Write(0x0338, 0x1C);
    AD_Write(0x0339, 0xC7);
    AD_Write(0x033a, 0xD1);
    AD_Write(0x033b, 0x2F);


//    AD_Write(0x0316,0xC8);  // 3401C71C71C8  fc = 2949.12 - 2350
//    AD_Write(0x0317,0x71);
//    AD_Write(0x0318,0x1C);
//    AD_Write(0x0319,0xC7);
//    AD_Write(0x031a,0x01);
//    AD_Write(0x031b,0x34);

    // 加了这个滤波器后会很影响性能，信噪比都差不少
//    AD6688_FIR();


    /*
     * Digital Output and Test Modes
     */
    AD_Write(0x056E,0x00); //Pll Freq
    AD_Write(0x058E,0x03); // M=4
    usleep(10000);


    AD_Write(0x0592, 0x80);		// HD
    AD_Write(0x058B, 0x07);		// scrambler OFF, 8 lines per link
	AD_Write(0x058C,0x00);		// F=1
	AD_Write(0x058D,0x1F); 		//K=32

	// physical lane and logical lane mapping
	AD_Write(0x05B2, 0x01);		// L1,L0
	AD_Write(0x05B3, 0x32);		// L2,L3
	AD_Write(0x05B5, 0x47);		// L5,L4
	AD_Write(0x05B6, 0x56);		// L6,L7

	AD_Write(0x05C0, 0x11);		// swing 0.85DRVDD
	AD_Write(0x05C1, 0x11);		// swing 0.85DRVDD
	AD_Write(0x05C2, 0x11);		// swing 0.85DRVDD
	AD_Write(0x05C3, 0x11);		// swing 0.85DRVDD

    AD_Write(0x1228, 0x4F);
    AD_Write(0x1228, 0x0F);
    AD_Write(0x1222, 0x00);	// JESD204B PLL calibration
    AD_Write(0x1222, 0x04);
    AD_Write(0x1222, 0x00);
    AD_Write(0x1262, 0x80);
    AD_Write(0x1262, 0x00);
//    AD_Write(0x0571, 0x14);  // Link on, enable FACI, not supported by JESD204C IP core
//    AD_Write(0x0571, 0x04);  // Link on, enable FACI but disable F character, not supported by JESD204C
    AD_Write(0x0571, 0x16);  // Link on, disable FACI


	i=10;
    while(i>0)
    {
    	ADrd = AD_Read(0x056F);
    	if((ADrd & 0x80) == 0x80)
    	{
    		break;
    	}
    	else if((ADrd & 0x08) == 0x08){ // loss of lock
    		AD_Write(0x0571, 0x15);
			AD_Write(0x0571, 0x16);  // Link on, disable FACI

    	}
    	i--;
    }
    if(i<=0)
    {
    	print("ADC JESD PLL LOCK FAILED!!!\n\r");
    	return;
    }
    else
    	print("ADC JESD PLL LOCK SUCCESS\n\r");

    /*
     * vref/analog input control
     */
}

void AD6688_FIR()
{



	AD_Write(0x0008,0x01);
	AD_Write(0x0DF8,0x02);
	AD_Write(0x0DF9,0x11);  // 6dB gain for X and Y coeff
	AD_Write(0x0008,0x02);
	AD_Write(0x0DF8,0x02);
	AD_Write(0x0DF9,0x11);
	usleep(100);
	FIR_Real48Tap();


}

void FIR_Real48Tap(){
	s16 coefix;
	u16 coefaddr,i;
	s16 coefqfix[24];
	u8 dlow,dhigh;

	// ADC A Filter coeff
	AD_Write(0x0008,0x01);
	coefaddr = 0x0E00;
	for(i=0;i<24;i++)
	{
		coefix =(s16)(BPFIL[i] * 32768);
		coefqfix[23-i] = coefix;
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
	}

	coefaddr = 0x0F00;
	for(i=0;i<24;i++)
	{
		coefix =coefqfix[i];
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
	}

	// ADC B Filter coeff
	AD_Write(0x0008,0x02);
	coefaddr = 0x0E00;
	for(i=0;i<24;i++)
	{
		coefix =(s16)(BPFIL[i] * 32768);
		coefqfix[23-i] = coefix;
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
	}

	coefaddr = 0x0F00;
	for(i=0;i<24;i++)
	{
		coefix =coefqfix[i];
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
	}

	AD_Write(0x000F,0x01);

}

void FIR_Real96Tap(){
	s16 coefix;
	u16 coefaddr,i;
	s16 coefqfix[48];
	u8 dlow,dhigh;

	// I path Filter coeff
	AD_Write(0x0008,0x01);
	coefaddr = 0x0E00;
	for(i=0;i<48;i++)
	{
		coefix =(s16)(BPFIL[i] * 32768);
		coefqfix[47-i] = coefix;
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
		if(coefaddr == 0x0E30)
			coefaddr = 0x0F00;
	}

	// Q path Filter coeff
	AD_Write(0x0008,0x02);
	coefaddr = 0x0E00;
	for(i=0;i<48;i++)
	{
		coefix =coefqfix[i];
		dlow =(u8) (coefix & 0x00FF);
		dhigh =(u8) ((coefix>>8) & 0x00FF);
		AD_Write(coefaddr,dlow);
		coefaddr++;
		AD_Write(coefaddr,dhigh);
		coefaddr++;
		if(coefaddr == 0x0E30)
			coefaddr = 0x0F00;
	}
	AD_Write(0x000F,0x01);


}


