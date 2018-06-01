/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: POWER.h
**创   建   人: 张斌
**最后修改日期: 2006-5-13
**描        述: LPC2200 功率控制应用软件包头文件。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 张斌
** 版  本: V1.0
** 日　期: 2006-5-13
** 描　述: LPC2200 功率控制应用软件包头文件。
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

#define	Set_PCONP(value)	PCONP = (value & 0x1fbe)	// 设置外设功率控制寄存器
#define	Power_Down()		PCON = 0x02					// 进入掉电模式
#define	IDLE()				PCON = 0x01					// 进入空闲模式	
