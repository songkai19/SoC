#include <reg52.h>               //包含寄存器的库文件       

sbit LED = P1^7;

unsigned char code LedChar[] = {   //用数组来表示数码管真值表
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
    0x80,0x90
};

unsigned char LedBuffer[] = {0xFF, 0xFF, 0xFF, 0xFF};
unsigned char LED_W[] = {0xF7, 0xFB, 0xFD, 0xFE};

void main()
{
    unsigned char i = 0;
	unsigned int cnt = 0;
	unsigned long sec = 0;

	TMOD = 0x01;
	TH0 = 0xFC;
	TL0 = 0x67;
	TR0 = 1;
	
    while(1)
    {
		if (TF0 == 1)
		{
			TF0 = 0;
			TH0 = 0xFC;
			TL0 = 0x67;
			cnt++;
			
			if(cnt >= 1000)     //判断定时器0溢出是否达到1000次
			{
				cnt = 0;
				++sec;
					
				LED = 0;
				
				LedBuffer[0] = LedChar[sec%10];
				LedBuffer[1] = LedChar[sec/10%10];
				LedBuffer[2] = LedChar[sec/100%10];
				LedBuffer[3] = LedChar[sec/1000%10];
				
				LED = 1;
			}
			
			switch(i)
			{
				case 0: P0 = LedBuffer[i]; P2 = LED_W[i]; i++; break;
				case 1: P0 = LedBuffer[i]; P2 = LED_W[i]; i++; break;
				case 2: P0 = LedBuffer[i]; P2 = LED_W[i]; i++; break;
				case 3: P0 = LedBuffer[i]; P2 = LED_W[i]; i = 0; break;
				default: break;
			}    //动态刷新
		}
	}
}