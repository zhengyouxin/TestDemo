#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"

void led_init(void);

#define ledH GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define ledL GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define led0 PCout(0)

#endif
