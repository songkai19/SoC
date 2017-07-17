#include <I2C.h>
#include <stdio.h>

void main(void)
{
	unsigned char dat[7];
	unsigned int second, minute, hour, day, month, year;
	I2C_Init();
	if (I2C_Gets(0xd1, 7, dat))
	{
		second = dat[0];
		putchar(dat[0]);
		minute = dat[1];
		hour = dat[2];
		day = dat[3];
		month = dat[4];
		year = dat[5];
	}
	
}