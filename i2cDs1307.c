#include<iostm8l152R8.h>  /*  ͷ�ļ� */
#include "i2cds1307.h"

#define uchar unsigned char


//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
uchar READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uchar WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302ʱ�ӳ�ʼ��2013��1��1�����ڶ�12��00��00�롣---//
//---�洢˳�������ʱ��������,�洢��ʽ����BCD��---//
uchar TIME[7] = {0, 0x40, 0x18, 0x01, 0x01, 0x02, 0x18};

//ǰ��λ��ʾ��/��/ʱ  0X39��ʾ39��

/*******************************************************************************
* �� �� ��         : void Close_DS1302()
* ��������	   : �ر�DS1302.
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Close_DS1302()
{
  PC_ODR_bit.ODR5=0;//RST
  PC_ODR_bit.ODR6=0;//DAT
  PC_ODR_bit.ODR7=0;//CLK
}
/*******************************************************************************
* �� �� ��         : Ds1302Init
* ��������	   : ��ʼ��DS1302.
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Ds1302Init()
{
  uchar n;
  PC_DDR_bit.DDR5=1;PC_CR1_bit.C15=1;PC_CR2_bit.C25=1;//RST
  PC_DDR_bit.DDR6=1;PC_CR1_bit.C16=1;PC_CR2_bit.C26=1;//DAT
  PC_DDR_bit.DDR7=1;PC_CR1_bit.C17=1;PC_CR2_bit.C27=0;//CLK
  
  
  Ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
  for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
  {
    Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
  }
  
  Ds1302Write(0x8E,0x80);		 //��д��������
}

/*******************************************************************************
* �� �� ��         : Set_1307Time()
* ��������	   : ����ʱ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Set_1307Time()
{
  uchar n;
  
  Ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
  for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
  {
    Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
  }
  Ds1302Write(0x8E,0x80);		 //��д��������
}

/*******************************************************************************
* �� �� ��         : Ds1302ReadTime
* ��������	   : ��ȡʱ����Ϣ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Ds1302ReadTime()
{
  uchar n;
  for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
  {
    TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
  }	
}

/*******************************************************************************
* �� �� ��         : Ds1302Write
* ��������	   : ��DS1302�����ַ+���ݣ�
* ��    ��         : addr,dat
* ��    ��         : ��
*******************************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
        SET_OUT();
	RST = 0;
	asm("nop");
	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	asm("nop");
	RST = 1; //Ȼ��RST(CE)�øߵ�ƽ��
	asm("nop");

	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO_OUT = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		asm("nop");
		SCLK = 0;
		asm("nop");
	}
	for (n=0; n<8; n++)//д��8λ����
	{
		DSIO_OUT = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		asm("nop");
		SCLK = 0;
		asm("nop");
	}	
		 
	RST = 0;//�������ݽ���
	asm("nop");
}

/*******************************************************************************
* �� �� ��         : Ds1302Read
* ��������		   : ��ȡһ����ַ������
* ��    ��         : addr
* ��    ��         : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
        SET_OUT();
	RST = 0;
	asm("nop");
	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	asm("nop");
	RST = 1;//Ȼ��RST(CE)�øߵ�ƽ��
	asm("nop");
        
	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO_OUT = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		asm("nop");
		SCLK = 0;//DS1302�½���ʱ����������
		asm("nop");
	}
	asm("nop");
        SET_IN();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = DSIO_IN;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		asm("nop");
		SCLK = 0;//DS1302�½���ʱ����������
		asm("nop");
	}
        SET_OUT();
	RST = 0;
	asm("nop");	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK = 1;
	asm("nop");
	DSIO_OUT = 0;
	asm("nop");
	DSIO_OUT = 1;
	asm("nop");
	return dat;
}