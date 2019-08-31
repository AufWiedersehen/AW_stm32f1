#include "usart_dma.h"            

void UART_DMA_Init(DMA_Channel_TypeDef* DMA_CHx,uint32_t Peripheral_Adrr,uint32_t Memory_Adrr,uint8_t Buff_SIZE)
{
// UARTx_Init(115200); // ��������û�г�ʼ������
	 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  //DMAʱ��
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	DMA_DeInit(DMA_CHx);   // DMA1_Channel4  (usart1)
	
 /*************   DMA��ʼ��  ***********/
	DMA_InitStructure.DMA_PeripheralBaseAddr =Peripheral_Adrr ;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =Memory_Adrr ;         //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;         //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = Buff_SIZE;             //DMAͨ����DMA����Ĵ�С
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;               //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //���ݿ��Ϊ8λ
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;          //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;         //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx,&DMA_InitStructure);        //DMA1_Channel4
	
}


void UART_DMA_MODE()                     //����usart_dmaģʽ��memory-->usart_tx
{
	UART_DMA_CMD(DMA1_Channel4);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
}


void UART_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx)   //ʹ�ܿ���dmaģʽ
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,50);    //�ֶ�����SEND_SIZE(50)
	DMA_Cmd(DMA_CHx,ENABLE);		
}

