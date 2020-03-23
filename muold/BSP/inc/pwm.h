#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
void TIM2_Init(u16 arr,u16 psc);
	
void TIM3_PWM_Init(void);
void open_door(void);
void close_door(void);
#endif //__PWM_H
