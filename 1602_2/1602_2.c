/*****************************************************
								液晶屏lcd1602驱动
								未定义各I/0口(RS,RW,E,DB)，需头文件外定义
******************************************************/

#include <1602_2.h>

/*
入口参数:time:延时长度
出口参数:无
函数功能:非精确延时
*/
void lcd1602_delay(unsigned int time)
{
		unsigned int		i;
		
		for(i = 0; i < time; i++)
				;
}


/*
入口参数:无
出口参数:无
函数功能:检测忙状态
*/
void lcd1602_busy(void)
{
		DB = 0x80;
		RS = 0;
		RW = 1;
		E = 1;
		while(DB & 0x80)
				;
		E = 0;	 
}

/*
入口参数:combuf：写入的指令；busy:是否检测忙状态（1检测，0不检测）
出口参数:无
函数功能:写入一个指令
*/
void lcd1602_write_com(unsigned char combuf, bit busy)
{
		if(busy)
				lcd1602_busy();
		RS = 0;
		RW = 0;
		DB = combuf;
		E = 1;
		E = 0;
}

/*
入口参数:databuf:写入的数据
出口参数:无
函数功能:写入一个数据
*/
void lcd1602_write_data(unsigned char databuf)
{
		lcd1602_busy();
		RS = 1;
		RW = 0;
		DB = databuf;
		E = 1;
		E = 0;
}

/*
入口参数:x,y:写入地址的坐标
出口参数:无
函数功能:写入地址
*/
void lcd1602_write_address(unsigned char x, unsigned char y)
{
	x &= 0x0f;	 //限定x在0~15
	y &= 0x01;	 //限定x在0~1
	
	if(y == 0)
			lcd1602_write_com(SET_DDRAM_ADDR + x, 1); //set CGRAM Address
	else
			lcd1602_write_com(SET_DDRAM_ADDR + 0x40 + x, 1); //set CGRAM with offset (row 2)
}

/*
入口参数:无
出口参数:无
函数功能:初始化lcd1602
*/
void lcd1602_initial(void)
{
	lcd1602_delay(1500);
	lcd1602_write_com(SET_MODE, 0);
	lcd1602_delay(500);
	lcd1602_write_com(SET_MODE, 0);
	lcd1602_delay(500);
	lcd1602_write_com(SET_MODE, 0);
	lcd1602_write_com(SET_MODE, 1);
	lcd1602_write_com(DISPLAY_ON, 1);
	lcd1602_write_com(ENTRY_MODE, 1);
	lcd1602_write_com(SET_MODE, 1);
	lcd1602_delay(500);
	lcd1602_write_com(CLEAR, 0);
	lcd1602_delay(500);
}

/*
入口参数:x,y:显示字符的坐标; databuf:显示的字符
出口参数:无
函数功能:指定位置显示一个字符
*/
void lcd1602_putchar(unsigned char x, unsigned char y, const unsigned char databuf)
{
		lcd1602_write_address(x, y);
		lcd1602_write_data(databuf);
}

/*
入口参数:x,y:显示字符串的起始位置; databuf:显示的字符串
出口参数:count: 成功显示的字符个数
函数功能:显示一个字符串（从起始位置开始输出，会替换起始
				 位置后所有已显示的字符，输出过程中会自动换行，
				 若超出显示位最后一位（16,1），自动转到起始位（0,0）
				 继续输出），并返回成功输出的个数
*/
unsigned char lcd1602_puts(unsigned char x, unsigned char y, const unsigned char databuf[])
{
		unsigned char		count		= 0;
		unsigned char		i;
		
		for(i = 0; databuf[i] != '\0'; i++)
		{
				lcd1602_putchar(x, y, databuf[i]);
				count++;
				
				if(++x >= LCD1602_COL)
				{
						x = 0;
						if(++y >= LCD1602_ROW)
								y = 0;
				}
		}
		
		return count;
}
