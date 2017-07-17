//youtube ��Ƶ39��
//https://www.youtube.com/watch?v=K18Hl0wdpao

#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define DB P1

//���ڶ���
sbit rs=P3^2;
sbit rw=P3^1;
sbit e=P3^0;

//���ö˿�
sbit psb=P3^3;
sbit reset=P3^4;

void write_12864(uchar com, uchar rsFlag);
//void write_12864dat(uchar dat);
void init_12864lcd(void);
void display1(void);
void display2(void);
void display3(void);
void delay_50us(uint us);
void delay_50ms(uint ms);

uchar table[]="��������20170710";
	
//��ʱ��������12MHz�ľ���Ƶ����
//��Լ50us����ʱ
void delay_50us(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=19;j>0;j--);
}

//��ʱ��������12MHz�ľ���Ƶ����
//��Լ50ms����ʱ
void delay_50ms(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=6245;j>0;j--);
}

void write_12864(uchar dat, uchar rsFlag)
{
	rw=0;
	rs=rsFlag;
	delay_50us(1);
	//todo: need to define data bus pins
	DB=dat;
	e=1;
	delay_50us(10);
	e=0;
}

//void write_12864dat(uchar dat)
//{
//	rw=0;
//	rs=1;
//	delay_50us(1);
//	//todo: need to define data bus pins
//	DB=dat;
//	e=1;
//	delay_50us(10);
//	e=0;
//}

void init_12864lcd(void)
{
	//�����ϵ�
	delay_50ms(2);
	write_12864(0x30, 0);
	delay_50us(4);
	write_12864(0x30, 0);
	delay_50us(4);
	
	//������Ļ����ʼ������
	write_12864(0x0f, 0);
	delay_50us(4);
	write_12864(0x01, 0);
	delay_50us(240);
	write_12864(0x06, 0);
	delay_50us(10);
	
}

void display1(void)
{
	uchar i;
	uchar a;
	a=0x10;
	write_12864(0x80, 0);
	delay_50us(1);
	for(i=0;i<16;i++)
	{
		write_12864(a+i, 1);
		delay_50us(1);
	}
}

void display2(void)
{
	uchar i;
	uchar a,b;
	a=0xBA;
	b=0xC0;
	
	write_12864(0x80, 0);
	for(i=0;i<16;i++)
	{
		write_12864(a, 1);
		write_12864(b+i, 1);
		delay_50us(1);
	}
	
}

void display3(void)
{
	uchar i;
	
	write_12864(0x80, 0);
	for(i=0;i<16;i++)
	{
		write_12864(table[i], 1);
		delay_50us(1);
	}
	
}