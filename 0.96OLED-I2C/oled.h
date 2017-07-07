//////////////////////////////////////////////////////////////////////////////////	 
//嵌入式开发网
//mcudev.taobao.com
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  接P12，低电平复位管脚（并行版本才有以下3个管脚）
//              DC   接P13，数据/命令控制管脚
//              CS   接P14, 片选信号管脚
//              ----------------------------------------------------------------
//******************************************************************************/

#include "REG51.h"
#ifndef __OLED_H__
#define __OLED_H__

#define u8 unsigned char
#define u32 unsigned int

// 命令参数定义
#define OLED_CMD  0	// 写命令
#define OLED_DATA 1	// 写数据
#define OLED_MODE 0 // 模式选择：0-4线串行模式；1-并行8080模式
	  
// OLED端口定义
sbit OLED_SCLK = P1^0; // I2C的SCLK时钟管脚
sbit OLED_SDA = P1^1; // I2C的SDA数据管脚

// OLED参数设置
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    										   

void delay_ms(unsigned int ms);

// OLED控制用函数
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

// I2C总线控制函数
void i2c_start();
void i2c_stop();
void write_com(u8 com);
void write_data(u8 dat);
void write_byte(u8 byt);
void wait_ack();
#endif