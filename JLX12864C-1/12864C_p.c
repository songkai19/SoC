	/* Test program for JLX12864C
Driver IC is:ST7920
JLX electronic Co.,ltd, http://www.jlxlcd.cn;http://www.jlxlcd.com.cn
本程序显示内容是在第一行第一位置显示中文字"深圳晶联讯"
*/
#include <reg51.H>
sbit rs=P3^2;
sbit wr=P3^1;
sbit e=P3^0;
sbit psb=P3^4;
sbit reset=P3^3;
#define data_bus P1
#define uchar unsigned char
char code yun[]=
{
/*-- 文字: 运 --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=31x31 --*/
/*-- 宽度不是8 的倍数，现调整为：宽度x 高度=32x32 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x0E,0x00,0x01,0x80,
0x07,0x0F,0xFF,0xC0,0x03,0x8F,0xFF,0xC0,0x03,0x80,0x00,0x00,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x70,0x03,0x3F,0xFF,0xF8,
0x7F,0x80,0x70,0x00,0x33,0x80,0xF0,0x00,0x03,0x00,0xE0,0x00,0x03,0x01,0xC0,0x00,
0x03,0x01,0x86,0x00,0x03,0x03,0x07,0x00,0x03,0x07,0x03,0x80,0x03,0x0E,0x01,0xC0,
0x03,0x1F,0xFF,0xE0,0x03,0x1F,0xFC,0xE0,0x03,0x0E,0x00,0xE0,0x0F,0x80,0x00,0x40,
0x3D,0xC0,0x00,0x00,0x78,0xF0,0x00,0x00,0x70,0x7F,0xC1,0xFC,0x20,0x1F,0xFF,0xF8,
0x00,0x03,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
char code xing[]={
/*-- 文字: 行 --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=31x31 --*/
/*-- 宽度不是8 的倍数，现调整为：宽度x 高度=32x32 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xE0,0x00,0xE0,
0x01,0xE3,0xFF,0xF0,0x03,0x83,0x00,0x00,0x03,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
0x0C,0x60,0x00,0x00,0x38,0x70,0x00,0x00,0x20,0xE0,0x00,0x30,0x00,0xEF,0xFF,0xF8,
0x01,0xCF,0xFF,0xFC,0x03,0x80,0x0E,0x00,0x03,0xC0,0x0E,0x00,0x07,0xC0,0x0E,0x00,
0x0F,0x80,0x0E,0x00,0x1D,0x80,0x0E,0x00,0x31,0x80,0x0E,0x00,0x61,0x80,0x0E,0x00,
0x01,0x80,0x0E,0x00,0x01,0x80,0x0E,0x00,0x01,0x80,0x0E,0x00,0x01,0x80,0x0E,0x00,
0x01,0x80,0x0E,0x00,0x01,0x80,0x0E,0x00,0x01,0x80,0x0E,0x00,0x01,0x81,0xFE,0x00,
0x01,0x80,0x7C,0x00,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char code char_R[]={
/*-- 文字: R --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=16x32 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF0,0x3C,0x78,
0x18,0x1C,0x18,0x1C,0x18,0x0C,0x18,0x0C,0x18,0x1C,0x18,0x1C,0x18,0x78,0x1F,0xF0,
0x19,0xC0,0x19,0xE0,0x18,0xE0,0x18,0xE0,0x18,0x70,0x18,0x70,0x18,0x78,0x18,0x38,
0x18,0x38,0x3C,0x3C,0x7E,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
char code char_U[]={
/*-- 文字: U --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=16x32 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x3E,0x78,0x1C,
0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,
0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x30,0x08,0x38,0x18,
0x38,0x18,0x1E,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char code char_N[]={
/*-- 文字: N --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=16x31 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3E,0x78,0x1C,
0x3C,0x08,0x3C,0x08,0x2E,0x08,0x2E,0x08,0x2F,0x08,0x27,0x08,0x27,0x88,0x23,0x88,
0x23,0xC8,0x21,0xC8,0x21,0xE8,0x20,0xE8,0x20,0xF8,0x20,0x78,0x20,0x78,0x20,0x38,
0x20,0x38,0x70,0x18,0xF8,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char code char_I[]={
/*-- 文字: I --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=16x31 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x03,0xC0,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x03,0xC0,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char code char_G[]={
/*-- 文字: G --*/
/*-- 宋体23; 此字体下对应的点阵为：宽x 高=16x31 --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF8,0x0E,0x78,
0x1C,0x38,0x38,0x18,0x30,0x18,0x70,0x08,0x70,0x00,0x70,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x7E,0x70,0x3C,0x70,0x18,0x70,0x18,0x30,0x18,0x38,0x18,
0x18,0x38,0x1E,0x78,0x07,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char code graphic1[]={
/*-- 调入了一幅图像：E:\work\图片收藏夹\12864c.bmp --*/
/*-- 宽度x 高度=128x64 --*/
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0xFC,0x01,0x11,0x01,0x00,0x80,0x00,0x40,0x20,0x1D,0xF8,0x12,0x00,0x02,0x01,
0x81,0x04,0x7E,0xA0,0xBF,0x80,0x80,0x7F,0xE0,0x21,0x15,0x08,0x12,0x07,0xFF,0x01,
0x81,0xFC,0x24,0x40,0x01,0x0F,0xF8,0x00,0x87,0xFF,0x95,0x08,0x21,0x00,0x02,0x01,
0x81,0x04,0x25,0xF0,0x09,0x08,0x88,0x03,0x00,0x40,0x15,0xF8,0x21,0x0F,0xFA,0x01,
0x81,0xFC,0x3C,0x43,0x89,0x08,0x88,0x02,0x00,0xFE,0x19,0x08,0x40,0x80,0x02,0x01,
0x80,0x00,0x24,0x40,0xBF,0x0F,0xF8,0xFF,0xF1,0x82,0x15,0xF8,0x88,0x43,0xE2,0x01,
0x87,0xDF,0x3F,0xF8,0x89,0x08,0x88,0x02,0x02,0xFE,0x15,0x49,0x08,0x22,0x22,0x01,
0x84,0x51,0x24,0x40,0x89,0x08,0x88,0x02,0x04,0x82,0x15,0x50,0x10,0x02,0x22,0x01,
0x87,0xDF,0x2E,0xA0,0xA9,0x0F,0xF8,0x02,0x00,0xFE,0x1D,0x20,0x12,0x03,0xE2,0x01,
0x84,0x51,0x74,0xA0,0xC9,0x40,0x82,0x02,0x00,0x82,0x11,0x10,0x21,0x02,0x22,0x01,
0x87,0xDF,0x05,0x10,0x88,0xC0,0x82,0x02,0x00,0x82,0x11,0x48,0x7F,0x80,0x0A,0x01,
0x84,0x51,0x06,0x08,0x08,0x40,0x7E,0x06,0x00,0x86,0x11,0x84,0x20,0x80,0x04,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x42,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x87,0xEA,0x08,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x4A,0x08,0x20,0x00,0x0F,0xB8,0x6E,0x10,0x70,0xE0,0xE0,0x87,0x80,0x08,0x01,
0x87,0xEA,0x0F,0xE0,0x00,0x02,0x10,0x24,0x30,0x89,0x11,0x21,0x88,0x80,0x18,0x01,
0x81,0x4A,0x00,0x00,0x00,0x02,0x10,0x24,0x10,0x89,0x12,0x02,0x90,0x00,0x08,0x01,
0x81,0x4A,0x7F,0xF8,0x00,0x02,0x10,0x18,0x10,0x08,0xA2,0xC4,0x90,0x00,0x08,0x01,
0x82,0x46,0x04,0x01,0x80,0x02,0x10,0x18,0x10,0x10,0xE3,0x24,0x90,0x3F,0x08,0x01,
0x84,0x20,0x0F,0xE1,0x80,0x02,0x10,0x18,0x10,0x21,0x12,0x28,0x90,0x00,0x08,0x01,
0x80,0x20,0x00,0x20,0x00,0x02,0x10,0x24,0x10,0x41,0x12,0x27,0x90,0x00,0x08,0x01,
0x83,0xFE,0x00,0x21,0x80,0x02,0x11,0x24,0x10,0x81,0x12,0x20,0x88,0x80,0x08,0x01,
0x80,0x20,0x02,0x41,0x80,0x12,0x3F,0x76,0x38,0xF8,0xE1,0xC1,0xC7,0x00,0x1C,0x01,
0x8F,0xFF,0x01,0x80,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x28,0x02,0x00,0x10,0x00,0x40,0x02,0x00,0x88,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x28,0x02,0x00,0x08,0x0F,0xFE,0x7F,0xF1,0x08,0x00,0x00,0x00,0x00,0x00,0x01,
0x87,0xFE,0x3F,0xF3,0xFF,0xC8,0x02,0x44,0x03,0xCF,0x04,0x1C,0x38,0x00,0x70,0x41,
0x81,0x28,0x22,0x10,0x42,0x10,0x04,0x5F,0xE2,0x51,0x0C,0x22,0x44,0x00,0x90,0xC1,
0x87,0xFF,0x22,0x10,0x42,0x07,0xF0,0x44,0x02,0x61,0x04,0x22,0x44,0x21,0x01,0x41,
0x84,0x21,0x22,0x10,0x22,0x00,0x20,0x49,0x02,0x51,0x04,0x02,0x28,0xA9,0x62,0x41,
0x8B,0xFE,0x3F,0xF0,0x24,0x00,0x40,0x5F,0xE3,0xC9,0x04,0x04,0x38,0x71,0x92,0x41,
0x82,0x24,0x22,0x10,0x14,0x1F,0xFE,0x41,0x02,0x49,0x04,0x08,0x44,0x71,0x14,0x41,
0x82,0x24,0x02,0x00,0x08,0x00,0x40,0x41,0x02,0x41,0x04,0x10,0x44,0xA9,0x13,0xC1,
0x82,0x24,0x02,0x00,0x14,0x00,0x40,0x5F,0xF2,0x41,0x04,0x20,0x44,0x21,0x10,0x41,
0x82,0x2C,0x02,0x00,0x63,0x00,0x40,0x81,0x03,0xC5,0x0E,0x3E,0x38,0x00,0xE0,0xE1,
0x80,0x20,0x02,0x03,0x80,0xC0,0xC0,0x81,0x02,0x42,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x08,0x01,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x08,0x7F,0x90,0x7F,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x81,0x08,0x12,0x20,0x41,0x00,0x20,0x00,0x01,0xC3,0x80,0x1F,0x1C,0x00,0x00,0x01,
0x81,0xC8,0x12,0x40,0x41,0x1F,0xFE,0x00,0x02,0x24,0x40,0x12,0x22,0x00,0x00,0x01,
0x82,0x48,0x12,0x90,0x7F,0x00,0x20,0x00,0x02,0x20,0x42,0x02,0x22,0x00,0x00,0x01,
0x82,0x4C,0x7F,0x20,0x48,0x04,0x20,0x00,0x02,0x21,0x8A,0x82,0x22,0xF9,0xF0,0x01,
0x85,0x8B,0x12,0x40,0x48,0x02,0x20,0x60,0x02,0x60,0x47,0x04,0x22,0x54,0xA8,0x01,
0x80,0x89,0x12,0x88,0x44,0x01,0x20,0x60,0x01,0xA0,0x47,0x04,0x22,0x54,0xA8,0x01,
0x81,0x08,0x12,0x10,0x84,0x00,0x20,0x00,0x00,0x20,0x4A,0x84,0x22,0x54,0xA8,0x01,
0x82,0x08,0x12,0x20,0x82,0x00,0x20,0x60,0x02,0x44,0x42,0x04,0x22,0x54,0xA8,0x01,
0x84,0x08,0x22,0x41,0x01,0x00,0x20,0x60,0x03,0x83,0x80,0x04,0x1C,0xD7,0xAC,0x01,
0x88,0x08,0x42,0x82,0x00,0xC0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
/*=============延时=====================*/
void delay(int i)
{
int j,k;
for(j=0;j<i;j++)
for(k=0;k<990;k++);
}
//=============delay time=====================
void delay1(int i)
{
int j,k;
for(j=0;j<i;j++)
for(k=0;k<2;k++);
}
/*====等待按键：P2.0 与地相接时按键生效=============*/
void Switch()
{
repeat:
if (P2&0x01) goto repeat;
else
delay(20);
}
//===检查忙标志位BF=================================
void check_busy1()
{
rs=0;
data_bus=0xff;
if(P1^7==1)
{
wr=1;
e=1;
e=0;
}
else;
}
/*===送指令到液晶模块驱动IC==========================*/
void transfer_command(int data1)
{
check_busy1();
rs=0;
wr=0;
data_bus=data1;
e=1;
delay1(5);
e=0;
delay1(5);
}
/*=========传送数据到液晶模块驱动IC=============*/
void transfer_data(int data1)
{
check_busy1();
rs=1;
wr=0;
data_bus=data1;
e=1;
e=0;
}
void clear_screen()
{
int i,j;
transfer_command(0x36); /*选用扩展指令集*/
for(j=0;j<32;j++)
{
transfer_command(0x80+j); /*垂直地址*/
transfer_command(0x80); /*水平地址*/
for(i=0;i<32;i++)
{
transfer_data(0x00);
}
}
}
void clear_ddram()
{
transfer_command(0x30);
transfer_command(0x01);
delay(10);
}
void display_3232(int y,int x,char code *p)
{
int i,j;
transfer_command(0x36); /*选用扩展指令集*/
delay(10);
for(j=0;j<32;j++)
{
transfer_command(0x80+(y-1)+j); /*垂直地址*/
transfer_command(0x80+(x-1)); /*水平地址*/
for(i=0;i<4;i++)
{
transfer_data(*p);
p++;
}
}
}
void display_1632(int y,int x,char code *p)
{
int i,j;
transfer_command(0x36);
for(j=0;j<32;j++)
{
transfer_command(0x80+(y-1)+j);
transfer_command(0x80+(x-1));
for(i=0;i<2;i++)
{
transfer_data(*p);
p++;
}
}
}
void display_12864(int y,int x,char code *p)
{
	int i,j;
transfer_command(0x36); /*选用扩展指令集*/
delay(10);
/*写上半部分的32 行数据*/
for(j=0;j<32;j++)
{
transfer_command(0x80+(y-1)+j); /*垂直地址*/
transfer_command(0x80+(x-1)); /*水平地址*/
for(i=0;i<8;i++) //
{
transfer_data(*p);
p++;
transfer_data(*p);
p++;
}
}
transfer_command(0x36); /*选用扩展指令集*/
delay(10);
/*写下半部分的32 行数据*/
for(j=0;j<32;j++)
{
transfer_command(0x80+(y-1)+j); /*垂直地址*/
transfer_command(0x80+(x-1)+8);/*水平地址*/
for(i=0;i<8;i++)
{
transfer_data(*p);
p++;
transfer_data(*p);
p++;
}
}
}
/*初始化*/
void Initial_ic()
{
transfer_command(0x30); /*选用基本指令集*/
delay(10);
transfer_command(0x01); /*清屏*/
delay(10);
transfer_command(0x06); //
delay(10);
transfer_command(0x0c); /*开显示，关光标*/
delay(10);
}
/*调用中文字库里的汉字*/
void display_char(int y,int x,int char_length,uchar *p)
{
uchar i=0;
transfer_command(0x30); /*选用基本指令集*/
delay(10);
// transfer_command(0x01); /*清屏*/
delay(10);
transfer_command(0x80+(y-1)*(0x10)+(x-1));
for(i=0;i<char_length;i++)
{
transfer_data(*p);
p++;
transfer_data(*p);/* 以上两行数据合起来显示一个汉字*/
p++;
}
}
/*主程序*/
void main(void)
{
int i,j,k;
psb=1; /*选择并口*/
Initial_ic(); /*初始化*/
while(1)
{
clear_ddram();
display_12864(1,1,graphic1);
Switch(); /*等待按键P2.0 口*/
//display_12864(1,1,graphic2); /*（从第1 行，第1 列开始，显示图片128*64 点阵*/
//Switch();
//display_12864(1,1,graphic3);
//Switch();
//display_12864(1,1,graphic4);
//Switch();
clear_ddram();
display_char(1,1,8,"深圳市晶联讯电子"); /*（在第1 行，第1 列，8 个汉字）调用显示汉字子程序*/
display_char(2,1,8,"JLX12864C,128*64"); /*（在第2 行，第2 列，8 个汉字）调用显示汉字子程序*/
display_char(1,9,8,"高品质，创口碑！"); /*（在第3 行，第1 列，8 个汉字）调用显示汉字子程序*/
display_char(2,9,8,"诚信服务持续经营"); /*（在第4 行，第1 列，8 个汉字）调用显示汉字子程序*/
Switch(); /*等待按键P2.0 口*/
clear_ddram();
clear_screen();
clear_ddram();
display_char(1,1,8,"欢迎使用12864C-1"); /*（在第1 行，第1 列，8 个汉字）调用显示汉字子程序*/
display_char(2,1,8,"JLX12864C,128*64"); /*（在第2 行，第2 列，8 个汉字）调用显示汉字子程序*/
display_char(1,9,8,"高品质，创口碑！"); /*（在第3 行，第1 列，8 个汉字）调用显示汉字子程序*/
display_char(2,9,8,"诚信服务持续经营"); /*（在第4 行，第1 列，8 个汉字）调用显示汉字子程序*/
Switch(); /*等待按键P2.0 口*/
display_3232(1,1,yun); /*第一个是垂直地址，第二个是水平地址，第三个是图案数据。显示32*32 点阵的字或图片*/
display_3232(1,3,xing);/*第一个是垂直地址，第二个是水平地址，第三个是图案数据*/
Switch();
clear_screen();
display_1632(1,1,char_R);/*第一个是垂直地址，第二个是水平地址，第三个是图案数据。显示16*32 点阵的字或图片*/
display_1632(1,2,char_U);
display_1632(1,3,char_N);
display_1632(1,4,char_N);
display_1632(1,5,char_I);
display_1632(1,6,char_N);
display_1632(1,7,char_G);
Switch(); /*等待按键P2.0 口*/
}
}