/*
 * clock.c
 *
 *  Created on: May 25, 2022
 *      Author: zhe
 */

#include "clock.h"


void HMC7044_Init(){
	int i;
	/* Disable all channels */
	for (i = 0; i < 14; i++)
		CLK_Write(0x00C8 + 0xA * i, 0);

	// Some optimal settings
	CLK_Write(0x009F, 0x4d);
	CLK_Write(0x00A0, 0xdf);
	CLK_Write(0x00A5, 0x06);
	CLK_Write(0x00A8, 0x06);
	CLK_Write(0x00B0, 0x04);


	CLK_Write(0x0005, 0x02); // Enable CLKIN1

	/* Program PLL2 */
	/* Select the VCO range */
	CLK_Write(0x0003, 0x0f);  // Select high VCO, enable internal SYSREF time reference, enable PLL1 and PLL2

	/* Program the dividers */
	CLK_Write(0x0033, 0x01); // R2 = 1
	CLK_Write(0x0034, 0x00);
	CLK_Write(0x0035, 0x18); // N2 = 24
	CLK_Write(0x0036, 0x00);

	/* Program the reference doubler */
	CLK_Write(0x0032, 0x01);

	/* Program PLL1 */
	/* Set the lock detect timer threshold */
	CLK_Write(0x0028, 0x1F);  // log2(fLCM * 4000 / pll1_loop_bw) = log2(fCLKIN1/2 * 4000 / 200) = 30.1946

	/* Set the LCM */
	CLK_Write(0x001D, 0x02); // fLCM = fCLKIN1 / 2
	CLK_Write(0x0020, 0x02); // OSCIN Prescaler = vcxo_freq/lcm_freqs

	/* Program the dividers */
	CLK_Write(0x0021, 0x07); // 61.44MHZ / 7, no larger than 10MHz
	CLK_Write(0x0022, 0x00);
	CLK_Write(0x0026, 0x0E); // 122.88MHz / 14
	CLK_Write(0x0027, 0x00);

	CLK_Write(0x0014, 0x55); // All Priority is CLKIN1

	/* Program the SYSREF timer */
	/* Set the divide ratio */
	CLK_Write(0x005c, 0x00);
	CLK_Write(0x005d, 0x04);

	/* Set the pulse generator mode configuration */
	CLK_Write(0x005a, 0x00); // level sensitive

	/* Enable the input buffers */
	CLK_Write(0x000a, 0x00);
	CLK_Write(0x000b, 0x15); // CLKIN1, enable input buffer, ac coupling input?
	CLK_Write(0x000c, 0x00);
	CLK_Write(0x000d, 0x00);
	CLK_Write(0x000e, 0x15); // OSCIN, enable input buffer, ac coupling input

	/* Set GPIOs */
	CLK_Write(0x0046, 0x00); // Disable all GPI
	CLK_Write(0x0047, 0x00);
	CLK_Write(0x0048, 0x00);
	CLK_Write(0x0049, 0x00);

	CLK_Write(0x0050, 0x1f); // enable GPIO1, CMOS MODE, PLL1 lock detect, not connect to LED
//	CLK_Write(0x0051, 0x2b); // enable GPIO2, CMOS MODE, PLL2 lock detect
	CLK_Write(0x0051, 0x1f); // GPIO2 for PLL1 lock detect
	CLK_Write(0x0052, 0x00);
	CLK_Write(0x0053, 0x00);

	usleep(10000);

	/* Program the output channels */
	/* DAC CLK */
	CLK_Write(0x00c9 + 0xA * 2, 0x06); // DAC CLK, divide by 6, 2949.12/6 = 491.52
	CLK_Write(0x00ca + 0xA * 2, 0x00); // DAC CLK MSB
	CLK_Write(0x00d0 + 0xA * 2, 0x08); // LVPECL MODE

	CLK_Write(0x00cb + 0xA * 2, 0x00); // Fine analog delay
	CLK_Write(0x00cc + 0xA * 2, 0x00); // Coarse digital delay
	CLK_Write(0x00cf + 0xA * 2, 0x00); // Channel divider output

	CLK_Write(0x00c8 + 0xA * 2, 0xc1); // High performance mode, SYNC enable, Asynchronous startup, channel enable

	/* DAC SYSREF */
	CLK_Write(0x00c9 + 0xA * 3, 0x80); // DAC SYSREF, divide by 384, 2949.12/384 = 7.68
	CLK_Write(0x00ca + 0xA * 3, 0x01); // DAC SYSREF MSB
	CLK_Write(0x00d0 + 0xA * 3, 0x08); // LVPECL MODE

	CLK_Write(0x00cb + 0xA * 3, 0x00); // Fine analog delay
	CLK_Write(0x00cc + 0xA * 3, 0x00); // Coarse digital delay
	CLK_Write(0x00cf + 0xA * 3, 0x00); // Channel divider output

	CLK_Write(0x00c8 + 0xA * 3, 0xc1); // High performance mode, SYNC enable, Asynchronous startup, channel enable

	/* FPGA CLK */
	CLK_Write(0x00c9 + 0xA * 12, 0x0c); // FPGA CLK, divide by 12, 2949.12/12 = 245.76MHz
	CLK_Write(0x00ca + 0xA * 12, 0x00); // FPGA CLK MSB
	CLK_Write(0x00d0 + 0xA * 12, 0x08); // LVDS MODE

	CLK_Write(0x00cb + 0xA * 12, 0x00); // Fine analog delay
	CLK_Write(0x00cc + 0xA * 12, 0x00); // Coarse digital delay
	CLK_Write(0x00cf + 0xA * 12, 0x00); // Channel divider output

	CLK_Write(0x00c8 + 0xA * 12, 0xc1); // High performance mode, SYNC enable, Asynchronous startup, channel enable

	/* FPGA SYSREF */
	CLK_Write(0x00c9 + 0xA * 13, 0x80); // FPGA SYSREF, divide by 384, 2949.12/384 = 7.68
	CLK_Write(0x00ca + 0xA * 13, 0x01); // FPGA SYSREF MSB
	CLK_Write(0x00d0 + 0xA * 13, 0x08); // LVDS MODE

	CLK_Write(0x00cb + 0xA * 13, 0x00); // Fine analog delay
	CLK_Write(0x00cc + 0xA * 13, 0x00); // Coarse digital delay
	CLK_Write(0x00cf + 0xA * 13, 0x00); // Channel divider output

	CLK_Write(0x00c8 + 0xA * 13, 0xc1); // High performance mode, SYNC enable, Asynchronous startup, channel enable

	usleep(10000);

	/* Do a restart to reset the system and initiate calibration */
	CLK_Write(0x0001, 0x02);
	usleep(1000);
	CLK_Write(0x0001, 0x00);
	usleep(1000);
}
