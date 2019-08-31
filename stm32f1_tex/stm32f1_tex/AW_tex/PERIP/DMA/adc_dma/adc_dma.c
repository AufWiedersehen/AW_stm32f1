#include "adc_dma.h"

uint8_t ADC_DMA_Buff[];  //memory_adrr

void Adc_DMA_GPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //GPIO ʱ��
/*************  Adc_DMA GPIO��ʼ��  ***********/		
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init( GPIOA,&GPIO_InitStructure);    //PA6 CH6
}

void Adc_DMA_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //ADCʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   //DMAʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //ADCԤ��Ƶʱ��
	
	Adc_DMA_GPIO();  //GPIO
	
	ADC_InitTypeDef   ADC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	ADC_DeInit(ADC1);
	
/*************    ADC��ʼ��   ***********/	
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);    //ʹ��adc
	
	ADC_DMA_Calibrate(ADC1); //У׼adc
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_239Cycles5); //adcͨ����ʼ��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //�������ʹ��
	
	DMA_DeInit(DMA1_Channel1);
	
/*************    DMA��ʼ��   ***********/	
	DMA_InitStructure.DMA_PeripheralBaseAddr =( uint32_t ) ( & ( ADC1->DR ) ) ;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =( uint32_t )ADC_DMA_Buff ;         //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;         //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize =50 ;             //DMAͨ����DMA����Ĵ�С
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;               //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //���ݿ��Ϊ8λ
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;          //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;         //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1,ENABLE);    // DMA�ж�ʹ��  DMA_IT_TC���������   DMA_IT_HT������һ��  DMA_IT_TE���������
	
/*************    NVIC��ʼ��    ***********/	
	NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
}

void Adc_DMA_Mode()                          //Adc_DMA_Mode�򿪣�ֱ�ӵ��ã�
{
	ADC_DMA_CMD(DMA1_Channel1);
	ADC_DMACmd(ADC1,ENABLE);
}


void ADC_DMA_Calibrate(ADC_TypeDef* ADCx)        //У׼ADC
{
	ADC_ResetCalibration(ADCx);	//ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADCx));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADCx);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADCx));	 //�ȴ�У׼����
}


void ADC_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx)  //ʹ��adc_dmaģʽ
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

