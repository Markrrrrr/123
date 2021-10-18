#include "TSM12.h"

extern void delay(unsigned int ms);

unsigned char TSM12_Value[3];
unsigned char Key_Value;

/*******************************************************************************
* �� ��: TSM12_Init()
* �� ��: TSM12M ���ܳ�ʼ��
* ��ڲ��� : ��
* ���ڲ��� : ��
* ˵ ��:
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
  /* Sensitivity setting ,0x00 ��������ߣ� */
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
* �� ��: TSM12_Into_Sleep_Mode()
* �� ��: TSM12M ����͹���ģʽ
* ��ڲ��� : ��
* ���ڲ��� : ��
* ˵ ��:
*******************************************************************************/
void TSM12_Into_Sleep_Mode(void)
{
  TSM12_TSM_EN=1;// ���֮ǰ�Ĳ����� TSM_EN_L �ز�����
  delay(100);
  TSM12_TSM_EN=0;//i2c enable
  delay(100);
  TSM12_Write_Byte(CTRL2, 0x07);
  TSM12_TSM_EN=1;
}

/*******************************************************************************
* �� ��: TSM12_Exit_Sleep_Mode()
* �� ��: TSM12M �˳��͹���ģʽ
* ��ڲ��� : ��
* ���ڲ��� : ��
* ˵ ��:
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

//��ʼ��IIC
void TSM12_IIC_Init(void)
{
  PE_DDR_bit.DDR0=1; PE_CR1_bit.C10=1;PE_CR2_bit.C20=1;//SDA
  PE_DDR_bit.DDR1=1; PE_CR1_bit.C11=1;PE_CR2_bit.C21=1;//SCL
  //PE_DDR_bit.DDR2=0; PE_CR1_bit.C12=1;PE_CR2_bit.C22=1;//IRQ
  PG_DDR_bit.DDR7=1; PG_CR1_bit.C17=1;PG_CR2_bit.C27=1;//I2C_EN
  //PD_DDR_bit.DDR0=1; PD_CR1_bit.C10=1;PD_CR2_bit.C20=1;//RESET
}
//����IIC��ʼ�ź�
void TSM12_IIC_Start(void)
{
  SDA_OUT();//sda�����
  TSM12_IIC_SDA=1;	  	  
  TSM12_IIC_SCL=1;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SDA=0;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void TSM12_IIC_Stop(void)
{
  SDA_OUT();//sda�����
  TSM12_IIC_SCL=0;
  TSM12_IIC_SDA=0;
  TSM12_IIC_Delay(2);
  TSM12_IIC_SCL=1; 
  TSM12_IIC_Delay(2);
  TSM12_IIC_SDA=1;//����I2C���߽����ź�
  TSM12_IIC_Delay(2);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 TSM12_IIC_Wait_Ack(void)
{
  u8 ucErrTime=0;
  SDA_OUT();
  TSM12_IIC_SDA=1;TSM12_IIC_Delay(2);	   
  TSM12_IIC_SCL=1;TSM12_IIC_Delay(2);	 
  SDA_IN();      //SDA����Ϊ����  
  while(TSN12_READ_SDA)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      TSM12_IIC_Stop();
      return 1;
    }
  }
  TSM12_IIC_SCL=0;//ʱ�����0 	   
  return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void TSM12_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
  SDA_OUT(); 	    
  TSM12_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
    TSM12_IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
    TSM12_IIC_Delay(2);   //��TEA5767��������ʱ���Ǳ����
    TSM12_IIC_SCL=1;
    TSM12_IIC_Delay(2); 
    TSM12_IIC_SCL=0;	
    TSM12_IIC_Delay(2);
  }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  SDA_IN();//SDA����Ϊ����
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
    TSM12_IIC_NAck();//����nACK
  else
    TSM12_IIC_Ack(); //����ACK   
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
���ܣ��� TSM12 д���ֽ����ݻ�����
���룺 writeData-- ��д���ݣ� WriteAddr-- ��д�����ݼĴ�����ַ
���أ� ErrorStatus
------------------------------------------------------------------------------*/
u8 TSM12_Write_Byte(u8 WriteAddr,u8 WriteData)
{
  /* ������ʼλ */
  TSM12_IIC_Start();//---------------------------OK
  /* ����������ַ */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);//------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//0X09---OK
  /* ���ʹ�д�����ݵļĴ�����ַ */
  TSM12_IIC_Send_Byte(WriteAddr);//0X0 ----------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//-------OK
  /* �������ݵ��Ĵ����� */
  TSM12_IIC_Send_Byte(WriteData);//--------------OK
  if(TSM12_IIC_Wait_Ack()) return ERROR;//-------OK
  /* ����ֹͣλ */
  TSM12_IIC_Stop();//----------------------------OK
  return SUCCESS;
}

/* ----------------------------------------------------------------------------
���ܣ��� GT216L д˫�ֽ����ݻ�����
���룺 writeData1 ��2--��д���ݣ� WriteAddr-- ��д�����ݼĴ�����ַ
���أ� ErrorStatus
------------------------------------------------------------------------------*/
u8 TSM12_TSM_Write_2Byte(u8 WriteAddr, u8 WriteData1, u8 WriteData2)
{
  /* ������ʼλ */
  TSM12_IIC_Start();
  /* ����������ַ */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* ���ʹ�д�����ݵļĴ�����ʼ��ַ */
  TSM12_IIC_Send_Byte(WriteAddr);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* �������� 1 ���Ĵ����У��� LSB */
  TSM12_IIC_Send_Byte(WriteData1);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* �������� 2 ���Ĵ����У��� MSB */
  TSM12_IIC_Send_Byte(WriteData2);
  if(TSM12_IIC_Wait_Ack()) return ERROR;
  /* ����ֹͣλ */
  TSM12_IIC_Stop();
  return SUCCESS;
}

/*******************************************************************************
* �� ��: TSM_ReadOneByte()
* �� ��: �������ֽ�����
* ��ڲ��� : writeAddr-- ��д�����ݼĴ�����ַ
* ���ڲ��� : ���������� , ERROR
* ˵ ��:
*******************************************************************************/
u8 TSM12_IIC_Read_Byte(u8 writeAddr)
{
  unsigned char buffer = 0;
  // ��һ�׶�
  /* ������ʼλ */
  TSM12_IIC_Start();//------------------OK
  /* ����������ַ */
  TSM12_IIC_Send_Byte(DEVICE_WRITE_ADDR);//0xd0-------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* ���ͱ������Ĵ����׵�ַ */
  TSM12_IIC_Send_Byte(writeAddr);//0X10---------------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* ����ֹͣλ */
  TSM12_IIC_Stop();//---------------------------------------OK
  // �ڶ��׶�
  /* ������ʼλ */
  TSM12_IIC_Start();//--------------------------------------OK
  /* ���� GT216L ������ַ */
  TSM12_IIC_Send_Byte(DEVICE_READ_ADDR);//0XD1--------------OK
  if(TSM12_IIC_Wait_Ack()) goto L_OUT;//--------------------OK
  /* ����һ��Ӧ��Ӧ���ź� */
  buffer = IIC_Read_Byte(0);//------------------------OK
  //I2C_SendAck(1);
  /* ����ֹͣλ */
  TSM12_IIC_Stop();//---------------------------------------
  return buffer;
  L_OUT:
  /* ����ֹͣλ */
  TSM12_IIC_Stop();
  return ERROR;
}





