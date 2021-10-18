/*    添加包含芯片的头文件    */
#include<iostm8l152R8.h>  /*  头文件 */
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
**函数名称：void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**功能描述：大概延时
**入口参数：unsigned int ms   输入大概延时数值
**输出：无
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 1000 ; y > 0 ; y--);
}

/*******************************************************************************
**函数名称：void ALL_LED_Init()
**功能描述：初始化LED1、LED2、LED3控制引脚设置为输出
**入口参数：无
**输出：无
*******************************************************************************/
void ALL_LED_Init()
{
   //LED1 GPIO  INIT
    PD_ODR_bit.ODR5=1;
    PD_DDR_bit.DDR5 = 1;    //设置端口PD->5的输入输出方向寄存器为输出方向
    PD_CR1_bit.C15 = 1;     //设置PD->5为推挽输出
    PD_CR2_bit.C25 = 1;     //设置PD->5的输出速率为10MHZ
    
    //LED2 GPIO  INIT
    PD_ODR_bit.ODR6=1;
    PD_DDR_bit.DDR6 = 1;    //设置端口PD->6的输入输出方向寄存器为输出方向
    PD_CR1_bit.C16 = 1;     //设置PD->6为推挽输出
    PD_CR2_bit.C26 = 1;     //设置PD->6的输出速率为10MHZ
    
    //LED3 GPIO  INIT
    PD_ODR_bit.ODR4=1;
    PD_DDR_bit.DDR4 = 1;    //设置端口PD->7的输入输出方向寄存器为输出方向
    PD_CR1_bit.C14 = 1;     //设置PD->7为推挽输出
    PD_CR2_bit.C24 = 1;     //设置PD->7的输出速率为10MHZ
    
    
    
    
    
}
/* 主函数 */
int main(void)
{
  asm("sim");
  CLK_CKDIVR = 0x00;      //内部时钟为1分频 = 16Mhz
  ALL_LED_Init();         //调用所有LED灯初始化函数
  
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
    
    delay(1000);           //大概周期延时
  }
}
