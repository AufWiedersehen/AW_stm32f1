#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define ADC_RCC_APB2Periph_GPIOx   RCC_APB2Periph_GPIOA
#define ADC_RCC_APB2Periph_ADCx    RCC_APB2Periph_ADC1
#define ADC_GPIOx                  GPIOA
#define ADC_GPIO_Pin_x             GPIO_Pin_5
#define ADC_x                      ADC1
                                                           //GPIO:PA5  CH5
void ADC_Config(void);
void ADC_Calibrate(ADC_TypeDef* ADCx);
uint16_t Acquire_ADC(ADC_TypeDef* ADCx,uint8_t ADC_Channel);
uint16_t ADCdata_Average(uint8_t times);
void Read_Voltage(void);

#endif
