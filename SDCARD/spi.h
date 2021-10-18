/**
  ******************************************************************************
  * @�ļ�         spi.h
  * @����         LGG
  * @�汾         V1.0.0
  * @����         2018-1-22
  * @ժҪ         spiͷ�ļ�
  ******************************************************************************
*/ 


/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef __SPI__H
#define __SPI__H

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include<iostm8l152R8.h>  /*  ͷ�ļ� */

#define uint8_t unsigned char
#define u16 unsigned int
#define u32 unsigned long int



/* �궨�� --------------------------------------------------------------------*/
/* ����SPI��IO PORT��PIN */
#define SPI_MISO_GPIO_PORT  (GPIOB)
#define SPI_MISO_GPIO_PINS  (GPIO_Pin_7)

#define SPI_MOSI_GPIO_PORT  (GPIOB)
#define SPI_MOSI_GPIO_PINS  (GPIO_Pin_6)

#define SPI_SCK_GPIO_PORT  (GPIOB)
#define SPI_SCK_GPIO_PINS  (GPIO_Pin_5)

#define SPI_NSS_GPIO_PORT  (GPIOB)
#define SPI_NSS_GPIO_PINS  (GPIO_Pin_4)

#define SPI_NSS_LOW()   PB_ODR_bit.ODR4=0
#define SPI_NSS_HIGH()  PB_ODR_bit.ODR4=1


/* ��������-------------------------------------------------------------------*/
void SPI1_Init(void);
uint8_t SPI1_ReadWriteByte(uint8_t data);


#endif

/************************ Copyright (C)2017 LGG. All Rights Reserved *****END OF FILE****/


