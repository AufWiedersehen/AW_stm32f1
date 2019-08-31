#include "capture.h"

void Capture_Init(uint16_t pres ,uint16_t peri)
{
	RCC_APB2PeriphClockCmd(CAP_RCC_APB2Periph_GPIOx,ENABLE); //GPIO时钟
	RCC_APB1PeriphClockCmd(CAP_RCC_APB1Periph_TIMx,ENABLE);  //TIM时钟
	
	GPIO_InitTypeDef         GPIO_Initstracture; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitstructure;
	TIM_ICInitTypeDef        TIM_ICInitstructure;
	NVIC_InitTypeDef         NVIC_Initsturcture;
	
 /*************   CAP_GPIO初始化  ***********/
	GPIO_Initstracture.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Initstracture.GPIO_Pin=CAP_GPIO_Pin_x;
	GPIO_Initstracture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(CAP_GPIOx,&GPIO_Initstracture);
	
	/*************  CAP_TIM参数初始化  ***********/
	TIM_TimeBaseInitstructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitstructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitstructure.TIM_Period=peri;
	TIM_TimeBaseInitstructure.TIM_Prescaler=pres;
	TIM_TimeBaseInit(CAP_TIMx,&TIM_TimeBaseInitstructure);
	/*************  CAP_捕获通道参数初始化  ***********/
	TIM_ICInitstructure.TIM_Channel=TIM_Channel_3;  //（手动设置通道）
	TIM_ICInitstructure.TIM_ICFilter=TIM_ICSelection_DirectTI;
	TIM_ICInitstructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitstructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitstructure.TIM_ICSelection=0x00;
	TIM_ICInit(CAP_TIMx,&TIM_ICInitstructure);
	
	TIM_ITConfig(CAP_TIMx,TIM_IT_CC3|TIM_IT_Update,ENABLE); //开启中断  （手动设置通道）
	
		/*************  CAP_NVIC中断参数初始化  ***********/
	NVIC_Initsturcture.NVIC_IRQChannel=TIM4_IRQn;       //（手动设置通道）
	NVIC_Initsturcture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initsturcture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initsturcture.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_Initsturcture);
	
	TIM_Cmd(CAP_TIMx,ENABLE);  //使能时钟
}

uint8_t  CAPTURE_STA;   //状态位，高位 置1完成捕获，高二位 置1 捕获高电平  低6位计数溢出数
uint16_t CAPTURE_CRT;   //从高电平到低电平时间计数

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_CC3)!=RESET); //触发电平中断
	{
		if(CAPTURE_STA&0x40) //捕获到低电平
		{
			CAPTURE_STA|=0x80; //标记完成
			CAPTURE_CRT=TIM_GetCapture3(TIM4);//记电平持续时间
			TIM_OC3PolarityConfig(TIM4,TIM_OCPolarity_High); //设置高电平捕获
		}
		else               //捕获到高电平
		{
			CAPTURE_CRT=0;   //参数都置0，开始计算
			CAPTURE_STA=0;
   		CAPTURE_STA|=0x40;//标记捕捉到高电平
			TIM_SetCounter(TIM4,0);
			TIM_OC3PolarityConfig(TIM4,TIM_OCPolarity_Low);//设置低电平捕获
		}
	 }	
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)   //时间过长触发更新中断
	{
		if((CAPTURE_STA&0x80)==RESET)    //没有完成
		{
			if(CAPTURE_STA&0x40)           //有高电平标志
			{
				if((CAPTURE_STA&0x3f)==0x3f) //持续时间超过储存空间
				{
					CAPTURE_STA|=0xa80; //标志捕获成功
					CAPTURE_CRT=0xffff;
				}
				else 
					CAPTURE_STA++;   //如果还在电平时间，计数
		  }
	  }
   } 
	 TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC3); //清除标志位
}


uint16_t  Amount_capture(uint16_t period)
{                                                  //计算总时间=溢出次数*1/f+高电平捕获时间  f由定时器决定
	uint16_t  Capture_Count;
	if(CAPTURE_STA&0x80)
	{
		Capture_Count=(CAPTURE_STA&0x3f)*period;
		Capture_Count+=CAPTURE_CRT;
	}
	return Capture_Count;
}    
                                  
