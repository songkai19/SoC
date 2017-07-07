/*******************************************************
*Name:(_main.c)
*Func:test
*Date:2014/09/09
*******************************************************/
#include <string.h>

#include "1602.h"
#include "fun.h" 

uint8 code hot[8]={                                  //temp
0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00
};
uint8 code pi[8]={                       
0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,0x00                 //pai
}; 
uint8 code strMCU[]="I love MCU!";
uint8 code strTest[]="LCD1602 Test!";
uint8 code blog[]="Welcome to my blog!"; 
uint8 i; 
void main()
{
    Init1602();                                      //init 1602 
    //自定义CGRAM
    Write1602_Str(0x40,8,hot);                       //temp in C
    Write1602_Str(0x48,8,pi);                        //pai
 
    Write1602_Str(0x80,strlen(strMCU),strMCU);           //"I love MCU!"
    Write1602_Str(0x80+0x40,strlen(strTest),strTest);    //"LCD1602 Test!"
 
    for(i=0;i<50;i++)                             //延时一段时间
       Delay(10000);
 
    Write1602_Com(CLEAR);                            //指令执行时间较长
    Delay(500);                                      //多加一些延时
    for(i=0;i<16;i++)
       Write1602_Dat(0);
   
    Write1602_Com(0xc0);                          //设置DDRAM地址
    for(i=0;i<16;i++)
       Write1602_Dat(1);
    for(i=0;i<50;i++)                             //延时一段时间
       Delay(10000);
   
    Write1602_Com(CLEAR);                            //指令执行时间较长
    Delay(500);                                      //多加一些延时
    Write1602_Str(0x80+0x10,strlen(blog),blog);          //写在显示之外
    while(1)
    {
       Write1602_Com(0x18);                      //左移
       for(i=0;i<20;i++)                         //延时
           Delay(10000);
    }
} 