/*
 *
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
 *
 * Revised by: Zhe Li 2022.04.04
 * contact: 904016301@qq.com
*/

#ifndef TIMER_INTR_H_
#define TIMER_INTR_H_

#include <stdio.h>
#include "xil_types.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xttcps.h"
#include "xparameters.h"
#include "sys_intr.h"


extern volatile int TcpTmrFlag;
extern volatile int TcpTmrErrorCount;
extern XTtcPs Timer;

typedef struct {
	u32 OutputHz;	/* Output frequency */
	XInterval Interval;	/* Interval value */
	u8 Prescaler;	/* Prescaler value */
	u16 Options;	/* Option settings */
} TmrCntrSetup;





//timer info
#define TIMER_DEVICE_ID     XPAR_XTTCPS_0_DEVICE_ID
#define TIMER_IRPT_INTR     XPAR_XTTCPS_0_INTR
#define TIMER_LOAD_VALUE    XPAR_CPU_CORTEXA72_0_CPU_CLK_FREQ_HZ / 8 //0.25S

void TimerIntrHandler(void *CallBackRef);
void Timer_start();
void Timer_Setup_Intr_System();
int Timer_init();
void platform_clear_interrupt( XTtcPs * TimerInstance );
#endif /* TIMER_INTR_H_ */
