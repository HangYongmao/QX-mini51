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
* �������ܡ��� LCD1602��ʾ���������			   			            			    
* ��ʹ��˵������ ������
**********************************************************************************/

/*
SMC1602A(16*2)ģ����߽��߷�ʽ
������ͼ:	
       ---------------------------------------------------
       |LCM-----51   |	LCM-----51   |	LCM------51      |
       --------------------------------------------------|
       |DB0-----P2.0 |	DB4-----P2.4 |	RW-------P0.6    |
       |DB1-----P2.1 |	DB5-----P2.5 |	RS-------P0.7    |
       |DB2-----P2.2 |	DB6-----P2.6 |	E--------P0.5    |
       |DB3-----P2.3 |	DB7-----P2.7 |	VLCD��1K���赽GND|
       ---------------------------------------------------
���ߣ�ģ��TRIG�� P1.2  ECH0 ��P1.1

������Դ��ֻ��ѧϰ�ο�������Ӧ������ҵ��;��������Ҫ����ϵ���ߡ�

[ע:AT89x51ʹ��12M��11.0592M����,ʵ��ʹ��11.0592M]
=============================================================*/
#include <AT89x51.H>		//���������ļ�
#include <intrins.h>
#define  RX  P1_1
#define  TX  P1_2

#define LCM_RW  P0_6 //����LCD����
#define LCM_RS  P0_7
#define LCM_E   P0_5
#define LCM_Data  P2


#define Busy    0x80 //���ڼ��LCM״̬���е�Busy��ʶ

void LCMInit(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);
void Decode(unsigned char ScanCode);
void WriteDataLCM(unsigned char WDLCM);
void WriteCommandLCM(unsigned char WCLCM,BuysC);

unsigned char ReadDataLCM(void);
unsigned char ReadStatusLCM(void);
unsigned char code mcustudio[] ={" WWW.QXMCU.COM  "};
unsigned char code email[] =    {"863888999@QQ.COM"};
unsigned char code Cls[] =      {"                "};
unsigned char code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};

static unsigned char DisNum = 0; //��ʾ��ָ��				  
       unsigned int  time=0;
	   unsigned long S=0;
	   bit      flag =0;
	   unsigned char disbuff[4]	   ={ 0,0,0,0,};


//д����
void WriteDataLCM(unsigned char WDLCM) 
{
	ReadStatusLCM(); //���æ
	LCM_Data = WDLCM;
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
	LCM_E = 0; //��ʱ
	LCM_E = 1;
}

//дָ��
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
	if (BuysC) ReadStatusLCM(); //������Ҫ���æ
	LCM_Data = WCLCM;
	LCM_RS = 0;
	LCM_RW = 0;	
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;	
}

//������
unsigned char ReadDataLCM(void)
{
	LCM_RS = 1; 
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}

//��״̬
unsigned char ReadStatusLCM(void)
{
	LCM_Data = 0xFF; 
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	while (LCM_Data & Busy); //���æ�ź�
	return(LCM_Data);
}

void LCMInit(void) //LCM��ʼ��
{
	LCM_Data = 0;
	WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 

	WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	WriteCommandLCM(0x08,1); //�ر���ʾ
	WriteCommandLCM(0x01,1); //��ʾ����
	WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
	WriteCommandLCM(0x0F,1); // ��ʾ�����������
}

//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	X |= 0x80; //���ָ����
	WriteCommandLCM(X, 1); //��������
	WriteDataLCM(DData); //������
}

//��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
	unsigned char ListLength;

  ListLength = 0;
	Y &= 0x1;
	X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	while (DData[ListLength]>0x19) //�������ִ�β���˳�
		{
			if (X <= 0xF) //X����ӦС��0xF
				{
					DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
					ListLength++;
					X++;
				}
		}
}

//5ms��ʱ
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

//400ms��ʱ
void Delay400Ms(void)
{
	unsigned char TempCycA = 5;
	unsigned int TempCycB;
	while(TempCycA--)
		{
			TempCycB=7269;
			while(TempCycB--);
		};
}
/********************************************************/
    void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //�������CM
	 if((S>=700)||flag==1) //����������Χ��ʾ��-��
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, ASCII[11]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[11]);
	  DisplayOneChar(3, 1, ASCII[11]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	  DisplayOneChar(0, 1, ASCII[disbuff[0]]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[disbuff[1]]);
	  DisplayOneChar(3, 1, ASCII[disbuff[2]]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	}
/********************************************************/
     void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
  {
    flag=1;							 //�ж������־
  }
/********************************************************/
     void  StartModule() 		         //����ģ��
  {
	  TX=1;			                     //����һ��ģ��
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TX=0;
  }
/********************************************************/ 
void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}
/*********************************************************/
void main(void)
{
	unsigned char TempCyc;
	Delay400Ms(); //�����ȴ�����LCM���빤��״̬
	LCMInit(); //LCM��ʼ��
	Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
	DisplayListChar(0, 0, mcustudio);
	DisplayListChar(0, 1, email);
	ReadDataLCM();//�����þ�������
	for (TempCyc=0; TempCyc<10; TempCyc++)
	Delay400Ms(); //��ʱ
	DisplayListChar(0, 1, Cls);	
	while(1)
	{
	 TMOD=0x01;		   //��T0Ϊ��ʽ1��GATE=1��
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //����T0�ж�
	 EA=1;			   //�������ж�			
	
	while(1)
	  {
	     StartModule();
		// DisplayOneChar(0, 1, ASCII[0]);
	     while(!RX);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(RX);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut();			//����
		 delayms(80);		//80MS
		 
	  }
	}
}



                