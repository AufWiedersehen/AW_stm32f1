#include "usart.h"

void UARTx_Init(uint32_t byte)
{
	RCC_APB2PeriphClockCmd(USART_RCC_APB2Periph_GPIOx,ENABLE);   //GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //USART1时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//USART2时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//USART3时钟
	
	GPIO_InitTypeDef  GPIO_Initstucture;
	USART_InitTypeDef USART_Initstucture;
	NVIC_InitTypeDef  NVIC_Initstucture;

/*************     usart_tx  GPIO初始化  ***********/
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=USART_PIN_TX;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO,&GPIO_Initstucture);
 
/*************     usart_rx  GPIO初始化  ***********/	
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Initstucture.GPIO_Pin=USART_PIN_RX;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO,&GPIO_Initstucture);

/*************     usart初始化  ***********/	
	USART_Initstucture.USART_BaudRate=byte;
	USART_Initstucture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Initstucture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Initstucture.USART_Parity=USART_Parity_No;
	USART_Initstucture.USART_StopBits=USART_StopBits_1;
	USART_Initstucture.USART_WordLength=USART_WordLength_8b;
  USART_Init(USARTX,&USART_Initstucture);	
	
	USART_ITConfig(USARTX,USART_IT_RXNE,ENABLE); //使能串口中断
	
/*************  usart_中断 初始化  ***********/		
	NVIC_Initstucture.NVIC_IRQChannel=USARTx_IRQn;
	NVIC_Initstucture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstucture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initstucture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_Initstucture);	
	
	USART_Cmd(USARTX,ENABLE); //使能串口
}

#define USART_RECEIVE_LEN    200     //传输储存的最大字节数
#define USART_RECEIVE_FLAG   0X0A    //接收成功标志位
#define USART_FH             0x0d    //帧头
uint8_t USART_BUF[];                 //储存数据
uint16_t USART_STA=0;                //状态位

void USARTx_IRQHandler()
{
	unsigned char Receive_data;
	
	if(USART_GetITStatus( USARTX,USART_IT_RXNE)!=RESET)  //触发接收中断
	 {
		Receive_data=USART_ReceiveData(USARTX);	           //接受数据
		if((USART_STA&0x8000)==RESET)                      //接受数据还未成功
		{
			if((USART_STA&0x4000)!=RESET)                    //接收到帧头
			{
				if(Receive_data==USART_RECEIVE_FLAG)  USART_STA|=0x8000; //接收到的数据成功，状态位
				  else                                USART_STA=0;       //数据接收错误，重新接收
			}
			else                                            //还没接收到帧头
			{
        if((Receive_data&USART_FH)!=RESET)    USART_STA|=0x4000;//接收到帧头，标志位
				else                                                    //还没接收到帧头（数据在传输或者error）
			{
					USART_BUF[USART_STA&0x3fff]=Receive_data;             //将数据放入数组储存
				  USART_STA++;
						
					if(USART_STA>(USART_RECEIVE_LEN-1)) 
					USART_STA=0;                                          //如果数据超过约定长度，重新接收
			 }
		  }				
		 }
	  }
}
  
void USATx_Send(uint8_t *data_buf,uint16_t data_len)
{
	USART_ClearFlag(USARTX,USART_FLAG_TC);
	USART_ClearITPendingBit(USARTX,USART_FLAG_TXE);
	while(data_len--)
	{
		USART_SendData(USART1,* data_buf );  //向串口1发送数据
		while(USART_GetFlagStatus(USARTX,USART_FLAG_TC)!=SET);//等待发送结束
		USART_ClearFlag(USARTX,USART_FLAG_TC);
	  USART_ClearITPendingBit(USARTX,USART_FLAG_TXE);  //清除标志位
		*data_buf++;
	}
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USARTX->SR&0X40)==0);//循环发送,直到发送完毕   
	USARTX->DR = (u8) ch;      
	return ch;
}                             //调用后可以直接输出printf("%d\r", i );

