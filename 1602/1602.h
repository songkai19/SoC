#ifndef __1602_H__
#define __1602_H__
 
#include <reg51.h>
#include "ZhangType.h"          //变量类型
#include "fun.h"            //常用函数 
 


#define CLEAR			0x01          //清屏,数据指针清零 （1）
#define RETURN     		0x02          //回车,数据指针清零 （1x）
#define ENTRY_MODE  	0x06          //读写字符后地址加1,屏显不移动 （110）
#define DISPLAY_ON		0x0f          //显示开,显示光标,光标闪烁 （1111）
#define SET_MODE		0x38          //8位数据接口, 2行显示，5*8点阵 （1110xx）
#define SET_CGRAM_ADDR	0x40          //设置CGRAM数据地址指针初始值
#define SET_DDRAM_ADDR	0x80          //设置DDRAM数据地址指针初始值，&&RW=0
#define BUSY_FLAG_ADDR	0x80          //设置BUSY所在地址，&&RW=1
#define PORT  			P2            //I/O口 
 
sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^2; 
 
void Init1602(void);        //初始化1602
void Write1602_Com(uint8 com);  //写命令
void Write1602_Dat(uint8 dat);  //写数据
//void CheckBusy(void);           //检查忙
//void Write1602_One_Dat(uint8 X,uint8 Y,uint8 dat);          //写一个数据
void Write1602_Str(uint8 addr,uint8 length,uint8 *pbuf);    //写一个数据串 
#endif//