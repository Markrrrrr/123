/************************************************
文件：oled.h
用途：OLED显示函数头文件
************************************************/
#ifndef __OLED_H__
#define __OLED_H__


#define OLEDPOW_ON  PD_ODR_bit.ODR6 = 1//1
#define OLEDPOW_OFF PD_ODR_bit.ODR6 = 0//1

#define uchar unsigned char
#define uint unsigned int

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

extern void OLEDIO_Init();
extern void Close_OLED_IO();
extern void OLED_Init(void);
extern void LED_SetPos(unsigned char ucIdxX, unsigned char ucIdxY); 
extern void LED_P6x8Char(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucData);
extern void LED_P6x8Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char NO);
extern void LED_P8x16Char(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char ucData);
extern void LED_P16x32Char(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char ucData);
extern void LED_P8x16Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[],unsigned char NegDis_,unsigned char FlashDis_);
extern void LED_P14x16Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_P16x16Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_P24x24Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_PXx16MixStr(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char ucDataStr[]);
extern void LED_Fill(unsigned char ucData);
extern void BMP(unsigned char p[]);
extern void LED_PrintChar(unsigned char ucIdxX, unsigned char ucIdxY, char cData);
extern void LED_PrintUchar(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char cData);
extern void LED_PrintUint(unsigned char ucIdxX, unsigned char ucIdxY, unsigned int cData);
extern void LED_PrintShort(unsigned char ucIdxX, unsigned char ucIdxY, int sData);
extern void LED_PrintImage(unsigned char *pucTable, unsigned int usRowNum, unsigned int usColumnNum);
extern void Image(unsigned char Page,unsigned char p[]);
extern void Image_LOGO(unsigned char X,unsigned char Y,unsigned char p[]);

#endif