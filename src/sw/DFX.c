/*
 * DFX.c
 *
 *  Created on: 2022Äê9ÔÂ28ÈÕ
 *      Author: zhe
 */


#include "DFX.h"

static FIL fil;		/* File object */
static FATFS fatfs;
static char FileName[32] = "design_1_wrapper.pdi";
static char *SD_File;
// 10M space for PDI file, too large, lwip will shutdown
//const u32 FileSize = (1000*1024);
//u8 DestinationAddress[1000*1024] __attribute__ ((aligned(32)));
const u32 FileSize = (1*1024);
u8 DestinationAddress[1*1024] __attribute__ ((aligned(32)));


int sd_init()
{
	FRESULT Res;
	BYTE work[FF_MAX_SS];

	TCHAR *Path = "0:/";

//	Res = f_mount(&fatfs, Path, 0);
	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		xil_printf("SD card Mount failed! \r\n");
		return XST_FAILURE;
	}

// Don't use, it will format SD card
//	Res = f_mkfs(Path, FM_FAT32, 0, work, sizeof work);
//	if (Res != FR_OK) {
//		return XST_FAILURE;
//	}

	return XST_SUCCESS;
}

int read_pdi()
{
	FRESULT Res;
	SD_File = (char *)FileName;
	UINT NumBytesRead;

//	Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	Res = f_open(&fil, SD_File, FA_OPEN_EXISTING | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
			&NumBytesRead);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_close(&fil);
	if (Res) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int dow_pdi()
{
	s32 Status;
	XFpga XFpgaInstance = {0U};
	UINTPTR KeyAddr = (UINTPTR)NULL;

	XFpga_Initialize(&XFpgaInstance);

	Status = XFpga_BitStream_Load(&XFpgaInstance, (UINTPTR)DestinationAddress, KeyAddr,
			BITSTREAM_SIZE, PDI_LOAD);

	if (Status == XFPGA_SUCCESS)
		xil_printf("Partial Reconfiguration Bitstream loaded into the PL successfully");
	else
		xil_printf("Partial Reconfiguration Bitstream loading into the PL failed\n\r");

	return Status;
}


