#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define SET_SPI_CS  GPIO_SetBits(GPIOB, GPIO_Pin_12);
#define CLR_SPI_CS  GPIO_ResetBits(GPIOB, GPIO_Pin_12);

#define SET_RC522RST GPIO_SetBits(GPIOB, GPIO_Pin_8);
#define CLR_RC522RST GPIO_ResetBits(GPIOB, GPIO_Pin_8);

void SPI2_RFID_Init(void);
u8 SPI2_WriteReadByte(u8 Byte);
#endif 
