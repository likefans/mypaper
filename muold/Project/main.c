//ͷ�ļ�
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

#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ��ģ�ע�⣺ָ��ģ��Ĭ��57600��
SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чģ�����
u8** kbd_tbl;
const  u8* kbd_menu[15]={"del_fp"," : ","add_fp","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//������
const  u8* kbd_delFR[15]={"BACK"," : ","del_all","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//������

void Add_FR(void);	//¼ָ��
void press_FR(void);//ˢָ��

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init();  
	USART1_Config();
	usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	
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


//¼ָ��
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
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						printf("Fingerprint correct\n");
						i=0;
						processnum=1;//�����ڶ���						
					}else printf("%x\n",ensure);				
				}else printf("%x\n",ensure);						
				break;
			
			case 1:
				i++;
				printf("Pleas touch finger again!\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//��������			
					if(ensure==0x00)
					{
						printf("Fingerprint correct\n");
						i=0;
						processnum=2;//����������
					}else printf("%x\n",ensure);	
				}else printf("%x\n",ensure);		
				break;

			case 2:
				printf("Compare twice fingerprint\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("Twice fingerprint are same\n");
					processnum=3;//�������Ĳ�
				}
				else 
				{
					printf("Compare fail,pleas touch again!\n");
					printf("%x\n",ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("Generate fingerprint template\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("Generate fingerprint success\n");
					processnum=4;//�������岽
				}else {processnum=0;printf("%x\n",ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{			
					printf("���ָ�Ƴɹ�\n");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					printf("ʣ��ָ�Ƹ���%d\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//��ʱ�������ʾ	
					return ;
				}else {processnum=0;printf("%x\n",ensure);}					
				break;				
		}
		delay_ms(800);
		if(i==5)//����5��û�а���ָ���˳�
		{
			break;	
		}				
	}
}

//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		printf("��ȡͼ��ɹ�\n");
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			printf("����ͼ��ɹ�\n");
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{					
				printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�IDop�ͷ���
				TIM_Cmd(TIM2,DISABLE);
				open_door();
				TIM_Cmd(TIM2,ENABLE);
			}
			else 
				printf("%x\n",ensure);					
	  }
		else
			printf("%x\n",ensure);
	 delay_ms(1000);//��ʱ�������ʾ
	}
		
}

//ɾ��ָ��
void Del_FR(void)
{
	u8  ensure;
	u16 num = 0xFF00;
	delay_ms(50);

	if(num==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(num,1);//ɾ������ָ��
	if(ensure==0)
	{
		printf("ɾ���ɹ�\n");
	}
  else
	printf("%x\n",ensure);
	delay_ms(1500);//��ʱ�������ʾ
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ�����
	printf("ʣ�໹��%d��ָ��\n",AS608Para.PS_max-ValidN);

}






