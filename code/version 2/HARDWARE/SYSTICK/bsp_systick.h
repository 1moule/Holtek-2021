#ifndef _BSP_SYSTICK_H
#define _BSP_SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);         // 单位1us


#define Delay_ms(x) Delay_us(1000*x)	 //单位ms

#endif /*_BSP_SYSTICK_H*/




