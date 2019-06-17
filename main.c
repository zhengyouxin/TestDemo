/**
  ******************************************************************************
  **说明：003项目写码程序
  **功能：答题卡写码
  **作者：Bruce
  **日期：2017.6.8
  ******************************************************************************
**/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "printf.h"
#include "USART2.h"
#include "LED.h"
#include "delay.h"
#include "nrf24L01.h"
#include "DataProc.h"
#include "tim.h"



/* Functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);

void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
    SCB->VTOR = FLASH_BASE | 0x10000; /* Vector Table Relocation in Internal FLASH. */

    RCC_Configuration();    //系统时钟设置
    
    NVIC_Configuration();   //中断向量配置
    
    IWDG_Init(4,625);   //(4*2^4)*625/40 ms = 1s 看门狗定时1s
    IWDG_Feed();
    
    TIM3_Int_Init(9,7199);
    
    led_init();             //LED IO配置
    
	USART_Configuration();  //串口1、3初始化配置  串口1波特率为115200 串口3波特率为4800
    
    USART2_Init(115200);    //串口2初始化 波特率为115200 Debug
    
    NRF24L01_Init();        //24L01 IO配置
    
    IWDG_Feed();
    while(NRF24L01_Check(SPI_REV)){	    //检测NRF24L01 1是否存在
		printf("The 2.4G Error!");
	}
    
    while(NRF24L01_Check(SPI_SEND)){	    //检测NRF24L01 2是否存在
		printf("The 2.4G Error!");
	}
    
    Data_Init();   //包数据初始化
    
    IWDG_Feed();
    led0=~led0;
    delay_ms(200);
    led0=~led0;
    
    IWDG_Feed();
	printf("\r\nPC写码软件正在运行\r\n");
    
//    printf("System Clock = %d\r\n",SystemCoreClock);
    
//    NRF24L01_TX_Mode(SPI_SEND);
    NRF24L01_TX_Mode(SPI_REV);
    
  /* Infinite loop */
    while (1)
    {
        
        DataProc_USART1();      //串口1数据处理，与PC端软件通信
        
        DataProc_1356();        //13.56M数据处理
        
        DataProc_Nrf24l01();    //24L01数据处理
//        DataProc_Nrf24l01_2();
        
//        DataProc_RecNrf24L01();
        
    }
}



void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                   //枚举变量，定义高速时钟的启动状态
	RCC_DeInit();                                   //RCC系统重置，用于Debug
	RCC_HSEConfig(RCC_HSE_ON);                      //使能高速时钟源HSE  
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     //等待HSE稳定
	if(HSEStartUpStatus == SUCCESS)
	{	
		RCC_HCLKConfig(RCC_SYSCLK_Div1);            // HCLK = SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);             // PCLK2 = HCLK
		RCC_PCLK1Config(RCC_HCLK_Div2);             // PCLK1 = HCLK/2
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08){}
	}
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void NVIC_Configuration(void)
{              
	NVIC_InitTypeDef NVIC_InitStructure;   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //设置串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //设置定时器3全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能
	NVIC_Init(&NVIC_InitStructure);
    
}

/*
*看门狗初始化函数
*prer:分频数：0-7，低3位有效
*分频因子：4*2^prer，最大256
*rlr:重装载寄存器值，低11位有效
*Tout=((4*2^prer)*rlr)/40 (ms)
*/
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // 使能对寄存器IWDG_PR和IWDG_RLR的操作
    IWDG_SetPrescaler(prer);    //设置IWDG预分频值:prer
    IWDG_SetReload(rlr);        //设置IWDG重载值
    IWDG_ReloadCounter();       //按照设置值，重载IWDG计数器
    IWDG_Enable();              //使能IWDG
}

/**
 * 喂狗
 */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /*reload*/
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
