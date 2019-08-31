#include "usart.h"

void UARTx_Init(uint32_t byte)
{
	RCC_APB2PeriphClockCmd(USART_RCC_APB2Periph_GPIOx,ENABLE);   //GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //USART1ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//USART2ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//USART3ʱ��
	
	GPIO_InitTypeDef  GPIO_Initstucture;
	USART_InitTypeDef USART_Initstucture;
	NVIC_InitTypeDef  NVIC_Initstucture;

/*************     usart_tx  GPIO��ʼ��  ***********/
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Initstucture.GPIO_Pin=USART_PIN_TX;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO,&GPIO_Initstucture);
 
/*************     usart_rx  GPIO��ʼ��  ***********/	
	GPIO_Initstucture.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Initstucture.GPIO_Pin=USART_PIN_RX;
	GPIO_Initstucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO,&GPIO_Initstucture);

/*************     usart��ʼ��  ***********/	
	USART_Initstucture.USART_BaudRate=byte;
	USART_Initstucture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Initstucture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Initstucture.USART_Parity=USART_Parity_No;
	USART_Initstucture.USART_StopBits=USART_StopBits_1;
	USART_Initstucture.USART_WordLength=USART_WordLength_8b;
  USART_Init(USARTX,&USART_Initstucture);	
	
	USART_ITConfig(USARTX,USART_IT_RXNE,ENABLE); //ʹ�ܴ����ж�
	
/*************  usart_�ж� ��ʼ��  ***********/		
	NVIC_Initstucture.NVIC_IRQChannel=USARTx_IRQn;
	NVIC_Initstucture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstucture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initstucture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_Initstucture);	
	
	USART_Cmd(USARTX,ENABLE); //ʹ�ܴ���
}

#define USART_RECEIVE_LEN    200     //���䴢�������ֽ���
#define USART_RECEIVE_FLAG   0X0A    //���ճɹ���־λ
#define USART_FH             0x0d    //֡ͷ
uint8_t USART_BUF[];                 //��������
uint16_t USART_STA=0;                //״̬λ

void USARTx_IRQHandler()
{
	unsigned char Receive_data;
	
	if(USART_GetITStatus( USARTX,USART_IT_RXNE)!=RESET)  //���������ж�
	 {
		Receive_data=USART_ReceiveData(USARTX);	           //��������
		if((USART_STA&0x8000)==RESET)                      //�������ݻ�δ�ɹ�
		{
			if((USART_STA&0x4000)!=RESET)                    //���յ�֡ͷ
			{
				if(Receive_data==USART_RECEIVE_FLAG)  USART_STA|=0x8000; //���յ������ݳɹ���״̬λ
				  else                                USART_STA=0;       //���ݽ��մ������½���
			}
			else                                            //��û���յ�֡ͷ
			{
        if((Receive_data&USART_FH)!=RESET)    USART_STA|=0x4000;//���յ�֡ͷ����־λ
				else                                                    //��û���յ�֡ͷ�������ڴ������error��
			{
					USART_BUF[USART_STA&0x3fff]=Receive_data;             //�����ݷ������鴢��
				  USART_STA++;
						
					if(USART_STA>(USART_RECEIVE_LEN-1)) 
					USART_STA=0;                                          //������ݳ���Լ�����ȣ����½���
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
		USART_SendData(USART1,* data_buf );  //�򴮿�1��������
		while(USART_GetFlagStatus(USARTX,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		USART_ClearFlag(USARTX,USART_FLAG_TC);
	  USART_ClearITPendingBit(USARTX,USART_FLAG_TXE);  //�����־λ
		*data_buf++;
	}
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USARTX->SR&0X40)==0);//ѭ������,ֱ���������   
	USARTX->DR = (u8) ch;      
	return ch;
}                             //���ú����ֱ�����printf("%d\r", i );

