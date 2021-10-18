/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "MMC_SD.h"
//#include "RTC.h"

#define SD_CARD	 0  //SD��,���Ϊ0

//��ʼ������
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
  u8 res=0;
  switch(pdrv)
  {
  case SD_CARD://SD��
    res=SD_Initialize();//SD����ʼ�� 
    break;
  default:
    res=1;
    //break;
  }
  if(res)return  STA_NOINIT;
  else return 0; //��ʼ���ɹ�
}

//��ô���״̬
/* 
//Tinv��FatFs ֻ֧��һ���洢ý�飬���� drv Ӧ��Ϊ O��
//ִ�����󷵻� 0�����󷵻ط� O��
*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{ 
	return 0;
} 

//������
//drv:���̱��0~9
//*buff:���ݽ��ջ����׵�ַ
//sector:������ַ
//count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
  u8 res=0;
  if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
  switch(pdrv)
  {
    case SD_CARD://SD��
      res=SD_ReadDisk(buff,sector,count);
      while(res)//������
      {
        SD_Initialize();	//���³�ʼ��SD��
        res=SD_ReadDisk(buff,sector,count);	
        //printf("sd rd error:%d\r\n",res);
      }
      break;
  default:
    res=1; 
  }
  //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
  if(res==0x00)return RES_OK;	 
  else return RES_ERROR;	   
}

//д����
//drv:���̱��0~9
//*buff:���������׵�ַ
//sector:������ַ
//count:��Ҫд���������
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
  u8 res=0;
  if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
  switch(pdrv)
  {
    case SD_CARD://SD��
      res=SD_WriteDisk((u8*)buff,sector,count);
      while(res)//д����
      {
        SD_Initialize();//���³�ʼ��SD��
        res=SD_WriteDisk((u8*)buff,sector,count);	
        //printf("sd wr error:%d\r\n",res);
      }
      break;
  default:
    res=1; 
  }
  //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
  if(res == 0x00)return RES_OK;	 
  else return RES_ERROR;	
}
#endif


//����������Ļ��
 //drv:���̱��0~9
 //ctrl:���ƴ���
 //*buff:����/���ջ�����ָ��
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res;
  if(pdrv==SD_CARD)//SD��
  {
    switch(cmd)
    {
    case CTRL_SYNC:
      res = RES_OK;
      break;
    case GET_SECTOR_SIZE:    //������С
      *(DWORD*)buff = 512;
      res = RES_OK;
      break;
    case GET_BLOCK_SIZE:   //����������Ϊ��λ�Ĵ洢���еĲ������С
      *(WORD*)buff = 8;
      res = RES_OK;
      break;
    case GET_SECTOR_COUNT:  //������
      *(DWORD*)buff = SD_GetSectorCount();
      res = RES_OK;
      break;
    default:
      res = RES_PARERR;
      break;
    }
  }
  else res=RES_ERROR;//�����Ĳ�֧��
  return res;
}
#endif
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	//return (((u32)(RTC_DateStr.RTC_Year-(RTC_DateStr.RTC_Year>>4)*6)+20) << 25) /* Year */
        //      | ((u32)(RTC_DateStr.RTC_Month-(RTC_DateStr.RTC_Month>>4)*6) << 21)      /* Month*/
        //      | ((u32)(RTC_DateStr.RTC_Date-(RTC_DateStr.RTC_Date>>4)*6) << 16)       /* Day*/
        //      | ((u32)(RTC_TimeStr.RTC_Hours-(RTC_TimeStr.RTC_Hours>>4)*6) << 11)      /* Hour*/
        //      | ((u32)(RTC_TimeStr.RTC_Minutes-(RTC_TimeStr.RTC_Minutes>>4)*6) << 5)     /* Min*/
        //      | ((u32)(RTC_TimeStr.RTC_Seconds-(RTC_TimeStr.RTC_Seconds>>4)*6) >> 1)     /* Sec*/
//;
  return 0;
}			 
