#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include <I2C.H>

#define uchar unsigned char
#define PCF8591 0x90    //PCF8591 ��ַ

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
ADC�����ֽ�[����]���ݺ���               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
	Start_I2c();              //��������
	SendByte(sla);            //����������ַ
	if(ack==0)return(0);
	SendByte(c);              //��������
	if(ack==0)return(0);
  	Stop_I2c();               //��������
   	return(1);
}

/*******************************************************************
ADC���ֽ����ݺ���               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  
	unsigned char c;
	Start_I2c();          //��������
	SendByte(sla+1);      //����������ַ
	if(ack==0)return(0);
   	c=RcvByte();          //��ȡ����0
   	Ack_I2c(1);           //���ͷǾʹ�λ
	Stop_I2c();           //��������
	return(c);
}
