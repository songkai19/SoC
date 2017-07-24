#include <uart.h>
#include <delay.h>
#include <1602_2.h>
#include <stdutils.h>

uint8_t T0RH = 0xFC;  //T0重载值的高字节
uint8_t T0RL = 0x67;  //T0重载值的低字节

uint8_t code openCmd[9] = {0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB};

void ConfigTimer0(uint8_t ms);

int main(void)
{
	int8_t i = 0;
	
	lcd1602_initial();
	UART_Init(9600);  // Initialize UARt at 9600 baud rate

	for (i = 0; i < 9; i++)
        UART_TxChar(openCmd[i]);

	while (1);
    return (0);

}

//给LED显示用的刷新定时器中断初始化
void ConfigTimer0(uint8_t ms)  //T0配置函数
{
	TMOD = 0x01;
	
	TH0 = T0RH;	 //加载 T0 重载值
	TL0 = T0RL;
	ET0 = 1;		//使能 T0 中断
	TR0 = 1;		//启动 T0
}