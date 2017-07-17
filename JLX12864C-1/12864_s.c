//youtube ��Ƶ39��
//https://www.youtube.com/watch?v=K18Hl0wdpao
#include <12864.h>

uchar table[]="��������20170710";
	
//��ʱ��������12MHz�ľ���Ƶ����
//��Լ50us����ʱ
void delay_50us(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=19;j>0;j--);
}

//��ʱ��������12MHz�ľ���Ƶ����
//��Լ50ms����ʱ
void delay_50ms(uint t)
{
	uchar j;
	for (;t>0;t--)
		for(j=6245;j>0;j--);
}

void write_12864(uchar dat, uchar rsFlag)
{
	int i;
	//����Ƭѡλ��ʹʱ���źű����Ч
	cs=1;
	
	//ˢ��ǰ��λ1 - ͬ��λ��
	//�˕r��ݔӋ���������ÁK�Ҵ��Ђ�ݔ����ͬ��
	for(i=0;i<5;i++)
	{
		sid=1;
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
	}
	//�ٸ��S�ăɂ�λԪ�ִ��քeָ����ݔ����λԪ��RW�����������x��λԪ��RS����
	//����ڰ˵�λԪ�t�騔0����
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
	
	sid=0; //βλ0ռλ;
	sclk=0;
	delay_50us(1);
	sclk=1;
	delay_50us(1);
	
	//�ڽ��յ�ͬ��λԪ��RW��RS�Y�ϵĆ�ʼλԪ�M�ᣬÿһ����λԪ��ָ����֞�ɂ�λԪ�M���յ���
	//�^��4λԪ��DB7~DB4����ָ���Y�ό��������ڵ�һ��λԪ�M��LSB���֣�
	//���^��4λԪ��DB3~DB0����ָ���Y�τt�������ڵڶ���λԪ�M��LSB���֣�������P������λԪ�t����0��
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
		//��λ���1/0
		dat<<=1;
	}
	
	//����λ����ռλ
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
		//��λ���1/0
		dat<<=1;
	}
	
	//����λ����ռλ
	for(i=0;i<4;i++)
	{
		sid=0;		
		sclk=0;
		delay_50us(1);
		sclk=1;
		delay_50us(1);
	}
	
	//����Ƭѡ������1�ֽ����ݴ���
	cs=0;
}

void init_12864lcd(void)
{
	//�����ϵ�
	reset=0;
	delay_50us(60);
	reset=1;
	delay_50ms(40);
	write_12864(0x30, 0);
	delay_50us(30);
	
	//������Ļ����ʼ������
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