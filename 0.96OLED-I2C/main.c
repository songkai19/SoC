//////////////////////////////////////////////////////////////////////////////////	 
//Ƕ��ʽ������
//mcudev.taobao.com
//	�� �� ��	 : main.c

//	��������	 : OLED 4�ӿ���ʾ����(51ϵ��)
//							˵��: 
//							----------------------------------------------------------------
//							GND		��Դ��
//							VCC	��5V��3.3v��Դ
//							D0	 P1^0��SCL��
//							D1	 P1^1��SDA��
//							RES	��P12
//							DC	 ��P13
//							CS	 ��P14							 
//							----------------------------------------------------------------

//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

 int main(void)
 {	
	 u8 t;
	 OLED_Init();			//��ʼ��OLED	
	 OLED_Clear()		; 
		
	 t=' ';
	 OLED_ShowCHinese(0,0,0);//Ƕ
	 OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//ʽ
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
	while(1) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//Ƕ
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//ʽ
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowString(6,3,"0.96' OLED TEST",16);
	
		OLED_ShowString(0,6,"ASCII:",16);	
		OLED_ShowString(63,6,"CODE:",16);	
		OLED_ShowChar(48,6,t,16);//��ʾASCII�ַ�		 
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			delay_ms(8000);
		delay_ms(8000);

					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		OLED_DrawBMP(0,0,128,8,BMP1);	//ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ��ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		OLED_DrawBMP(0,0,128,8,BMP1);
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
	}		
	
}
	