#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

//共阳数字编码  0.1.2.3.4....9
uchar code LedChar[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 
0x80, 0x90};

uchar LedBuff[4] = {  //数码管
	0xFF, 0xFF, 0xFF, 0xFF
};

uchar T0RH = 0;  //T0重载值的高字节
uchar T0RL = 0;  //T0重载值的低字节
uchar RxdByte = 0;  //串口接收到的字节

void ConfigTimer0(uint ms);
void ConfigUART(uint baud);

void main ()
{
	P0 = 0xFF;  //P0口初始化
	P2_0 = 0;	  //打开第一个数码管
	EA = 1;	 //开总中断
	ConfigTimer0(1);   //配置 T0 定时 1ms
	ConfigUART(9600);  //配置波特率为 9600

	while(1)
	{   //将接收的数字原样显示在KR-51位数码管的第一个
		//出于测试目的，只接收输入0~9的数字
		LedBuff[0] = LedChar[RxdByte];
		//LedBuff[1] = LedChar[RxdByte >> 4];
	}
}

//给LED显示用的刷新定时器中断初始化
void ConfigTimer0(uint ms)  //T0配置函数
{
	unsigned long tmp;

	tmp = 11059200 / 12;	  //定时器计数频率
	tmp = (tmp * ms) / 1000;  //计算所需的计数值
	tmp = 65536 - tmp;		//计算定时器重载值
	tmp = tmp + 31;		   //修正中断响应延时造成的误差

	T0RH = (unsigned char)(tmp >> 8);  //定时器重载值拆分为高低字节
	T0RL = (unsigned char)tmp;
	TMOD &= 0xF0;   //清零 T0 的控制位
	TMOD |= 0x01;   //配置 T0 为模式 1
	TH0 = T0RH;	 //加载 T0 重载值
	TL0 = T0RL;
	ET0 = 1;		//使能 T0 中断
	TR0 = 1;		//启动 T0
}

//给数据传输用的定时器中断初始化
void ConfigUART(uint baud)  //串口配置函数， baud 为波特率
{
	SCON = 0x50;   //配置串口为模式 1
	TMOD &= 0x0F;  //清零 T1 的控制位
	TMOD |= 0x20;  //配置 T1 为模式 2
	TH1 = 256 - (11059200/12/32) / baud;  //计算 T1 重载值
	TL1 = TH1;	 //初值等于重载值
	ET1 = 0;	   //禁止 T1 中断
	ES  = 1;	   //使能串口中断
	TR1 = 1;	   //启动 T1
}

//输出字符到LED
void LedScan()  //LED显示扫描函数
{
	P0 = 0xFF;				 //关闭所有段选位，显示消隐
	P0 = LedBuff[0];	   //相应显示缓冲区的值赋值到 P0 口
}

void InterruptTimer0() interrupt 1  //T0中断服务函数
{
	TH0 = T0RH;  //定时器重新加载重载值
	TL0 = T0RL;
	LedScan();   //LED扫描显示
}

void InterruptUART() interrupt 4
{
	if (RI)  //接收到字节
	{
		RI = 0;  //手动清零接收中断标志位
		RxdByte = SBUF;  //接收到的数据保存到接收字节变量中
		SBUF = RxdByte;  //接收到的数据又直接发回，这叫回显 -"echo" ，以提示用户输入的信息是否已正确接收
	}
	if (TI)  //字节发送完毕
	{
		TI = 0;  //手动清零发送中断标志位
	}
}
