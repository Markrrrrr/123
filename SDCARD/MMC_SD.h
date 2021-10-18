//移植说明：采用硬件SPI模式读写SD卡，SPI1
//SD_DT3--SD_CS------------PB4
//SD_CMD--SPI1_MOSI--------PB6
//SD_SCK--SPI1_SCK---------PB5
//SD_DT0--SPI1_MISO--------PB7
//
//
//
//
#ifndef _MMC_SD_H_
#define _MMC_SD_H_		 

#include<iostm8l152R8.h>  /*  头文件 */
#include "spi.h"

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int


#define SPI_BaudRatePrescaler_2 2
#define SPI_BaudRatePrescaler_256 255
 								    	 
// SD卡类型定义  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
//这部分应根据具体的连线来修改!
#define  SD_CS_H PB_ODR_bit.ODR4=1;  //SD卡片选引脚	PB4				    	  
#define  SD_CS_L PB_ODR_bit.ODR4=0;

extern unsigned char  SD_Type;//SD卡的类型
//函数申明区 
void SD_SPI_Init(void);
unsigned char SD_SPI_ReadWriteByte(unsigned char data);
unsigned char SD_WaitReady(void);							//等待SD卡准备
unsigned char SD_GetResponse(unsigned char Response);					//获得相应
unsigned char SD_Initialize(void);							//初始化
unsigned char SD_ReadDisk(unsigned char*buf,u32 sector,unsigned char cnt);		//读块
unsigned char SD_WriteDisk(unsigned char*buf,u32 sector,unsigned char cnt);		//写块
u32 SD_GetSectorCount(void);   					//读扇区数
unsigned char SD_GetCID(unsigned char *cid_data);                     //读SD卡CID
unsigned char SD_GetCSD(unsigned char *csd_data);                     //读SD卡CSD
 
#endif




