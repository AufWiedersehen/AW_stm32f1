#include "adc.h"
#include "delay.h"
void ADC_Config()
{
	/*************     时钟初始化（包括ADC/GPIO/ADC分频时钟） ***********/	
	RCC_APB2PeriphClockCmd(ADC_RCC_APB2Periph_GPIOx|ADC_RCC_APB2Periph_ADCx,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef   GPIO_InitStructure;
  ADC_InitTypeDef	   ADC_InitStructure;
 /*************   GPIO初始化  ***********/		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=ADC_GPIO_Pin_x;
	GPIO_Init(ADC_GPIOx, &GPIO_InitStructure);
	
	ADC_DeInit(ADC_x);
	/*************   ADC初始化  ***********/	
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC_x,ENABLE);//使能ADCx
	ADC_Calibrate(ADC_x); //校准ADC（直接调用）
}



void ADC_Calibrate(ADC_TypeDef* ADCx)        //校准ADC
{
	ADC_ResetCalibration(ADCx);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADCx));	//等待复位校准结束
	ADC_StartCalibration(ADCx);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADCx));	 //等待校准结束
}


uint16_t Acquire_ADC(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
  ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADCx); //返回最近一次ADC1规则组的转换结果	
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
}                                              //获取ADC较准确数据


void Read_Voltage()
{
	float  temp;
	uint16_t adc_data;	
	adc_data=ADCdata_Average(10);
	temp=(float)adc_data*(3.3/4096);	
	adc_data=temp; //整数部分
  temp-=adc_data;
  temp*=1000;   //小数部分
}                                         //adc读取电压



/*  https://blog.csdn.net/human_eating_potato/article/details/81069142
    
   ADC_Init(&ADC_InitStructure)

 { uint32_t ADC_Mode;                        (一般选择独立模式，ADC之间不存在联系)          
   FunctionalState ADC_ScanConvMode;        （多通道同时读取数据时使用）
   FunctionalState ADC_ContinuousConvMode;   （连续转换/单次转换）
   uint32_t ADC_ExternalTrigConv;             （触发方式/软件或者是IT触发等）
   uint32_t ADC_DataAlign;                    （左对齐或者右对齐）
   uint8_t ADC_NbrOfChannel;                   （初始化ADCx开启的通道数）
  }ADC_InitTypeDef；

*/        


/*

  ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
 
  ADC_Channel   为目标ADC通道编号                    如：ADC_Channel_0  总共有： ADC_Channel_0—ADC_Channel_17（16个外部，2个内部）
  
	Rank          为该通道在顺序规则转换的队伍中的序号   当开启几个通道时（ADC_NbrOfChannel！=1），转换通道的顺序
  
 ADC_SampleTime 用于独立地选择每个通道的采样时间    
*/
