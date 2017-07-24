#include <reg52.h>
#include <string.h>
#include <1602_2.h>
#include <stdutils.h>

uint8_t T0RH = 0;  //T0重载值的高字节
uint8_t T0RL = 0;  //T0重载值的低字节
uint8_t RxdBytesBuffer[9];
uint8_t ParsedData[2];
uint8_t bufferCounter = 0;
bit receivedOneFrame = 0;
uint8_t t0Counter = 1000;   //设置一个中断触发计数器，每一个计数周期我们从传感器那一次数据
// 注意：t0中断每一次是2ms，这里我们1000次计数就是2秒钟一个检测周期
uint8_t check = 0;

uint8_t code openCmd[9] = {0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB};
uint8_t code dataCmd[9] = {0xAA,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xBB};
uint8_t code closeCmd[9] = {0xAA,0x03,0x00,0x00,0x00,0x00,0x01,0x68,0xBB};
uint8_t code dummyCmd[9] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void ConfigTimer0(uint16_t ms);
void ConfigUART(uint16_t baud);
void LcdScan();
void uartSendCmd(uint16_t cmd);
bit checkSum();
void parseData(void);
void Serial_send(uint8_t send);

void main(void)
{
	uint8_t i;
	lcd1602_initial();
	
	ConfigUART(9600);  //配置波特率为 9600	
	ConfigTimer0(1);   //配置 T0 定时 2ms
	EA = 1;			//开总中断
	
	//发送开机命令
	//uartSendCmd(1);
	for(i=0;i<9;i++)
	{
		Serial_send(openCmd[i]);
	}

//	while(1)
//	{
//		//一个t0计数周期2s结束，我们开始采集数据
//		if (t0Counter == 0) {
//			//重置计数器，为下次检测预留2s时间
//			t0Counter = T0COUNTER2SEC;
//			
//			if (bufferCounter == 0)
//			{
//				if (receivedOneFrame)
//				{
//					//校验接收数据的有效性
//					if (checkSum() && RxdBytesBuffer[1] == 0x02)
//					{
//						//把数据解析到可以用作LCD显示的格式
//						parseData();
//					}
//				}
//				else 
//				{
//					//发送接收数据命令
//					uartSendCmd(2);
//				}
//			}
//		}
//	}

}

//给数据传输用的定时器中断初始化
void ConfigUART(uint16_t baud)  //串口配置函数， baud 为波特率
{
	//硬件UART由SCON寄存器，发送和接收电路组成
	//这里设置SCON为0b1010 0000，意在选择串口模式1
	SCON = 0x50;   //配置串口为模式 1
	PCON = 0X00;
	
	TMOD &= 0x0F;  //清零 T1 的控制位，T1控制位在TMOD高八位
	//选择时钟模式2：模式 2 中，不再是 TH0 代表高 8 位，TL0 代表低 8 位了，而只有 TL0 在进行计数了。
	//当 TL0 溢出后，不仅仅会让 TF0 变 1 ，而且还会将 TH0 中的内容重新自动装到 TL0 中。
	//这样有一个好处，我们可以把我们想要的定时器初值提前存在 TH0 中，当 TL0 溢出后， 
	//TH0 自动把初值就重新送入 TL0 了，全自动的，不需要程序上再给 TL0 重新赋值了
	TMOD |= 0x20;  //配置 T1 为模式 2
	
	//一个系统时钟有12个时钟周期，这里是求每个时钟周期内，单位采样时间内的晶振值
	//告诉计时器TH1和TH1，我们想多少个晶振内出发一次T1中断，
	//所以要计算出单次采样内的晶振次数告诉系统，系统计数该单此采样的晶振次数到了就发出中断请求
	//也就是说系统不通过时间来判断何时发送中断，而是通过单位采样内晶振数来计时
	//这里如果设置9600波特率为入口参数，那就是1/9600秒的间隔会出发一次T1中断
	//注意：/2是因为UART是全双工模式，所以 收/发 总共32次采样
	TH1 = 256 - 11059200UL/(long)(32*12*baud);  //计算 T1 重载值
	TL1 = TH1;	 //初值等于重载值
	ET1 = 0;	   //禁止 T1 中断
	ES  = 1;	   //使能串口中断
	TR1 = 1;	   //启动 T1
}

//给LED显示用的刷新定时器中断初始化
void ConfigTimer0(uint16_t ms)  //T0配置函数
{
	uint32_t tmp;

	TMOD &= 0xF0;   //清零 T0 的控制位，T0控制位在TMOD低八位
	//注意：T0中断 - 模式1：同时使用计时器的高位和低位同时来计时，所以他的溢出值是4个字节=10进制65536
	TMOD |= 0x01;   //配置 T0 为模式 1
	
	//一个系统时钟有12个时钟周期，这里是求每个时钟周期内，单位毫秒内的晶振值
	//告诉计时器TH0和TH0，我们想多少个晶振内出发一次T0中断，所以要计算出2ms内的晶振次数告诉系统，系统计数2ms晶振次数到了就发出中断请求
	//也就是说系统不通过时间来判断何时发送中断，而是通过单位时间内晶振数来计时
	//这里如果设置2ms入口参数，那就是2ms的间隔会出发一次T0中断
	tmp = 11059200UL / 12;	  //定时器计数频率
	tmp = (tmp * ms) / 1000;  //计算所需的计数值
	tmp = 65536 - tmp;		//计算定时器重载值
	//tmp = tmp + 31;		   //修正中断响应延时造成的误差

	T0RL = (uint8_t)tmp;
	T0RH = (uint8_t)(tmp >> 8);  //定时器重载值拆分为高低字节
	
	TH0 = T0RH;	 //加载 T0 重载值
	TL0 = T0RL;
	ET0 = 1;		//使能 T0 中断
	TR0 = 1;		//启动 T0
}

void InterruptUART() interrupt 4
{
	if (RI)  //接收到字节
	{
		RI = 0;  //手动清零接收中断标志位
		
		//接收1帧数据9个字节，然后清零重新接收
		if (bufferCounter < 9)
		{
			RxdBytesBuffer[bufferCounter] = SBUF;  //接收到的数据保存到接收字节变量中
			bufferCounter++;
		}
		else
		{
			bufferCounter = 0;
			receivedOneFrame = 1;
		}
	}
}

void InterruptTimer0() interrupt 1  //T0中断服务函数
{
	//计数器到1000，也就是2s后，在main函数启动数据获取逻辑
	t0Counter--;
	
	TH0 = T0RH;  //定时器重新加载重载值
	TL0 = T0RL;
	LcdScan();   //LCD扫描显示
}

//输出字符到LED
void LcdScan()  //LED显示扫描函数
{
	char* strFormat = "PM2.5: ";
	strcat(strFormat, &ParsedData[0]);
	lcd1602_write_str(SET_DDRAM_ADDR, strlen(strFormat), strFormat);
	
	strFormat = "PM10: ";
	strcat(strFormat, &ParsedData[1]);
	lcd1602_write_str(SET_DDRAM_ADDR|LINE_2_OFFSET, strlen(strFormat), strFormat);
}

void uartSendCmd(uint16_t cmd)
{
	uint8_t i;
	uint8_t *cmdStr;
	
	switch (cmd) {
		case 1: 
			cmdStr = openCmd;
			break;
		case 2: 
			cmdStr = dataCmd;
			break;
		case 3:
			cmdStr = closeCmd;
			break;
		default:
			cmdStr = dummyCmd;
	}
		
	for (i = 0; i < 9; i++)
	{
		SBUF = *(cmdStr + i);
		
		//等待传送数据结束，手动置0传送标志位，让机器下次知道在传送结束后置1
		while (!TI);
		TI = 0;
	}
}

bit checkSum()
{
	uint8_t j;
	uint8_t tempq = 0;
	
	//帧头		帧命令		帧内容		校验和		帧尾
	//0xAA		6种命令		字节3~6		字节7~8		0xBB
	for(j= 0; j <= 5; j++)
	{
		tempq += RxdBytesBuffer[j];
	}
	tempq += RxdBytesBuffer[8];
	
	//字节7存放的校验位的高八位，字节8存放的低八位
	check = ((uint16_t)RxdBytesBuffer[6] << 8) + (uint16_t)RxdBytesBuffer[7];
	
	return (tempq == check);
}

void parseData()
{
	uint16_t PM25,PM10;
//	uint8_t tempv;

	//字节5存放的校验位的高八位，字节6存放的低八位
	PM25 = ((uint16_t)RxdBytesBuffer[4] << 8) + (uint16_t)RxdBytesBuffer[5];
	//字节3存放的校验位的高八位，字节4存放的低八位
	PM10 = ((uint16_t)RxdBytesBuffer[2] << 8) + (uint16_t)RxdBytesBuffer[3];
	
	//空气质量差到这样就别再网上显示更大的值了
	if(PM25 > 999) PM25 = 999;
	if(PM10 > 1500) PM10 = 1500;
	
	ParsedData[0] = PM25;
	ParsedData[1] = PM10;

	//下面是为数码管做的数据格式转换，用lcd就不用了，直接输出字符串ASC码
//	531
//	/1000 = 0 	a[0]
//	%1000 = 531 temp

//	531
//	/100 = 5   a[1]
//	%100 = 31  temp

//	31
//	/10 = 3   a[2]
//	%10 = 1   a[3]

//	a[4] = {0, 5, 3, 1} 取千百十个 四位

//	//分解PM2.5的值
// 	ParsedPm25Data[0] = PM25 / 1000;
//	tempv = PM25 % 1000;
//	ParsedPm25Data[1] = tempv / 100;
//	tempv = tempv % 100;
//	ParsedPm25Data[2] = tempv / 10;
//	ParsedPm25Data[3] = tempv % 10;	
//	
//	//分解PM10的值
//	ParsedPm10Data[0] = PM10 / 1000;
//	tempv = PM10 % 1000;
//	ParsedPm25Data[1] = tempv / 100;
//	tempv = tempv % 100;
//	ParsedPm25Data[2] = tempv / 10;
//	ParsedPm25Data[3] = tempv % 10;	
}

void Serial_send(uint8_t send)
{	
	SBUF = send;
	while(TI==0);
	TI=0;
}