#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"


//-----------------------------------------------------------------
//  SPI PORT define	
typedef enum SPI_TYPE{SPI_PORT1,SPI_PORT2}SPIType;             // SPI_PORT1---->SPI1;  SPI_PORT2---->SPI2

//---------------------------------------------------------------
void SPI1_Init(void);  //SPI1��ʼ��
void SPI2_Init(void);  //SPI2��ʼ��

u8 SPI_ReadWriteByte(SPIType spiNum,u8 dat); //SPI2��дһ���ֽ�
void SPI_SetSpeed(SPIType spiNum,u8 SPI_BaudRatePrescaler);//����SPI2���ٶ�

#endif

