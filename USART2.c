#include "USART2.h"



void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
#if USART2_EN

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//GPIOAʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART2ʱ��ʹ��

	//USART2_TX   GPIOA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART2_RX	  GPIOA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //USART���� 
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ�����ݿ�����
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  	USART_Init(USART2, &USART_InitStructure); 		//��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//��ʼ���ڽ����ж�
  
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
u8 USART2_TX_CNT;       //���ͼ���
u8 USART2_RX_CNT;       //���ռ���
u16 USART2_REC_Status;  //������ɱ�־λ

#endif 

void USART2_IRQHandler(void)
{
    u8 res;
    
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//����жϱ�־
        
		res = USART_ReceiveData(USART2);
        
        USART2_RX_BUF[USART2_RX_CNT] = res;
        
		USART2_RX_CNT ++;
    }
    
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR
		USART_ReceiveData(USART2); //��DR
	}
}



