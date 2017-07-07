//////////////////////////////////////////////////////////////////////////////////	 
//Ƕ��ʽ������
//mcudev.taobao.com
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   P1^0��SCL��
//              D1   P1^1��SDA��
//              RES  ��P12���͵�ƽ��λ�ܽţ����а汾��������3���ܽţ�
//              DC   ��P13������/������ƹܽ�
//              CS   ��P14, Ƭѡ�źŹܽ�
//              ----------------------------------------------------------------
//******************************************************************************/

#include "REG51.h"
#ifndef __OLED_H__
#define __OLED_H__

#define u8 unsigned char
#define u32 unsigned int

// �����������
#define OLED_CMD  0	// д����
#define OLED_DATA 1	// д����
#define OLED_MODE 0 // ģʽѡ��0-4�ߴ���ģʽ��1-����8080ģʽ
	  
// OLED�˿ڶ���
sbit OLED_SCLK = P1^0; // I2C��SCLKʱ�ӹܽ�
sbit OLED_SDA = P1^1; // I2C��SDA���ݹܽ�

// OLED��������
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    										   

void delay_ms(unsigned int ms);

// OLED�����ú���
void oled_wr_byte(unsigned dat,unsigned cmd);  							   		    
void oled_init(void);
void oled_clear(void);
void oled_showchar(u8 x,u8 y,u8 chr,u8 char_size);
void oled_showno(u8 x,u8 y,u32 num,u8 len,u8 no_size);
void oled_showstr(u8 x,u8 y, u8 *p,u8 char_size);	 
void oled_setposition(u8 x, u8 y);
void oled_showcnchar(u8 x,u8 y,u8 no);
void oled_drawbmp(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
//void fill_picture(u8 fill_Data);

// I2C���߿��ƺ���
void i2c_start();
void i2c_stop();
void write_com(u8 com);
void write_data(u8 dat);
void write_byte(u8 byt);
void wait_ack();
#endif