#include <reg52.h>

//sbit a1=P2^0;
//sbit b1=P2^1;
//sbit a2=P2^2;
//sbit b2=P2^3;
//sbit a3=P2^4;
//sbit b3=P2^5;
sbit a4=P2^6;
sbit b4=P2^7;

//KR-51开发板的以下4个引脚有对应的LED灯，可以直接查看测试结果
//低电平点亮LED
//sbit y1=P1^1;
//sbit y2=P1^2;
//sbit y3=P1^3;
//sbit y4=P1^4;

//延时函数，在12MHz的晶振频率下
//大约50ms的延时
void delay_50ms(unsigned int t)
{
	unsigned char j;
	for (;t>0;t--)
		for(j=6245;j>0;j--);
}

void main(void)
{
	while (1)
	{
		a4=0;
		b4=0;
		delay_50ms(5000);
		
//		a4=0;
//		b4=1;
//		delay_50ms(5000);
		
//		a4=1;
//		b4=0;
//		delay_50ms(5000);
//		
//		a4=1;
//		b4=1;
//		delay_50ms(5000);
	}
}