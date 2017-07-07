#include <reg52.h>
sbit LED = P0^6;

void delay(int z)  
{  
    int x,y;  
    for(x=z;x>0;x--)  
    for(y=110;y>0;y--);  
}

void main()
{
	while(1)
	{
		LED = 1;
		delay(500);
		LED = 0;
		delay(500);
	}
}