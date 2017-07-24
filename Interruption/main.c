#include <reg52.h>               //包含寄存器的库文件                  
sbit  ADDR0 = P2^0;
sbit  ADDR1 = P2^1;
sbit  ADDR2 = P2^2;
sbit  ADDR3 = P2^3;

unsigned char code LedChar[] = {   //用数组来表示数码管真值表
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
    0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8e,
};
unsigned char LedNumber[4] = {0};
unsigned char j = 0;
unsigned int counter = 0;

void main()
{
    unsigned long stopwatch =0;
    ADDR3 = 1; P0 = 0XFF;   //74HC138和P0初始化部分
	
    TMOD = 0x01;  //设置定时器0为模式1
    TH0  = 0xFC;
    TL0  = 0x67;  //定时值初值,定时1ms
    TR0  = 1;     //打开定时器0
    EA = 1;       //打开中中断
    ET0 = 1;      //打开定时器0中断
	
    while(1)
    {
        if(1000 == counter)     //判断定时器0溢出是否达到1000次
        {
            counter = 0;
            stopwatch++;
            LedNumber[0] = stopwatch%10;
            LedNumber[1] = stopwatch/10%10;
            LedNumber[2] = stopwatch/100%10;
            LedNumber[3] = stopwatch/1000%10;
        }
    }
}
void InterruptTimer0() interrupt 1               //中断函数的特殊写法，数字’1’为中断入口号
{
    TH0 = 0xFC;   //溢出后进入中断重新赋值
    TL0 = 0x67;
    counter++;   //计数值counter加1
    P0 = 0xFF;   //消隐
	
    switch(j)
    {
        case 0: ADDR0=0; ADDR1=0; ADDR2=0; j++; P0=LedChar[LedNumber[0]]; break;
        case 1: ADDR0=1; ADDR1=0; ADDR2=0; j++; P0=LedChar[LedNumber[1]]; break;
        case 2: ADDR0=0; ADDR1=1; ADDR2=0; j++; P0=LedChar[LedNumber[2]]; break;
        case 3: ADDR0=1; ADDR1=1; ADDR2=0; j++; P0=LedChar[LedNumber[3]]; break;
        default: break;
    }    //动态刷新
}   
