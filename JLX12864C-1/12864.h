#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define DB P1

//���ڶ���
//sbit rs=P3^2;
//sbit rw=P3^1;
//sbit e=P3^0;

//���ڶ���
sbit cs=P3^2;
sbit sid=P3^1;
sbit sclk=P3^0;

//���ö˿�
sbit psb=P3^4;
sbit reset=P3^3;

void write_12864(uchar com, uchar rsFlag);
//void write_12864dat(uchar dat);
void init_12864lcd(void);
void display1(void);
void display2(void);
void display3(void);
void delay_50us(uint us);
void delay_50ms(uint ms);