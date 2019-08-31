#ifndef __ADC_DMA_H
#define __ADC_DMA_H

#include "stm32f10x.h"

void Adc_DMA_GPIO(void);
void Adc_DMA_Init(void);
void Adc_DMA_Mode(void);
void ADC_DMA_Calibrate(ADC_TypeDef* ADCx);
void ADC_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx);

#endif
