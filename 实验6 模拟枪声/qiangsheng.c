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
* �������ܡ��� ����������-ģ��ǹ��			   			            			    
* ��ʹ��˵������ �̽ӷ�����J1��ñ������S1����
**********************************************************************************/

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

sbit  BEEP= P3^6;
sbit  K1  = P3^5;
sbit  K2  = P3^4;
sbit  K3  = P3^3;
sbit  K4  = P3^2;

uchar  H_count,L_count;

/*********************************************************/
void  main()
{	
 	P0=0;
	BEEP=1;
    TMOD=0x01; 
    TH0=0xff; 
    TL0=0xa0;
    H_count=0xff;
    L_count=0xa0;      
    EA=1;
    ET0=1; 
    
    while(1)
    {
     if(K1 == 0)  TR0=1;
	 if(K2 == 0)  TR0=1;
	 if(K3 == 0)  TR0=1;
	 if(K4 == 0)  TR0=1;
    }
}

/*********************************************************
  Time0�жϺ���
**********************************************************/
void Time0(void) interrupt 1 using 0
{
	BEEP=~BEEP;
	if( L_count!=0x00 )
	{
	   L_count--;
	   TH0=H_count; 
       TL0=L_count;
       return;
     }else  H_count--;
		
	if( H_count!=0xfc )
	{
	   L_count--;
	   TH0=H_count; 
       TL0=L_count;
       return;
	}
	else
     {
        H_count=0xff;
        L_count=0xa0;
        TH0=H_count;
        TL0=L_count;
        BEEP=1;
        TR0=0;
     }   
}
   
/*********************************************************/

