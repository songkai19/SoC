#include <string.h>
#include <1602_2.h>

uint8 code tempSymbol[8]={ 0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00 };
uint8 code piSymbol[8]={ 0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,0x00 };

uint8 code strMCU[] = "abcdefghijklmn";
uint8 code strTest[] = "OPQRSTUVWXYZ";
char blog[] = "www.example.com";
uint8 i;

void main()
{
	lcd1602_initial();
	
	// ��CGRAM��ַд���Զ����ַ�
	lcd1602_write_str(SET_CGRAM_ADDR, 8, tempSymbol);
	lcd1602_write_str(SET_CGRAM_ADDR|0x08, 8, piSymbol);
	
	// ��DDRAMд������ַ���
	lcd1602_write_str(SET_DDRAM_ADDR, strlen(strMCU), strMCU);
	lcd1602_write_str(SET_DDRAM_ADDR|LINE_2_OFFSET, strlen(strTest), strTest);
	
	for (i = 0; i < 50; i++) lcd1602_delay(10000);
	lcd1602_write_com(CLEAR, 1);
	lcd1602_delay(500);
	
	// CGRAM�ĵ�ַƫ����Ϊ�㣬��������0λ�������涨��д����¶ȷ��ţ�1λ����pi����
	for (i = 0; i < 16; i++) lcd1602_write_dat(0);
	lcd1602_write_com(SET_DDRAM_ADDR|LINE_2_OFFSET);
	for (i = 0; i < 16; i++) lcd1602_write_dat(1);
	
	for (i = 0; i < 50; i++) lcd1602_delay(10000);
	lcd1602_write_com(CLEAR, 1);
	lcd1602_delay(500);
	
	// ����д��DDRAM��ʾ��ַ֮��
	lcd1602_write_str(SET_DDRAM_ADDR|0x10, strlen(blog), blog);
	while(1)
	{
		 // ������ʾ��������
		lcd1602_write_com(0x18, 1);
			
		for (i = 0; i < 20; i++) lcd1602_delay(2000);		
	}
		
}