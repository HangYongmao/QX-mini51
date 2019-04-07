/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)  http://qx-mcu.taobao.com/  ���ܵ꣩
* ��ʵ��ƽ̨���� QX-MINI51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio3	
* �������ܡ��� �����������ʾ��LEDָʾ��			   			            			    
* ��ʹ��˵������ 

STC89c52��HC-06�������ߣ�
	RXD    TXD
	TXD    RXD
	�������ӷ�ʽ����ձ��ļ��µ�ͼƬ
�ֻ�������������ͨ������
����Ѹ���Android�Ա�����ͨ�����
����ģ�鰲װ��������ձ��ļ����µ�ͼƬ01
����ģ�鹺���ַ��http://item.taobao.com/item.htm?id=27378740053

**********************************************************************************/

#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
uchar tmp;
uint c=0;
sbit led1=P1^0;		    //ָʾ��0
sbit led2=P1^1;			//ָʾ��1
sbit led3=P1^2;			//ָʾ��3
sbit led7=P1^7;			//ָʾ��3

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};

void delay1(uint z)//1ms
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);	
}
void display(uchar value)
{
    uchar shu1,value1;
	value1=value-48;
	//bai=value1/16;
	shu1=value1%16;


   P0=table[shu1];
   P2_0 = 0;
   delay1(1);
   P2_0 = 1;
}	
	  
void init();   //���ڳ�ʼ��
void send(unsigned char a);	//���ֽڷ��ͺ���
void ctrl(); //���մ�����

void main()
{
		init();
          while(1)
          {         
                  display(SBUF);
				  if(RI==1)                     // �Ƿ������ݵ���
                  {
                   RI = 0;
				  ctrl();
				  }       
          }
}

 void init()	//���ڳ�ʼ��
 {
 		  ES=0;								  //���ж�
		  SCON = 0x50;                        // REN=1�����н���״̬�����ڹ���ģʽ1,
											  //10λUART��1λ��ʼλ��8λ����λ��1λֹͣλ������żУ�飩�������ʿɱ�

		  TMOD = 0x20;                        // ��ʱ��1�����ڷ�ʽ2��8λ�Զ�����ģʽ, ���ڲ���������
          TH1=TL1=0xFD;                       // ������9600 �����β��Բ��þ���Ϊ11.0592��
          
          PCON &= 0x7f;                       // �����ʲ�����
          TR1 = 1;							  //��ʱ��1��ʼ����������������
											  //���ͱ�־λ��0
		  TI=0;								  //���ձ�־λ��0
		  RI=0;
		  
		  //EA=0;
	      ES=1;
 }
   
void send(unsigned char a)	//���ֽ����ݷ���
{							//ע�⣺����Ƭ��TXD��P3.1��������������������P3.1�˽��������衣���β�����Ҫ����������
	TI=0;	
	SBUF=a;
	while(TI==0);
	TI=0;

	if(c%2)					//����ָʾ�Ʊ�־��ÿ����һ�Σ��˵�������
	 led7=1;
	else
	 led7=0;
	c++;
} 

void ctrl()		            //���մ�����
{
  				  switch(tmp)
				  {
				  	case '1':
						led1=1;		       	    //�յ��ַ�1��ָʾ��0��
						send(tmp);
						break;					
					case '2':					 //�յ��ַ�2��ָʾ��1��
						 led2=1; 
						 send(tmp);
						 break;
					case '3':				    //�յ��ַ�3��ָʾ��0��1��
						 led1=0;
						 led2=0;
						 send(tmp);
						 break;
					case '4':					//�յ��ַ�4��ָʾ��0��1��
						  led1=1;
						  led2=1;
						  send(tmp);
						  break;
					case '5':					//�յ��ַ�5��ָʾ��3��
						led3=0;
						send(tmp);
						break;
					default:                   //��������ȫ��
						  led1=1;
						  led2=1;
						  led3=1;
						  send(tmp);
				  }
}