/**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: printf.C
**
**------------------------------------------------------------------------------------------------------*/
/******************************************************************
       波特率              9600
       校验位               无
       数据位               8
       停止位               1
       流控制              无
*******************************************************************/

#include "printf.h"

#define PRINTF_EN

volatile unsigned char UARTtoConsole_RxBuffer[100];
volatile unsigned char UARTtoConsole_RxBufferWr = 0; 
volatile unsigned char UARTtoConsole_RxBufferRd = 0;


extern u8 Flag_update;

/*下面是USART模块的初始化*/

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : USART设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
    
#if USART1_EN
	/* Enable the USART Clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1| RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
/************************************************************************/

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开始串口1接收中断

	USART_Cmd(USART1, ENABLE);
#endif

#if USART3_EN
    
/* Enable the USART Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//USART3_TX   GPIOB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB10
	   
	  //USART3_RX	  GPIOB11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.3  
/************************************************************************/

	USART_InitStructure.USART_BaudRate = 4800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);

    USART_Init(USART3, &USART_InitStructure); 		//初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开始串口接收中断
    
	USART_Cmd(USART3, ENABLE);
    
#endif

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC);  
	return;

}
/********************************************************************
函数功能：串口1发送数据
入口参数：DatBuf：要发送的数据缓存区。	 len:要发送的数据长度
返    回：无。
备    注：
********************************************************************/
void Usart1_SendData(u8 *DatBuf, u32 len)
{
  u32 i;
  for(i=0;i<len;i++)
  {
		/* Loop until the end of transmission */
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //检查发送是否完成
		/* Place your implementation of fputc here */
		/* e.g. write a character to the USART */
		USART_SendData(USART1, DatBuf[i]);
          
  }
}

//-------------------------------------------------
char SendUartByte (u16 Sendbyte,USART_TypeDef* Uart)
{	// send a byte down a uart

 while(USART_GetFlagStatus(Uart, USART_FLAG_TXE) == RESET)		   //wait for send out
 {
 }
 USART_SendData(Uart,Sendbyte);
 return 1;
}


char SendUartStr(char *s, USART_TypeDef* Uart)
{	// Send a string down a uart
	char	c;

	if (!s) return 1;
	for (;*s;)
	{																   
		c = *s++;
		if (c == '\n')
	 	{	// insert a CR (\r) before the LF (\n)
  			if (!SendUartByte('\r', Uart)) return 0;			// Send byte
		}
		if (!SendUartByte(c, Uart)) return 0;					// Send byte
	}  
	return 1;
}

char SendUartRStr(const char *s, USART_TypeDef* Uart)
{	// send a string from rom
	char	c;

	if (!s) return 1;

	for (;*s;)
	{
		c = *s++;
		if (c == '\n')
	 	{	// insert a CR (\r) before the LF (\n)
  			if (!SendUartByte('\r', Uart)) return 0;				// Send byte
		}
		if (!SendUartByte(c, Uart)) return 0;						// Send byte
	}

	return 1;
}

char SendUartData(char *s, u16 len, USART_TypeDef* Uart)
{	// Send data down a uart
	char	c;

	if (!s) return 1;

	for (; len; len--)
	{
		c = *s++;
		if (!SendUartByte(c, Uart)) return 0;					// Send byte
	}
	return 1;
}

void SendConsoleByte(char d)
{	// send data from ram
	SendUartByte(d, CONSOLE_UART);
}

void SendConsoleStr(char *d)
{
	SendUartStr(d,CONSOLE_UART);
}

void SendConsoleData(char *d, u16 len)
{	// send data from ram
	SendUartData(d, len, CONSOLE_UART);
}
/********************************************************************
函数功能：串口1接收数据
入口参数：DatBuf：要发送的数据缓存区。	 len:要发送的数据长度
返    回：无。
备    注：
********************************************************************/
//void Usart1_ReceiveData()
//{
//	
//	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET);
//	
//}

/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


#if EN_USART1_RX

u8 USART1_RX_BUF[USART_REC_LEN];   //接收缓冲,最大USART_REC_LEN个字节.
u8 USART1_TX_CNT;       //发送计数
u8 USART1_RX_CNT;       //接收计数
u16 USART1_REC_Status;  //接收完成标志位
u16 USART_APP_RX;   //APP接收长度计数
u8 USART1_RX_DAT[USART_APP_LEN] __attribute__ ((at(0X20001000)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20001000.

#endif

void USART1_IRQHandler(void)
{
	u8 res;
    
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {
//        UARTtoConsole_RxBuffer[UARTtoConsole_RxBufferWr++] = USART_ReceiveData(USART1);		// save new byte into the buffer
//        if (UARTtoConsole_RxBufferWr >= sizeof(UARTtoConsole_RxBuffer)) UARTtoConsole_RxBufferWr = 0;	// wap awound
//    }
    
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);			//清除中断标志
        
		res = USART_ReceiveData(USART1);
        
//        USART_SendData(USART2,res);
        
        if(Flag_update)
        {
            
            USART1_RX_DAT[USART_APP_RX] = res;
            
            USART_APP_RX ++;
        }
        else
        {
            USART1_RX_BUF[USART1_RX_CNT] = res;
            
            USART1_RX_CNT ++;
    
        }
        
    }
    
//    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) //溢出
//	{
//		USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR
//		USART_ReceiveData(USART1); //读DR
//	}
}

#if EN_USART3_RX
    u8 USART3_RX_BUF[USART_REC_LEN];   
    u8 USART3_TX_CNT;       //发送计数
    u8 USART3_RX_CNT;       //接收计数
    u16 USART3_REC_Status;  //接收完成标志位
#endif

void USART3_IRQHandler(void)
{
    u8 res;
    
    if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);			//清除中断标志
        
		res = USART_ReceiveData(USART3);
        
//        USART_SendData(USART2,res); //串口3接收到的数据通过串口2打印出来
        
        USART3_RX_BUF[USART3_RX_CNT] = res;
		USART3_RX_CNT ++;
		USART3_RX_CNT &= 0X0F;      //最大一帧接收15个位
    }
    
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET) //溢出
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE); //读SR
		USART_ReceiveData(USART3); //读DR
	}
}

#ifdef PRINTF_EN
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART1->DR = (u8)ch;

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	return ch;
}
#endif
