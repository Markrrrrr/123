#ifndef __I2CDs1307_H__
#define __I2CDs1307_H__
//#include<iostm8l152c6.h>

#define uchar unsigned char

#define SCLK     PC_ODR_bit.ODR7
#define DSIO_OUT PC_ODR_bit.ODR6
#define DSIO_IN  PC_IDR_bit.IDR6
#define RST      PC_ODR_bit.ODR5

#define SET_OUT() {PC_DDR_bit.DDR6=1; PC_CR1_bit.C16=1;PC_CR2_bit.C26=0;}
#define SET_IN()  {PC_DDR_bit.DDR6=0; PC_CR1_bit.C16=1;PC_CR2_bit.C26=0;}




void Close_DS1302();
void Ds1302Write(uchar addr, uchar dat);
uchar Ds1302Read(uchar addr);
void Ds1302Init();
void Set_1307Time();
void Ds1302ReadTime();


#endif