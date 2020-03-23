//Í·ÎÄ¼þ
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "RTC_Time.h" 
#include "pwm.h"
#include <stdio.h>
#include "systick.h"
#include "rc522.h"
//#include "oled.h"
#include "MAIN.h"
#include "rc522.h"
#include "usart.h"
#include "string.h"
#include "rfid.h"

//int i,j;

int main(void)
{	
  delay_init();  
	USART1_Config();
  InitializeSystem( );
  TIM3_PWM_Init();
	TIM2_Init(10000,7200);
	while(1)
	{
	findcard();
	}
}









