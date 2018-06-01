/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			main.c
** Last modified Date:  2004-09-16
** Last Version:		1.0
** Descriptions:		The main() function example template
**
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-09-16
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#include "config.h"

/* 定义串口模式设置数据结构 */
typedef  struct  UartMode
{  uint8 datab;         // 字长度，5/6/7/8
   uint8 stopb;         // 停止位，1/2
   uint8 parity;    	// 奇偶校验位，0为无校验，1奇数校验，2为偶数校验
}  UARTMODE;


/****************************************************************************
* 名称：UART0_Init()
* 功能：初始化串口0。设置其工作模式及波特率。
* 入口参数：baud                波特率
*          set          模式设置(UARTMODE数据结构)
* 出口参数：返回值为1时表示初始化成功，为0表示参数出错
****************************************************************************/
uint8  UART0_Init(uint32 baud, UARTMODE set)
{  uint32  bak;
   
   /* 参数过滤 */
   if( (0==baud)||(baud>115200) ) return(0);
   if( (set.datab<5)||(set.datab>8) ) return(0);
   if( (0==set.stopb)||(set.stopb>2) ) return(0);
   if( set.parity>4 ) return(0);

   /* 设置串口波特率 */
   U0LCR = 0x80;                        // DLAB位置1
   bak = (Fpclk>>4)/baud;
   U0DLM = bak>>8;
   U0DLL = bak&0xff;
   
   /* 设置串口模式 */
   bak = set.datab-5;                   // 设置字长度
   if(2==set.stopb) bak |= 0x04;        // 判断是否为2位停止位  
   
   if(0!=set.parity) {set.parity = set.parity-1; bak |= 0x08;}
   bak |= set.parity<<4;              	// 设置奇偶校验
      
   U0LCR = bak;
   
   return(1);
}


/****************************************************************************
* 名称：SendByte()
* 功能：向串口发送字节数据，并等待发送完毕。
* 入口参数：data                要发送的数据
* 出口参数：无
****************************************************************************/
void  SendByte(uint8 data)
{  U0THR = data;                        // 发送数据

   while( (U0LSR&0x20)==0 );            // 等待数据发送
}



/****************************************************************************
* 名称：PC_DispChar()
* 功能：向PC机发送显示字符。
* 入口参数：no       显示位置 
*          chr      显示的字符，不能为0xff
* 出口参数：无
****************************************************************************/
void  PC_DispChar(uint8 no, uint8 chr)
{  SendByte(0xff);
   SendByte(0x81);
   SendByte(no);
   SendByte(chr);
   SendByte(0x00);
}


uint8  const SHOWTABLE[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
/****************************************************************************
* 名称：SendTimeRtc()
* 功能：读取RTC的时间值，并将读出的时分秒值由串口发送到上位机显示。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  SendTimeRtc(void)
{  uint32  datas;
   uint32  times;
   uint32  bak;
    
   times = CTIME0;			// 读取完整时钟寄存器
   datas = CTIME1;
   
   bak = (datas>>16)&0xFFF;     // 取得年值
   PC_DispChar(0, SHOWTABLE[bak/1000]);
   bak = bak%1000;
   PC_DispChar(1, SHOWTABLE[bak/100]);
   bak = bak%100;
   PC_DispChar(2, SHOWTABLE[bak/10]);
   PC_DispChar(3, SHOWTABLE[bak%10]);
  
   bak = (datas>>8)&0x0F;       // 取得月值
   PC_DispChar(4, SHOWTABLE[bak/10]);
   PC_DispChar(5, SHOWTABLE[bak%10]);
   
   bak = datas&0x1F;            // 取得日值
   PC_DispChar(6, SHOWTABLE[bak/10]);
   PC_DispChar(7, SHOWTABLE[bak%10]);
   
   bak = (times>>24)&0x07;      // 取得星期值
   PC_DispChar(8, SHOWTABLE[bak]);
   
   
   bak = (times>>16)&0x1F;		// 取得时的值
   PC_DispChar(9, SHOWTABLE[bak/10]);
   PC_DispChar(10, SHOWTABLE[bak%10]);
   
   bak = (times>>8)&0x3F;		// 取得分的值
   PC_DispChar(11, SHOWTABLE[bak/10]);
   PC_DispChar(12, SHOWTABLE[bak%10]);
   
   bak = times&0x3F;			// 取得秒的值
   PC_DispChar(13, SHOWTABLE[bak/10]);
   PC_DispChar(14, SHOWTABLE[bak%10]);
}




/****************************************************************************
* 名称：RTCIni()
* 功能：初始化实时时钟。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  RTCInit(void)
{  PREINT = Fpclk / 32768 - 1;	// 设置基准时钟分频器
   PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
   
   YEAR = 2006;				    // 初始化年
   MONTH = 8;				    // 初始化月
   DOM = 14;				    // 初始化日
   DOW = 1;                     // 初始化对应的星期几
   HOUR = 20;
   MIN = 30;
   SEC = 10;
   
   
   CIIR = 0x01;				    // 设置秒值的增量产生一次中断
   
   CCR = 0x01;				    // 启动RTC
}


/****************************************************************************
* 名称：main()
* 功能：运行RTC进行计时，并将所时间值不断的通过串口向上位机发送。上位机使用EasyARM
*      软件，在仿真的万年历显示器上观察结果。
*      通讯波特率115200，8位数据位，1位停止位，无奇偶校验。  
****************************************************************************/
int  main(void)
{  UARTMODE  uart0_set;   

   PINSEL0 = 0x00000005;            // 设置I/O连接到UART0
   PINSEL1 = 0x00000000;              
     
   uart0_set.datab = 8;             // 8位数据位
   uart0_set.stopb = 1;             // 1位停止位
   uart0_set.parity = 0;            // 无奇偶校验
   UART0_Init(115200, uart0_set);    // 初始化串口模式
   U0FCR = 0x01;                    // 使能FIFO
   
   
   RTCInit();					    // 初始化RTC
   while(1)	
   {  while( 0==(ILR&0x01) );       // 等待RTC增量中断标志
      ILR = 0x01;				    // 清除中断标志
      SendTimeRtc();                // 读取时钟值,并向UART0发送
      
      
      while( 0==(ILR&0x01) );
      ILR = 0x01;
      SendTimeRtc();
   }
   
   return(0);
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
