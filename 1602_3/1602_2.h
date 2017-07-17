#ifndef		_LCD1602_2_H_
#define		_LCD1602_2_H_

#include <reg52.h>
//#include <STC89C5xRC.H>

#define uint8		unsigned char
#define uint16	 unsigned short int
#define uint32	 unsigned long int
#define int8		 signed char
#define int16		signed short int
#define int32		signed long int
#define uint64	 unsigned long long int
#define int64		signed long long int

#define CLEAR			0x01          //����,����ָ������ ��1��
#define RETURN     		0x02          //�س�,����ָ������ ��1x��
#define ENTRY_MODE  	0x06          //��д�ַ����ַ��1,���Բ��ƶ� ��110��
#define DISPLAY_ON		0x0c          //��ʾ��,����ʾ���,��겻��˸ ��1100��
#define DISPLAY_OFF		0x08          //��ʾ��,����ʾ���,��겻��˸ ��1000��
#define SET_MODE		0x38          //8λ���ݽӿ�, 2����ʾ��5*8���� ��1110xx��

#define SET_CGRAM_ADDR	0x40          //����CGRAM���ݵ�ַָ���ʼֵ
#define SET_DDRAM_ADDR	0x80          //����DDRAM���ݵ�ַָ���ʼֵ
#define LINE_2_OFFSET	0x40		  //�ڶ�����ʼ��ַ
#define BUSY_FLAG_ADDR	0x80          //����BUSY���ڵ�ַ

#define DB	P0			// Data bus

sbit RW = P2^1;
sbit RS = P2^0;
sbit E = P2^2;

void lcd1602_check_busy(void);
void lcd1602_delay(unsigned int time);
void lcd1602_initial(void);
void lcd1602_write_char(uint8 x, uint8 y, uint8 chr);
void lcd1602_write_str(uint8 addr, uint8 strLen, uint8 *strBuf);
void lcd1602_write_dat(uint8 dat);
void lcd1602_write_com(uint8 com, bit toCheckBusy);
#endif