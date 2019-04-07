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
**********************************************************************************/

# include <reg51.h>
# include <intrins.h>
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL;

sbit rs=P0^7;
sbit rw=P0^6;
sbit ep=P0^5;

BYTE code dis1 []={"     QXMCU     "};
BYTE code dis2 []={"HTTP://QXMCU.COM"};

void delay (int m)
{
 unsigned char i,j;
 for (i=0;i<m;i++)
 for (j=0;j<253;j++);
}

 BOOL lcd_bz()
{
  BOOL result;
  rs=0;           // ��æ�ź�
  rw=1;
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (BOOL)(P2&0x80);
  ep=0;
  return result;
}

void lcd_wcmd (BYTE cmd)
{
  while (lcd_bz());
  rs=0;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=cmd ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_pos (BYTE pos)
{
  lcd_wcmd (pos|0x80);
}

void lcd_wdat (BYTE dat)
{
   while (lcd_bz());
  rs=1;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=dat ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_init ()
{
  lcd_wcmd (0x38);
  delay (1);
  lcd_wcmd (0x0c);
  delay (1);
  lcd_wcmd (0x06);
  delay (1);
  lcd_wcmd (0x01);
  delay (1);
}

main ()
{
  BYTE i;
  lcd_init ();
  delay (10);

  lcd_pos (0);
  i=0;
  while (dis1 [i]!='\0')
   {
     lcd_wdat (dis1 [i]);
     i++;	
   } 

   lcd_pos (0x40);
  i=0;
  while (dis2 [i]!='\0')
   {
     lcd_wdat (dis2 [i]);
     i++;	
   } 
   while (1);
} 