#ifndef __RFID_H
#define __RFID_H

#include "stm32f10x.h"
#include "main.h"
#include "systick.h"
#include "rc522.h"
#include "stdio.h"
#include "pwm.h"

void GPIO_ini(void);
void InitializeSystem(void );
void findcard(void);
#endif
