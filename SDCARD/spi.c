/**
  ******************************************************************************
  * @�ļ�         spi.c
  * @����         LGG
  * @�汾         V1.0.0
  * @����         2018-1-22
  * @ժҪ         spiԴ�ļ�
  ******************************************************************************
*/ 

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "spi.h"


/**
  * @������       SPI_Config
  * @��  ��       SPI����
  * @��  ��       ��
  * @����ֵ       ��
  */
void SPI1_Init(void)
{
  /* ʹ��SPI1ʱ�� */
  //CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  CLK_PCKENR1_PCKEN14=1;//��SPI1����ʱ��
  /* ����MISO����Ϊ�������� */
  //GPIO_Init(SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PINS, GPIO_Mode_In_PU_No_IT);
  PB_DDR_bit.DDR7=0; PB_CR1_bit.C17=1;PB_CR2_bit.C27=0;
  /* ����MOSI����Ϊ������� */
  //GPIO_Init(SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR6=1; PB_CR1_bit.C16=1;PB_CR2_bit.C26=1;
  /* ����SCK����Ϊ������� */
  //GPIO_Init(SPI_SCK_GPIO_PORT, SPI_SCK_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR5=1; PB_CR1_bit.C15=1;PB_CR2_bit.C25=1;
  /* ����NSS����Ϊ������� */
  //GPIO_Init(SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast);
  PB_DDR_bit.DDR4=1; PB_CR1_bit.C14=1;PB_CR2_bit.C24=1;
  /* ����NSS���� */
  SPI_NSS_HIGH();
  
  /* ��ʼ��SPI1����λ�ȷ��ͣ� ʱ��8��Ƶ��SPI��ģʽ��������λ�ߣ�
     �ڶ���ʱ���ط������ݣ�NSS����������ƣ�CRCУ�飺0x55
 */
  //SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
          // SPI_CPOL_High, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
  
  SPI1_CR1_SPE=0;//�ر�SPI�豸
  //���ô��в�����
  SPI1_CR1_BR=0;//fmaster/2=1M
  //����CPOL��CPHA���������ݴ���ʹ���ʱ�Ӽ����λ��ϵ
  SPI1_CR1_CPHA=0;//���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
  SPI1_CR1_CPOL=0;//����״̬ʱ��SCK���ֵ͵�ƽ  
   //����֡��ʽ
  SPI1_CR1_LSBFIRST=0;//�ȷ���MSB 
  //ʹ�ܴ��豸����//��ģʽ��ͨ���ı�SSIλ ������SPI_SEL
  SPI1_CR2_SSM=1;//��ֹ������豸
  SPI1_CR2_SSI=1;
  //�����豸ģʽѡ��
  SPI1_CR1_MSTR=1;//��Ϊ���豸
  SPI1_CR2_RXONLY=0;//ȫ˫��
  SPI1_CR2_BDM=0;//ѡ��������ģʽ
  SPI1_CR1_SPE=1;//����SPI�豸
  
  
  
  //SPI_Cmd(SPI1, ENABLE);      //ʹ��SPI
  
  
}


/**
  * @������       SPI_RW_Byte
  * @��  ��       SPI��дһ���ֽ�
  * @��  ��       data��д����ֽ�
  * @����ֵ       temp����ȡ���ֽ�
  */
uint8_t SPI1_ReadWriteByte(uint8_t data)
{
  uint8_t temp;
  
  /* �ȴ�SPI1���� */
  while (!(SPI1_SR_TXE));
  
  /* SPI1�������� */
  //SPI_SendData(SPI1, data);
  SPI1_DR=data;//д����Ҫ�����ļĴ�����ַ,
  /* �ȴ�����һ�ֽ����� */
  while (!(SPI1_SR_RXNE));
  
  /* ��ȡ���յ����� */
  temp = SPI1_DR;
  
  /* ���ؽ��յ����� */
  return temp;
}

/************************ Copyright (C)2017 LGG. All Rights Reserved *****END OF FILE****/

