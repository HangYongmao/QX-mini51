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
* �������ܡ��� 1602Һ����ʾ			   			            			    
* ��ʹ��˵������ 1.ʹ��1602Һ��ǰ�Ȱε������J6��ñ���������Һ�����ܹ���
                 2.��1602Һ���ӵ�L2��ĸ�ϣ�Һ�����򿪷����ⲿ
				 3.��������д��ɺ�1602Һ������ʾ������ڿ��������·��ĵ�λ��RV1
				 S1ѡ�����ʱ���룬S2�ӣ�S3����S4ȷ�ϡ�				 
**********************************************************************************/

#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int 
sbit lcdrs  = P0^7;	//Һ�����Ŷ���
sbit led_cs = P0^6;
sbit lcden  = P0^5;

sbit key1 = P3^5;
sbit key2 = P3^4;
sbit key3 = P3^3;
sbit keyE = P3^2;
uchar num,key1num,count,hour,min,sec;
uchar code cg_1602[]={ 0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,
0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,
0x0f,0x09,0x09,0x0f,0x09,0x09,0x0f,0x00};//"������"
/*��ʱ����*/
void delay(uint xms)
{
	uint i,j; 
	for(i=xms;i>0;i--)
	for(j=114;j>0;j--);
}
/*1602Һ��*/
void write_com(uchar com)//д����
{
	lcdrs=0;
	lcden=0;
	P2=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar date)//д����
{
	lcdrs=1;
	lcden=0;
	P2=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_by_xy(uchar x,uchar y)//���궨λ
{
	uchar address;
	if(y==0) 
	address=0x80+x;//y=0,д��һ��
	else 
	address=0xc0+x;//y=1,д�ڶ���
	write_com(address); 
}
/*void write_char(uchar x,uchar y,uchar date)//���궨λ��ʾ
{
	write_by_xy(x,y);//�ȶ�λ 
	write_data(date);//��д��
}*/
void write_string(uchar x,uchar y,uchar *s)//д�ַ��������ָ�뻹���󶮰���
{
	write_by_xy(x,y);//�趨��ʾλ�� 
	while(*s)//write character
	{
	P2=*s;
	write_data(*s); 
	s++;
	}
}
void write_sfm(uchar add,uchar date)//дʱ���뺯��
{
	uchar hour,ge;
	hour=date/10;//�ֽ�һ��2λ����ʮλ�͸�λ
	ge=date%10;
	write_com(0x80+0x40+add);//������ʾλ��
	write_data(0x30+hour);//��ȥҺ����ʾʮλ
	write_data(0x30+ge); //��ȥҺ����ʾ��λ
}
void write_cg()//д��̶����ֺͷ���
{
	write_com(0x40); //�趨CGRAM��ַ 
	for(num=0;num<24;num++) //���Զ���������д��CGRAM�� 
	{ 
		write_data(cg_1602[num]); 
	}
	write_com(0x84);//�� 
	{ 
		write_data(0); 
	} 
	write_com(0x87);//�� 
	{ 
		write_data(1); 
	}
	write_com(0x8a);//�� 
	{ 
		write_data(2); 
	}
	write_string(2,1,":");
	write_string(5,1,":");
	write_string(9,1,"QX-MCU");
}
void init_1602()//��ʼ��
{
	led_cs=0;
	hour=22;min=33;sec=55;key1num=0;count=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_string(0,0,"2014");
	write_string(5,0,"07");
	write_string(8,0,"12");
	write_sfm(0,hour);
	write_sfm(3,min);
	write_sfm(6,sec);
	write_cg();
	TMOD=0x01;//���ö�ʱ��0����ģʽ1
	TH0=(65536-50000)/256;//��ʱ��װ��ֵ
	TL0=(65536-50000)%256;
	EA=1; //�����ж�
	ET0=1; //����ʱ��0�ж�
	TR0=1; //������ʱ��0
}
/*����ɨ�躯��*/
void keyscan()//����ɨ�躯��
{
	keyE =0;
	if(key1==0) 
	{
		delay(5);
		if(key1==0)//ȷ�Ϲ��ܼ�������
		{ 	
			key1num++;//���ܼ����´�����¼
			while(!key1);//�ͷ�ȷ��
			if(key1num==1)//��һ�α�����ʱ
			{
				TR0=0; //�رն�ʱ��
				write_com(0x80+0x40+7);//��궨λ����λ��
				write_com(0x0f); //��꿪ʼ��˸
			}
			if(key1num==2)//�ڶ��ΰ��¹����˸��λ������λ��
			{
				write_com(0x80+0x40+4);
			}
			if(key1num==3)//�����ΰ��¹����˸��λ��Сʱλ��
			{
				write_com(0x80+0x40+1);
			}
			if(key1num==4)//���Ĵΰ���
			{
				key1num=0;//��¼����������
				write_com(0x0c);//ȡ�������˸
				TR0=1; //������ʱ��ʹʱ�ӿ�ʼ��
			} 
		}
	}
	if(key1num!=0)//ֻ�й��ܼ������º����Ӻͼ�С������Ч
	{
		if(key2==0)
		{
			delay(5);
			if(key2==0)//���Ӽ�ȷ�ϱ�����
			{
				while(!key2);//�����ͷ�
				if(key1num==1)//�����ܼ���һ�ΰ���
				{
					sec++; //��������1
					if(sec==60)//����60������
					sec=0;
					write_sfm(6,sec);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
				}
				if(key1num==2)//�����ܼ��ڶ��ΰ���
				{
					min++;//��������Ӽ�1
					if(min==60)//����60������
					min=0;
					write_sfm(3,min);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+4);//��ʾλ�����»ص����ڴ�
				}
				if(key1num==3)//�����ܼ������ΰ���
				{
					hour++;//�����Сʱ��1
					if(hour==24)//����24������
					hour=0;
					write_sfm(0,hour);;//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+1);//��ʾλ�����»ص����ڴ�
				}
			}
		}
		if(key3==0)
		{
			delay(5);
			if(key3==0)//ȷ�ϼ�С��������
			{
				while(!key3);//�����ͷ�
				if(key1num==1)//�����ܼ���һ�ΰ���
				{
					sec--;//��������1
					if(sec==-1)//����������������������Ϊ59
						sec=59;
					write_sfm(6,sec);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
				}
				if(key1num==2)//�����ܼ��ڶ��ΰ���
				{
					min--;//��������Ӽ�1
					if(min==-1)//����������������������Ϊ59
						min=59;
					write_sfm(3,min);//ÿ����һ����Һ����ʾһ��
				write_com(0x80+0x40+4);//��ʾλ�����»ص����ڴ�
				}
				if(key1num==3)//�����ܼ��ڶ��ΰ���
				{
					hour--;//�����Сʱ��1
					if(hour==-1)//����������������������Ϊ23
						hour=23;
					write_sfm(0,hour);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+1);//��ʾλ�����»ص����ڴ�
				}
			}
		}
	}
}
/*������*/
void main()
{
	led_cs=0;
	init_1602();
	while(1)
	{
		keyscan();
	} 
}
/*�жϺ���*/
void timer0() interrupt 1//��ʱ��0�жϷ������
{
	TH0=(65536-50000)/256;//�ٴ�װ��ʱ����ֵ
	TL0=(65536-50000)%256;
	count++; //�жϴ����ۼ�
	if(count==20) //20��50����Ϊ1��
	{
		count=0;
		sec++;
		if(sec==60)//��ӵ�60���λ����
		{
			sec=0;//ͬʱ��������
			min++;
			if(min==60)//���Ӽӵ�60���λСʱ
			{
				min=0;//ͬʱ����������
				hour++;
				if(hour==24)//Сʱ�ӵ�24��Сʱ����
				{
					hour=0;
				}
				write_sfm(0,hour);//Сʱ���仯������д��
			}
			write_sfm(3,min);//�������仯������д��
		}
		write_sfm(6,sec);//�����仯������д�� 
	} 
}