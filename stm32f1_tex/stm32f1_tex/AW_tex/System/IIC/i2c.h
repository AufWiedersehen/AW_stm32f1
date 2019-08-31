#ifndef __I2C_H
#define __I2C_H

#include "sys.h"

/**  I2C_data  资料查询
  *  https://blog.csdn.net/hanjing_csdn/article/details/82108324 *
                                                                   **/

#define SCL_H         GPIOB->BSRR=GPIO_Pin_6
#define SCL_L         GPIOB->BRR=GPIO_Pin_6
#define SDA_H         GPIOB->BSRR=GPIO_Pin_7
#define SDA_L         GPIOB->BRR=GPIO_Pin_7
#define SDA_read      ((GPIOB->IDR&GPIO_Pin_7)!=0)?1:0     //SDA: PB7

/************* GPIO初始化  ***********/	
void IIC_GPIO_Config(void);
void SDA_IO_OUT(void);
void SDA_IO_IN(void);

/*************  IIC时序 ***********/	
void IIC_START(void);
void IIC_STOP(void);
uint8_t IIC_SlaveAck(void);
static void I2C_delay(void);

/********  IIC写入/读取 8byte *******/
void IIC_SendByte(uint8_t byte);
uint8_t IIC_RecvByte(void);

/*  */
void IIC_Write_OneByte(uint8_t Slave_Address,uint8_t Master_Address,uint8_t Data);
uint8_t IIC_Read_OneByte(uint8_t Slave_Address,uint8_t Master_Address);
unsigned short int IIC_Read_TwoByte(uint8_t Slave_Address,uint8_t Master_Address);
unsigned long int IIC_Read_ThreeByte(uint8_t Slave_Address,uint8_t Master_Address);

#endif


