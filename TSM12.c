#include "TSM12.h"

extern void delay(unsigned int ms);

unsigned char TSM12_Value[3];
unsigned char Key_Value;

/*******************************************************************************
* 名 称: TSM12_Init()
* 功 能: TSM12M 功能初始化
* 入口参数 : 无
* 出口参数 : 无
* 说 明:
*******************************************************************************/
void TSM12_Init(void)
{
  TSM12_IIC_Init();
  /* Power ON */
  TSM12_TSM_EN=0;
  delay(500);
  /* H/W Reset, enable software reset */
  TSM12_Write_Byte(CTRL2, 0x0F);//enable sleep mode
  delay(200);
  /* Sensitivity setting ,0x00 灵敏度最高， */
  TSM12_Write_Byte(SENS1, CH_SENS(7,7));// 9, 6
  TSM12_Write_Byte(SENS2, CH_SENS(7,7));// 3, #
  TSM12_Write_Byte(SENS3, CH_SENS(7,7));// 8, 5
  TSM12_Write_Byte(SENS4, CH_SENS(7,7));// 2, 0
  TSM12_Write_Byte(SENS5, CH_SENS(7,7));// *, 1
  TSM12_Write_Byte(SENS6, CH_SENS(7,7));// 4, 7
  /* General Control Register1 setting */
  TSM12_Write_Byte(CTRL1, 0x33);
  TSM12_TSM_Write_2Byte(REF_RST1, 0x00, 0x00);//renew value of everyone channel
  TSM12_TSM_Write_2Byte(CH_HOLD1, 0x00, 0x00);//enable all channels
  //TSM_EN=1;//TSM12 I2C disable
  TSM12_Write_Byte(CTRL2, 0x03);
  //reset off
  TSM12_Exit_Sleep_Mode();
}

/*******************************************************************************
* 名 称: TSM12_Into_Sleep_Mode()
* 功 能: TSM12M 进入低功耗模式
* 入口参数 : 无
* 出口参数 : 无
* 说 明:
*******************************************************************************/
void TSM12_Into_Sleep_Mode(void)
{
  TSM12_TSM_EN=1;// 如果之前的操作过 TSM_EN_L 必不可少
  delay(100);
  TSM12_TSM_EN=0;//i2c enable
  delay(100);
  TSM12_Write_Byte(CTRL2, 0x07);
  TSM12_TSM_EN=1;
}

/*******************************************************************************
* 名 称: TSM12_Exit_Sleep_Mode()
* 功 能: TSM12M 退出低功耗模式
* 入口参数 : 无
* 出口参数 : 无
* 说 明:
*******************************************************************************/
void TSM12_Exit_Sleep_Mode(void)
{
  TSM12_TSM_EN=0;//i2c enable
  delay(100);
  TSM12_Write_Byte(CTRL2, 0x03);
}

u8 TSM12_READ_KEY(u8 *Temp)
{
  //*Temp=TSM_ReadOneByte(CTRL1);
  *Temp=TSM12_IIC_Read_Byte(OUTPUT1);
  *(Temp+1)=TSM12_IIC_Read_Byte(OUTPUT2);
  *(Temp+2)=TSM12_IIC_Read_Byte(OUTPUT3);
  return 1;
}

void TSM12_GET_Key(void)
{
  TSM12_READ_KEY(TSM12_Value);
  if(TSM12_Value[2]==0x03) Key_Value=0x01;
  else if(TSM12_Value[1]==0xc0) Key_Value=0x02;
  else if(TSM12_Value[0]==0xc0) Key_Value=0x03;
  else if(TSM12_Value[2]==0x0c) Key_Value=0x04;
  else if(TSM12_Value[1]==0x30) Key_Value=0x05;
  else if(TSM12_Value[0]==0x30) Key_Value=0x06;
  else if(TSM12_Value[2]==0x30) Key_Value=0x07;
  else if(TSM12_Value[1]==0x0c) Key_Value=0x08;
  else if(TSM12_Value[0]==0x0c) Key_Value=0x09;
  else if(TSM12_Value[2]==0xc0) Key_Value=0x0a;
  else if(TSM12_Value[1]==0x03) Key_Value=0x00;
  else if(TSM12_Value[0]==0x03) Key_Value=0x0b;
  
}

//初始化IIC
void TSM12_IIC_Init(void)
{
  PE_DDR_bit.DDR0=1; PE_CR1_bit.C10=1;PE_CR2_bit.C20=1;//SDA
  PE_DDR_bit.DDR1=1; PE_CR1_bit.C11=1;PE_CR2_bit.C21=1;//SCL
  //PE_DDR_bit.DDR2=0; PE_CR1_bit.C12=1;PE_CR2_bit.C22=1;//IRQ
  PG_DDR_bit.DDR7=1; PG_CR1_bit.C17=1;PG_CR2_bit.C27=1;//I2C_EN
  //PD_DDR_bit.DDR0=1; PD_CR1_bit.C10=1;PD_CR2_bit.C20=1;//RESET
}
//产生IIC起始信号
void TSM12_IIC_Start(void)
{
  SDA_OUT();//sda线输出
  TSM12_IIC_SDA=1;	  	  
  TSM12_IIC_SCL=1;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SDA=0;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void TSM12_IIC_Stop(void)
{
  SDA_OUT();//sda线输出
  TSM12_IIC_SCL=0;
  TSM12_IIC_SDA=0;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=1; 
  TSM12_IIC_Delay(2);
  TSM12_IIC_SDA=1;//发送I2C总线结束信号
  TSM12_IIC_Delay(2);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 TSM12_IIC_Wait_Ack(void)
{
  u8 ucErrTime=0;
  SDA_OUT();
  TSM12_IIC_SDA=1;TSM12_IIC_Delay(2);	   
  TSM12_IIC_SCL=1;TSM12_IIC_Delay(2);	 
  SDA_IN();      //SDA设置为输入  
  while(TSN12_READ_SDA)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      TSM12_IIC_Stop();
      return 1;
    }
  }
  TSM12_IIC_SCL=0;//时钟输出0 	   
  return 0;  
} 
//产生ACK应答
void TSM12_IIC_Ack(void)
{
  TSM12_IIC_SCL=0;
  SDA_OUT();
  TSM12_IIC_SDA=0;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=1;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=0;
}
//不产生ACK应答		    
void TSM12_IIC_NAck(void)
{
  TSM12_IIC_SCL=0;
  SDA_OUT();
  TSM12_IIC_SDA=1;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=1;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void TSM12_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
  SDA_OUT(); 	    
  TSM12_IIC_SCL=0;//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    TSM12_IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
    TSM12_IIC_Delay(2);   //对TEA5767这三个延时都是必须的
    TSM12_IIC_SCL=1;
    TSM12_IIC_Delay(2); 
    TSM12_IIC_SCL=0;	
    TSM12_IIC_Delay(2);
  }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
  {
    TSM12_IIC_SCL=0; 
    TSM12_IIC_Delay(2);
    TSM12_IIC_SCL=1;
    receive<<=1;
    if(TSN12_READ_SDA)receive++;   
    TSM12_IIC_Delay(2); 
  }
  if (!ack)
    TSM12_IIC_NAck();//发送nACK
  else
    TSM12_IIC_Ack(); //发送ACK   
  return receive;
}


void TSM12_IIC_Delay(unsigned char dat)
{
  for(u8 i=0; i<dat; i++)
  {
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
  }
}

/*-----------------------------------------------------------------------------
功能：向 TSM12 写单字节数据或命令
输入： writeData-- 被写数据， WriteAddr-- 待写入数据寄存器地址
返回： ErrorStatus
------------------------------------------------------------------------------*/
u8 TSM12_Write_Byte(u8 WriteAddr,u8 WriteData)
{
  /* 发送起始位 */
  TSM12_IIC_Start();//---------------------------OK
  /* 发送器件地址 */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);//------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//0X09---OK
  /* 发送待写入数据的寄存器地址 */
  TSM12_IIC_Send_Byte(WriteAddr);//0X0 ----------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//-------OK
  /* 发送数据到寄存器中 */
  TSM12_IIC_Send_Byte(WriteData);//--------------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//-------OK
  /* 产生停止位 */
  TSM12_IIC_Stop();//----------------------------OK
  return SUCCESS;
}

/* ----------------------------------------------------------------------------
功能：向 GT216L 写双字节数据或命令
输入： writeData1 ，2--被写数据， WriteAddr-- 待写入数据寄存器地址
返回： ErrorStatus
------------------------------------------------------------------------------*/
u8 TSM12_TSM_Write_2Byte(u8 WriteAddr, u8 WriteData1, u8 WriteData2)
{
  /* 发送起始位 */
  TSM12_IIC_Start();
  /* 发送器件地址 */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* 发送待写入数据的寄存器起始地址 */
  TSM12_IIC_Send_Byte(WriteAddr);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* 发送数据 1 到寄存器中，即 LSB */
  TSM12_IIC_Send_Byte(WriteData1);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* 发送数据 2 到寄存器中，即 MSB */
  TSM12_IIC_Send_Byte(WriteData2);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* 产生停止位 */
  TSM12_IIC_Stop();
  return SUCCESS;
}

/*******************************************************************************
* 名 称: TSM_ReadOneByte()
* 功 能: 读单个字节数据
* 入口参数 : writeAddr-- 待写入数据寄存器地址
* 出口参数 : 被读出数据 , ERROR
* 说 明:
*******************************************************************************/
u8 TSM12_IIC_Read_Byte(u8 writeAddr)
{
  unsigned char buffer = 0;
  // 第一阶段
  /* 发送起始位 */
  TSM12_IIC_Start();//------------------OK
  /* 发送器件地址 */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);//0xd0-------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* 发送被操作寄存器首地址 */
  TSM12_IIC_Send_Byte(writeAddr);//0X10---------------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* 产生停止位 */
  TSM12_IIC_Stop();//---------------------------------------OK
  // 第二阶段
  /* 发送起始位 */
  TSM12_IIC_Start();//--------------------------------------OK
  /* 发送 GT216L 器件地址 */
  TSM12_IIC_Send_Byte(DEVICE_READ_ADDR);//0XD1--------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* 产生一个应该应答信号 */
  buffer = IIC_Read_Byte(0);//------------------------OK
  //I2C_SendAck(1);
  /* 产生停止位 */
  TSM12_IIC_Stop();//---------------------------------------
  return buffer;
  L_OUT:
  /* 产生停止位 */
  TSM12_IIC_Stop();
  return ERROR;
}





