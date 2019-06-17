/**
  ******************************************************************************
  **˵����003��Ŀд�����
  **���ܣ����⿨д��
  **���ߣ�Bruce
  **���ڣ�2017.6.8
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

    RCC_Configuration();    //ϵͳʱ������
    
    NVIC_Configuration();   //�ж���������
    
    IWDG_Init(4,625);   //(4*2^4)*625/40 ms = 1s ���Ź���ʱ1s
    IWDG_Feed();
    
    TIM3_Int_Init(9,7199);
    
    led_init();             //LED IO����
    
	USART_Configuration();  //����1��3��ʼ������  ����1������Ϊ115200 ����3������Ϊ4800
    
    USART2_Init(115200);    //����2��ʼ�� ������Ϊ115200 Debug
    
    NRF24L01_Init();        //24L01 IO����
    
    IWDG_Feed();
    while(NRF24L01_Check(SPI_REV)){	    //���NRF24L01 1�Ƿ����
		printf("The 2.4G Error!");
	}
    
    while(NRF24L01_Check(SPI_SEND)){	    //���NRF24L01 2�Ƿ����
		printf("The 2.4G Error!");
	}
    
    Data_Init();   //�����ݳ�ʼ��
    
    IWDG_Feed();
    led0=~led0;
    delay_ms(200);
    led0=~led0;
    
    IWDG_Feed();
	printf("\r\nPCд�������������\r\n");
    
//    printf("System Clock = %d\r\n",SystemCoreClock);
    
//    NRF24L01_TX_Mode(SPI_SEND);
    NRF24L01_TX_Mode(SPI_REV);
    
  /* Infinite loop */
    while (1)
    {
        
        DataProc_USART1();      //����1���ݴ�����PC�����ͨ��
        
        DataProc_1356();        //13.56M���ݴ���
        
        DataProc_Nrf24l01();    //24L01���ݴ���
//        DataProc_Nrf24l01_2();
        
//        DataProc_RecNrf24L01();
        
    }
}



void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                   //ö�ٱ������������ʱ�ӵ�����״̬
	RCC_DeInit();                                   //RCCϵͳ���ã�����Debug
	RCC_HSEConfig(RCC_HSE_ON);                      //ʹ�ܸ���ʱ��ԴHSE  
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     //�ȴ�HSE�ȶ�
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

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //���ô���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //���ô���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //���ô���1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //���ö�ʱ��3ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ��
	NVIC_Init(&NVIC_InitStructure);
    
}

/*
*���Ź���ʼ������
*prer:��Ƶ����0-7����3λ��Ч
*��Ƶ���ӣ�4*2^prer�����256
*rlr:��װ�ؼĴ���ֵ����11λ��Ч
*Tout=((4*2^prer)*rlr)/40 (ms)
*/
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR�Ĳ���
    IWDG_SetPrescaler(prer);    //����IWDGԤ��Ƶֵ:prer
    IWDG_SetReload(rlr);        //����IWDG����ֵ
    IWDG_ReloadCounter();       //��������ֵ������IWDG������
    IWDG_Enable();              //ʹ��IWDG
}

/**
 * ι��
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
