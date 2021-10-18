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

#define SD_CARD	 0  //SD卡,卷标为0

//初始化磁盘
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
  u8 res=0;
  switch(pdrv)
  {
  case SD_CARD://SD卡
    res=SD_Initialize();//SD卡初始化 
    break;
  default:
    res=1;
    //break;
  }
  if(res)return  STA_NOINIT;
  else return 0; //初始化成功
}

//获得磁盘状态
/* 
//Tinv—FatFs 只支持一个存储媒介，所以 drv 应恒为 O。
//执行无误返回 0，错误返回非 O。
*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{ 
	return 0;
} 

//读扇区
//drv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
  u8 res=0;
  if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
  switch(pdrv)
  {
    case SD_CARD://SD卡
      res=SD_ReadDisk(buff,sector,count);
      while(res)//读出错
      {
        SD_Initialize();	//重新初始化SD卡
        res=SD_ReadDisk(buff,sector,count);	
        //printf("sd rd error:%d\r\n",res);
      }
      break;
  default:
    res=1; 
  }
  //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
  if(res==0x00)return RES_OK;	 
  else return RES_ERROR;	   
}

//写扇区
//drv:磁盘编号0~9
//*buff:发送数据首地址
//sector:扇区地址
//count:需要写入的扇区数
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
  u8 res=0;
  if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
  switch(pdrv)
  {
    case SD_CARD://SD卡
      res=SD_WriteDisk((u8*)buff,sector,count);
      while(res)//写出错
      {
        SD_Initialize();//重新初始化SD卡
        res=SD_WriteDisk((u8*)buff,sector,count);	
        //printf("sd wr error:%d\r\n",res);
      }
      break;
  default:
    res=1; 
  }
  //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
  if(res == 0x00)return RES_OK;	 
  else return RES_ERROR;	
}
#endif


//其他表参数的获得
 //drv:磁盘编号0~9
 //ctrl:控制代码
 //*buff:发送/接收缓冲区指针
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res;
  if(pdrv==SD_CARD)//SD卡
  {
    switch(cmd)
    {
    case CTRL_SYNC:
      res = RES_OK;
      break;
    case GET_SECTOR_SIZE:    //扇区大小
      *(DWORD*)buff = 512;
      res = RES_OK;
      break;
    case GET_BLOCK_SIZE:   //返回以扇区为单位的存储阵列的擦除块大小
      *(WORD*)buff = 8;
      res = RES_OK;
      break;
    case GET_SECTOR_COUNT:  //扇区数
      *(DWORD*)buff = SD_GetSectorCount();
      res = RES_OK;
      break;
    default:
      res = RES_PARERR;
      break;
    }
  }
  else res=RES_ERROR;//其他的不支持
  return res;
}
#endif
//获得时间
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
