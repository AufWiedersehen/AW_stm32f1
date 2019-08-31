#ifndef __USART_DMA_H
#define __USART_DMA_H

#include "stm32f10x.h"
#include "usart.h"

void UART_DMA_MODE(void);
void UART_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx);

/**
  * @brief      UART_DMA_Init����
  * @parameter  DMA_CHx             (DMAͨ��  �磺DMA1_Channel4) 
  *             Peripheral_Adrr     (�����ַ  ��:(u32)&USART1->DR )
	*             Memory_Adrr         (�ڴ��ַ  ��:(u32)DMA_SendBuff)
	*             Buff_SIZE           ���������ݸ�����
  */
void UART_DMA_Init(DMA_Channel_TypeDef* DMA_CHx,uint32_t Peripheral_Adrr,uint32_t Memory_Adrr,uint8_t Buff_SIZE);

#endif
