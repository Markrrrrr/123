#ifndef __TSM12_H
#define __TSM12_H
#include<iostm8l152R8.h>  /*  头文件 */

//IO方向设置
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int

#define SDA_IN()  {PE_DDR_bit.DDR6=0; PE_CR1_bit.C16=1;PE_CR2_bit.C26=0;}
#define SDA_OUT() {PE_DDR_bit.DDR6=1; PE_CR1_bit.C16=1;PE_CR2_bit.C26=1;}

//IO操作函数	 
#define TSM12_IIC_SCL    PE_ODR_bit.ODR1 //SCL
#define TSM12_IIC_SDA    PE_ODR_bit.ODR0 //SDA
#define TSN12_READ_SDA   PE_IDR_bit.IDR0==1  //输入SDA 
#define TSM12_TSM_EN     PG_ODR_bit.ODR7
//#define TSM_RESET  PD_ODR_bit.ODR0


#define DEVICE_WRITE_ADDR 0xD0
#define DEVICE_READ_ADDR 0xD1
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define CH_SENS(a,b) 0x##a##b

/* TSM12M Register Map */
typedef enum{
SENS1 = 0x02, //Sensitivity, Channel 2 and 1
SENS2 = 0x03, //Channel 4 and 3
SENS3 = 0x04, //Channel 6 and 5
SENS4 = 0x05, //Channel 8 and 7
SENS5 = 0x06, //Channel 10 and 9
SENS6 = 0x07, //Channel 12 and 11
CTRL1 = 0x08,
CTRL2 = 0x09,
REF_RST1 = 0x0A,
REF_RST2 = 0x0B,
CH_HOLD1 = 0x0C,
CH_HOLD2 = 0x0D,
CAL_HOLD1 = 0x0E,
CAL_HOLD2 = 0x0F,
OUTPUT1 = 0x10,
OUTPUT2 = 0x11,
OUTPUT3 = 0x12,
}TSM_RegMapAddr_TypeDef;




void TSM12_Init(void);
void TSM12_Into_Sleep_Mode(void);
void TSM12_Exit_Sleep_Mode(void);
u8 TSM12_READ_KEY(u8 *Temp);

void TSM12_GET_Key(void);


//IIC所有操作函数
void TSM12_IIC_Init(void);                //初始化IIC的IO口				 
void TSM12_IIC_Start(void);				//发送IIC开始信号
void TSM12_IIC_Stop(void);	  			//发送IIC停止信号
void TSM12_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 TSM12_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 TSM12_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void TSM12_IIC_Ack(void);					//IIC发送ACK信号
void TSM12_IIC_NAck(void);				//IIC不发送ACK信号
void TSM12_IIC_Delay(unsigned char dat);

u8 TSM12_Write_Byte(u8 WriteAddr,u8 WriteData);
u8 TSM12_TSM_Write_2Byte(u8 WriteAddr, u8 WriteData1, u8 WriteData2);
u8 TSM12_TSM_ReadOneByte(u8 writeAddr);

#endif
















