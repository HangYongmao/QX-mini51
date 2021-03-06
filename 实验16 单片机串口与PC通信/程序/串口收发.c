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
* 【程序功能】： 串口中断程序实验——通过单片机向电脑发送数据		   			            			    
* 【使用说明】： 比特率9600，校验位：无 停止位：1
**********************************************************************************/

/*预处理命令*/
#include <reg52.h> 		  //包含单片机寄存器的头文件
#define uchar unsigned char
#define uint unsigned int
uchar num,a;

/*
********************************************************************************
** 函数名称 ： main(void)
** 函数功能 ： 主函数
********************************************************************************
*/
void main()
{
	TMOD=0x20;		   //用定时器设置串口波特率	   9600 
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;          //串口初始化
	SM0=0;
	SM1=1;
	EA=1;           //开启总中断
	ES=1;
	while(1)
	{
		if(num==1)    //判断是否有串口数据的传送
		{
			ES=0;
			num=0;
			SBUF=a;			 //发送数据a到SBUF，即将单片机的数据发送到计算机
			while(!TI);
			TI=0;
			ES=1;
		}
	}	
}
void ser() interrupt 4
{
	RI=0;
	P2=SBUF;			//接收数据SBUF，即将计算机的数据接收。
	a=SBUF;
	num=1;
}