#include "exit.h"
#include "delay.h"
void Exit_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//IO口/外设时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);    //外部中断映射关系
	
	GPIO_InitTypeDef  GPIO_Initstucture;
  EXTI_InitTypeDef	EXTI_Initstucture;
	NVIC_InitTypeDef  NVIC_Initstucture;
/*************    exit  GPIO初始化  ***********/	
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Initstucture.GPIO_Pin=GPIO_Pin_12;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstucture);               //PB12
	
/*************    exit  参数初始化  ***********/		
	EXTI_Initstucture.EXTI_Line=EXTI_Line12;
	EXTI_Initstucture.EXTI_LineCmd=ENABLE;
	EXTI_Initstucture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_Initstucture.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_Initstucture);
	
/*************    exit  中断初始化  ***********/		
	NVIC_Initstucture.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_Initstucture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstucture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_Initstucture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_Initstucture);
}


void EXTI15_10_IRQHandler()
{
	if(EXTI_GetFlagStatus(EXTI_Line12)!=RESET)
	{
		delay_ms(100);
		if(KEY==0)
		{
			
		}		
	}
}

