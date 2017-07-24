#include "reg52.h"

sbit LED = P1^0;

void main()
{
	unsigned char cnt = 0;
	
	TMOD = 0x01;
	TH0 = 0xBB;
	TL0 = 0x00;
	TR0 = 1;
	
	while (1)
	{
		if (TF0 == 1)
		{
			TF0 = 0;
			TH0 = 0xBB;
			TL0 = 0x00;
			cnt++;
			if (cnt >= 50)
			{
				cnt = 0;
				LED = ~LED;
			}
		}
	}
}