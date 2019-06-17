/**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: printf.h
**��   ��   ��: ���Ʒ�
**����޸�����: 
**��        ��: 
** �̼�����Ϣ��V3.4.0             
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ���Ʒ�
** ��  ��: v0.01
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾��Ϣ----------------------------------------------------------------------------
** ������: ���Ʒ�
** ��  ��: v0.01
** �ա���: 
** �衡��: ��ǰ�汾
**
**------------------------------------------------------------------------------------------------------*/

#ifndef PRINTF_H
#define PRINTF_H

#include "stm32f10x.h"
#include "stdio.h"
#include "printf.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART_APP_LEN  			20*1024 //�����������ֽ��� 20K

#define	CONSOLE_UART		USART1	 		//���Կ�

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
extern u8 USART3_TX_CNT;       //���ͼ���
extern u8 USART3_RX_CNT;       //���ռ���
extern u16 USART3_REC_Status;  //������ɱ�־λ


extern u8 USART1_RX_BUF[USART_REC_LEN];   //���ջ���,���USART_REC_LEN���ֽ�.
extern u8 USART1_TX_CNT;       //���ͼ���
extern u8 USART1_RX_CNT;       //���ռ���
extern u16 USART1_REC_Status;  //������ɱ�־λ


extern volatile unsigned char UARTtoConsole_RxBuffer[100];
extern volatile unsigned char UARTtoConsole_RxBufferWr ; 
extern volatile unsigned char UARTtoConsole_RxBufferRd ;

#endif

