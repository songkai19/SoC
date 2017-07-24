#include <reg52.h>
#include <string.h>
 
#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long

sbit LED = P1^7; //sending command data
sbit LED2 = P1^6; //reached predefined overflow times (100)
sbit LED3 = P1^4;

uint cnt = 0;
uint i,k;

uchar USART_RX_STB = 0;
uchar complete_flag = 0;
uchar USART_RX_BUF[9];
uchar T0RH = 0;
uchar T0RL = 0;
uint HR_crc = 0;
uint check = 0;
uchar R_data=0;

uchar code openComm[9]={0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB};
uchar code dataComm[9]={0xAA,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xBB};
uchar code LedChar[] = {   //用数组来表示数码管真值表
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
    0x80,0x90
};
uchar PM_ASC[] = {0xFF, 0xFF, 0xFF, 0xFF};
uchar LED_W[] = {0xF7, 0xFB, 0xFD, 0xFE};

void ConfigUART(uint baud);
void Serial_send(uchar send);
void ConfigTimer0(uint ms);
uchar *ParseData(void); 
void FormatData(uchar parsedData[]);
bit FucCheckSum(uchar buff[]);

void main(void)
{
	uchar *parsedData;
	ConfigUART(9600);
	ConfigTimer0(1);
	EA = 1;
	
	LED = 0;
	for(i=0;i<9;i++)
	{
		Serial_send(openComm[i]);
	}
	LED = 1;
	
	while (1)
	{
		if (cnt >= 50)
		{
			cnt = 0;
			LED2 = !LED2;
			
			LED = 0;
			for(i=0;i<9;i++)
			{
				Serial_send(dataComm[i]);
			}
			LED = 1;
			
			if (complete_flag == 1)
			{
				LED3 = 0;
				complete_flag = 0;
				
				parsedData = ParseData();
				if(FucCheckSum(parsedData) && (parsedData[1] == 0x02))
				{
					FormatData(parsedData);
					LED3 = 1;
				}
			}
		}

	}
}

void ConfigUART(uint baud)
{
	TMOD &= 0x0F;
	TMOD |= 0X20;
	
	PCON = 0X00;
	SCON = 0X50;
	
	TH1 = 256 - (11059200/12/32) / baud;
	TL1 = TH1;
	ET1 = 0;
	ES  = 1;
	TR1 = 1;
}

void InterruptUART() interrupt 4 using 2
{
	if(RI)
	{
		RI = 0;
		
		if (USART_RX_STB < 9)
		{
			USART_RX_BUF[USART_RX_STB] = SBUF;
			USART_RX_STB++;
		}
		else
		{
			USART_RX_STB = 0;
			complete_flag = 1;
		}
	}
}

void Serial_send(uchar send)
{	
	SBUF = send;
	while(!TI);
	TI=0;
}

void ConfigTimer0(uint ms)
{
	ulong tmp;
	
	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	tmp = 11059200 / 12;
	tmp = (tmp * ms) / 1000;
	tmp = 65536 - tmp;

	T0RL = (unsigned char)tmp;
	T0RH = (unsigned char)(tmp >> 8);
	TH0 = T0RH;
	TL0 = T0RL;
	
	ET0 = 1;
	TR0 = 1;
}

void InterruptTimer0() interrupt 1 using 1
{
	TL0 = T0RH;
    TH0 = T0RL;
	cnt++;
	P0 = 0xFF;
	
	switch(i)
	{
		case 0: P0 = PM_ASC[k]; P2 = LED_W[k]; k++; break;
		case 1: P0 = PM_ASC[k]; P2 = LED_W[k]; k++; break;
		case 2: P0 = PM_ASC[k]; P2 = LED_W[k]; k++; break;
		case 3: P0 = PM_ASC[k]; P2 = LED_W[k]; k = 0; break;
		default: break;
	}
}

bit FucCheckSum(uchar buff[])
{
	uchar j;
	uint tempq = 0;
	for(j = 0; j <= 5; j++)
	{
		tempq += buff[j];
	}
	tempq += buff[8];
	
	check = (uint)((buff[6]<<8) | buff[7]);
	
	return (tempq == check);
}

uchar *ParseData(void)
{
	uchar iLoop, jLoop;
	uchar tempChar[9];
	for (iLoop = 0; iLoop < 9; iLoop++)
	{
		for (jLoop = 0; jLoop < 8; jLoop++)
		{
			tempChar[iLoop] <<= 1;
			if(USART_RX_BUF[iLoop] & 0x01)
				tempChar[iLoop] |= 1;
			
			USART_RX_BUF[iLoop] >>= 1;
		}
	}
	
	return tempChar;
}

void FormatData(uchar parsedData[])
{	
	uint PM25,PM10;
 
	PM25 = ((uint)parsedData[4]<<8) + parsedData[5];
	PM10 = ((uint)parsedData[2]<<8) + parsedData[3];
	
	if(PM25>999) PM25=999;
	if(PM10>1500) PM10=1500;

	PM_ASC[0] = LedChar[PM25%10];
	PM_ASC[1] = LedChar[PM25/10%10];
	PM_ASC[2] = LedChar[PM25/100%10];
	PM_ASC[3] = LedChar[PM25/1000%10];
}