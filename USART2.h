#ifndef _USART2_H
#define _USART2_H
#include "stm32f10x.h"

void USART2_Init(u32 bound);
void SendUSART2Str(char *d);
void SendUSART1Data(u8 *d, u16 len);
void SendUSART2Data(u8 *d, u16 len);



#define USART2_EN 1
#define EN_USART2_RX 1

#define  USART2_REC_LEN   64


#endif

