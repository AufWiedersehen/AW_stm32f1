#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

#define RCC_APB1Periph_TIMx    RCC_APB1Periph_TIM2   //定时器时钟
#define TIM_x                  TIM3                 //定时器

void TIMx_Init(uint16_t pres ,uint16_t peri);

#endif
