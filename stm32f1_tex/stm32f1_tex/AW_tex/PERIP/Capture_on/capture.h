#ifndef __CAPTURE__H
#define __CAPTURE__H

#include "stm32f10x.h"

#define CAP_RCC_APB2Periph_GPIOx         RCC_APB2Periph_GPIOB
#define CAP_RCC_APB1Periph_TIMx          RCC_APB1Periph_TIM4
#define CAP_GPIOx                        GPIOB
#define CAP_GPIO_Pin_x                   GPIO_Pin_8
#define CAP_TIMx                         TIM4 
                                                      //pb8
void Capture_Init(uint16_t pres ,uint16_t peri);
uint16_t Amount_capture(uint16_t period);

#endif
