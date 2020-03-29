#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "pwm.h"

#define USART2_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern vu16 USART2_RX_STA;   						//��������״̬

void USART1_Config(void);
void USART1_IRQHandler(void);	
int fputc(int ch, FILE *f);
void USART1_Putc(u8 c);
void USART1_Puts(char * str);

void usart2_init(u32 bound);
#endif
