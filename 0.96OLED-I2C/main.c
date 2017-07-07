//////////////////////////////////////////////////////////////////////////////////	 
//Ƕ��ʽ������
//	�� �� ��	 : main.c

//	��������	 : OLED 4�ӿ���ʾ����(51ϵ��)
//				˵��: 
//				----------------------------------------------------------------
//				GND	 ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   P1^0��SCL��
//              D1   P1^1��SDA��
//              RES  ��P12���͵�ƽ��λ�ܽţ����а汾��������3���ܽţ�
//              DC   ��P13������/������ƹܽ�
//              CS   ��P14, Ƭѡ�źŹܽ�						 
//				----------------------------------------------------------------

//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

 int main(void)
 {	
	 u8 t;
	 oled_init();			//��ʼ��OLED	
	 oled_clear()		; 
		
	 t=' ';
	 oled_showcnchar(0,0,0);//Ƕ
	 oled_showcnchar(18,0,1);//��
		oled_showcnchar(36,0,2);//ʽ
		oled_showcnchar(54,0,3);//��
		oled_showcnchar(72,0,4);//��
		oled_showcnchar(90,0,5);//��
	while(1) 
	{		
		oled_clear();
		oled_showcnchar(0,0,0);//Ƕ
		oled_showcnchar(18,0,1);//��
		oled_showcnchar(36,0,2);//ʽ
		oled_showcnchar(54,0,3);//��
		oled_showcnchar(72,0,4);//��
		oled_showcnchar(90,0,5);//��
		oled_showstr(6,3,"0.96' OLED TEST",16);
	
		oled_showstr(0,6,"ASCII:",16);	
		oled_showstr(63,6,"CODE:",16);	
		oled_showchar(48,6,t,16);//��ʾASCII�ַ�		 
		t++;
		if(t>'~')t=' ';
		oled_showno(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			delay_ms(8000);
		delay_ms(8000);

					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		oled_drawbmp(0,0,128,8,BMP1);	//ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		oled_drawbmp(0,0,128,8,BMP1);
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
	}		
	
}
	