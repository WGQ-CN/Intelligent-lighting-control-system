#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include <I2C.H>

#define uchar unsigned char
#define  PCF8591 0x90    //PCF8591��ַ

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

// ��������
unsigned long xdata  LedOut[8];

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 disp[4];
u8 code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

unsigned char date;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

// ��������
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
		temp=IRcvByte(PCF8591);		// AIN2 ��������
	}
	i++;
	disp[0]=smgduan[temp/1000];//ǧλ
	disp[1]=smgduan[temp%1000/100];//��λ
	disp[2]=smgduan[temp%1000%100/10];//��λ
	disp[3]=smgduan[temp%1000%100%10];		
}

void DigDisplay()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ	
		}
		P0=disp[3-i];//��������
		//delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}		
}

void main()
{
	while(1)
	{
		datapros();	 //���ݴ�����
		DigDisplay();//�������ʾ����
	}
}


void delay(uchar i)
{
	uchar j,k; 
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
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
