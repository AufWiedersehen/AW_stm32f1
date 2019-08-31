#include "tim.h"

void TIMx_Init(uint16_t pres ,uint16_t peri)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx,ENABLE);
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitstucture;
	NVIC_InitTypeDef         NVIC_Initstucture;
	
/**************  TIM参数配置  *****************/		
	TIM_TimeBaseInitstucture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitstucture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitstucture.TIM_Period=peri;
	TIM_TimeBaseInitstucture.TIM_Prescaler=pres;
	TIM_TimeBaseInit(TIM_x,&TIM_TimeBaseInitstucture);
	
	TIM_ITConfig(TIM_x,TIM_IT_Update,ENABLE);
/************** 中断参数配置  *****************/	
	NVIC_Initstucture.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_Initstucture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstucture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_Initstucture.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_Initstucture);
	
	TIM_Cmd(TIM_x,ENABLE);
}


void TIM_x_IRQHandler()
{
	if ((TIM_GetITStatus(TIM_x,TIM_IT_Update)!=RESET))
	{
		
				
	}
	
	TIM_ClearITPendingBit(TIM_x,TIM_IT_Update);
}

