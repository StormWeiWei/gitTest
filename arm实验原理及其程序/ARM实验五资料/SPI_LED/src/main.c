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
/*********************************************************************************************************
**点阵LED驱动实验。跳线连接：	JP6			COM13
**					短接		P0.4		SCK
**					短接		P0.6		SI
**					短接		P0.18		STR
**					JP6中P0.7短接BEEP，JP7跳线短接。用于将P0.7脚（SSEL功能）拉至高电平
**********************************************************************************************************/
#include "config.h"

//纵向取模(16×16)
							
const 	uint8	Lattice_TBL1[] = {
//广 
								   0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0x05,0x06,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
								   0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//州							
								   0x00,0xE0,0x00,0x00,0xFF,0x20,0xC0,0x00,0xFE,0x10,0x60,0x80,0x00,0xFF,0x00,0x00,
								   0x01,0x80,0x60,0x18,0x07,0x00,0x00,0x00,0x7F,0x00,0x00,0x01,0x00,0xFF,0x00,0x00,
//致							
								   0x00,0x22,0x32,0x2E,0xE2,0x2A,0x72,0x22,0x30,0xDF,0x12,0x10,0xF0,0x10,0x10,0x00,
								   0x00,0x21,0x21,0x21,0x3F,0x11,0x91,0x51,0x20,0x13,0x0C,0x12,0x21,0xC0,0x40,0x00,
//远
								   0x40,0x42,0x4C,0xC4,0x20,0x22,0x22,0xE2,0x22,0x22,0xE2,0x22,0x22,0x20,0x20,0x00,
								   0x00,0x40,0x20,0x1F,0x20,0x48,0x44,0x43,0x40,0x40,0x47,0x48,0x48,0x48,0x4E,0x00,								
//电
								   0x00,0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,0x48,0x48,0x48,0xF8,0x00,0x00,0x00,
								   0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x3F,0x44,0x44,0x44,0x44,0x4F,0x40,0x70,0x00,
//子
								   0x00,0x00,0x02,0x02,0x02,0x02,0x02,0xE2,0x12,0x0A,0x06,0x02,0x00,0x80,0x00,0x00,
								   0x01,0x01,0x01,0x01,0x01,0x41,0x81,0x7F,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
//有
								   0x00,0x04,0x84,0x44,0xE4,0x34,0x2C,0x27,0x24,0x24,0x24,0xE4,0x04,0x04,0x04,0x00,
								   0x02,0x01,0x00,0x00,0xFF,0x09,0x09,0x09,0x29,0x49,0xC9,0x7F,0x00,0x00,0x00,0x00,
//限
								   0xFE,0x02,0x32,0x4E,0x82,0x00,0xFE,0x4A,0xCA,0x4A,0x4A,0x4A,0x7E,0x00,0x00,0x00,
								   0xFF,0x00,0x02,0x04,0x03,0x00,0xFF,0x40,0x20,0x03,0x0C,0x12,0x21,0x60,0x20,0x00,
//公
								   0x00,0x00,0x80,0x40,0x30,0x0E,0x84,0x00,0x00,0x0E,0x10,0x60,0xC0,0x80,0x80,0x00,
								   0x00,0x01,0x20,0x70,0x28,0x24,0x23,0x31,0x10,0x10,0x14,0x78,0x30,0x01,0x00,0x00,
//司
								   0x00,0x10,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x12,0x02,0x02,0xFE,0x00,0x00,
								   0x00,0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x20,0x40,0x3F,0x00,0x00,
								  };



#define	Lattice_CS		(1 << 18)	// 点阵LED控制芯片，74HC595片选信号线，P0.18
/*定义SPI的管脚*/
#define	SCK0	(1<<8)				// SCK0  为P0.4的第二功能
#define	MISO0	(1<<10)				// MISO0 为P0.5的第二功能
#define	MOSI0	(1<<12)				// MOSI0 为P0.6的第二功能
#define	SSEL0	(1<<14)				// SSEL0 为P0.7的第二功能
 

uint8	LED_Point_Buf[64];			// 点阵LED缓冲区
uint8	LED_Point_Buf_Bak[64];		// 点阵LED缓冲区
uint8	Remove_Times;				// 移动显示的列数
uint8	Remove_Bytes;				// 需要显示的"字"的个数


/*********************************************************************************************************
**函数名称：SPI_Init（uint8 MODE,uint32	F_spi,uint8	INT_En）
**函数功能：初始化SPI接口
**入口参数：MODE 	SPI模式选择，0--从机，1--主机
**			F_spi	SPI通信速率，最大为Fpclk/8.该参数在从机模式下是无效的	
**			INT_En	中断使能位,0－－中断禁止，1－－中断使能
**出口参数：1--操作正确
**			0--操作失败
**说明：初始化函数设置：CPHA=0,CPOL=1,LSBF=0
********************************************************************************************************/
uint8	SPI_Init(uint8 MODE,uint32	F_spi,uint8	INT_En)
{
	uint8	tmp;
	if((MODE != 0)&&(MODE != 1)) return(0);
	PINSEL0 = (PINSEL0 & (~(0xff<<8)))|SCK0|MISO0|MOSI0|SSEL0;	//设置管脚连接SPI
	if(F_spi > (Fpclk/8))	F_spi = Fpclk/8; 
	SPI_SPCCR = Fpclk/F_spi;	//设置SPI时钟分频
	SPI_SPCR = 	(0<<3)|			//CPHA=0,数据在SCK的第一个跳变沿采样
				(1<<4)|			//CPOL=1,时钟为低有效
				(MODE<<5)|		//MODE=1,设置为主机
				(0<<6)|			//LSBF=0,数据传输MSB在先
				(INT_En<<7);	//设置中断使能位
	tmp = S0PSR;				//清除SPI传输完成标志
	S0PINT = 0x01;				//清除SPI中断标志位
	return(1);				
}

/*********************************************************************************************************
** 函数名称：Change_Buf()
** 函数功能：将16×16点阵的纵向字模更改为点阵LED的显示扫描字模。
********************************************************************************************************/
void  Change_Buf(uint8 *point)
{ 
	uint8  times = 0;
	Remove_Times = 0;
	Remove_Bytes = 1;
	for(;times < 16;times++)
	{
		LED_Point_Buf[4 * times + 3] = *(point + times);
		LED_Point_Buf[4 * times + 2] = *(point + times + 16);
		LED_Point_Buf[4 * times + 1] = (uint8)(0xffff & (~(1 << times)));
		LED_Point_Buf[4 * times]     = (uint8)((0xffff & (~(1 << times))) >> 8);
	}
	for(times = 0;times < 16;times++)
	{
		LED_Point_Buf_Bak[4 * times + 3] = *(point + 32 + times);
		LED_Point_Buf_Bak[4 * times + 2] = *(point + 32 + times + 16);
		LED_Point_Buf_Bak[4 * times + 1] = (uint8)( 0xffff & (~(1 << times)));
		LED_Point_Buf_Bak[4 * times]     = (uint8)((0xffff & (~(1 << times))) >> 8);
	}
}

/*********************************************************************************************************
** 函数名称：Disp_Buf_Hard()
** 函数功能：将点阵LED显示缓冲区中的数据发送出去。
********************************************************************************************************/
void  Disp_Buf_Hard(void)
{
	uint8  tmp,times = 0;
	for(;times < 16;times++)					// 共16列
	{	// 发送一列数据，一列数据4个字节
		IO0CLR = Lattice_CS;
		SPI_SPDR = (LED_Point_Buf[4 * times]);			
		while((SPI_SPSR & 0x80) ==0);			// 等待SPIF置位，即等待数据发送完毕
	
		tmp = SPI_SPSR;
		SPI_SPDR = (LED_Point_Buf[4 * times + 1]);	
		while((SPI_SPSR & 0x80) ==0);			// 等待SPIF置位，即等待数据发送完毕

		SPI_SPDR = (LED_Point_Buf[4 * times + 2]);		
		while((SPI_SPSR & 0x80) ==0);			// 等待SPIF置位，即等待数据发送完毕
		
		SPI_SPDR = (LED_Point_Buf[4 * times + 3]);	
		while((SPI_SPSR & 0x80) ==0);			// 等待SPIF置位，即等待数据发送完毕

		IO0SET = Lattice_CS;		
	}
}
/*********************************************************************************************************
** 函数名称：main()
** 函数功能：点阵LED显示实验。
** 说明：	 P0.4短接SCK，P0.6短接SI，P0.18短接STR。
             JP6中P0.7短接BEEP，JP7跳线短接。用于将P0.7脚（SSEL功能）拉至高电平
********************************************************************************************************/
int main (void)
{
    PINSEL1 = PINSEL1 &(~(3<<4));   // 将74HC595（点阵LED）的片选线设置为GPIO输出模式
	IO0DIR |=Lattice_CS;		
	IO0SET = Lattice_CS;
	
	SPI_Init(1,400000,0);			// SPI主机接口，通信速率400K，中断禁止
	Change_Buf((uint8 *)(&(Lattice_TBL1[0])));	//"广"
	
	while(1)
	{
		
		Disp_Buf_Hard();
				
	}
	
	return 0;															
}

  
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
