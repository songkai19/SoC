/*   头文件    */
#include <reg52.h>
#include <1602_2.h>
 
/*   宏定义    */
#define uint  unsigned int
#define uchar unsigned char

#define FOSC 11059200L
#define T1MS (65536-FOSC/12/500)   //2ms timer calculation method in 12T mode
/* define SFR */
sbit TEST_LED = P1^0;               //work LED, flash once per second
/*     变量定义      */
uint count=100;  //周期计时
uchar cnt_PM25=0;	//数码管PM2.5循环计数
uchar R_data=0; //读数据标志
/*------激光传感器-------------*/
uchar USART_RX_STB=0;      
uchar complete_flag=0;
uchar data_processed = 0;
uchar USART_RX_BUF[16];  //接收缓冲,最大16个字节.
uchar jj=0;              //判断串口一帧数据是否接收完成的计数变量
uchar PM25_data[2] = {0};
uchar PM10_data[2] = {0};
uchar PM_ASC[8];				 //数据分解后的存储数组
uint HR_crc = 0;
uint check = 0;
//-------开机、读数据、关机指令----------------------------------
uchar code open[9]={0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB};
uchar code shuju[9]={0xAA,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xBB};
uchar code close[9]={0xAA,0x03,0x00,0x00,0x00,0x00,0x01,0x68,0xBB};
//------------数码管-------------------------------------
//uchar code DIS_SEG7[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};	 		 //共阳极数码管编码0~9
uchar code DIS_SEG7[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};			 //共阴极数码管编码0~9
uchar code DIS_BIT[8]={0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87}; 									 //位选，选择哪一位数码管显示
uchar DISP[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//---------函数声明------------------------------------
void Serial_Init(void);
void Timer0_Init(void);
void Serial_send(uchar send);
void Delay_1ms(uint z);
void shuju_jiexi(void);
uint FucCheckSum(uchar dd[]);
/****************************************************/
//main function
//
/****************************************************/
void main()
{
 	uchar i;//,ttt;     
  lcd1602_initial();
	Serial_Init();	 //串口初始化
	Timer0_Init();	 //T0 初始化
//---------开机指令--------------------
	 for(i=0;i<9;i++)
	 {
	   	Serial_send(open[i]);
			Delay_1ms(1);
	 }	
//-------------------------------------------
 	while(1)
 	{         				
     if(count==0)
		 {
		    count = 1000;	 
				TEST_LED = !TEST_LED;
			/*---------------------------
			//	  发送读数据指令				 */
					 for(i=0;i<9;i++)
					 {
					   	Serial_send(shuju[i]);
							Delay_1ms(2);
					 } 	
					 R_data=1;			 
		 /*------------------------------------------------
		 //   在接收一帧数据后，首先检验接收数据是否正确
		 //   若正确，将数据分解，然后显示	*/
				{
				  if(complete_flag==1) 
		       {
		         HR_crc = FucCheckSum(USART_RX_BUF);//校验
						 check = ((uint)USART_RX_BUF[6]<<8)+USART_RX_BUF[7];
						 if((HR_crc == check)) //判断接收的数据是否正确
						 {
								if(USART_RX_BUF[1]==0x02)
								{
									PM10_data[0] = USART_RX_BUF[2];
									PM10_data[1] = USART_RX_BUF[3];
									PM25_data[0] = USART_RX_BUF[4];
									PM25_data[1] = USART_RX_BUF[5];
									
									shuju_jiexi(); //将检测数值由十六进制转成ASCII码
								}
						  }
						}	     
				}
		 //----------------------------------------
    }
 	} 
}
/****************************************************/
//serial Timer0 initial
/****************************************************/
void Serial_Init(void)
{
	 TMOD = 0X20; //T1  work in mode 2
	 PCON = 0X00;
	 SCON = 0X50;
	 TH1  = 0XFD; //BRUD 9600
	 TL1  = 0XFD;
	 TR1  = 1;  //open timer 1
	 ES   = 1;  //open the serial
}
void Timer0_Init(void)
{
    TMOD |= 0x01;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch
		count=1000;
}
/****************************************************/
//serial interrupt function		 串口中断函数
/****************************************************/
void Serial() interrupt 4 using 2
{
	 if(RI)
	 {
		 RI = 0;
     USART_RX_BUF[USART_RX_STB] = SBUF;		
	   jj=0;//中断超时方法
		 USART_RX_STB++;
	   if(USART_RX_STB==9)
     {
        complete_flag=1;
     }
	 }
}
/****************************************************
     Timer0 interrupt routine 	 T0中断函数
*****************************************************/
void tm0_isr() interrupt 1 using 1
{
	  TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
 //-----------------------
		count--;
 //-------------------------
	//在读取传感器检测数据时，同时显示到数码管
		if(R_data==1)
		{
//			P2 = 0;            
//			P0 = DIS_SEG7[DISP[PM_ASC[cnt_PM25]]];          
//			P2 = DIS_BIT[cnt_PM25];
//			cnt_PM25++;
//			if(cnt_PM25>=8)
//			cnt_PM25=0;
			
			lcd1602_write_char(cnt_PM25, 0, PM_ASC[cnt_PM25]);
			
			cnt_PM25++;
			if(cnt_PM25>=8)
				cnt_PM25=0;
		}
	//-----------UART读取PM2.5---------------------------   
		jj++;	
		if(jj>2) {USART_RX_STB=0;jj=0;}
	//-------------------------------------- 
}
/****************************************************/
//mcu send data to serial		 串口发送函数
/****************************************************/
void Serial_send(uchar send)
{	
  SBUF = send;
  while(TI==0);
  TI=0;
}
/****************************************************/
//          校验 
/****************************************************/
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
/****************************************************/
//      数据解析
/****************************************************/
//=======数据解析===========
void shuju_jiexi(void)
{
 uint PM25,PM10;
 uint sss;
 
 PM25 = ((uint)PM25_data[0]<<8)+PM25_data[1];
 PM10 = ((uint)PM10_data[0]<<8)+PM10_data[1];
//------------------------
	if(PM25>999)
		PM25=999;
	if(PM10>1500)
		PM10=1500;

 //===========分解PM2.5的值=========================
 	PM_ASC[0] = PM25/1000;
	sss = PM25%1000;
	PM_ASC[1] = sss/100;
	sss = sss%100;
	PM_ASC[2] = sss/10;
	PM_ASC[3] = sss%10;	
//
 //============分解PM10的值=========================		
	PM_ASC[4] = PM10/1000;
	sss = PM10%1000;
	PM_ASC[5] = sss/100;
	sss = sss%100;
	PM_ASC[6] = sss/10;
	PM_ASC[7] = sss%10;	
}
/****************************************************/
//delay for z*1ms		 延时函数
/****************************************************/
void Delay_1ms(uint z)
{
 uint i,j;
 for(i=z;i>0;i--)
  for(j=110;j>0;j--);
}
