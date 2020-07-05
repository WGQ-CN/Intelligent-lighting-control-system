#include<reg52.h>    //包含单片机寄存器的头文件
#include <I2C.H>

#define uchar unsigned char
#define PCF8591 0x90    //PCF8591 地址

sbit Man=P3^0;

sbit led0=P2^4;
sbit led1=P2^5;
sbit led2=P2^6;
sbit led3=P2^7;

unsigned int light;
unsigned int voltage;

bit ISendByte(unsigned char sla,unsigned char c);
unsigned char IRcvByte(unsigned char sla);

void main()
{  
	while(1)
	{
		if(Man==1)
		{
			ISendByte(PCF8591,0x41);
			light=IRcvByte(PCF8591);
			ISendByte(PCF8591,0x40); 								    
			voltage=IRcvByte(PCF8591);
			
			if(light>230)
			{
				led0=0;
				led1=0;
				led2=0;
				led3=0;
			}
			else if(light>220)
			{
				led0=1;
				led1=0;
				led2=0;
				led3=0;
			}
			else if(light>210)
			{
				led0=1;
				led1=0;
				led2=0;
				led3=1;
			}
			else if(light>200)
			{
				led0=1;
				led1=1;
				led2=0;
				led3=1;
			}
			else
			{
				led0=1;
				led1=1;
				led2=1;
				led3=1;
			}
		}
		else
		{
			led0=1;
			led1=1;
			led2=1;
			led3=1;
		}
   	}
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
