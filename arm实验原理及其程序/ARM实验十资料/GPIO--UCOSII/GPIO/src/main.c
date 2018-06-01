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
** Descriptions:		uCOS-II����ʵ�飬����GPIO������P2.16��P2.23�ֱ�̽�LED0��KED7��
**
********************************************************************************************************/
#include "config.h"
#include "stdlib.h"
#include "GPIO.h"

#define		TaskStkLengh	128			//Define the Task0 stack length �����û�����0�Ķ�ջ����
#define	    Boad_KEY1		(1 << 28)	//P0.28����KEY1
#define	    Boad_KEY2		(1 << 27)	//P0.27����KEY2
#define	    Boad_KEY3		(1 << 25)	//P0.25����KEY3

#define	    Boad_LED1		(1 << 22)	//P1.22����LED1
#define	    Boad_LED2		(1 << 23)	//P1.23����LED2
#define	    Boad_LED3		(1 << 24)	//P1.24����LED3
#define	    Boad_LED4		(1 << 25)	//P1.25����LED4

#define		LED0			(1 << 16)	//P2.16����LED0
#define		LED1			(1 << 17)	//P2.17����LED1
#define		LED2			(1 << 18)	//P2.18����LED2
#define		LED3			(1 << 19)	//P2.19����LED3
#define		LED4			(1 << 20)	//P2.20����LED4
#define		LED5			(1 << 21)	//P2.21����LED5
#define		LED6			(1 << 22)	//P2.22����LED6
#define		LED7			(1 << 23)	//P2.23����LED7
#define		LED				(0xff << 16)
 
OS_STK	Task0Stk [TaskStkLengh];		//Define the Task0 stack �����û�����0�Ķ�ջ
OS_STK	Task1Stk [TaskStkLengh];		//����1�Ķ�ջ
OS_STK	Task2Stk [TaskStkLengh];		//����2�Ķ�ջ
	
OS_EVENT	*Mbox;						//����Task0��Task1֮��ͨ�ŵ�����
const	uint32	LED_TBL[] =	{
							0x00,0xff,									//ȫ��Ϩ��
							0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,	//�����������
							0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff,	//�����������
							0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,	//��������ݼ�
							0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81,	//������£��ֿ�
							0x81,0xc3,0xe7,0xff,0xff,0xe7,0xc3,0x81		//�����ߵ��Ӻ�ݼ�
							};							


void 	Task0(void *pdata);			//Task0 ����0������LED����ʾ
void 	Task1(void *pdata);			//Task1 ����1����ⰴ��
void 	Task2(void *pdata);			//Task2 ����2����ˮ����ʾ

/*********************************************************************************************************
** �������ƣ�main()
** �������ܣ�����QuickARM���ϵ�KEY1��KEY3������QuickARM���ϵ�LED1��LED3��ͬʱ����������ʾ��ˮ�ơ�
** ˵����	����P2.16��P2.23�ֱ�̽�LED0��KED7��	
********************************************************************************************************/
int main (void)
{
	OSInit ();
	Mbox = OSMboxCreate((void *)0);	// ����һ�����䣬�������ݰ�����Ϣ																						
	OSTaskCreate (Task0,(void *)0, &Task0Stk[TaskStkLengh - 1], 2);		
	OSStart ();
	return 0;															
}
/*********************************************************************************************************
** �������ƣ�Task0 
** �����ܣ���������Task1������������Ϣ��������QuickARM���ϵ�LED��ʾ��
********************************************************************************************************/
void Task0(void *pdata)
{
	uint8  *MboxPoint;
	uint8  Err;
	pdata = pdata;
	TargetInit ();
	OSTaskCreate (Task1,(void *)0, &Task1Stk[TaskStkLengh - 1], 3);    // ��������Task1
	OSTaskCreate (Task2,(void *)0, &Task2Stk[TaskStkLengh - 1], 4);    // �������� Task2

	P0_GPIOInit(Boad_KEY1|Boad_KEY2|Boad_KEY3,0);				//����KEY1~KEY3����Ϊ����ģʽ
	P1_GPIOInit(Boad_LED1|Boad_LED2|Boad_LED3|Boad_LED4,1);		//LED1~LED4����ΪGPIO���ģʽ
	P1_GPIOClr(Boad_LED4);										//����LED4
	while (1)
	{
		P1_GPIOSet(Boad_LED1|Boad_LED2|Boad_LED3);
		MboxPoint = OSMboxPend(Mbox,0,&Err);	//����������Ϣ
		switch(*MboxPoint)
		{
			case '1':							//����LED1
				P1_GPIOClr(Boad_LED1);
				break;
			case '2':							//����LED2
				P1_GPIOClr(Boad_LED2);		
				break;
			case '3':							//����LED3
				P1_GPIOClr(Boad_LED3);
				break;
			default:
				break;
		}
		OSTimeDly(10);
	}
}
/*********************************************************************************************************
** �������ƣ�Task1 
** �����ܣ���ⰴ��KEY1��KEY3����������Task0����������Ϣ��
********************************************************************************************************/
void Task1	(void *pdata)
{
	uint8 KeyValue;
	uint8 Err;
	pdata = pdata;
	while (1)
	{
		if((Read_P0() & Boad_KEY1) == 0)
		{										// ����KEY1����
			KeyValue = '1';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		if((Read_P0() & Boad_KEY2) == 0)
		{										// ����KEY2����
			KeyValue = '2';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		if((Read_P0() & Boad_KEY3) == 0)
		{										// ����KEY3����
			KeyValue = '3';
			Err = OSMboxPost(Mbox,&KeyValue);			
		}
		OSTimeDly(10);
	}
}
/*********************************************************************************************************
** �������ƣ�Task2 
** �����ܣ���ˮ����ʾʵ�顣
********************************************************************************************************/
void Task2	(void *pdata)
{
	uint8  i = 0;
	pdata = pdata;
	P2_GPIOInit(LED,1);			//��LED0��LED7����ΪGPIO���ģʽ
	while (1)
	{
		Write_P2(~(LED_TBL[((i++) % 42)] << 16));
		OSTimeDly(30);
	}
}   
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
