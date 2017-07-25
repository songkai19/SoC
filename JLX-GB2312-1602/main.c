#include "reg52.h"
#include "string.h"

#define uint8 unsigned char
#define uint16 unsigned int 
#define uint32 unsigned long

sbit SI = P1^6; // HOST SDO to peripheral unit SI
sbit SO = P1^5; // HOST SDI to peripheral unit SO
sbit SCLK = P1^7;
sbit CS = P1^4;

void delay_us(uint16 n_us);
void jlx_gb2312_send_cmd(uint8 cmd);
static uint8 jlx_gb2312_get_byte();
void jlx_gb2312_get_8x16(uint32 fontAddr, uint8 column);
void jlx_gb2312_get_16x16(uint32 fontAddr, uint8 column);
void jlx_gb2312_display_string(uint8 column, uint8 *textStr);

void main()
{
	jlx_gb2312_display_string(0, "宋");
}

void jlx_gb2312_display_string(uint8 column, uint8 *textStr)
{
	uint8 i = 0;
	uint32 fontAddr;
	
	while (textStr[i] > 0x00)
	{
		if(((textStr[i] >= 0xb0) && (textStr[i] <= 0xf7)) && (textStr[i+1] >= 0xa1))
		{
			//国标简体（GB2312）汉字在晶联讯字库IC 中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
			fontAddr = (textStr[i] - 0xb0) * 94;
			fontAddr += (textStr[i+1] - 0xa1) + 846;
			fontAddr = (uint32)(fontAddr * 32);
			jlx_gb2312_get_16x16(fontAddr, column); //从指定地址读出字符写到液晶屏指定（page,column)座标中
			
			i += 2;
			column += 16;
		}
		else if(((textStr[i] >= 0xa1) && (textStr[i] <= 0xa3)) && (textStr[i+1] >= 0xa1))
		{
			//国标简体（GB2312）15x16 点的字符在晶联讯字库IC 中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
			fontAddr = (textStr[i] - 0xa1) * 94;
			fontAddr += (textStr[i+1] - 0xa1);
			fontAddr = (uint32)(fontAddr * 32);
			jlx_gb2312_get_16x16(fontAddr, column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
			
			i += 2;
			column += 16;
		}
		else if((textStr[i] >= 0x20) && (textStr[i] <= 0x7e))
		{
			fontAddr = (textStr[i] - 0x20);
			fontAddr = (uint32)(fontAddr * 16);
			fontAddr = (uint32)(fontAddr + 0x3cf80);
			jlx_gb2312_get_8x16(fontAddr,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
			
			i += 1;
			column += 8;
		}
		else
			i++;
	}
}

void jlx_gb2312_get_8x16(uint32 fontAddr, uint8 column)
{
	uint8 i, j, byteOfChar;
	
	// 拉低串口片选ROM_CS开始字符读取时钟
	CS = 0;
	// READ读指令代码，1字节（FAST_READ:0x0B）
	// 给字库IC发出读指令 
	jlx_gb2312_send_cmd(0x03);
	
	// 发送将要显示字符的IC库地址给字库IC ROM
	// 一次取8位存入字库IC ROM等待显示读取
	jlx_gb2312_send_cmd((fontAddr & 0xff0000) >> 16); //地址的高8 位,共24 位
	jlx_gb2312_send_cmd((fontAddr & 0xff00) >> 8); //地址的中8 位,共24 位
	jlx_gb2312_send_cmd(fontAddr & 0xff); //地址的低8 位,共24 位
	
	// 字符构成是竖置横排，所以一般是逐列->逐行输出
	// 8x16就是8列16行，每个行列焦点为1bit，1个led
	// 扫描顺序是第1行8列,1列8bit=1byte；然后第2行
	// byte1		btye2			btye3			btye4			btye5			btye6			btye7			btye8
	// |B0|			|				|				|				|				|				|				|
	// |B1|			|				|				|				|				|				|				|
	// |B2|			|				|				|				|				|				|				|
	// |B3|			|				|				|				|				|				|				|
	// |B4|			|				|				|				|				|				|				|
	// |B5|			|				|				|				|				|				|				|
	// |B6|			|				|				|				|				|				|				|
	// |B7|			|				|				|				|				|				|				| <-- 第一行结束
	// byte8		btye9			btye10			btye11			btye12			btye13			btye14			btye15
	// |B0|			|				|				|				|				|				|				|
	// |B1|			|				|				|				|				|				|				|
	// |B2|			|				|				|				|				|				|				|
	// |B3|			|				|				|				|				|				|				|
	// |B4|			|				|				|				|				|				|				|
	// |B5|			|				|				|				|				|				|				|
	// |B6|			|				|				|				|				|				|				|
	// |B7|			|				|				|				|				|				|				| <-- 第二行结束
	
	for(j = 0; j < 2; j++)
	{
		//lcd_address(page+j,column);
		for(i = 0; i < 8; i++)
		{
			byteOfChar = jlx_gb2312_get_byte();
			//transfer_data_lcd(byteOfChar); //写数据到LCD,每写完1 字节的数据后列地址自动加1
		}
	}

	// 拉高ROM_CS结束串口ROM_OUT的输出
	CS=1;
}

void jlx_gb2312_get_16x16(uint32 fontAddr, uint8 column)
{
	uint8 i, j, byteOfChar;
	
	CS = 0;
	jlx_gb2312_send_cmd(0x03);
	jlx_gb2312_send_cmd( (fontAddr & 0xff0000) >> 16 ); //地址的高8 位,共24 位
	jlx_gb2312_send_cmd( (fontAddr & 0xff00) >> 8 ); //地址的中8 位,共24 位
	jlx_gb2312_send_cmd( fontAddr & 0xff ); //地址的低8 位,共24 位
	
	for(j = 0; j < 2; j++)
	{
		//lcd_address(page+j,column);
		for(i = 0; i < 16; i++)
		{
			byteOfChar = jlx_gb2312_get_byte();
			//transfer_data_lcd(disp_data); //写数据到LCD,每写完1 字节的数据后列地址自动加1
			//todo: try to display the data on lcd12864 to verify the result,
			//or we can use led to read the data
		}
	}
	
	CS=1;
}

void jlx_gb2312_send_cmd(uint8 cmd)
{
	uint8 i;
	for(i = 0; i < 8; i++)
	{
		SCLK = 0;
		delay_us(1); // 下降沿跨度时间，其实应该是ns级别
		// 从高位逐位发送
		if(cmd & 0x80) 
			SI = 1;
		else
			SI = 0;
		
		cmd <<= 1;
		SCLK=1;
		delay_us(1);
	}
}

static uint8 jlx_gb2312_get_byte()
{
	uint8 i;
	uint8 retData = 0;
	
	for(i = 0; i < 8; i++)
	{
		// 字符的字节数据通过SO=ROM_OUT串口移位输出
		SO = 1; // 拉高等待输出
		SCLK = 0;
		delay_us(1);
		
		retData <<= 1;
		if(SO)
			retData |= 1;
		
		// 拉高时钟上沿等待下次输出
		SCLK=1;
		delay_us(1);
	}
	return(retData);
}

//非精确短延时
void delay_us(uint16 n_us)
{
	uint16 j,k;
	for(j = 0; j < n_us; j++)
	for(k = 0; k < 1; k++);
}