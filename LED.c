#include "LED.h"

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//PC0 LED0
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
    
	GPIO_SetBits(GPIOC,GPIO_Pin_0);     //初始为关闭LED
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			 //Switch 扫描枪开关  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
    
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);   //初始化为关
    
}



