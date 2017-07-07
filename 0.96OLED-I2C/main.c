//////////////////////////////////////////////////////////////////////////////////	 
//嵌入式开发网
//	文 件 名	 : main.c

//	功能描述	 : OLED 4接口演示例程(51系列)
//				说明: 
//				----------------------------------------------------------------
//				GND	 电源地
//              VCC  接5V或3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  接P12，低电平复位管脚（并行版本才有以下3个管脚）
//              DC   接P13，数据/命令控制管脚
//              CS   接P14, 片选信号管脚						 
//				----------------------------------------------------------------

//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

 int main(void)
 {	
	 u8 t;
	 oled_init();			//初始化OLED	
	 oled_clear()		; 
		
	 t=' ';
	 oled_showcnchar(0,0,0);//嵌
	 oled_showcnchar(18,0,1);//入
		oled_showcnchar(36,0,2);//式
		oled_showcnchar(54,0,3);//开
		oled_showcnchar(72,0,4);//发
		oled_showcnchar(90,0,5);//网
	while(1) 
	{		
		oled_clear();
		oled_showcnchar(0,0,0);//嵌
		oled_showcnchar(18,0,1);//入
		oled_showcnchar(36,0,2);//式
		oled_showcnchar(54,0,3);//开
		oled_showcnchar(72,0,4);//发
		oled_showcnchar(90,0,5);//网
		oled_showstr(6,3,"0.96' OLED TEST",16);
	
		oled_showstr(0,6,"ASCII:",16);	
		oled_showstr(63,6,"CODE:",16);	
		oled_showchar(48,6,t,16);//显示ASCII字符		 
		t++;
		if(t>'~')t=' ';
		oled_showno(103,6,t,3,16);//显示ASCII字符的码值 	
			delay_ms(8000);
		delay_ms(8000);

					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		oled_drawbmp(0,0,128,8,BMP1);	//图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		oled_drawbmp(0,0,128,8,BMP1);
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
	}		
	
}
	