#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "exit.h"
#include "tim.h"
#include "pwm.h"
#include "capture.h"
#include "adc.h"
#include "usart_dma.h"
#include "i2c.h"
#include "adc_dma.h"

#define period_capture    65535  //输入捕获周期（uint16_t pres决定）  2^16

#define  SEND_BUFF_DMA   50
uint8_t DMA_SendBuff[SEND_BUFF_DMA];// USAART_DMA

 int main(void)
 {	 	
	 uint16_t len;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 UARTx_Init(115200);       //usart1
	 IIC_GPIO_Config();        //IIC_GPIO
	 TIMx_Init(4999 ,7199);    //TIM3_EXTI
	 Exit_Init();              //exti
	 TIMx_PWM_Init(2999,7199); //TIM2_PWM
	 Capture_Init(0xffff,71);  //TIM4_capture
   delay_init();
   UART_DMA_Init(DMA1_Channel4,(u32)&USART1->DR,(u32)DMA_SendBuff,SEND_BUFF_DMA);//dam_1
	 Adc_DMA_GPIO();  //dma2
	 Adc_DMA_Init();  //dma2  
	 while(1)
	{
 /*******   usart  ************/	 
		len=USART_STA&0x3fff; //发送字长
		USATx_Send(USART_BUF,len);		
 /*******   pwm  ************/			
		Breathe_Led();		
 /*******  capture_up  ************/
    Amount_capture(period_capture);		
 /*******  ADC_transition  *********/	
//		uint16_t ADC_Buf=0;
//		ADC_Buf=ADCdata_Average(10);	
 /*******  USART_DMA  *********/
		UART_DMA_MODE(); //开启dma  memory--usart
		
		Adc_DMA_Mode();  //Adc_DMA_Mode打开		
		
	}
 }
