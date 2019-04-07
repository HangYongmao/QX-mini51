/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)  http://qx-mcu.taobao.com/  ���ܵ꣩
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52RC
* �����뻷������ Keil ��Visio4	
* �������ܡ��� Һ����ʾң�ظ������ļ���ֵ   �硰IR-CODE:8DH�����ü��ļ���Ϊ0x8D			   			            			    
* ��ʹ��˵������ ʹ��Һ��ǰע��ȡ�������J6��ñ
**********************************************************************************/


/*Ԥ��������*/
#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include<intrins.h> //����_nop_()���������ͷ�ļ�
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

sbit IRIN=P3^2;         //���������������
sbit LCD_RS = P0^7;             
sbit LCD_RW = P0^6;
sbit LCD_EN = P0^5;


uchar code  cdis1[ ] = {" Red Control "};
uchar code  cdis2[ ] = {"  IR-CODE: --H  "};
uchar IRCOM[7];
/***************��ʱ����*****************************/
void delay(unsigned char x)    //x*0.14MS
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++) {}
	}
}

void delay1(int ms)
{
	unsigned char y;
	while(ms--)
	{
		for(y = 0; y<250; y++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}
/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/ 

bit lcd_busy()
{                          
	bit result;	
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	delayNOP();
	result = (bit)(P2&0x80);
	LCD_EN = 0;
	return(result); 
}

/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/

void lcd_wcmd(uchar cmd)

{                          
	while(lcd_busy());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	_nop_();
	_nop_(); 
	P2 = cmd;
	delayNOP();
	LCD_EN = 1;
	delayNOP();
	LCD_EN = 0;  
}

/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/

void lcd_wdat(uchar dat)
{                          
	while(lcd_busy());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	P2 = dat;
	delayNOP();
	LCD_EN = 1;
	delayNOP();
	LCD_EN = 0; 
}

/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/

void lcd_init()
{ 
	delay1(15);                   
	lcd_wcmd(0x38);      //16*2��ʾ��5*7����8λ����
	delay1(5);
	lcd_wcmd(0x38);         
	delay1(5);
	lcd_wcmd(0x38);         
	delay1(5);
	
	lcd_wcmd(0x0c);      //��ʾ�����ع��
	delay1(5);
	lcd_wcmd(0x06);      //�ƶ����
	delay1(5);
	lcd_wcmd(0x01);      //���LCD����ʾ����
	delay1(5);
}

/*******************************************************************/
/*                                                                 */
/*  �趨��ʾλ��                                                   */
/*                                                                 */
/*******************************************************************/

void lcd_pos(uchar pos)
{                          
	lcd_wcmd(pos | 0x80);  //����ָ��=80+��ַ����
}

/*******************************************************************/
void main()
{
	uchar m;	
	IRIN=1;                    //I/O�ڳ�ʼ��
	delay1(10);                 //��ʱ
	lcd_init();                //��ʼ��LCD
	lcd_pos(0);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�	
	m=0;
	while(cdis1[m] != '\0')
	{                         //��ʾ�ַ�
		lcd_wdat(cdis1[m]);
		m++;
	}
	
	lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ��е�1���ַ�
	m=0;
	while(cdis2[m] != '\0')
	{
		lcd_wdat(cdis2[m]);      //��ʾ�ַ�
		m++;
	}
		
	IE=0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	TCON=0x01;               //������ʽΪ���帺���ش���
	
	while(1);

} 
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
	unsigned char j,k,N=0;
	EX0 = 0;   
	delay(15);
	if (IRIN==1) 
	{ 
		EX0 =1;
		return;
	} 
	           //ȷ��IR�źų���
	while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
		delay(1);
	
	for (j=0;j<4;j++)         //�ռ���������
	{ 
		for (k=0;k<8;k++)        //ÿ��������8λ
		{
			while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
			{delay(1);}
			while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
				delay(1);
			while (IRIN)           //����IR�ߵ�ƽʱ��
			{
				delay(1);
				N++;           
				if (N>=30)
				{ 
					EX0=1;
					return;
				}                  //0.14ms���������Զ��뿪��
			}                        //�ߵ�ƽ�������                
			IRCOM[j]=IRCOM[j] >> 1;     //�������λ����0��
			if(N>=8)
			{
				IRCOM[j] = IRCOM[j] | 0x80;		  //�������λ����1��
			}  
			N=0;
		}
	}
	
	if(IRCOM[2]!=~IRCOM[3])
	{ 
		EX0=1;
		return; 
	}
	
	IRCOM[5]=IRCOM[2] & 0x0F;     //ȡ����ĵ���λ
	IRCOM[6]=IRCOM[2] >> 4;       //����4�Σ�����λ��Ϊ����λ
	
	if(IRCOM[5]>9)
	{ 
		IRCOM[5]=IRCOM[5]+0x37;
	}
	else
	IRCOM[5]=IRCOM[5]+0x30;
	
	if(IRCOM[6]>9)
	{ 
		IRCOM[6]=IRCOM[6]+0x37;
	}
	else
	IRCOM[6]=IRCOM[6]+0x30;
	
	lcd_pos(0x4b);             
	lcd_wdat(IRCOM[6]);        //��һλ����ʾ 
	lcd_pos(0x4c);             
	lcd_wdat(IRCOM[5]);        //�ڶ�λ����ʾ	
	EX0 = 1; 
}



