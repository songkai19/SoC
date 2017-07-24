//youtube 视频39期
//https://www.youtube.com/watch?v=K18Hl0wdpao
#include <12864.h>

uchar table[]="万事如意20170710";
	
//延时函数，在12MHz的晶振频率下
//大约50us的延时
void delay_50us(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=19;j>0;j--);
}

//延时函数，在12MHz的晶振频率下
//大约50ms的延时
void delay_50ms(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=6245;j>0;j--);
}

void write_12864(uchar dat, uchar rsFlag)
{
	int i;
	//拉高片选位，使时钟信号变得有效
	cs=1;
	
	//刷入前五位1 - 同步位串
	//此時傳輸計數將被重置並且串列傳輸將被同步
	for(i=0;i<5;i++)
	{
		sid=1;
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
	}
	//再跟隨的兩個位元字串分別指定傳輸方向位元（RW）及暫存器選擇位元（RS），
	//最後第八的位元則為〝0〞。
	sid=0; //rw=0;
	sclk=0;
	delay_50us(1);
	sclk=1;
	delay_50us(1);
	
	sid=rsFlag; //rs=0/1 com/data;
	sclk=0;
	delay_50us(1);
	sclk=1;
	delay_50us(1);
	
	sid=0; //尾位0占位;
	sclk=0;
	delay_50us(1);
	sclk=1;
	delay_50us(1);
	
	//在接收到同步位元及RW和RS資料的啟始位元組後，每一個八位元的指令將被分為兩個位元組接收到：
	//較高4位元（DB7~DB4）的指令資料將會被放在第一個位元組的LSB部分，
	//而較低4位元（DB3~DB0）的指令資料則會被放在第二個位元組的LSB部分，至於相關的另四位元則都為0。
	for(i=0;i<4;i++)
	{
		//0x80 = 0b10000000
		if(dat&0x80) 
			sid=1;
		else
			sid=0;
		
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
		//逐位检查1/0
		dat<<=1;
	}
	
	//第四位用零占位
	for(i=0;i<4;i++)
	{
		sid=0;
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
	}
	
	for(i=0;i<4;i++)
	{
		//0x80 = 0b10000000
		if(dat&0x80) 
			sid=1;
		else
			sid=0;
		
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
		//逐位检查1/0
		dat<<=1;
	}
	
	//第四位用零占位
	for(i=0;i<4;i++)
	{
		sid=0;		
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
	}
	
	//拉低片选，结束1字节数据传输
	cs=0;
}

void init_12864lcd(void)
{
	//启动上电
	reset=0;
	delay_50us(60);
	reset=1;
	delay_50ms(40);
	write_12864(0x30, 0);
	delay_50us(30);
	
	//开启屏幕，初始化设置
	write_12864(0x0f, 0);
	delay_50us(4);
	/*write_12864(0x01, 0);
	delay_50us(240);
	write_12864(0x06, 0);
	delay_50us(10);
	*/
}

void display1(void)
{
	uchar i;
	uchar a;
	a=0x10;
	write_12864(0x80, 0);
	delay_50us(1);
	for(i=0;i<16;i++)
	{
		write_12864(a+i, 1);
		delay_50us(1);
	}
}

void display2(void)
{
	uchar i;
	uchar a,b;
	a=0xBA;
	b=0xC0;
	
	write_12864(0x80, 0);
	for(i=0;i<16;i++)
	{
		write_12864(a, 1);
		write_12864(b+i, 1);
		delay_50us(1);
	}
	
}

void display3(void)
{
	uchar i;
	
	write_12864(0x80, 0);
	for(i=0;i<16;i++)
	{
		write_12864(table[i], 1);
		delay_50us(1);
	}
	
}