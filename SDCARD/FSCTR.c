#include<iostm8l152R8.h>
#include "spi.h"
#include "mmc_sd.h"
#include "ff.h"  

#include "OLED.h"


FATFS file;
FIL fil;
UINT w;
unsigned char bufftest[]={"�Ѿ���ʽ�´������Ϊ�����ڽ�һ����ǿ���������ۺ�Ƶ����Ŀ�����֪ͨ��"};
unsigned char res;


void FSCTR(void)
{
  f_mount(&file,"0:",1);
  res = f_open(&fil,"�Ӣ.txt",FA_CREATE_ALWAYS|FA_WRITE);
  if(res == FR_OK)
  {
    do
    {
      res = f_write(&fil,bufftest,sizeof(bufftest),&w);
      res = f_write(&fil,bufftest,sizeof(bufftest),&w);
      res = f_write(&fil,bufftest,sizeof(bufftest),&w);
    }
    while(0);
  }
  f_close(&fil);
 
  LED_P8x16Char(0,3,'O');
  LED_P8x16Char(8,3,'K');
  LED_P8x16Char(50,3,'O');
  LED_P8x16Char(58,3,'K');
}