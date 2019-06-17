#ifndef _NRF_GPIO_H
#define _NRF_GPIO_H

#include "stm32f10x.h"
#include "spi.h"

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO口操作,只对单一的IO口!
//确保n的值小于16!

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//--------------------------------------------------------------
//  IO Port define

#define	SPI1_IRQ_Port	GPIOA
#define	SPI1_IRQ_Pin	GPIO_Pin_0

#define	SPI1_CS_Port	GPIOA				//PORTA
#define	SPI1_CS_Pin		GPIO_Pin_4

#define	SPI1_SCK_Port	GPIOA
#define	SPI1_SCK_Pin	GPIO_Pin_5

#define	SPI1_MISO_Port	GPIOA
#define	SPI1_MISO_Pin	GPIO_Pin_6

#define	SPI1_MOSI_Port	GPIOA
#define	SPI1_MOSI_Pin	GPIO_Pin_7

#define	SPI1_CE_Port	GPIOB				// PORTB
#define	SPI1_CE_Pin		GPIO_Pin_6

//----------------------------------------
#define	SPI2_CS_Port	GPIOB
#define	SPI2_CS_Pin		GPIO_Pin_12

#define	SPI2_SCK_Port	GPIOB
#define	SPI2_SCK_Pin	GPIO_Pin_13

#define	SPI2_MISO_Port	GPIOB
#define	SPI2_MISO_Pin	GPIO_Pin_14

#define	SPI2_MOSI_Port	GPIOB
#define	SPI2_MOSI_Pin	GPIO_Pin_15

#define	SPI2_CE_Port	GPIOB
#define	SPI2_CE_Pin		GPIO_Pin_9

#define	SPI2_IRQ_Port	GPIOB
#define	SPI2_IRQ_Pin	GPIO_Pin_8


//SI24R1----SPI1 位操作
#define SPI1_CE  		PBout(6) 		//	SPI1 片选信号
#define SPI1_CSN  		PAout(4) 		//	SPI片选信号	   
#define SPI1_IRQ  		PAin(0)  		//	IRQ主机数据输入

//SI24R1----SPI2 位操作
#define SPI2_CE  		PBout(9) 		//	SPI1 片选信号
#define SPI2_CSN  		PBout(12) 		//	SPI片选信号	   
#define SPI2_IRQ  		PBin(8)  		//	IRQ主机数据输入




#endif
