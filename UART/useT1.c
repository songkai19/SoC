#include <reg52.h>

void ConfigUART(unsigned int baud);

void main ()
{
    ConfigUART(9600);  //配置波特率为 9600

    while(1);
}

void ConfigUART(unsigned int baud)  //串口配置函数， baud 为波特率
{
    SCON = 0x50;   //配置串口为模式 1
    TMOD &= 0x0F;  //清零 T1 的控制位
    TMOD |= 0x20;  //配置 T1 为模式 2
    TH1 = 256 - (11059200/12/32) / baud;  //计算 T1 重载值
    TL1 = TH1;     //初值等于重载值
    ET1 = 0;       //禁止 T1 中断
    TR1 = 1;       //启动 T1
    ES = 1;   //打开串口中断
    EA = 1;   //打开总中断
}

void InterruptUART() interrupt 4
{
    if (RI)  //接收到字节
    {
        RI = 0;  //手动清零接收中断标志位
        SBUF = SBUF + 1;//接收数据 +1 发回去 ，左边为发送SBUF，右边为接收 SBUF 。
    }
    if (TI)  //字节发送完毕
    {
        TI = 0;  //手动清零发送中断标志位
    }  
}
