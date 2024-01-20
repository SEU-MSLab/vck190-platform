 /*sys_intr.c
 * Created on: 2016年11月22日
 * www.osrc.cn
 * copyright by liyang mi lian dian zi www.osrc.cn
*/
#include "sys_intr.h"

/*
 * Init XScuGic and Exception
 */
int Init_Intr_System()
{
	int Status;

	XScuGic_Config *IntcConfig;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&Intc, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Enable interrupts from the hardware */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)INTC_HANDLER,
			//TODO:确定能否改为(void)Intc
			(void *)&Intc);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}



