#include <12864.h>

void main(void)
{
	psb=0;
	delay_50us(3);
	init_12864lcd();
	delay_50us(30);
	while(1)
	{
		//display1();
		//display2();
		display3();
		delay_50ms(10);
	}
}