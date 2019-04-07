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
* �������ܡ��� �����ģ������ʱ����ʾ				   			            			    
* ��ʹ��˵������ ����ñ�̽������J6����
**********************************************************************************/

#include <reg52.h>
#define uchar unsigned char 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

static unsigned char second,minute,hour;
unsigned int tcount;
unsigned char m;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};
void delay(uchar i)
{
  uchar j,k; 
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display()
{
  
   P0=table[minute/10];
   P2_0 = 0;
   delay(5);
   P2_0 = 1;

   P0=(table[minute%10]& 0x7f);
   P2_1 = 0;
   delay(5);
   P2_1 = 1;

   P0=table[second/10];
   P2_2 = 0;
   delay(5);
   P2_2 = 1;

    P0=table[second%10];
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}

/*��ʾ������*/
void main(void)
{
    unsigned int  a;
	TMOD=0x02;
    TH0=0x06;
    TL0=0x06;
    TR0=1;
    ET0=1;
    EA=1;
   
 while(1)
 {
 
   for(a=100;a>0;a--)
   {
     display();
   }
 
 }
}


/*
********************************************************************************
** �������� �� t0(void) interrupt 1 using 0
** �������� �� ��ʱ���ж�
********************************************************************************
*/

 void t0(void) interrupt 1 using 0 
  {	tcount++;

    if(tcount==4000)
	  {tcount=0;
	    second++;

		 if(second==60)
		  {second=0;
		   minute++;

		    if(minute==60)
			 {minute=0;
			  hour++;

			  if(hour==24)
			   {hour=0;
			   }
			 }
		  }
	  }
	}
