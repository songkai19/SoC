#ifndef		_LCD1602_2_H_
#define		_LCD1602_2_H_

//#include <reg52.h>
#include <STC89C5xRC.H>

#define uint8		unsigned char
#define uint16	 unsigned short int
#define uint32	 unsigned long int
#define int8		 signed char
#define int16		signed short int
#define int32		signed long int
#define uint64	 unsigned long long int
#define int64		signed long long int

#define	LCD1602_ROW		2		 //最大显示行数
#define	LCD1602_COL		16		//最大显示列数

#define CLEAR			0x01          //清屏,数据指针清零 （1）
#define RETURN     		0x02          //回车,数据指针清零 （1x）
#define ENTRY_MODE  	0x06          //读写字符后地址加1,屏显不移动 （110）
#define DISPLAY_ON		0x0c          //显示开,不显示光标,光标不闪烁 （1100）
#define SET_MODE		0x38          //8位数据接口, 2行显示，5*8点阵 （1110xx）

#define SET_CGRAM_ADDR	0x40          //设置CGRAM数据地址指针初始值
#define SET_DDRAM_ADDR	0x80          //设置DDRAM数据地址指针初始值
#define LINE_2_OFFSET	0x40		  //第二行起始地址
#define BUSY_FLAG_ADDR	0x80          //设置BUSY所在地址

#define DB	P1			// Data bus

sbit RW = P2^0;
sbit RS = P2^1;
sbit E = P2^2;

void lcd1602_busy(void);
void lcd1602_delay(unsigned int time);
void lcd1602_initial(void);
void lcd1602_putchar(unsigned char x, unsigned char y, const unsigned char databuf);
unsigned char lcd1602_puts(unsigned char x, unsigned char y, const unsigned char databuf[]);
void lcd1602_write_address(unsigned char x, unsigned char y);
void lcd1602_write_com(unsigned char combuf, bit busy);
void lcd1602_write_address(unsigned char x, unsigned char y);

#endif