/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: PWM.c
**创   建   人: 张斌
**最后修改日期: 2006-5-12
**描        述: LPC2200 PWM操作软件包。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 张斌
** 版  本: V1.0
** 日　期: 2006-5-12
** 描　述: LPC2200 PWM操作软件包。
**
**------------------------------------------------------------------------------------------------------
** 修改人:
** 版  本:
** 日　期:
** 描　述:
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"

/*********************************************************************************************************
** 函数名称：uint8	SingleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 PWMMRn,uint8 PWMn,uint8 INTEn)
** 功能描述：单边沿PWM初始化。
** 入口参数：F_PWM		PWM频率控制寄存器，该值直接写入到PWMMR0寄存器中
**			 PR_Data	预分频器的值
**			 PWMMRn		PWM匹配寄存器，控制PWM边沿的位置
**			 PWMn		PWM通道选择，有效值为1～6
**			 INTEn		中断使能
**						>0	TC值与PWMMR0匹配时产生中断
**						0	TC值与PWMMR0匹配时不产生中断
** 出口参数：0	初始化失败
**           1	初始化成功
** 说明：使用PWMMR0来控制PWM的频率。匹配时，复位PWMTC；
**		 包含有引脚初始化操作。
********************************************************************************************************/
uint8	SingleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 PWMMRn,uint8 PWMn,uint8 INTEn)
{
	if((PWMn > 6) || (PWMn==0))		return(0);	//参数有错误，直接返回
	PWMTCR = 0x02;		//复位PWMTC
	PWMPR  = PR_Data;	//设置分频值
	if(INTEn == 0)	PWMMCR = 0x02;		//PWMMR0匹配时，复位PWMT
	else			PWMMCR = 0x03;		//PWMMR0匹配时，复位PWMTC、产生中断
	PWMPCR = (PWMPCR&(~(1<<PWMn)))|(1<<(PWMn+8));	//允许PWMn输出，单边沿PWM,不影响其它位
	PWMMR0 = F_PWM;		//设置PWM输出频率
	//进行引脚初始化
	switch(PWMn)		//PWMn = 1~6
	{
		case	1:		//PWM1是引脚P0.0
			PINSEL0 = (PINSEL0&(~(0x03)))|(0x02);
			PWMMR1 = PWMMRn;	//设置匹配寄存器
			break;
		case	2:		//PWM2是引脚P0.7
			PINSEL0 = (PINSEL0&(~(0x03<<14)))|(0x02<<14);
			PWMMR2 = PWMMRn;	//设置匹配寄存器
			break;
		case	3:		//PWM3是引脚P0.1
			PINSEL0 = (PINSEL0&(~(0x03<<2)))|(0x02<<2);
			PWMMR3 = PWMMRn;	//设置匹配寄存器
			break;
		case	4:		//PWM4是引脚P0.8
			PINSEL0 = (PINSEL0&(~(0x03<<16)))|(0x02<<16);
			PWMMR4 = PWMMRn;	//设置匹配寄存器
			break;
		case	5:		//PWM5是引脚P0.21
			PINSEL1 = (PINSEL1&(~(0x03<<10)))|(0x01<<10);
			PWMMR5 = PWMMRn;	//设置匹配寄存器
			break;
		case	6:		//PWM6是引脚P0.9
			PINSEL0 = (PINSEL0&(~(0x03<<18)))|(0x02<<18);
			PWMMR6 = PWMMRn;	//设置匹配寄存器
			break;
		default:		
			break;
	}
	PWMLER = PWMLER|(1<<PWMn)|0x01;	//PWM0和PWMn匹配锁存
	PWMTCR = 0x09;					//启动PWM输出
	return(1);
}
/*********************************************************************************************************
** 函数名称：uint8	DoubleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 SetPWMMRn,
**									   uint32 ClrPWMMRn,uint8 PWMn,uint8 INTEn)
** 功能描述：双边沿PWM初始化。
** 入口参数：F_PWM		PWM频率控制寄存器，该值直接写入到PWMMR0寄存器中
**			 PR_Data	预分频器的值
**			 SetPWMMRn	PWM置位匹配寄存器，控制PWM上升沿的位置
**			 ClrPWMMRn	PWM复位匹配寄存器，控制PWM下降沿的位置
**			 PWMn		PWM通道选择：2、3、4、5、6为有效通道
**			 INTEn		中断使能
**						>0	TC值与PWMMR0匹配时产生中断
**						0	TC值与PWMMR0匹配时不产生中断
** 出口参数：0	初始化失败
**           1	初始化成功
** 说明：使用PWMMR0来控制PWM的频率。匹配时，复位PWMTC；
**		 包含有引脚初始化操作。
********************************************************************************************************/
uint8	DoubleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 SetPWMMRn,
						   uint32 ClrPWMMRn,uint8 PWMn,uint8 INTEn)
{	
	if((PWMn > 6) || (PWMn < 2))		return(0);	//参数有错误，直接返回
	PWMTCR = 0x02;				//复位PWMTC
	PWMPR  = PR_Data;			//设置分频值
	if(INTEn == 0)	PWMMCR = 0x02;		//PWMMR0匹配时，复位PWMT
	else			PWMMCR = 0x03;		//PWMMR0匹配时，复位PWMTC、产生中断
	//进行引脚初始化和寄存器设置
	switch(PWMn)		//PWMn = 2~6
	{
		case	2:		//PWM2是引脚P0.7
			PINSEL0 = (PINSEL0&(~(0x03<<14)))|(0x02<<14);
			PWMMR1 = SetPWMMRn;		//PWMn的置位寄存器为：PWMMRn-1
			PWMMR2 = ClrPWMMRn;		//PWMn的复位寄存器为：PWMMRn
			break;
		case	3:		//PWM3是引脚P0.1
			PINSEL0 = (PINSEL0&(~(0x03<<2)))|(0x02<<2);
			PWMMR2 = SetPWMMRn;		//PWMn的置位寄存器为：PWMMRn-1
			PWMMR3 = ClrPWMMRn;		//PWMn的复位寄存器为：PWMMRn
			break;
		case	4:		//PWM4是引脚P0.8
			PINSEL0 = (PINSEL0&(~(0x03<<16)))|(0x02<<16);
			PWMMR3 = SetPWMMRn;		//PWMn的置位寄存器为：PWMMRn-1
			PWMMR4 = ClrPWMMRn;		//PWMn的复位寄存器为：PWMMRn
			break;
		case	5:		//PWM5是引脚P0.21
			PINSEL1 = (PINSEL1&(~(0x03<<10)))|(0x01<<10);
			PWMMR4 = SetPWMMRn;		//PWMn的置位寄存器为：PWMMRn-1
			PWMMR5 = ClrPWMMRn;		//PWMn的复位寄存器为：PWMMRn
			break;
		case	6:		//PWM6是引脚P0.9
			PINSEL0 = (PINSEL0&(~(0x03<<18)))|(0x02<<18);
			PWMMR5 = SetPWMMRn;		//PWMn的置位寄存器为：PWMMRn-1
			PWMMR6 = ClrPWMMRn;		//PWMn的复位寄存器为：PWMMRn
			break;
		default:	
			break;
	}
	PWMPCR = PWMPCR|(1<<PWMn)|(1<<(PWMn+8));	//允许PWMn输出，单边沿PWM,不影响其它位
	PWMMR0 = F_PWM;								//设置PWM输出频率	
	PWMLER = PWMLER|(3<<(PWMn-1))|0x01;			//PWM0和PWMn的复位、置位匹配锁存
	PWMTCR = 0x09;								//启动PWM输出
	return(1);
}
/*********************************************************************************************************
** 函数名称：void	TimePWM_Init(uint32 PWM_time,uint32 PR_Data,uint8 TMODE)
** 功能描述：PWM初始化为32位定时器
** 入口参数：PWM_time	定时时间,该值会直接写入到PWMMR0寄存器中
**			 PR_Data	预分频器的值
**			 TMODE		工作模式，按位操作
**						bit0--中断，为1时，产生中断
**						bit1--复位，为1时，PWMTC复位
**						bit2--停止，为1时，PWMTC停止
** 出口参数：无
** 说明：PWM使用PWMMR0完成定时器的初始化工作。
********************************************************************************************************/
void	TimePWM_Init(uint32 PWM_time,uint32 PR_Data,uint8 TMODE)
{
	PWMTCR = 0x02;				//复位PWMTC
	PWMPR  = PR_Data;			//设置分频值
	PWMMCR = (TMODE & 0x07);	//设置匹配发生时的工作模式
	PWMMR0 = PWM_time;			//设置定时时间
	PWMTCR = 0x01;				//启动定时器
}