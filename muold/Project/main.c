//头文件
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
#include "as608.h"

#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;
const  u8* kbd_menu[15]={"del_fp"," : ","add_fp","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表
const  u8* kbd_delFR[15]={"BACK"," : ","del_all","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表

void Add_FR(void);	//录指纹
void press_FR(void);//刷指纹

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init();  
	USART1_Config();
	usart2_init(usart2_baund);//初始化串口2,用于与指纹模块通讯
	
  InitializeSystem( );
  TIM3_PWM_Init();
	TIM2_Init(10000,7200);
	PS_StaGPIO_Init();	
	while(1)
	{
	findcard();
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
	{
		press_FR();
		
	}	
}
}


//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						printf("Fingerprint correct\n");
						i=0;
						processnum=1;//跳到第二步						
					}else printf("%x\n",ensure);				
				}else printf("%x\n",ensure);						
				break;
			
			case 1:
				i++;
				printf("Pleas touch finger again!\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征			
					if(ensure==0x00)
					{
						printf("Fingerprint correct\n");
						i=0;
						processnum=2;//跳到第三步
					}else printf("%x\n",ensure);	
				}else printf("%x\n",ensure);		
				break;

			case 2:
				printf("Compare twice fingerprint\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("Twice fingerprint are same\n");
					processnum=3;//跳到第四步
				}
				else 
				{
					printf("Compare fail,pleas touch again!\n");
					printf("%x\n",ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("Generate fingerprint template\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("Generate fingerprint success\n");
					processnum=4;//跳到第五步
				}else {processnum=0;printf("%x\n",ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
					printf("添加指纹成功\n");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					printf("剩余指纹个数%d\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//延时后清除显示	
					return ;
				}else {processnum=0;printf("%x\n",ensure);}					
				break;				
		}
		delay_ms(800);
		if(i==5)//超过5次没有按手指则退出
		{
			break;	
		}				
	}
}

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		printf("获取图像成功\n");
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			printf("生成图像成功\n");
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{					
				printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//显示匹配指纹的IDop和分数
				TIM_Cmd(TIM2,DISABLE);
				open_door();
				TIM_Cmd(TIM2,ENABLE);
			}
			else 
				printf("%x\n",ensure);					
	  }
		else
			printf("%x\n",ensure);
	 delay_ms(1000);//延时后清除显示
	}
		
}

//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num = 0xFF00;
	delay_ms(50);

	if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
		printf("删除成功\n");
	}
  else
	printf("%x\n",ensure);
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数数
	printf("剩余还有%d个指纹\n",AS608Para.PS_max-ValidN);

}






