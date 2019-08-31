#include "adc.h"
#include "delay.h"
void ADC_Config()
{
	/*************     ʱ�ӳ�ʼ��������ADC/GPIO/ADC��Ƶʱ�ӣ� ***********/	
	RCC_APB2PeriphClockCmd(ADC_RCC_APB2Periph_GPIOx|ADC_RCC_APB2Periph_ADCx,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef   GPIO_InitStructure;
  ADC_InitTypeDef	   ADC_InitStructure;
 /*************   GPIO��ʼ��  ***********/		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=ADC_GPIO_Pin_x;
	GPIO_Init(ADC_GPIOx, &GPIO_InitStructure);
	
	ADC_DeInit(ADC_x);
	/*************   ADC��ʼ��  ***********/	
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC_x,ENABLE);//ʹ��ADCx
	ADC_Calibrate(ADC_x); //У׼ADC��ֱ�ӵ��ã�
}



void ADC_Calibrate(ADC_TypeDef* ADCx)        //У׼ADC
{
	ADC_ResetCalibration(ADCx);	//ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADCx));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADCx);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADCx));	 //�ȴ�У׼����
}


uint16_t Acquire_ADC(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
  ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADCx); //�������һ��ADC1�������ת�����	
}

uint16_t ADCdata_Average(uint8_t times)
{
	uint8_t   i=0;
	uint16_t  adc_data;
	for(;i<times;i++)
	{
		adc_data+=Acquire_ADC(ADC_x,ADC_Channel_5);
		delay_ms(5);
	}
	return adc_data/times;
}                                              //��ȡADC��׼ȷ����


void Read_Voltage()
{
	float  temp;
	uint16_t adc_data;	
	adc_data=ADCdata_Average(10);
	temp=(float)adc_data*(3.3/4096);	
	adc_data=temp; //��������
  temp-=adc_data;
  temp*=1000;   //С������
}                                         //adc��ȡ��ѹ



/*  https://blog.csdn.net/human_eating_potato/article/details/81069142
    
   ADC_Init(&ADC_InitStructure)

 { uint32_t ADC_Mode;                        (һ��ѡ�����ģʽ��ADC֮�䲻������ϵ)          
   FunctionalState ADC_ScanConvMode;        ����ͨ��ͬʱ��ȡ����ʱʹ�ã�
   FunctionalState ADC_ContinuousConvMode;   ������ת��/����ת����
   uint32_t ADC_ExternalTrigConv;             ��������ʽ/���������IT�����ȣ�
   uint32_t ADC_DataAlign;                    �����������Ҷ��룩
   uint8_t ADC_NbrOfChannel;                   ����ʼ��ADCx������ͨ������
  }ADC_InitTypeDef��

*/        


/*

  ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
 
  ADC_Channel   ΪĿ��ADCͨ�����                    �磺ADC_Channel_0  �ܹ��У� ADC_Channel_0��ADC_Channel_17��16���ⲿ��2���ڲ���
  
	Rank          Ϊ��ͨ����˳�����ת���Ķ����е����   ����������ͨ��ʱ��ADC_NbrOfChannel��=1����ת��ͨ����˳��
  
 ADC_SampleTime ���ڶ�����ѡ��ÿ��ͨ���Ĳ���ʱ��    
*/
