#include "capture.h"

void Capture_Init(uint16_t pres ,uint16_t peri)
{
	RCC_APB2PeriphClockCmd(CAP_RCC_APB2Periph_GPIOx,ENABLE); //GPIOʱ��
	RCC_APB1PeriphClockCmd(CAP_RCC_APB1Periph_TIMx,ENABLE);  //TIMʱ��
	
	GPIO_InitTypeDef         GPIO_Initstracture; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitstructure;
	TIM_ICInitTypeDef        TIM_ICInitstructure;
	NVIC_InitTypeDef         NVIC_Initsturcture;
	
 /*************   CAP_GPIO��ʼ��  ***********/
	GPIO_Initstracture.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Initstracture.GPIO_Pin=CAP_GPIO_Pin_x;
	GPIO_Initstracture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(CAP_GPIOx,&GPIO_Initstracture);
	
	/*************  CAP_TIM������ʼ��  ***********/
	TIM_TimeBaseInitstructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitstructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitstructure.TIM_Period=peri;
	TIM_TimeBaseInitstructure.TIM_Prescaler=pres;
	TIM_TimeBaseInit(CAP_TIMx,&TIM_TimeBaseInitstructure);
	/*************  CAP_����ͨ��������ʼ��  ***********/
	TIM_ICInitstructure.TIM_Channel=TIM_Channel_3;  //���ֶ�����ͨ����
	TIM_ICInitstructure.TIM_ICFilter=TIM_ICSelection_DirectTI;
	TIM_ICInitstructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitstructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitstructure.TIM_ICSelection=0x00;
	TIM_ICInit(CAP_TIMx,&TIM_ICInitstructure);
	
	TIM_ITConfig(CAP_TIMx,TIM_IT_CC3|TIM_IT_Update,ENABLE); //�����ж�  ���ֶ�����ͨ����
	
		/*************  CAP_NVIC�жϲ�����ʼ��  ***********/
	NVIC_Initsturcture.NVIC_IRQChannel=TIM4_IRQn;       //���ֶ�����ͨ����
	NVIC_Initsturcture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initsturcture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initsturcture.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_Initsturcture);
	
	TIM_Cmd(CAP_TIMx,ENABLE);  //ʹ��ʱ��
}

uint8_t  CAPTURE_STA;   //״̬λ����λ ��1��ɲ��񣬸߶�λ ��1 ����ߵ�ƽ  ��6λ���������
uint16_t CAPTURE_CRT;   //�Ӹߵ�ƽ���͵�ƽʱ�����

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_CC3)!=RESET); //������ƽ�ж�
	{
		if(CAPTURE_STA&0x40) //���񵽵͵�ƽ
		{
			CAPTURE_STA|=0x80; //������
			CAPTURE_CRT=TIM_GetCapture3(TIM4);//�ǵ�ƽ����ʱ��
			TIM_OC3PolarityConfig(TIM4,TIM_OCPolarity_High); //���øߵ�ƽ����
		}
		else               //���񵽸ߵ�ƽ
		{
			CAPTURE_CRT=0;   //��������0����ʼ����
			CAPTURE_STA=0;
   		CAPTURE_STA|=0x40;//��ǲ�׽���ߵ�ƽ
			TIM_SetCounter(TIM4,0);
			TIM_OC3PolarityConfig(TIM4,TIM_OCPolarity_Low);//���õ͵�ƽ����
		}
	 }	
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)   //ʱ��������������ж�
	{
		if((CAPTURE_STA&0x80)==RESET)    //û�����
		{
			if(CAPTURE_STA&0x40)           //�иߵ�ƽ��־
			{
				if((CAPTURE_STA&0x3f)==0x3f) //����ʱ�䳬������ռ�
				{
					CAPTURE_STA|=0xa80; //��־����ɹ�
					CAPTURE_CRT=0xffff;
				}
				else 
					CAPTURE_STA++;   //������ڵ�ƽʱ�䣬����
		  }
	  }
   } 
	 TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC3); //�����־λ
}


uint16_t  Amount_capture(uint16_t period)
{                                                  //������ʱ��=�������*1/f+�ߵ�ƽ����ʱ��  f�ɶ�ʱ������
	uint16_t  Capture_Count;
	if(CAPTURE_STA&0x80)
	{
		Capture_Count=(CAPTURE_STA&0x3f)*period;
		Capture_Count+=CAPTURE_CRT;
	}
	return Capture_Count;
}    
                                  
