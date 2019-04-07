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
* �������ܡ��� ������ʾ�������ֵ0-9 ������ 9600			   			            			    
* ��ʹ��˵������ ʹ�ú������ʱ��Ӧ���������ͷ�嵽������ָ��λ�ã�ע�ⷽ��
**********************************************************************************/

/*Ԥ��������*/
#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include<intrins.h> //����_nop_()���������ͷ�ļ�
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

sbit IRIN=P3^2;         //���������������



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

//************************************************************
void delay1ms()
{
 unsigned int i;
 for(i=8024;i>0;i--);  //��ʱ124*8+10=1002us
}


 //*****���ڳ�ʼ������***********

//******************************
Initial_com()
{
 EA=1;        //�����ж�
 ES=1;        //�������ж�
 ET1=1;        //����ʱ��T1���ж�
 TMOD=0x20;   //��ʱ��T1���ڷ�ʽ2�жϲ���������
 PCON=0x00;   //SMOD=0
 SCON=0x50;   // ��ʽ1 �ɶ�ʱ������
 TH1=0xfd;    //����������Ϊ9600
 TL1=0xfd;
 TR1=1;       //����ʱ��T1���п���λ

}

/*******************************************************************/
void main()
{

	IRIN=1;                    //I/O�ڳ�ʼ��
	 Initial_com();	
	
	IE=0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	TCON=TCON|0x01;               //������ʽΪ���帺���ش���
	
	 while(1)
	 {
		
	
		 
		
	  }


} 
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
	unsigned char j,k,N=0;
	unsigned char q=0;
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

	q= (((IRCOM[6]&0x0f)<<4) + (IRCOM[5]&0x0f));
    

	switch(q)    //�жϰ�������ֵ
	{
		case 0x16:SBUF=0x00;break; //���ڷ���0
		case 0x03:SBUF=0x01;break;  //���ڷ���01
		case 0x18:SBUF=0x02;break;  //���ڷ���02
		case 0x55:SBUF=0x03;break;  //���ڷ���03
		case 0x08:SBUF=0x04;break;  //���ڷ���04
		case 0x13:SBUF=0x05;break;  //���ڷ���05
		case 0x51:SBUF=0x06;break;  //���ڷ���06
		case 0x42:SBUF=0x07;break;  //���ڷ���07
		case 0x52:SBUF=0x08;break;  //���ڷ���08
		case 0x41:SBUF=0x09;break;  //���ڷ���09
	
		default :break; 
	}

	 EX0 = 1; 
}



