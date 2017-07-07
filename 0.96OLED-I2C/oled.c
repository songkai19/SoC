//////////////////////////////////////////////////////////////////////////////////	 
//嵌入式开发网
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  接P12，低电平复位管脚（并行版本才有以下3个管脚）
//              DC   接P13，数据/命令控制管脚
//              CS   接P14, 片选信号管脚
//              ----------------------------------------------------------------

//******************************************************************************/。

#include "oled.h"
#include "oledfont.h"

//OLED的显存
//存放格式如下. 8页，每页128位显示字符，总共可以显示8*16字节=128个8*8字符/屏,64个8*16字符/屏
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

void delay_ms(unsigned int ms)
{
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}

// 主机在I2C总线上发出开始信号，即从高到低拉低SDA和SCLK
void i2c_start()
{

	OLED_SCLK = 1 ;
	OLED_SDA = 1;
	OLED_SDA = 0;
	OLED_SCLK = 0;
}

void i2c_stop()
{
	OLED_SCLK = 0;
	OLED_SDA = 0;
	OLED_SDA = 1;
}

void wait_ack()
{
	OLED_SCLK = 1;
	OLED_SCLK = 0;
}

void write_byte(u8 byt)
{
	u8 i;
	u8 m,da;
	da=byt;
	OLED_SCLK = 0;
	
	for(i=0;i<8;i++)
	{
		m=da;
		m=m&0x80;
		
		if(m==0x80) OLED_SDA = 1;
		else OLED_SDA = 0;
		
		da = da<<1;
		OLED_SCLK = 1;
		OLED_SCLK = 0;
	}
}

void write_com(u8 com)
{
	i2c_start();
	write_byte(0x78);            //Slave address,SA0=0
	wait_ack();	
   write_byte(0x00);			//write command
	wait_ack();	
   write_byte(com); 
	wait_ack();	
   i2c_stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void write_data(u8 dat)
{
   i2c_start();
   write_byte(0x78);			//D/C#=0; R/W#=0
	wait_ack();	
   write_byte(0x40);			//write data
	wait_ack();	
   write_byte(dat);
	wait_ack();	
   i2c_stop();
}
void oled_wr_byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   write_data(dat);
   
		}
	else {
   write_com(dat);
		
	}
}

/********************************************
// fill_Picture
********************************************/
//void fill_picture(unsigned char fill_Data)
//{
//	unsigned char m,n;
//	for(m=0;m<8;m++)
//	{
//		oled_wr_byte(0xb0+m,0);		//page0-page1
//		oled_wr_byte(0x00,0);		//low column start address
//		oled_wr_byte(0x10,0);		//high column start address
//		for(n=0;n<128;n++)
//			{
//				oled_wr_byte(fill_Data,1);
//			}
//	}
//}

//坐标设置

	void oled_setposition(unsigned char x, unsigned char y) 
{ 	oled_wr_byte(0xb0+y,OLED_CMD);
	oled_wr_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	oled_wr_byte((x&0x0f),OLED_CMD); 
}   	  
////开启OLED显示    
//void OLED_Display_On(void)
//{
//	oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
//	oled_wr_byte(0X14,OLED_CMD);  //DCDC ON
//	oled_wr_byte(0XAF,OLED_CMD);  //DISPLAY ON
//}
////关闭OLED显示     
//void OLED_Display_Off(void)
//{
//	oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
//	oled_wr_byte(0X10,OLED_CMD);  //DCDC OFF
//	oled_wr_byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void oled_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_wr_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		oled_wr_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		oled_wr_byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)oled_wr_byte(0,OLED_DATA); 
	} //更新显示
}

//void OLED_On(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		oled_wr_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//		oled_wr_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
//		oled_wr_byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
//		for(n=0;n<128;n++)oled_wr_byte(1,OLED_DATA); 
//	} //更新显示
//}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void oled_showchar(u8 x,u8 y,u8 chr,u8 char_size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(char_size ==16)
			{
			oled_setposition(x,y);	
			for(i=0;i<8;i++)
			oled_wr_byte(F8X16[c*16+i],OLED_DATA);
			oled_setposition(x,y+1);
			for(i=0;i<8;i++)
			oled_wr_byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				oled_setposition(x,y);
				for(i=0;i<6;i++)
				oled_wr_byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void oled_showno(u8 x,u8 y,u32 num,u8 len,u8 no_size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				oled_showchar(x+(no_size/2)*t,y,' ',no_size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	oled_showchar(x+(no_size/2)*t,y,temp+'0',no_size); 
	}
} 
//显示一个字符号串
void oled_showstr(u8 x,u8 y,u8 *chr,u8 char_size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		oled_showchar(x,y,chr[j],char_size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void oled_showcnchar(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	oled_setposition(x,y);	
    for(t=0;t<16;t++)
		{
				oled_wr_byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		oled_setposition(x,y+1);	
    for(t=0;t<16;t++)
			{	
				oled_wr_byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oled_drawbmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		oled_setposition(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	oled_wr_byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//初始化SSD1306					    
void oled_init(void)
{ 	
 
oled_wr_byte(0xAE,OLED_CMD);//--display off
	oled_wr_byte(0x00,OLED_CMD);//---set low column address
	oled_wr_byte(0x10,OLED_CMD);//---set high column address
	oled_wr_byte(0x40,OLED_CMD);//--set start line address  
	oled_wr_byte(0xB0,OLED_CMD);//--set page address
	oled_wr_byte(0x81,OLED_CMD); // contract control
	oled_wr_byte(0xFF,OLED_CMD);//--128   
	oled_wr_byte(0xA1,OLED_CMD);//set segment remap 
	oled_wr_byte(0xA6,OLED_CMD);//--normal / reverse
	oled_wr_byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_wr_byte(0x3F,OLED_CMD);//--1/32 duty
	oled_wr_byte(0xC8,OLED_CMD);//Com scan direction
	oled_wr_byte(0xD3,OLED_CMD);//-set display offset
	oled_wr_byte(0x00,OLED_CMD);//
	
	oled_wr_byte(0xD5,OLED_CMD);//set osc division
	oled_wr_byte(0x80,OLED_CMD);//
	
	oled_wr_byte(0xD8,OLED_CMD);//set area color mode off
	oled_wr_byte(0x05,OLED_CMD);//
	
	oled_wr_byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	oled_wr_byte(0xF1,OLED_CMD);//
	
	oled_wr_byte(0xDA,OLED_CMD);//set com pin configuartion
	oled_wr_byte(0x12,OLED_CMD);//
	
	oled_wr_byte(0xDB,OLED_CMD);//set Vcomh
	oled_wr_byte(0x30,OLED_CMD);//
	
	oled_wr_byte(0x8D,OLED_CMD);//set charge pump enable
	oled_wr_byte(0x14,OLED_CMD);//
	
	oled_wr_byte(0xAF,OLED_CMD);//--turn on oled panel
}  





























