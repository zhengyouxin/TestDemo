#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"


//-----------------------------------------------------------------
//  SPI PORT define	
typedef enum SPI_TYPE{SPI_PORT1,SPI_PORT2}SPIType;             // SPI_PORT1---->SPI1;  SPI_PORT2---->SPI2

//---------------------------------------------------------------
void SPI1_Init(void);  //SPI1初始化
void SPI2_Init(void);  //SPI2初始化

u8 SPI_ReadWriteByte(SPIType spiNum,u8 dat); //SPI2读写一个字节
void SPI_SetSpeed(SPIType spiNum,u8 SPI_BaudRatePrescaler);//设置SPI2的速度

#endif

