#include "i2c.h"
#include "delay.h"

void IIC_GPIO_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //GPIO时钟
	
	GPIO_InitTypeDef  GPIO_InitStructure;
/*************   GPIO初始化  ***********/		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	delay_ms(10);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); //默认高电平，线空闲
}

/*************   SDA线_输出数据 ***********/	
void SDA_IO_OUT()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}

/*************   SDA线_输入数据 ***********/	
void SDA_IO_IN()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}

static void I2C_delay(void)
{
	
	
}

/*****  I2C_start   ******/	
void IIC_START()
{
  SDA_IO_OUT();
  SCL_H;
  SDA_H;
  I2C_delay();
  SDA_L;
  I2C_delay();
}

/*****  I2C_stop   ******/	
void IIC_STOP()
{
  SDA_IO_OUT();
  SCL_L;
  I2C_delay();
  SDA_L;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SDA_H;
  I2C_delay();
}

/*****  I2C_ACK   ******/	
uint8_t IIC_SlaveAck()
{
	SDA_IO_OUT();
  SCL_L;
  I2C_delay();
  SDA_H;
  
  SDA_IO_OUT();
  I2C_delay();
  SCL_H;
  
  I2C_delay();
  
  if(SDA_read)
  {
    SCL_L;
    return 0;
  }
  SCL_L;
  I2C_delay();
  return 1;
}

/*****  I2C_send  8位   ******/	
void IIC_SendByte(uint8_t byte)
{
	uint8_t i = 8;
  SDA_IO_OUT();
  while (i--)
  {
    SCL_L;
    I2C_delay();
    if (byte & 0x80)
      SDA_H;
    else
      SDA_L;
    byte <<= 1;
    I2C_delay();
    SCL_H;
    I2C_delay();
  }
	
  SCL_L;
  if(IIC_SlaveAck()==0)
  {
    return ;
  }
}

/*****  I2C_REC  8位   ******/
uint8_t IIC_RecvByte()
{
	uint8_t i;
  uint8_t Data = 0;
  SDA_IO_IN();
  for (i=0; i<8; i++)
  {
    Data <<= 1;
    SCL_H;
    I2C_delay();
    Data |= SDA_read;
    SCL_L;
    I2C_delay();
  }
  return Data;
}


/*************    Read_Gyro  ***********/

void IIC_Write_OneByte(uint8_t Slave_Address,uint8_t Master_Address,uint8_t Data)  //发送（写入）一字节（8 byte） Master --->  Slave
{
	IIC_START();
  IIC_SendByte(Slave_Address);
  IIC_SendByte(Master_Address);
  IIC_SendByte(Data);
  IIC_STOP();
}

uint8_t IIC_Read_OneByte(uint8_t Slave_Address,uint8_t Master_Address)           //接收（读取）一字节（8 byte） Master --->  Slave
{
  uint8_t Read_Data;
  IIC_START();
  IIC_SendByte(Slave_Address);
  IIC_SendByte(Master_Address);
  IIC_START();
  IIC_SendByte(Slave_Address+1);
  Read_Data=IIC_RecvByte();
  IIC_SlaveAck();
  IIC_STOP();
  return Read_Data;
}


unsigned short int IIC_Read_TwoByte(uint8_t Slave_Address,uint8_t Master_Address)      //接收（读取）2字节（32 byte） Master --->  Slave
{
	uint8_t IIC_Data_H, IIC_Data_L;
	IIC_Data_H=IIC_Read_OneByte( Slave_Address, Master_Address);
	IIC_Data_L=IIC_Read_OneByte( Slave_Address, (Master_Address+1));
	
	return  ((unsigned short int)IIC_Data_H<<8|IIC_Data_L);
}

unsigned long int IIC_Read_ThreeByte(uint8_t Slave_Address,uint8_t Master_Address)    ////接收（读取）2字节（32 byte） Master --->  Slave
{
	uint8_t IIC_Data_H, IIC_Data_M,IIC_Data_L;
	IIC_Data_H=IIC_Read_OneByte( Slave_Address, Master_Address);
	IIC_Data_M=IIC_Read_OneByte( Slave_Address, (Master_Address+1));
	IIC_Data_L=IIC_Read_OneByte( Slave_Address, (Master_Address+2));
	
	return ((unsigned long int)IIC_Data_H<<16|(unsigned long int)IIC_Data_M<<8|IIC_Data_L);
}

/*************    Read_Gyro  ***********/

