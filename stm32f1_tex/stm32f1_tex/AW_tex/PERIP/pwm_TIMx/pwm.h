#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

#define  TIMx_pwm                    TIM2
#define  RCC_APB1Periph_TIMx_pwm     RCC_APB1Periph_TIM2
#define  RCC_APB2Periph_GPIOx_pwm    RCC_APB2Periph_GPIOA
#define  GPIOx_pwm                   GPIOA
#define  CH1_GPIO_Pin_x_pwm          GPIO_Pin_0
#define  CH2_GPIO_Pin_x_pwm          GPIO_Pin_1
#define  CH3_GPIO_Pin_x_pwm          GPIO_Pin_2
#define  CH4_GPIO_Pin_x_pwm          GPIO_Pin_3
                                                  //PA0/PA1/PA2/PA3
void TIMx_PWM_Init(uint16_t pres ,uint16_t peri );
void Breathe_Led(void);
#endif

