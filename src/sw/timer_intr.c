/*
 *
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
*/

#include "timer_intr.h"

volatile int TcpTmrFlag;

XTtcPs Timer;

TmrCntrSetup SettingsTable[1] = {
	{4, 0, 0, 0}, /* Only need to set output frequency */

};


void TimerIntrHandler(void *CallBackRef)
{
	u32 StatusEvent;

	XTtcPs *TimerInstancePtr = (XTtcPs *) CallBackRef;
	StatusEvent = XTtcPs_GetInterruptStatus(TimerInstancePtr);

	if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent)) {
		TcpTmrFlag = 1;
	}
	else {
		/*
		 * The Interval event should be the only one enabled. If it is
		 * not it is an error
		 */
		xil_printf("LwIP Timer interrupt error! \n\r");
	}

	platform_clear_interrupt(TimerInstancePtr);
}


void Timer_start()
{
	XTtcPs_Start(&Timer);
}


void Timer_Setup_Intr_System()
{
	XScuGic_RegisterHandler(INTC_BASE_ADDR, TIMER_IRPT_INTR,
					(Xil_ExceptionHandler)TimerIntrHandler,//set up the timer interrupt
					(void *)&Timer);

	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR);//enable the interrupt for the Timer at GIC
	XTtcPs_EnableInterrupts(&Timer, XTTCPS_IXR_INTERVAL_MASK);//enable interrupt on the timer
 }

int Timer_init()
{
	int Status;
	TmrCntrSetup *TimerSetup;
	XTtcPs_Config *TMRConfigPtr;     //timer config

	//Setup Timer
	TMRConfigPtr = XTtcPs_LookupConfig(TIMER_DEVICE_ID);
	Status = XTtcPs_CfgInitialize(&Timer, TMRConfigPtr,TMRConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scutimer Cfg initialization failed...\r\n",
		__func__);
		return 0;
	}

	//Setup options
	TimerSetup = &SettingsTable[TIMER_DEVICE_ID];
	TimerSetup->Options |= (XTTCPS_OPTION_INTERVAL_MODE |
							      XTTCPS_OPTION_WAVE_DISABLE);
	XTtcPs_SetOptions(&Timer, TimerSetup->Options);

	//Setup Interval
	XTtcPs_CalcIntervalFromFreq(&Timer, TimerSetup->OutputHz,
			&(TimerSetup->Interval), &(TimerSetup->Prescaler));

	XTtcPs_SetInterval(&Timer, TimerSetup->Interval);
	XTtcPs_SetPrescaler(&Timer, TimerSetup->Prescaler);

	//Setup Interrupt
	Timer_Setup_Intr_System();

	//Start Timer is in lwip_sys_init

     return XST_SUCCESS;
}

void platform_clear_interrupt( XTtcPs * TimerInstance )
{
	u32 StatusEvent;

	StatusEvent = XTtcPs_GetInterruptStatus(TimerInstance);
	XTtcPs_ClearInterruptStatus(TimerInstance, StatusEvent);
}

