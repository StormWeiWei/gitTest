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

/* ���崮��ģʽ�������ݽṹ */
typedef  struct  UartMode
{  uint8 datab;         // �ֳ��ȣ�5/6/7/8
   uint8 stopb;         // ֹͣλ��1/2
   uint8 parity;    	// ��żУ��λ��0Ϊ��У�飬1����У�飬2Ϊż��У��
}  UARTMODE;


/****************************************************************************
* ���ƣ�UART0_Init()
* ���ܣ���ʼ������0�������乤��ģʽ�������ʡ�
* ��ڲ�����baud                ������
*          set          ģʽ����(UARTMODE���ݽṹ)
* ���ڲ���������ֵΪ1ʱ��ʾ��ʼ���ɹ���Ϊ0��ʾ��������
****************************************************************************/
uint8  UART0_Init(uint32 baud, UARTMODE set)
{  uint32  bak;
   
   /* �������� */
   if( (0==baud)||(baud>115200) ) return(0);
   if( (set.datab<5)||(set.datab>8) ) return(0);
   if( (0==set.stopb)||(set.stopb>2) ) return(0);
   if( set.parity>4 ) return(0);

   /* ���ô��ڲ����� */
   U0LCR = 0x80;                        // DLABλ��1
   bak = (Fpclk>>4)/baud;
   U0DLM = bak>>8;
   U0DLL = bak&0xff;
   
   /* ���ô���ģʽ */
   bak = set.datab-5;                   // �����ֳ���
   if(2==set.stopb) bak |= 0x04;        // �ж��Ƿ�Ϊ2λֹͣλ  
   
   if(0!=set.parity) {set.parity = set.parity-1; bak |= 0x08;}
   bak |= set.parity<<4;              	// ������żУ��
      
   U0LCR = bak;
   
   return(1);
}


/****************************************************************************
* ���ƣ�SendByte()
* ���ܣ��򴮿ڷ����ֽ����ݣ����ȴ�������ϡ�
* ��ڲ�����data                Ҫ���͵�����
* ���ڲ�������
****************************************************************************/
void  SendByte(uint8 data)
{  U0THR = data;                        // ��������

   while( (U0LSR&0x20)==0 );            // �ȴ����ݷ���
}



/****************************************************************************
* ���ƣ�PC_DispChar()
* ���ܣ���PC��������ʾ�ַ���
* ��ڲ�����no       ��ʾλ�� 
*          chr      ��ʾ���ַ�������Ϊ0xff
* ���ڲ�������
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
* ���ƣ�SendTimeRtc()
* ���ܣ���ȡRTC��ʱ��ֵ������������ʱ����ֵ�ɴ��ڷ��͵���λ����ʾ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  SendTimeRtc(void)
{  uint32  datas;
   uint32  times;
   uint32  bak;
    
   times = CTIME0;			// ��ȡ����ʱ�ӼĴ���
   datas = CTIME1;
   
   bak = (datas>>16)&0xFFF;     // ȡ����ֵ
   PC_DispChar(0, SHOWTABLE[bak/1000]);
   bak = bak%1000;
   PC_DispChar(1, SHOWTABLE[bak/100]);
   bak = bak%100;
   PC_DispChar(2, SHOWTABLE[bak/10]);
   PC_DispChar(3, SHOWTABLE[bak%10]);
  
   bak = (datas>>8)&0x0F;       // ȡ����ֵ
   PC_DispChar(4, SHOWTABLE[bak/10]);
   PC_DispChar(5, SHOWTABLE[bak%10]);
   
   bak = datas&0x1F;            // ȡ����ֵ
   PC_DispChar(6, SHOWTABLE[bak/10]);
   PC_DispChar(7, SHOWTABLE[bak%10]);
   
   bak = (times>>24)&0x07;      // ȡ������ֵ
   PC_DispChar(8, SHOWTABLE[bak]);
   
   
   bak = (times>>16)&0x1F;		// ȡ��ʱ��ֵ
   PC_DispChar(9, SHOWTABLE[bak/10]);
   PC_DispChar(10, SHOWTABLE[bak%10]);
   
   bak = (times>>8)&0x3F;		// ȡ�÷ֵ�ֵ
   PC_DispChar(11, SHOWTABLE[bak/10]);
   PC_DispChar(12, SHOWTABLE[bak%10]);
   
   bak = times&0x3F;			// ȡ�����ֵ
   PC_DispChar(13, SHOWTABLE[bak/10]);
   PC_DispChar(14, SHOWTABLE[bak%10]);
}




/****************************************************************************
* ���ƣ�RTCIni()
* ���ܣ���ʼ��ʵʱʱ�ӡ�
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  RTCInit(void)
{  PREINT = Fpclk / 32768 - 1;	// ���û�׼ʱ�ӷ�Ƶ��
   PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
   
   YEAR = 2006;				    // ��ʼ����
   MONTH = 8;				    // ��ʼ����
   DOM = 14;				    // ��ʼ����
   DOW = 1;                     // ��ʼ����Ӧ�����ڼ�
   HOUR = 20;
   MIN = 30;
   SEC = 10;
   
   
   CIIR = 0x01;				    // ������ֵ����������һ���ж�
   
   CCR = 0x01;				    // ����RTC
}


/****************************************************************************
* ���ƣ�main()
* ���ܣ�����RTC���м�ʱ��������ʱ��ֵ���ϵ�ͨ����������λ�����͡���λ��ʹ��EasyARM
*      ������ڷ������������ʾ���Ϲ۲�����
*      ͨѶ������115200��8λ����λ��1λֹͣλ������żУ�顣  
****************************************************************************/
int  main(void)
{  UARTMODE  uart0_set;   

   PINSEL0 = 0x00000005;            // ����I/O���ӵ�UART0
   PINSEL1 = 0x00000000;              
     
   uart0_set.datab = 8;             // 8λ����λ
   uart0_set.stopb = 1;             // 1λֹͣλ
   uart0_set.parity = 0;            // ����żУ��
   UART0_Init(115200, uart0_set);    // ��ʼ������ģʽ
   U0FCR = 0x01;                    // ʹ��FIFO
   
   
   RTCInit();					    // ��ʼ��RTC
   while(1)	
   {  while( 0==(ILR&0x01) );       // �ȴ�RTC�����жϱ�־
      ILR = 0x01;				    // ����жϱ�־
      SendTimeRtc();                // ��ȡʱ��ֵ,����UART0����
      
      
      while( 0==(ILR&0x01) );
      ILR = 0x01;
      SendTimeRtc();
   }
   
   return(0);
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
