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
void lcd1602_delay(uint16 time)
{
	while(time--);
}


/*
入口参数:无
出口参数:无
函数功能:检测忙状态
*/
void lcd1602_check_busy(void)
{
	DB = BUSY_FLAG_ADDR;
	RS = 0;
	RW = 1;
	E = 1;
	while(DB & 0x80);
	E = 0;	 
}

/*
入口参数:com：待写入的指令；toCheckBusy:是否检测忙状态（1检测，0不检测）
出口参数:无
函数功能:向IC芯片发出命令（包括功能设置，以及设置CGRAM和DDRAM的地址）
*/
void lcd1602_write_com(uint8 com, bit toCheckBusy)
{
	if(toCheckBusy)
		lcd1602_check_busy();
	RS = 0;
	RW = 0;
	DB = com;
	E = 1;
	E = 0;
}

/*
入口参数:dat:待写入的字符
出口参数:无
函数功能:向DDRAM写入一个字符
*/
void lcd1602_write_dat(uint8 dat)
{
	lcd1602_check_busy();
	RS = 1;
	RW = 0;
	DB = dat;
	E = 1;
	E = 0;
}

/*
入口参数:无
出口参数:无
函数功能:初始化lcd1602
*/
void lcd1602_initial(void)
{
	lcd1602_delay(1500);
	lcd1602_write_com(SET_MODE, 0); // 预热指令
	lcd1602_delay(500);
	lcd1602_write_com(SET_MODE, 0); // 预热指令
	lcd1602_delay(500);
	lcd1602_write_com(SET_MODE, 0); // 预热指令
	lcd1602_write_com(SET_MODE, 1); // 设置显示模式
	lcd1602_write_com(DISPLAY_OFF, 1); // 关闭显示屏
	lcd1602_write_com(CLEAR, 1); // 清屏
	lcd1602_write_com(ENTRY_MODE, 1); // 设置数据读取和移动方式
	lcd1602_write_com(DISPLAY_ON, 1); // 打开显示屏
}


/*
入口参数:x,y:写入地址的坐标
出口参数:无
函数功能:向显示屏输出一个字符
*/
void lcd1602_write_char(uint8 x, uint8 y, uint8 chr)
{
	x &= 0x0f;	 //限定x在0~15
	y &= 0x01;	 //限定x在0~1
	
	// 如果是第二行就加上偏移量
	if(y) x |= LINE_2_OFFSET;
	x |= SET_DDRAM_ADDR;
	
	// 设置DDRAM起始地址，根据上面1还是2行判断结果为准
	lcd1602_write_com(x, 1);
	lcd1602_write_dat(chr);
}

/*
入口参数:addr：写入目标地址；strLen待显示字符串长度；strBuf：待显示的字符串
出口参数:无
函数功能:向显示屏输出一个字符串，或向CGRAM写入自定义字符
*/
void lcd1602_write_str(uint8 addr, uint8 strLen, uint8 *strBuf)
{
	uint8 i;
	// 设置写入目标地址：CGRAM 或者 DDRAM
	lcd1602_write_com(addr, 1);
	
	for (i = 0; i < strLen; i++)
	{
		lcd1602_write_dat(strBuf[i]);
	}
}
