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
** Modified by:			zhangbin
** Modified date:		2006-6-2
** Version:				
** Descriptions:		uCOS-II基础实验，测试GPIO。跳线P2.16～P2.23分别短接LED0～KED7。
**
********************************************************************************************************/
#include "config.h"
#include "stdlib.h"
#include "GPIO.h"

#define		TaskStkLengh	128			//Define the Task0 stack length 定义用户任务0的堆栈长度
#define	    Boad_KEY1		(1 << 28)	//P0.28控制KEY1
#define	    Boad_KEY2		(1 << 27)	//P0.27控制KEY2
#define	    Boad_KEY3		(1 << 25)	//P0.25控制KEY3

#define	    Boad_LED1		(1 << 22)	//P1.22控制LED1
#define	    Boad_LED2		(1 << 23)	//P1.23控制LED2
#define	    Boad_LED3		(1 << 24)	//P1.24控制LED3
#define	    Boad_LED4		(1 << 25)	//P1.25控制LED4

#define		LED0			(1 << 16)	//P2.16控制LED0
#define		LED1			(1 << 17)	//P2.17控制LED1
#define		LED2			(1 << 18)	//P2.18控制LED2
#define		LED3			(1 << 19)	//P2.19控制LED3
#define		LED4			(1 << 20)	//P2.20控制LED4
#define		LED5			(1 << 21)	//P2.21控制LED5
#define		LED6			(1 << 22)	//P2.22控制LED6
#define		LED7			(1 << 23)	//P2.23控制LED7
#define		LED				(0xff << 16)
 
OS_STK	Task0Stk [TaskStkLengh];		//Define the Task0 stack 定义用户任务0的堆栈
OS_STK	Task1Stk [TaskStkLengh];		//任务1的堆栈
OS_STK	Task2Stk [TaskStkLengh];		//任务2的堆栈
	
OS_EVENT	*Mbox;						//任务Task0与Task1之间通信的邮箱
const	uint32	LED_TBL[] =	{
							0x00,0xff,									//全部熄灭
							0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,	//依次逐个点亮
							0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff,	//依次逐个叠加
							0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,	//依次逐个递减
							0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81,	//两个靠拢后分开
							0x81,0xc3,0xe7,0xff,0xff,0xe7,0xc3,0x81		//从两边叠加后递减
							};							


void 	Task0(void *pdata);			//Task0 任务0，控制LED的显示
void 	Task1(void *pdata);			//Task1 任务1，检测按键
void 	Task2(void *pdata);			//Task2 任务2，流水灯显示

/*********************************************************************************************************
** 函数名称：main()
** 函数功能：按下QuickARM板上的KEY1～KEY3，点亮QuickARM板上的LED1～LED3。同时，主板上显示流水灯。
** 说明：	跳线P2.16～P2.23分别短接LED0～KED7。	
********************************************************************************************************/
int main (void)
{
	OSInit ();
	Mbox = OSMboxCreate((void *)0);	// 建立一个邮箱，用来传递按键信息																						
	OSTaskCreate (Task0,(void *)0, &Task0Stk[TaskStkLengh - 1], 2);		
	OSStart ();
	return 0;															
}
/*********************************************************************************************************
** 任务名称：Task0 
** 任务功能：接收任务Task1发来的邮箱信息，并控制QuickARM板上的LED显示。
********************************************************************************************************/
void Task0(void *pdata)
{
	uint8  *MboxPoint;
	uint8  Err;
	pdata = pdata;
	TargetInit ();
	OSTaskCreate (Task1,(void *)0, &Task1Stk[TaskStkLengh - 1], 3);    // 建立任务Task1
	OSTaskCreate (Task2,(void *)0, &Task2Stk[TaskStkLengh - 1], 4);    // 建立任务 Task2

	P0_GPIOInit(Boad_KEY1|Boad_KEY2|Boad_KEY3,0);				//按键KEY1~KEY3设置为输入模式
	P1_GPIOInit(Boad_LED1|Boad_LED2|Boad_LED3|Boad_LED4,1);		//LED1~LED4设置为GPIO输出模式
	P1_GPIOClr(Boad_LED4);										//点亮LED4
	while (1)
	{
		P1_GPIOSet(Boad_LED1|Boad_LED2|Boad_LED3);
		MboxPoint = OSMboxPend(Mbox,0,&Err);	//接收邮箱信息
		switch(*MboxPoint)
		{
			case '1':							//点亮LED1
				P1_GPIOClr(Boad_LED1);
				break;
			case '2':							//点亮LED2
				P1_GPIOClr(Boad_LED2);		
				break;
			case '3':							//点亮LED3
				P1_GPIOClr(Boad_LED3);
				break;
			default:
				break;
		}
		OSTimeDly(10);
	}
}
/*********************************************************************************************************
** 任务名称：Task1 
** 任务功能：检测按键KEY1～KEY3，并给任务Task0发送邮箱信息。
********************************************************************************************************/
void Task1	(void *pdata)
{
	uint8 KeyValue;
	uint8 Err;
	pdata = pdata;
	while (1)
	{
		if((Read_P0() & Boad_KEY1) == 0)
		{										// 按键KEY1按下
			KeyValue = '1';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		if((Read_P0() & Boad_KEY2) == 0)
		{										// 按键KEY2按下
			KeyValue = '2';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		if((Read_P0() & Boad_KEY3) == 0)
		{										// 按键KEY3按下
			KeyValue = '3';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		OSTimeDly(10);
	}
}
/*********************************************************************************************************
** 任务名称：Task2 
** 任务功能：流水灯显示实验。
********************************************************************************************************/
void Task2	(void *pdata)
{
	uint8  i = 0;
	pdata = pdata;
	P2_GPIOInit(LED,1);			//将LED0～LED7设置为GPIO输出模式
	while (1)
	{
		Write_P2(~(LED_TBL[((i++) % 42)] << 16));
		OSTimeDly(30);
	}
}   
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
