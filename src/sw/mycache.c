/*
 * mycache.c
 *
 *  Created on: 2022Äê8ÔÂ22ÈÕ
 *      Author: zhe
 */
#include "mycache.h"

// migrate A9 funciton is impracticable!!!
//void Xil_DCacheInvalidateRange(INTPTR adr, u32 len)
//{
//	const u32 cacheline = 32U;
//	u32 end;
//	u32 tempadr = adr;
//	u32 tempend;
//	u32 currmask;
//	volatile u32 *L2CCOffset = (volatile u32 *)(XPS_L2CC_BASEADDR +
//				    XPS_L2CC_CACHE_INVLD_PA_OFFSET);
//
//	currmask = mfcpsr();
//	mtcpsr(currmask | IRQ_FIQ_MASK);
//
//	if (len != 0U) {
//		end = tempadr + len;
//		tempend = end;
//		/* Select L1 Data cache in CSSR */
//		mtcp(XREG_CP15_CACHE_SIZE_SEL, 0U);
//
//		if ((tempadr & (cacheline-1U)) != 0U) {
//			tempadr &= (~(cacheline - 1U));
//
//			Xil_L1DCacheFlushLine(tempadr);
//#ifndef USE_AMP
//			/* Disable Write-back and line fills */
//			Xil_L2WriteDebugCtrl(0x3U);
//			Xil_L2CacheFlushLine(tempadr);
//			/* Enable Write-back and line fills */
//			Xil_L2WriteDebugCtrl(0x0U);
//			Xil_L2CacheSync();
//#endif
//			tempadr += cacheline;
//		}
//		if ((tempend & (cacheline-1U)) != 0U) {
//			tempend &= (~(cacheline - 1U));
//
//			Xil_L1DCacheFlushLine(tempend);
//#ifndef USE_AMP
//			/* Disable Write-back and line fills */
//			Xil_L2WriteDebugCtrl(0x3U);
//			Xil_L2CacheFlushLine(tempend);
//			/* Enable Write-back and line fills */
//			Xil_L2WriteDebugCtrl(0x0U);
//			Xil_L2CacheSync();
//#endif
//		}
//
//		while (tempadr < tempend) {
//#ifndef USE_AMP
//			/* Invalidate L2 cache line */
//			*L2CCOffset = tempadr;
//			Xil_L2CacheSync();
//#endif
//
//	/* Invalidate L1 Data cache line */
//#if defined (__GNUC__) || defined (__ICCARM__)
//			asm_cp15_inval_dc_line_mva_poc(tempadr);
//#else
//			{ volatile register u32 Reg
//				__asm(XREG_CP15_INVAL_DC_LINE_MVA_POC);
//			  Reg = tempadr; }
//#endif
//			tempadr += cacheline;
//		}
//	}
//
//	dsb();
//	mtcpsr(currmask);
//}
