/**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: printf.h
**创   建   人: 王云飞
**最后修改日期: 
**描        述: 
** 固件库信息：V3.4.0             
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 王云飞
** 版  本: v0.01
** 日　期: 
** 描　述: 原始版本
**
**--------------当前版本信息----------------------------------------------------------------------------
** 创建人: 王云飞
** 版  本: v0.01
** 日　期: 
** 描　述: 当前版本
**
**------------------------------------------------------------------------------------------------------*/

#ifndef PRINTF_H
#define PRINTF_H

#include "stm32f10x.h"
#include "stdio.h"
#include "printf.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART_APP_LEN  			20*1024 //定义最大接收字节数 20K

#define	CONSOLE_UART		USART1	 		//调试口

void USART_Configuration(void);

void Usart1_SendData(u8 *DatBuf, u32 len);
void SendConsoleByte(char d);
void SendConsoleStr(char *d);
void SendConsoleData(char *d, u16 len);

#define USART1_EN 1
#define USART3_EN 1

#define EN_USART1_RX 1
#define EN_USART3_RX 1

extern u8 USART3_RX_BUF[USART_REC_LEN];   
extern u8 USART3_TX_CNT;       //发送计数
extern u8 USART3_RX_CNT;       //接收计数
extern u16 USART3_REC_Status;  //接收完成标志位


extern u8 USART1_RX_BUF[USART_REC_LEN];   //接收缓冲,最大USART_REC_LEN个字节.
extern u8 USART1_TX_CNT;       //发送计数
extern u8 USART1_RX_CNT;       //接收计数
extern u16 USART1_REC_Status;  //接收完成标志位


extern volatile unsigned char UARTtoConsole_RxBuffer[100];
extern volatile unsigned char UARTtoConsole_RxBufferWr ; 
extern volatile unsigned char UARTtoConsole_RxBufferRd ;

#endif

