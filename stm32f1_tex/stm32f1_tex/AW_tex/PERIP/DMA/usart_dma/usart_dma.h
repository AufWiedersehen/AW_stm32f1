#ifndef __USART_DMA_H
#define __USART_DMA_H

#include "stm32f10x.h"
#include "usart.h"

void UART_DMA_MODE(void);
void UART_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx);

/**
  * @brief      UART_DMA_Init（）
  * @parameter  DMA_CHx             (DMA通道  如：DMA1_Channel4) 
  *             Peripheral_Adrr     (外设地址  如:(u32)&USART1->DR )
	*             Memory_Adrr         (内存地址  如:(u32)DMA_SendBuff)
	*             Buff_SIZE           （发送数据个数）
  */
void UART_DMA_Init(DMA_Channel_TypeDef* DMA_CHx,uint32_t Peripheral_Adrr,uint32_t Memory_Adrr,uint8_t Buff_SIZE);

#endif
