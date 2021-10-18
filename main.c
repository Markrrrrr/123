/*    ��Ӱ���оƬ��ͷ�ļ�    */
#include<iostm8l152R8.h>  /*  ͷ�ļ� */
#include "OLED.h"
#include "i2cds1307.h"
#include "TSM12.h"
#include "AT24C02.h"

#include "mmc_sd.h"

extern void FSCTR(void);

extern uchar TIME[7];
extern unsigned char TSM12_Value[3];
extern uchar Key_Value;
/*******************************************************************************
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
    for(y = 1000 ; y > 0 ; y--);
}

/*******************************************************************************
**�������ƣ�void ALL_LED_Init()
**������������ʼ��LED1��LED2��LED3������������Ϊ���
**��ڲ�������
**�������
*******************************************************************************/
void ALL_LED_Init()
{
   //LED1 GPIO  INIT
    PD_ODR_bit.ODR5=1;
    PD_DDR_bit.DDR5 = 1;    //���ö˿�PD->5�������������Ĵ���Ϊ�������
    PD_CR1_bit.C15 = 1;     //����PD->5Ϊ�������
    PD_CR2_bit.C25 = 1;     //����PD->5���������Ϊ10MHZ
    
    //LED2 GPIO  INIT
    PD_ODR_bit.ODR6=1;
    PD_DDR_bit.DDR6 = 1;    //���ö˿�PD->6�������������Ĵ���Ϊ�������
    PD_CR1_bit.C16 = 1;     //����PD->6Ϊ�������
    PD_CR2_bit.C26 = 1;     //����PD->6���������Ϊ10MHZ
    
    //LED3 GPIO  INIT
    PD_ODR_bit.ODR4=1;
    PD_DDR_bit.DDR4 = 1;    //���ö˿�PD->7�������������Ĵ���Ϊ�������
    PD_CR1_bit.C14 = 1;     //����PD->7Ϊ�������
    PD_CR2_bit.C24 = 1;     //����PD->7���������Ϊ10MHZ
    
    
    
    
    
}
/* ������ */
int main(void)
{
  asm("sim");
  CLK_CKDIVR = 0x00;      //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz
  ALL_LED_Init();         //��������LED�Ƴ�ʼ������
  
  delay(1000);
  //IIC2402_Init();
  //SD_SPI_Init();
  OLED_Init();
  Ds1302Init();
  //TSM12_IIC_Init();
  TSM12_Init();
  
  //FSCTR();
  
  //asm("rim");
  //I2C_AT24C02_Test();
  while(1)
  {
    Ds1302ReadTime();
    LED_P8x16Char(0,0,'0'+TIME[2]/16);
    LED_P8x16Char(8,0,'0'+TIME[2]%16);
    
    LED_P8x16Char(24,0,'0'+TIME[1]/16);
    LED_P8x16Char(32,0,'0'+TIME[1]%16);
    
    LED_P8x16Char(48,0,'0'+TIME[0]/16);
    LED_P8x16Char(56,0,'0'+TIME[0]%16);
    //TSM_Write_Byte(0xd0,0x55);
    
    //TSM12_Init();
    TSM12_GET_Key();
    
    LED_P8x16Char(0,2,'0'+TSM12_Value[0]/100);
    LED_P8x16Char(8,2,'0'+TSM12_Value[0]/10);
    LED_P8x16Char(16,2,'0'+TSM12_Value[0]%10);
    
    LED_P8x16Char(32,2,'0'+TSM12_Value[1]/100);
    LED_P8x16Char(40,2,'0'+TSM12_Value[1]/10);
    LED_P8x16Char(48,2,'0'+TSM12_Value[1]%10);
    
    LED_P8x16Char(64,2,'0'+TSM12_Value[2]/100);
    LED_P8x16Char(72,2,'0'+TSM12_Value[2]/10);
    LED_P8x16Char(80,2,'0'+TSM12_Value[2]%10);
    
    LED_P8x16Char(0,1,'0'+Key_Value/10);
    LED_P8x16Char(8,1,'0'+Key_Value%10);
    
    delay(1000);           //���������ʱ
  }
}
