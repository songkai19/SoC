#include <string.h>
#include <1602_2.h>

//uint8 code tempSymbol[8]={                                  //temp
//0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00
//};
//uint8 code paiSymbol[8]={                       
//0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,0x00                 //pai
//}; 

//uint8 code strMCU[]="I love MCU!";
uint8 code strTest[]="LCD1602 RUNNING TEST\n";
//uint8 code blog[]="Welcome to my blog!"; 
//uint8 myChar = 'S';

void main()
{
	lcd1602_initial();
	
	// 设置CGRAM地址，准备写入自定义字符
	//lcd1602_write_com(SET_CGRAM_ADDR, 
	
	lcd1602_puts(0, 0, strTest);
	lcd1602_delay(5000);
	while (1);
}