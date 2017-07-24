/*   头文件    */
#include <reg52.h>
#include <1602_2.h>
#include <string.h>
 
/*   宏定义    */
#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
	
//#define FOSC 11059200L //STC89RC52的晶振值

/* define SFR */
sbit TEST_LED = P1^0;               //work LED, flash once per second

/*     变量定义      */
uint count=10;  //周期计时
uchar cnt_PM25=0;	//数码管PM2.5循环计数
uchar T0RH = 0;  //T0重载值的高字节
uchar T0RL = 0;  //T0重载值的低字节
uchar R_data = 0;  //读数据标志


/*------激光传感器-------------*/
uchar USART_RX_STB=0;
uchar complete_flag=0;
uchar USART_RX_BUF[16];  //接收缓冲,最大16个字节.
uchar ParsedData[2];
uchar jj=0;              //判断串口一帧数据是否接收完成的计数变量
uchar PM25_data[2] = {0};
uchar PM10_data[2] = {0};
uint PM_ASC[8];				 //数据分解后的存储数组
uint HR_crc = 0;
uint check = 0;

//-------开机、读数据、关机指令----------------------------------
uchar code openComm[9]={0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB};
uchar code retrieveComm[9]={0xAA,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xBB};
uchar code closeComm[9]={0xAA,0x03,0x00,0x00,0x00,0x00,0x01,0x68,0xBB};

//------------数码管-------------------------------------
uchar code DIS_SEG7[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};	 		 //共阳极数码管编码0~9
//uchar code DIS_SEG7[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};			 //共阴极数码管编码0~9
uchar code DIS_BIT[8]={0xA0,0xA1,0xA2,0xA3}; 						 //位选，选择哪一位数码管显示
uchar DISP[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

//LCD 1602 
char* resultStr;
char* tempStr;
uint i;

//---------函数声明------------------------------------
void ConfigUART(uint baud);
void ConfigTimer0(uint ms);
void Serial_send(uchar send);
void Delay_1ms(uint z);
void ParseData(void);
uint FucCheckSum(uchar dd[]);


void main()
{
	uchar i;
	//P0 = 0xFF;  //P0口初始化
	lcd1602_initial();
	ConfigUART(9600);	//配置串口中断，设置波特率为 9600
	ConfigTimer0(1);	 	//T0 初始化
	EA = 1;			//开总中断

	//开机指令
	for(i=0;i<9;i++)
	{
		Serial_send(openComm[i]);
		//Delay_1ms(1);
	}
	
	while(1)
 	{
		if(count==0)
		{
			count = 10;	 
			TEST_LED = !TEST_LED;
			for(i=0;i<9;i++)
			{
				Serial_send(retrieveComm[i]);
				//Delay_1ms(2);
			}
			
			R_data=1;
			{
				if(complete_flag==1)
				{
					HR_crc = FucCheckSum(USART_RX_BUF);    //待校验值					
					check = ((uint)USART_RX_BUF[6]<<8) + USART_RX_BUF[7];   //校验值位
					
					if((HR_crc == check)) //判断接收的数据是否正确
					{
						if(USART_RX_BUF[1]==0x02)   //2次校验数据标志位，以确保正确性
						{
							PM10_data[0] = USART_RX_BUF[2];
							PM10_data[1] = USART_RX_BUF[3];
							PM25_data[0] = USART_RX_BUF[4];
							PM25_data[1] = USART_RX_BUF[5];
							ParseData(); //将检测数值由十六进制转成ASCII码
						}
					}
				}
			}
		}
 	}
}

void ConfigUART(uint baud)
{
	TMOD &= 0x0F;  //清零 T1 的控制位
	TMOD |= 0X20; //配置T1 为模式2
	
	PCON = 0X00;
	SCON = 0X50; //配置串口为模式1，0b0101 0000
	
	TH1 = 256 - (11059200/12/32) / baud;  //计算 T1 重载值 //BRUD 9600, 0b1111 1101
	TL1 = TH1;	 //初值等于重载值
	ET1 = 0;	   //禁止 T1 中断
	ES  = 1;	   //使能串口中断
	TR1 = 1;	   //启动 T1
}

//给LED显示用的刷新定时器中断初始化
void ConfigTimer0(uint ms)  //T0配置函数
{
	ulong tmp;
	
	TMOD &= 0xF0;   //清零 T0 的控制位
	TMOD |= 0x01;   //配置 T0 为模式 1
	
	tmp = 11059200 / 12;	  //定时器计数频率
	tmp = (tmp * ms) / 1000;  //计算所需的计数值
	tmp = 65536 - tmp;		//计算定时器重载值
	//tmp = tmp + 31;		  //修正中断响应延时造成的误差

	T0RL = (unsigned char)tmp;  //定时器重载值拆分为高低字节
	T0RH = (unsigned char)(tmp >> 8);  
	TH0 = T0RH;	 //加载 T0 重载值
	TL0 = T0RL;
	
	ET0 = 1;		//使能 T0 中断
	TR0 = 1;		//启动 T0
	EA = 1;			//打开总中断
	
	count=10;
}

void InterruptTimer0() interrupt 1 using 1
{
	TL0 = T0RH;   ////定时器重新加载重载值
    TH0 = T0RL;
	count--;
	
//	//在读取传感器检测数据时，同时显示到数码管
//	if(R_data==1)
//	{
//		P0 = 0xFF;  //P0口初始化
//		P2 = 0;
//		P0 = DIS_SEG7[DISP[PM_ASC[cnt_PM25]]];          
//		P2 = DIS_BIT[cnt_PM25];
//		cnt_PM25++;
//		if(cnt_PM25>=4)
//			cnt_PM25=0;
//	}
	
	//换1602显示结果
	if (R_data == 1)
	{
		char* strFormat = "PM2.5: ";
		strcat(strFormat, &ParsedData[0]);
		lcd1602_write_str(SET_DDRAM_ADDR, strlen(strFormat), strFormat);
		
		strFormat = "PM10: ";
		strcat(strFormat, &ParsedData[1]);
		lcd1602_write_str(SET_DDRAM_ADDR|LINE_2_OFFSET, strlen(strFormat), strFormat);
	}
	
	jj++;	
	if(jj>2) {
		USART_RX_STB=0;
		jj=0;
	}
}

void InterruptUART() interrupt 4 using 2
{
	if(RI) //接收到字节
	{
		RI = 0; //手动清零接收中断标志位
		USART_RX_BUF[USART_RX_STB] = SBUF;
		jj=0; //中断超时方法
		USART_RX_STB++;
		
		if(USART_RX_STB==9)
		{
			complete_flag=1;
		}
	}
}

void Serial_send(uchar send)
{	
	SBUF = send;
	while(!TI);
	TI=0;
}

//把接收到的数据1~6和8位相加
uint FucCheckSum(uchar dd[])
{
	uchar j;
	uint tempq=0;
	for(j=0;j<=5;j++)
	{
		tempq+=dd[j];
	}
	tempq=tempq+dd[8];
	return(tempq);
}

void ParseData(void)
{
	uint PM25,PM10;

	//字节5存放的校验位的高八位，字节6存放的低八位
	PM25 = ((uint)PM25_data[0] << 8) + (uint)PM25_data[1];
	//字节3存放的校验位的高八位，字节4存放的低八位
	PM10 = ((uint)PM10_data[0] << 8) + (uint)PM10_data[1];
	
	//空气质量差到这样就别再网上显示更大的值了
	if(PM25 > 999) PM25 = 999;
	if(PM10 > 1500) PM10 = 1500;
	
	ParsedData[0] = PM25;
	ParsedData[1] = PM10;
	
//	uint PM25,PM10;
//	uint sss;
// 
//	PM25 = ((uint)PM25_data[0]<<8)+PM25_data[1];
//	PM10 = ((uint)PM10_data[0]<<8)+PM10_data[1];
//	
//	//no exceed of upper limit
//	if(PM25>999)
//		PM25=999;
//	if(PM10>1500)
//		PM10=1500;

//	//PM2.5
// 	PM_ASC[0] = PM25/1000;
//	sss = PM25%1000;
//	PM_ASC[1] = sss/100;
//	sss = sss%100;
//	PM_ASC[2] = sss/10;
//	PM_ASC[3] = sss%10;	
//	
//	//PM10
//	PM_ASC[4] = PM10/1000;
//	sss = PM10%1000;
//	PM_ASC[5] = sss/100;
//	sss = sss%100;
//	PM_ASC[6] = sss/10;
//	PM_ASC[7] = sss%10;	
}

//delay for z*1ms		 延时函数
void Delay_1ms(uint z)
{
	uint i,j;
	
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}
