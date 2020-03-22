#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
void TIM2_PWM_Init(void);
	
void TIM3_PWM_Init(u16 arr,u16 psc);
void open_door(void);
void close_door(void);
#endif //__PWM_H
