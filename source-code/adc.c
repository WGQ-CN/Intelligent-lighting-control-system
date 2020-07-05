#include<reg52.h>    //包含单片机寄存器的头文件
#include <I2C.H>

#define uchar unsigned char
#define  PCF8591 0x90    //PCF8591地址

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

// 变量定义
unsigned long xdata  LedOut[8];

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 disp[4];
u8 code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

unsigned char date;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

// 函数申明
void display();
void delay(uchar i);
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);
bit ISendByte(unsigned char sla,unsigned char c);
unsigned char IRcvByte(unsigned char sla);
void Initial_com(void);
//******************************************************************/

void datapros()
{
	u16 temp;
	static u8 i;
	if(i==50)
	{
		i=0;
		ISendByte(PCF8591,0x41);
		temp=IRcvByte(PCF8591);		// AIN2 光敏电阻
	}
	i++;
	disp[0]=smgduan[temp/1000];//千位
	disp[1]=smgduan[temp%1000/100];//百位
	disp[2]=smgduan[temp%1000%100/10];//个位
	disp[3]=smgduan[temp%1000%100%10];		
}

void DigDisplay()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位	
		}
		P0=disp[3-i];//发送数据
		//delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}		
}

void main()
{
	while(1)
	{
		datapros();	 //数据处理函数
		DigDisplay();//数码管显示函数
	}
}


void delay(uchar i)
{
	uchar j,k; 
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
}

/*******************************************************************
ADC发送字节[命令]数据函数               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
	Start_I2c();              //启动总线
	SendByte(sla);            //发送器件地址
	if(ack==0)return(0);
	SendByte(c);              //发送数据
	if(ack==0)return(0);
	Stop_I2c();               //结束总线
	return(1);
}

/*******************************************************************
ADC读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{
	unsigned char c;
	Start_I2c();          //启动总线
	SendByte(sla+1);      //发送器件地址
	if(ack==0)return(0);
	c=RcvByte();          //读取数据0
	Ack_I2c(1);           //发送非就答位
	Stop_I2c();           //结束总线
	return(c);
}
