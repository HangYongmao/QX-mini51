/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ (直销店)  http://qx-mcu.taobao.com/  （总店）
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52RC
* 【编译环境】： Keil μVisio4	
* 【程序功能】： 无源蜂鸣器测试-八月桂花			   			            			    
* 【使用说明】： 短接蜂鸣器J1跳帽
**********************************************************************************/

#include <reg52.h>    
#include <intrins.h>    
//关于如何编制音乐代码, 其实十分简单,各位可以看以下代码.    
//频率常数即音乐术语中的音调,而节拍常数即音乐术语中的多少拍;    
//所以拿出谱子, 试探编吧!    

sbit Beep =  P3^6 ; 
   
unsigned char n=0;  //n为节拍常数变量    
unsigned char code music_tab[] ={   
0x18, 0x30, 0x1C , 0x10, //格式为: 频率常数, 节拍常数, 频率常数, 节拍常数,    
0x20, 0x40, 0x1C , 0x10,   
0x18, 0x10, 0x20 , 0x10,   
0x1C, 0x10, 0x18 , 0x40,   
0x1C, 0x20, 0x20 , 0x20,   
0x1C, 0x20, 0x18 , 0x20,   
0x20, 0x80, 0xFF , 0x20,   
0x30, 0x1C, 0x10 , 0x18,   
0x20, 0x15, 0x20 , 0x1C,   
0x20, 0x20, 0x20 , 0x26,   
0x40, 0x20, 0x20 , 0x2B,   
0x20, 0x26, 0x20 , 0x20,   
0x20, 0x30, 0x80 , 0xFF,   
0x20, 0x20, 0x1C , 0x10,   
0x18, 0x10, 0x20 , 0x20,   
0x26, 0x20, 0x2B , 0x20,   
0x30, 0x20, 0x2B , 0x40,   
0x20, 0x20, 0x1C , 0x10,   
0x18, 0x10, 0x20 , 0x20,   
0x26, 0x20, 0x2B , 0x20,   
0x30, 0x20, 0x2B , 0x40,   
0x20, 0x30, 0x1C , 0x10,   
0x18, 0x20, 0x15 , 0x20,   
0x1C, 0x20, 0x20 , 0x20,   
0x26, 0x40, 0x20 , 0x20,   
0x2B, 0x20, 0x26 , 0x20,   
0x20, 0x20, 0x30 , 0x80,   
0x20, 0x30, 0x1C , 0x10,   
0x20, 0x10, 0x1C , 0x10,   
0x20, 0x20, 0x26 , 0x20,   
0x2B, 0x20, 0x30 , 0x20,   
0x2B, 0x40, 0x20 , 0x15,   
0x1F, 0x05, 0x20 , 0x10,   
0x1C, 0x10, 0x20 , 0x20,   
0x26, 0x20, 0x2B , 0x20,   
0x30, 0x20, 0x2B , 0x40,   
0x20, 0x30, 0x1C , 0x10,   
0x18, 0x20, 0x15 , 0x20,   
0x1C, 0x20, 0x20 , 0x20,   
0x26, 0x40, 0x20 , 0x20,   
0x2B, 0x20, 0x26 , 0x20,   
0x20, 0x20, 0x30 , 0x30,   
0x20, 0x30, 0x1C , 0x10,   
0x18, 0x40, 0x1C , 0x20,   
0x20, 0x20, 0x26 , 0x40,   
0x13, 0x60, 0x18 , 0x20,   
0x15, 0x40, 0x13 , 0x40,   
0x18, 0x80, 0x00   
};   
  
void delay (unsigned char m)   //控制频率延时    
{   
 unsigned i=3*m;   
 while(--i);   
}   
void delayms(unsigned char a)  //豪秒延时子程序    
{   
  while(--a);     
}    
void main()   
{ 
	unsigned char p,m;   //m为频率常数变量    
	unsigned char i=0;   
	TMOD&=0x0f;   
	TMOD|=0x01;   
	TH0=0xd8;TL0=0xef;   
	IE=0x82;   
play:
	while(1)   
    {   
	a:  p=music_tab[i];   
		if(p==0x00)       
		{ 
			i=0, delayms(1000); goto play; //如果碰到结束符,延时1秒,回到开始再来一遍  
		}       
		else 
			if(p==0xff)  
			{ 
				i=i+1;delayms(100),TR0=0; goto a;  //若碰到休止符,延时100ms,继续取下一音符
			}      
			else
			{
				m=music_tab[i++], n=music_tab[i++];	 //取频率常数 和 节拍常数
			}      
			TR0=1;                                             //开定时器0    
		while(n!=0) 		   //等待节拍完成,输出音频  
		{
			Beep=~Beep;
			delay(m);    
		} 
		TR0=0;                                             //关定时器0    
    }   
} 
void int0() interrupt 1   //采用中断定时器0 控制节拍    
{  
	TH0=0xd8;   
	TL0=0xef;   
	n--;   
} 