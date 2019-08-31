#include "usart_dma.h"            

void UART_DMA_Init(DMA_Channel_TypeDef* DMA_CHx,uint32_t Peripheral_Adrr,uint32_t Memory_Adrr,uint8_t Buff_SIZE)
{
// UARTx_Init(115200); // 主函数若没有初始化串口
	 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  //DMA时钟
	
	DMA_InitTypeDef  DMA_InitStructure;
	
	DMA_DeInit(DMA_CHx);   // DMA1_Channel4  (usart1)
	
 /*************   DMA初始化  ***********/
	DMA_InitStructure.DMA_PeripheralBaseAddr =Peripheral_Adrr ;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr =Memory_Adrr ;         //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;         //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = Buff_SIZE;             //DMA通道的DMA缓存的大小
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;               //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //数据宽度为8位
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;          //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;         //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx,&DMA_InitStructure);        //DMA1_Channel4
	
}


void UART_DMA_MODE()                     //开启usart_dma模式，memory-->usart_tx
{
	UART_DMA_CMD(DMA1_Channel4);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
}


void UART_DMA_CMD(DMA_Channel_TypeDef* DMA_CHx)   //使能开启dma模式
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,50);    //手动配置SEND_SIZE(50)
	DMA_Cmd(DMA_CHx,ENABLE);		
}

