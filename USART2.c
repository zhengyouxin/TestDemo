#include "USART2.h"



void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
#if USART2_EN

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//GPIOA时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART2时钟使能

	//USART2_TX   GPIOA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART2_RX	  GPIOA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //USART配置 
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  	USART_Init(USART2, &USART_InitStructure); 		//初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开始串口接收中断
  
  	USART_Cmd(USART2, ENABLE);
    
#endif
}

//-------------------------------------------------
//char SendUartByte (u16 Sendbyte,USART_TypeDef* Uart)
//{	// send a byte down a uart

//    while(USART_GetFlagStatus(Uart, USART_FLAG_TXE) == RESET)		   //wait for send out
//    {
//    }
//    USART_SendData(Uart,Sendbyte);
//    return 1;
//}

//char SendUartStr(const char *s, USART_TypeDef* Uart)
//{	// send a string from rom
//	char	c;

//	if (!s) return 1;

//	for (;*s;)
//	{
//		c = *s++;
//		if (c == '\n')
//	 	{	// insert a CR (\r) before the LF (\n)
//  			if (!SendUartByte('\r', Uart)) return 0;				// Send byte
//		}
//		if (!SendUartByte(c, Uart)) return 0;						// Send byte
//	}

//	return 1;
//}

//char SendUartData(char *s, u16 len, USART_TypeDef* Uart)
//{	// Send data down a uart
//	char	c;

//	if (!s) return 1;

//	for (; len; len--)
//	{
//		c = *s++;
//		if (!SendUartByte(c, Uart)) return 0;					// Send byte
//	}
//	return 1;
//}

//void SendUSART1Data(u8 *d, u16 len)
//{	// send data from ram
//	SendUartData(d, len,USART1);
//}

//void SendUSART2Data(u8 *d, u16 len)
//{	// send data from ram
//	SendUartData(d, len,USART2);
//}

//void SendUSART2Str(char *d)
//{
//	SendUartStr(d,USART2);
//}

#if EN_USART2_RX

u8 USART2_RX_BUF[USART2_REC_LEN];   
u8 USART2_TX_CNT;       //发送计数
u8 USART2_RX_CNT;       //接收计数
u16 USART2_REC_Status;  //接收完成标志位

#endif 

void USART2_IRQHandler(void)
{
    u8 res;
    
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//清除中断标志
        
		res = USART_ReceiveData(USART2);
        
        USART2_RX_BUF[USART2_RX_CNT] = res;
        
		USART2_RX_CNT ++;
    }
    
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
		USART_ReceiveData(USART2); //读DR
	}
}



