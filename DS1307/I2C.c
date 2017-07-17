/* 
 I2C.c 
 标准80C51单片机模拟I2C总线的主机程序 
 
*/
#include "I2C.h"  

//定义延时变量，用于宏I2C_Delay()  
unsigned char data I2C_Delay_t;  
  
/* 
宏定义：I2C_Delay() 
功能：延时，模拟I2C总线专用 
*/  
#define I2C_Delay()\  
{\  
 I2C_Delay_t = (I2C_DELAY_VALUE);\  
 while ( --I2C_Delay_t != 0 );\  
}  
  
//void uart_flag(void)  
//{  
// UART_TXD=1;  
// UART_TXD=0;  
// UART_TXD=1;  
//}  

/* 
函数：I2C_Init() 
功能：I2C总线初始化，使总线处于空闲状态 
说明：在main()函数的开始处，通常应当要执行一次本函数 
*/  
void I2C_Init()  
{
	I2C_SCL = 1;  
	I2C_Delay();
	I2C_SDA = 1;
	I2C_Delay();  
}  
  
  
/* 
函数：I2C_Start() 
功能：产生I2C总线的起始状态 
说明： 
 SCL处于高电平期间，当SDA出现下降沿时启动I2C总线 
 不论SDA和SCL处于什么电平状态，本函数总能正确产生起始状态 
 本函数也可以用来产生重复起始状态 
 本函数执行后，I2C总线处于忙状态 
*/  
void I2C_Start()  
{  
 EA=0;  
  
 I2C_SCL = 1;  
 I2C_Delay();  
 I2C_SDA = 1;  
 I2C_Delay();  //起始条件建立时间大于4.7us延时  
 I2C_SDA = 0;  //发送起始信号  
 I2C_Delay();  
 I2C_SCL = 0;  //钳住I2C总线，准备发送或接收数据  
 I2C_Delay();  
 I2C_Delay();  
 I2C_Delay();  
}  
  
/* 
函数：I2C_Stop() 
功能：产生I2C总线的停止状态 
说明： 
 SCL处于高电平期间，当SDA出现上升沿时停止I2C总线 
 不论SDA和SCL处于什么电平状态，本函数总能正确产生停止状态 
 本函数执行后，I2C总线处于空闲状态 
*/  
void I2C_Stop()  
{
	unsigned int t = I2C_STOP_WAIT_VALUE;  

	I2C_SDA = 0;  //发送结束条件的数据信号
	I2C_Delay();
	
	I2C_SCL = 1;  //发送结束条件的时钟信号
	I2C_Delay();
	I2C_SDA = 1;  //发送I2C总线结束信号
	I2C_Delay();
	EA=1;
	
	while ( --t != 0 );  //在下一次产生Start之前，要加一定的延时  
}
  
  
/* 
函数：I2C_Write() 
功能：向I2C总线写1个字节的数据 
参数： 
 dat：要写到总线上的数据 
*/  
void I2C_Write(unsigned char dat)  
{
	/*发送1，在SCL为高电平时使SDA信号为高*/
	/*发送0，在SCL为高电平时使SDA信号为低*/
	unsigned char t ;
	for(t=0;t<8;t++)
	{
		// 串口通行都是从字节的高位往地位发送，所以要左移位操作
		I2C_SDA = (bit)(dat & 0x80);
		I2C_Delay();
		I2C_SCL = 1;  //置时钟线为高，通知被控器开始接收数据位
		I2C_Delay();
		I2C_SCL = 0;
		I2C_Delay();
		dat <<= 1;
	}
}
  
/* 
函数：I2C_Read() 
功能：从从机读取1个字节的数据 
返回：读取的一个字节数据 
*/  
unsigned char I2C_Read()  
{
	unsigned char dat=0;
	unsigned char t ;
	bit temp;
	
	I2C_Delay();
	I2C_Delay();
	I2C_SDA = 1; //在读取数据之前，要把SDA拉高
	I2C_Delay();
	
	for(t=0;t<8;t++)
	{
		I2C_SCL = 0; /*接受数据*/
		I2C_Delay();
		I2C_SCL = 1;//置时钟线为高使数据线上升沿数据有效
		I2C_Delay();
		temp = I2C_SDA;
		dat <<=1;
		if (temp==1) dat |= 0x01;
	}
	
	I2C_SCL = 0;
	I2C_Delay();
	return dat;
}
  
  
/* 
函数：I2C_GetAck() 
功能：读取从机应答位 
返回： 
 0：从机应答 
 1：从机非应答 
说明： 
 从机在收到每个字节的数据后，要产生应答位 
 从机在收到最后1个字节的数据后，一般要产生非应答位 
*/  
bit I2C_GetAck()  
{
	bit ack;
	unsigned char Error_time=255;  

	I2C_Delay();
	I2C_SDA = 1; /*8位发送完后释放数据线，准备接收应答位 释放总线*/
	I2C_Delay();
	
	I2C_SCL = 1; /*接受数据*/
	I2C_Delay();
	
	do {
		ack = I2C_SDA;
		Error_time--;
		if(Error_time==0)
		{
			//从机非应答，释放总线
			I2C_SCL = 0;
			I2C_Delay();
			return 1;
		}
	} while(ack);   //判断是否接收到应答信 - SDA低电平为ACK应答信号！
	
	I2C_SCL = 0;  //清时钟线，钳住I2C总线以便继续接收
	I2C_Delay();
	I2C_Delay();
	I2C_Delay();
	return 0;  
}
  
  
/* 
函数：I2C_PutAck() 
功能：主机产生应答位或非应答位 
参数： 
 ack=0：主机产生应答位 
 ack=1：主机产生非应答位 
说明： 
 主机在接收完每一个字节的数据后，都应当产生应答位 
 主机在接收完最后一个字节的数据后，应当产生非应答位 
*/  
void I2C_PutAck(bit ack)  
{
	I2C_SDA = ack;  //在此发出应答或非应答信号
	I2C_Delay();
	I2C_SCL = 1;   //把应答信号刷出去
	I2C_Delay();
	
	I2C_SCL = 0;  //清时钟线，钳住I2C总线以便继续接收  ，继续占用
	I2C_Delay();  //等待时钟线的释放
	I2C_Delay();
	I2C_Delay();
	I2C_Delay();
}  
  
/* 
函数：I2C_Puts() 
功能：主机通过I2C总线向从机发送多个字节的数据 
参数： 
 SlaveAddr：从机地址（高7位是从机地址，最低位是写标志0） 
 SubAddr：从机的子地址 
 Size：数据的字节数 
 *dat：要发送的数据 
返回： 
 0：发送成功 
 1：在发送过程中出现异常 
*/  
bit I2C_Puts(unsigned char SlaveAddr, unsigned char SubAddr, unsigned char Size, char *dat)  
{ 
	//检查长度
	if ( Size == 0 ) return 0;  
	
	//确保从机地址最低位是0（写）
	SlaveAddr &= 0xFE;
	
	//启动I2C总线，发出START信号
	I2C_Start();
	
	//发送从机地址
	I2C_Write(SlaveAddr);
	
	if ( I2C_GetAck() )
	{
		I2C_Stop();
		return 1;
	}

	//发送从机写入地址
	I2C_Write(SubAddr);
	
	//必须获得确认信号ACK才能开始下一字节传输
	if ( I2C_GetAck() )
	{
		I2C_Stop();
		return 1; 
	}
	
	//逐字节发送数据，每个字节发送结束都必须等待ACK确认信号
	do {
		I2C_Write(*dat++);
		if ( I2C_GetAck() )
		{
			I2C_Stop();
			return 1;
		}
	} while ( --Size != 0 );
	
	//发送完毕，停止I2C总线，并返回结果
	I2C_Stop();
	return 0;  
}  
  
  
/* 
函数：I2C_Put() 
功能：主机通过I2C总线向从机发送1个字节的数据 
参数： 
 SlaveAddr：从机地址（高7位是从机地址，最低位是写标志0） 
 SubAddr：从机的子地址 
 dat：要发送的数据 
返回： 
 0：发送成功 
 1：在发送过程中出现异常 
*/  
bit I2C_Put(unsigned char SlaveAddr, unsigned char SubAddr, char dat)  
{  
 return I2C_Puts(SlaveAddr,SubAddr,1,&dat);  
}  
  
  
/* 
函数：I2C_Gets() 
功能：主机通过I2C总线从从机接收多个字节的数据 
参数： 
 SlaveAddr：从机地址（高7位是从机地址，最低位是读标志1） 
 SubAddr：从机的子地址 
 Size：数据的字节数 
 *dat：保存接收到的数据 
返回： 
 0：接收成功 
 1：在接收过程中出现异常 
*/  
bit I2C_Gets(unsigned char SlaveAddr, unsigned char Size, unsigned char *dat)  
{
	//检查长度
	if ( Size == 0 ) return 0;
	
	//确保从机地址最低位是1，代表读操作
	SlaveAddr &= 0xFF;
	
	//启动I2C总线
	I2C_Start();
	
	//发送从机地址
	I2C_Write(SlaveAddr);
	if ( I2C_GetAck() )
	{
		I2C_Stop();
		return 1;
	}
	
//	//发送从机读取地址
//	I2C_Write(SubAddr);
//	if ( I2C_GetAck() )
//	{
//		I2C_Stop();
//		return 1;
//	}
//	
//	//发送重复起始条件
//	I2C_Start();
//	
//	//发送从机地址
//	SlaveAddr |= 0x01;
//	I2C_Write(SlaveAddr);
//	if ( I2C_GetAck() )  
//	{
//		I2C_Stop();
//		return 1;
//	}
	
	//接收数据  
	for (;;)  
	{
		*dat++ = I2C_Read();
		if ( --Size == 0 )
		{
			I2C_PutAck(1);
			break;
		}
		I2C_PutAck(0);
	}
	
	//接收完毕，停止I2C总线，并返回结果
	I2C_Stop();
	return 0;  
}  
  
  
/* 
函数：I2C_Get() 
功能：主机通过I2C总线从从机接收1个字节的数据 
参数： 
 SlaveAddr：从机地址（高7位是从机地址，最低位是读标志1） 
 SubAddr：从机的子地址 
 *dat：保存接收到的数据 
返回： 
 0：接收成功 
 1：在接收过程中出现异常 
*/  
bit I2C_Get(unsigned char SlaveAddr, unsigned char *dat)  
{
	return I2C_Gets(SlaveAddr,1,dat);  
}