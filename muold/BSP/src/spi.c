#include "spi.h"

void SPI2_RFID_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15 ; //clk  miso mosi
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //NSS
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_DataSize =  SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode =  SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_Cmd(SPI2,ENABLE);
}

u8 SPI2_WriteReadByte(u8 Byte)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)== RESET){}
	SPI_I2S_SendData(SPI2,Byte);
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)== RESET){}
	return SPI_I2S_ReceiveData(SPI2);
}


