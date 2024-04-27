 /*sys_intr.h
 * Created on: 2016Äê11ÔÂ22ÈÕ
 * www.osrc.cn
 * copyright by liyang mi lian dian zi www.osrc.cn
*/

#ifndef SYS_INTR_H_
#define SYS_INTR_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"

extern XScuGic Intc; //GIC

#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC_HANDLER		XScuGic_InterruptHandler
#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR

int Init_Intr_System();

#endif /* SYS_INTR_H_ */
