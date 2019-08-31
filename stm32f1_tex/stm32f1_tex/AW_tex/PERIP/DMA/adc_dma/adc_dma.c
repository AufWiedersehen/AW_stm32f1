#include "adc_dma.h"

uint8_t ADC_DMA_Buff[];  //memory_adrr

void Adc_DMA_GPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //GPIO 时钟
/*************  Adc_DMA GPIO初始化  ***********/		
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init( GPIOA,&GPIO_InitStructure);    //PA6 CH6
}

void Adc_DMA_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //ADC时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   //DMA时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //ADC预分频时钟
	
	Adc_DMA_GPIO();  //GPIO
	
	ADC_InitTypeDef   ADC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	ADC_DeInit(ADC1);
	
/*************    ADC初始化   ***********/	
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);    //使能adc
	
	ADC_DMA_Calibrate(ADC1); //校准adc
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_239Cycles5); //adc通道初始化
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //软件触发使能
	
	DMA_DeInit(DMA1_Channel1);
	
/*************    DMA初始化   ***********/	
	DMA_InitStructure.DMA_PeripheralBaseAddr =( uint32_t ) ( & ( ADC1->DR ) ) ;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr =( uint32_t )ADC_DMA_Buff ;         //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;         //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize =50 ;             //DMA通道的DMA缓存的大小
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;               //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //数据宽度为8位
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;          //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;         //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1,ENABLE);    // DMA中断使能  DMA_IT_TC：传输完成   DMA_IT_HT：传输一半  DMA_IT_TE：传输错误
	
/*************    NVIC初始化    ***********/	
	NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
}

void Adc_DMA_Mode()                          //Adc_DMA_Mode打开（直接调用）
{
	ADC_DMA_CMD(DMA1_Channel1);
	ADC_DMACmd(ADC1,ENABLE);
}


void ADC_DMA_Calibrate(ADC_TypeDef* ADCx)        //校准ADC
{
	ADC_ResetCalibration(ADCx);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADCx));	//等待复位校准结束
	ADC_StartCalibration(ADCx);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADCx));	 //等待校准结束
}


void ADC_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx)  //使能adc_dma模式
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,50);
	DMA_Cmd(DMA_CHx,ENABLE);		
}



void DMA1_Channel1_IRQHandler()
{
	   if(DMA_GetITStatus(DMA_IT_TC)!=RESET)
     DMA_ClearITPendingBit(DMA1_IT_TC1);
}

