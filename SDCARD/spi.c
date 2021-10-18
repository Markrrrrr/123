/**
  ******************************************************************************
  * @文件         spi.c
  * @作者         LGG
  * @版本         V1.0.0
  * @日期         2018-1-22
  * @摘要         spi源文件
  ******************************************************************************
*/ 

/* 包含头文件 ----------------------------------------------------------------*/
#include "spi.h"


/**
  * @函数名       SPI_Config
  * @功  能       SPI配置
  * @参  数       无
  * @返回值       无
  */
void SPI1_Init(void)
{
  /* 使能SPI1时钟 */
  //CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  CLK_PCKENR1_PCKEN14=1;//打开SPI1外设时钟
  /* 配置MISO引脚为上拉输入 */
  //GPIO_Init(SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PINS, GPIO_Mode_In_PU_No_IT);
  PB_DDR_bit.DDR7=0; PB_CR1_bit.C17=1;PB_CR2_bit.C27=0;
  /* 配置MOSI引脚为推挽输出 */
  //GPIO_Init(SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR6=1; PB_CR1_bit.C16=1;PB_CR2_bit.C26=1;
  /* 配置SCK引脚为推挽输出 */
  //GPIO_Init(SPI_SCK_GPIO_PORT, SPI_SCK_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR5=1; PB_CR1_bit.C15=1;PB_CR2_bit.C25=1;
  /* 配置NSS引脚为推挽输出 */
  //GPIO_Init(SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR4=1; PB_CR1_bit.C14=1;PB_CR2_bit.C24=1;
  /* 拉高NSS引脚 */
  SPI_NSS_HIGH();
  
  /* 初始化SPI1，高位先发送， 时钟8分频，SPI主模式，空闲相位高，
     第二个时钟沿发送数据，NSS引脚软件控制，CRC校验：0x55
 */
  //SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
          // SPI_CPOL_High, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
  
  SPI1_CR1_SPE=0;//关闭SPI设备
  //设置串行波特率
  SPI1_CR1_BR=0;//fmaster/2=1M
  //配置CPOL和CPHA，定义数据传输和串行时钟间的相位关系
  SPI1_CR1_CPHA=0;//数据采样从第一个时钟边沿开始
  SPI1_CR1_CPOL=0;//空闲状态时，SCK保持低电平  
   //定义帧格式
  SPI1_CR1_LSBFIRST=0;//先发送MSB 
  //使能从设备管理//主模式需通过改变SSI位 来控制SPI_SEL
  SPI1_CR2_SSM=1;//禁止软件从设备
  SPI1_CR2_SSI=1;
  //主从设备模式选择
  SPI1_CR1_MSTR=1;//作为主设备
  SPI1_CR2_RXONLY=0;//全双工
  SPI1_CR2_BDM=0;//选择单向数据模式
  SPI1_CR1_SPE=1;//开启SPI设备
  
  
  
  //SPI_Cmd(SPI1, ENABLE);      //使能SPI
  
  
}


/**
  * @函数名       SPI_RW_Byte
  * @功  能       SPI读写一个字节
  * @参  数       data：写入的字节
  * @返回值       temp：读取的字节
  */
uint8_t SPI1_ReadWriteByte(uint8_t data)
{
  uint8_t temp;
  
  /* 等待SPI1空闲 */
  while (!(SPI1_SR_TXE));
  
  /* SPI1发送数据 */
  //SPI_SendData(SPI1, data);
  SPI1_DR=data;//写入需要操作的寄存器地址,
  /* 等待接受一字节数据 */
  while (!(SPI1_SR_RXNE));
  
  /* 读取接收的数据 */
  temp = SPI1_DR;
  
  /* 返回接收的数据 */
  return temp;
}

/************************ Copyright (C)2017 LGG. All Rights Reserved *****END OF FILE****/

