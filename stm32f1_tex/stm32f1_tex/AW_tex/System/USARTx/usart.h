/* 
****  fuction: usart
****  writer : AW
****  time   : 2019.8.28
*/
#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"

//     USART1
#define USARTX  USART1

#define USART_RCC_APB2Periph_GPIOx       RCC_APB2Periph_GPIOA
#define USART_GPIO                       GPIOA       
#define USART_PIN_TX                     GPIO_Pin_9    //TX
#define USART_PIN_RX                     GPIO_Pin_10   //rx
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler


/*    USART2

#define USARTX  USART2

#define RCC_APB2Periph_GPIOx        RCC_APB2Periph_GPIOA
#define USART_GPIO                  GPIOA       
#define USART_PIN_TX                GPIO_Pin_2    //TX
#define USART_PIN_RX                GPIO_Pin_3    //rx
#define USARTx_IRQn                 USART2_IRQn
#define USARTx_IRQHandler           USART2_IRQHandler
*/



/*   USART3

#define USARTX  USART3

#define RCC_APB2Periph_GPIOx        RCC_APB2Periph_GPIOB
#define USART_GPIO                  GPIOA       
#define USART_PIN_TX                GPIO_Pin_10    //TX
#define USART_PIN_RX                GPIO_Pin_11    //rx
#define USARTx_IRQn                 USART3_IRQn
#define USARTx_IRQHandler           USART3_IRQHandler
*/

struct __FILE 
{ 
	int handle; 

}; 
/* FILE is typedef�� d in stdio.h. */   

extern uint8_t USART_BUF[];
extern uint16_t USART_STA;
void UARTx_Init(uint32_t bound);
void USATx_Send(uint8_t *data_buf,uint16_t len);

#endif

/*****
USART_ClearFlag()     /    USART_ClearITPendingBit()  //���������־λ��������ڲ���//״̬��������ڲ���

#define USART_FLAG_CTS     //CTS��־λ                   
#define USART_FLAG_LBD     //LIN�жϼ���־λ                
#define USART_FLAG_TXE     //�������ݼĴ����ձ�־λ                
#define USART_FLAG_TC      //������ɱ�־λ                  
#define USART_FLAG_RXNE    //�������ݼĴ����ǿձ�־λ                
#define USART_FLAG_IDLE    //�������߱�־λ                  
#define USART_FLAG_ORE     //��������־λ                
#define USART_FLAG_NE      //���������־λ                 
#define USART_FLAG_FE      //֡�����־λ               
#define USART_FLAG_PE      //��ż�����־λ                  

*****/	


