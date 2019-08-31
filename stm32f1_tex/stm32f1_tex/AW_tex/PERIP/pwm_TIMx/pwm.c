#include "pwm.h"

void TIMx_PWM_Init(uint16_t pres ,uint16_t peri)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx_pwm,ENABLE);  //TIM 时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_pwm,ENABLE); //GPIO时钟使能
	
	GPIO_InitTypeDef         GPIO_Initstucture;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitstucture;
	TIM_OCInitTypeDef        TIM_OCInitstructure;
	
/**************  PWM_CH1_GPIO配置  *****************/	
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=CH1_GPIO_Pin_x_pwm;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_pwm,&GPIO_Initstucture);
	
/**************  PWM_CH2_GPIO配置  *****************/		
/*
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=CH2_GPIO_Pin_x_pwm;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_pwm,&GPIO_Initstucture);
*/
/**************  PWM_CH3_GPIO配置  *****************/		
/*	
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=CH3_GPIO_Pin_x_pwm;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_pwm,&GPIO_Initstucture);
*/	
/**************  PWM_CH4_GPIO配置  *****************/	
/*
  GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=CH4_GPIO_Pin_x_pwm;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_pwm,&GPIO_Initstucture);
*/
/**************  PWM_TIMx配置  *****************/		
	TIM_TimeBaseInitstucture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitstucture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitstucture.TIM_Period=peri;
	TIM_TimeBaseInitstucture.TIM_Prescaler=pres;
	TIM_TimeBaseInit(TIMx_pwm,&TIM_TimeBaseInitstucture);

/**************  PWM_CH1_mode配置  *****************/	
  TIM_OCInitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIMx_pwm,&TIM_OCInitstructure);
	
	TIM_OC1PolarityConfig(TIMx_pwm,TIM_OCPolarity_High); //CH1 预装载使能
	
/**************  PWM_CH2_mode配置  *****************/		
/*TIM_OCInitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIMx_pwm,&TIM_OCInitstructure);
	
	TIM_OC2PolarityConfig(TIMx_pwm,TIM_OCPolarity_High); //CH2 预装载使能
*/
/**************  PWM_CH3_mode配置  *****************/	
/*TIM_OCInitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3Init(TIMx_pwm,&TIM_OCInitstructure);
	
	TIM_OC3PolarityConfig(TIMx_pwm,TIM_OCPolarity_High); //CH3 预装载使能
*/	
/**************  PWM_CH4_mode配置  *****************/	
/*TIM_OCInitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC4Init(TIMx_pwm,&TIM_OCInitstructure);
	
	TIM_OC4PolarityConfig(TIMx_pwm,TIM_OCPolarity_High); //CH4 预装载使能
*/

	TIM_ARRPreloadConfig(TIMx_pwm,ENABLE); //自动重装载使能
}



void Breathe_Led()
{
	unsigned char border;
	uint8_t led_arr;
	int pwm_tim=301;

	if(border)   led_arr++;
	 else        led_arr--;
	
	   if(led_arr==pwm_tim) border=0;
	   if(led_arr==0)       border=1;
	
	  TIM_SetCompare1(TIMx_pwm,led_arr); //CH1_Compare(占空比)

/*  TIM_SetCompare2(TIMx_pwm,led_arr); //CH2_Compare(占空比)
	  TIM_SetCompare3(TIMx_pwm,led_arr); //CH3_Compare(占空比)
	  TIM_SetCompare4(TIMx_pwm,led_arr); //CH4_Compare(占空比)
*/
}


