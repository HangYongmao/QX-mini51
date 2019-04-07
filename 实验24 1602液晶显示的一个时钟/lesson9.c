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
* �������ܡ��� LCD1602Һ��ʱ����ʾ���ɵ���			   			            			    
* ��ʹ��˵������ ����S4ѡ����Ҫ������ʱ�������S3ֵ���ӣ�����S2ֵ���� 
                 1.ʹ��1602Һ��ǰ�Ȱε������J6��ñ���������Һ�����ܹ���
                 2.��1602Һ���ӵ�L2��ĸ�ϣ�Һ�����򿪷����ⲿ
				 3.��������д��ɺ�1602Һ������ʾ������ڿ��������·��ĵ�λ��RV1	
**********************************************************************************/

#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int



sbit rs=P0^7;
sbit lcden=P0^5;
sbit rw=P0^6;

sbit s1=P3^2;		   // ��������K4 
sbit s2=P3^3;		   // ��������K3 
sbit s3=P3^4;		   // ��������K2 
	  
uchar count,s1num;
char miao,shi,fen;
uchar code table[]="   2014-4-01";
uchar code table1[]="    00:00:00";
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void write_com(uchar com)
{
	rs=0;
	lcden=0;
	P2=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void write_date(uchar date)
{
	rs=1;
	lcden=0;
	P2=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void init()
{
	uchar num;
	rw=0;
	lcden=0;
//	fen=59;
//	miao=53;
//	shi=23;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
	for(num=0;num<15;num++)
		{
			write_date(table[num]);
			delay(5);
		}
	write_com(0x80+0x40);
	for(num=0;num<12;num++)
		{
			write_date(table1[num]);
			delay(5);
		}
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
}

void write_sfm(uchar add,uchar date)
{
	uchar shi,ge;
	shi=date/10;
	ge=date%10;
	write_com(0x80+0x40+add);
	write_date(0x30+shi);
	write_date(0x30+ge);
}

void keyscan()
{

	if(s1==0)					   
	{
		delay(5);
		if(s1==0)
		{	s1num++;
			while(!s1);
			if(s1num==1)
			{
				TR0=0;
				write_com(0x80+0x40+10);
				write_com(0x0f);
			}
	}
			if(s1num==2)
			{
				write_com(0x80+0x40+7);
			}
			if(s1num==3)
			{
				write_com(0x80+0x40+4);
			}
			if(s1num==4)
			{
				s1num=0;
				write_com(0x0c);
				TR0=1;
			}

				
		}									    
		if(s1num!=0)
		{
			if(s2==0)
			{
				delay(5);
				if(s2==0)
				{
					while(!s2);
					if(s1num==1)
					{
						miao++;
						if(miao==60)
							miao=0;
						write_sfm(10,miao);
						write_com(0x80+0x40+10);
						
						
					}
					if(s1num==2)
					{
						fen++;
						if(fen==60)
							fen=0;
						write_sfm(7,fen);
						write_com(0x80+0x40+7);
					}
					if(s1num==3)
					{
						shi++;
						if(shi==24)
							shi=0;
						write_sfm(4,shi);
						write_com(0x80+0x40+4);
					}
				}
			}										  
			if(s3==0)
			{
				delay(5);
				if(s3==0)
				{
					while(!s3);
					if(s1num==1)
					{
					/*	if(miao==0)
						{
							miao=59;
							write_sfm(10,miao);
							write_com(0x80+0x40+10);
						}*/
						miao--;
						if(miao==-1)
							miao=59;
						write_sfm(10,miao);
						write_com(0x80+0x40+10);
					}
					if(s1num==2)
					{
						fen--;
						if(fen==-1)
							fen=59;
						write_sfm(7,fen);
						write_com(0x80+0x40+7);
					}
					if(s1num==3)
					{
						shi--;
						if(shi==-1)
							shi=23;
						write_sfm(4,shi);
						write_com(0x80+0x40+4);
					}
				}
			}
		}
}
void main()
{
	init();
	while(1)
	{
		keyscan();
		
	}
//	while(1);
}

void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	count++;
if(count==18)
		{
			count=0;
			miao++;
			if(miao==60)
			{
				miao=0;
				fen++;
				if(fen==60)
				{
					fen=0;
					shi++;
					if(shi==24)
					{
						shi=0;
					}
					write_sfm(4,shi);
				}
				write_sfm(7,fen);
			}
			write_sfm(10,miao);
			
		}	
}

