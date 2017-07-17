/* 
 I2C.h 
 标准80C51单片机模拟I2C总线的主机程序头文件 
*/  
#ifndef _I2C_H_  
#define _I2C_H_ 
  
#include "reg52.h"
  
//模拟I2C总线的引脚定义  
#define I2C_SCL  SMBUS_SCL
#define I2C_SDA  SMBUS_SDA
sbit I2C_SCL = P2^6;  
sbit I2C_SDA = P2^7; 

//定义I2C总线时钟的延时值，要根据实际情况修改，取值1～255  
//SCL信号周期约为(I2C_DELAY_VALUE*4+15)个机器周期  
#define I2C_DELAY_VALUE  20  
  
//定义I2C总线停止后在下一次开始之前的等待时间，取值1～65535  
//等待时间约为(I2C_STOP_WAIT_VALUE*8)个机器周期  
//对于多数器件取值为1即可；但对于某些器件来说，较长的延时是必须的  
#define I2C_STOP_WAIT_VALUE 20  
  
//I2C总线初始化，使总线处于空闲状态  
void I2C_Init();  
  
//主机通过I2C总线向从机发送多个字节的数据  
bit I2C_Puts(unsigned char SlaveAddr, unsigned char SubAddr, unsigned char Size, char *dat);  
  
//主机通过I2C总线向从机发送1个字节的数据  
bit I2C_Put(unsigned char SlaveAddr, unsigned char SubAddr, char dat);  
  
//主机通过I2C总线从从机接收多个字节的数据  
bit I2C_Gets(unsigned char SlaveAddr, unsigned char Size, unsigned char *dat);  
  
//主机通过I2C总线从从机接收1个字节的数据  
bit I2C_Get(unsigned char SlaveAddr, unsigned char *dat);  
  
#endif //_I2C_H_  