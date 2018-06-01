/****************************************Copyright (c)**************************************************
**                               ������������Ƭ����չ���޹�˾
**                                     ��    ��    ��
**                                        ��Ʒһ�� 
**
**                                 http://www.zlgmcu.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: PWM.c
**��   ��   ��: �ű�
**����޸�����: 2006-5-12
**��        ��: LPC2200 PWM�����������
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ű�
** ��  ��: V1.0
** �ա���: 2006-5-12
** �衡��: LPC2200 PWM�����������
**
**------------------------------------------------------------------------------------------------------
** �޸���:
** ��  ��:
** �ա���:
** �衡��:
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���: 
** �ա���:
** �衡��:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"

/*********************************************************************************************************
** �������ƣ�uint8	SingleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 PWMMRn,uint8 PWMn,uint8 INTEn)
** ����������������PWM��ʼ����
** ��ڲ�����F_PWM		PWMƵ�ʿ��ƼĴ�������ֱֵ��д�뵽PWMMR0�Ĵ�����
**			 PR_Data	Ԥ��Ƶ����ֵ
**			 PWMMRn		PWMƥ��Ĵ���������PWM���ص�λ��
**			 PWMn		PWMͨ��ѡ����ЧֵΪ1��6
**			 INTEn		�ж�ʹ��
**						>0	TCֵ��PWMMR0ƥ��ʱ�����ж�
**						0	TCֵ��PWMMR0ƥ��ʱ�������ж�
** ���ڲ�����0	��ʼ��ʧ��
**           1	��ʼ���ɹ�
** ˵����ʹ��PWMMR0������PWM��Ƶ�ʡ�ƥ��ʱ����λPWMTC��
**		 ���������ų�ʼ��������
********************************************************************************************************/
uint8	SingleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 PWMMRn,uint8 PWMn,uint8 INTEn)
{
	if((PWMn > 6) || (PWMn==0))		return(0);	//�����д���ֱ�ӷ���
	PWMTCR = 0x02;		//��λPWMTC
	PWMPR  = PR_Data;	//���÷�Ƶֵ
	if(INTEn == 0)	PWMMCR = 0x02;		//PWMMR0ƥ��ʱ����λPWMT
	else			PWMMCR = 0x03;		//PWMMR0ƥ��ʱ����λPWMTC�������ж�
	PWMPCR = (PWMPCR&(~(1<<PWMn)))|(1<<(PWMn+8));	//����PWMn�����������PWM,��Ӱ������λ
	PWMMR0 = F_PWM;		//����PWM���Ƶ��
	//�������ų�ʼ��
	switch(PWMn)		//PWMn = 1~6
	{
		case	1:		//PWM1������P0.0
			PINSEL0 = (PINSEL0&(~(0x03)))|(0x02);
			PWMMR1 = PWMMRn;	//����ƥ��Ĵ���
			break;
		case	2:		//PWM2������P0.7
			PINSEL0 = (PINSEL0&(~(0x03<<14)))|(0x02<<14);
			PWMMR2 = PWMMRn;	//����ƥ��Ĵ���
			break;
		case	3:		//PWM3������P0.1
			PINSEL0 = (PINSEL0&(~(0x03<<2)))|(0x02<<2);
			PWMMR3 = PWMMRn;	//����ƥ��Ĵ���
			break;
		case	4:		//PWM4������P0.8
			PINSEL0 = (PINSEL0&(~(0x03<<16)))|(0x02<<16);
			PWMMR4 = PWMMRn;	//����ƥ��Ĵ���
			break;
		case	5:		//PWM5������P0.21
			PINSEL1 = (PINSEL1&(~(0x03<<10)))|(0x01<<10);
			PWMMR5 = PWMMRn;	//����ƥ��Ĵ���
			break;
		case	6:		//PWM6������P0.9
			PINSEL0 = (PINSEL0&(~(0x03<<18)))|(0x02<<18);
			PWMMR6 = PWMMRn;	//����ƥ��Ĵ���
			break;
		default:		
			break;
	}
	PWMLER = PWMLER|(1<<PWMn)|0x01;	//PWM0��PWMnƥ������
	PWMTCR = 0x09;					//����PWM���
	return(1);
}
/*********************************************************************************************************
** �������ƣ�uint8	DoubleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 SetPWMMRn,
**									   uint32 ClrPWMMRn,uint8 PWMn,uint8 INTEn)
** ����������˫����PWM��ʼ����
** ��ڲ�����F_PWM		PWMƵ�ʿ��ƼĴ�������ֱֵ��д�뵽PWMMR0�Ĵ�����
**			 PR_Data	Ԥ��Ƶ����ֵ
**			 SetPWMMRn	PWM��λƥ��Ĵ���������PWM�����ص�λ��
**			 ClrPWMMRn	PWM��λƥ��Ĵ���������PWM�½��ص�λ��
**			 PWMn		PWMͨ��ѡ��2��3��4��5��6Ϊ��Чͨ��
**			 INTEn		�ж�ʹ��
**						>0	TCֵ��PWMMR0ƥ��ʱ�����ж�
**						0	TCֵ��PWMMR0ƥ��ʱ�������ж�
** ���ڲ�����0	��ʼ��ʧ��
**           1	��ʼ���ɹ�
** ˵����ʹ��PWMMR0������PWM��Ƶ�ʡ�ƥ��ʱ����λPWMTC��
**		 ���������ų�ʼ��������
********************************************************************************************************/
uint8	DoubleEdgePWM_Init(uint32 F_PWM,uint32 PR_Data,uint32 SetPWMMRn,
						   uint32 ClrPWMMRn,uint8 PWMn,uint8 INTEn)
{	
	if((PWMn > 6) || (PWMn < 2))		return(0);	//�����д���ֱ�ӷ���
	PWMTCR = 0x02;				//��λPWMTC
	PWMPR  = PR_Data;			//���÷�Ƶֵ
	if(INTEn == 0)	PWMMCR = 0x02;		//PWMMR0ƥ��ʱ����λPWMT
	else			PWMMCR = 0x03;		//PWMMR0ƥ��ʱ����λPWMTC�������ж�
	//�������ų�ʼ���ͼĴ�������
	switch(PWMn)		//PWMn = 2~6
	{
		case	2:		//PWM2������P0.7
			PINSEL0 = (PINSEL0&(~(0x03<<14)))|(0x02<<14);
			PWMMR1 = SetPWMMRn;		//PWMn����λ�Ĵ���Ϊ��PWMMRn-1
			PWMMR2 = ClrPWMMRn;		//PWMn�ĸ�λ�Ĵ���Ϊ��PWMMRn
			break;
		case	3:		//PWM3������P0.1
			PINSEL0 = (PINSEL0&(~(0x03<<2)))|(0x02<<2);
			PWMMR2 = SetPWMMRn;		//PWMn����λ�Ĵ���Ϊ��PWMMRn-1
			PWMMR3 = ClrPWMMRn;		//PWMn�ĸ�λ�Ĵ���Ϊ��PWMMRn
			break;
		case	4:		//PWM4������P0.8
			PINSEL0 = (PINSEL0&(~(0x03<<16)))|(0x02<<16);
			PWMMR3 = SetPWMMRn;		//PWMn����λ�Ĵ���Ϊ��PWMMRn-1
			PWMMR4 = ClrPWMMRn;		//PWMn�ĸ�λ�Ĵ���Ϊ��PWMMRn
			break;
		case	5:		//PWM5������P0.21
			PINSEL1 = (PINSEL1&(~(0x03<<10)))|(0x01<<10);
			PWMMR4 = SetPWMMRn;		//PWMn����λ�Ĵ���Ϊ��PWMMRn-1
			PWMMR5 = ClrPWMMRn;		//PWMn�ĸ�λ�Ĵ���Ϊ��PWMMRn
			break;
		case	6:		//PWM6������P0.9
			PINSEL0 = (PINSEL0&(~(0x03<<18)))|(0x02<<18);
			PWMMR5 = SetPWMMRn;		//PWMn����λ�Ĵ���Ϊ��PWMMRn-1
			PWMMR6 = ClrPWMMRn;		//PWMn�ĸ�λ�Ĵ���Ϊ��PWMMRn
			break;
		default:	
			break;
	}
	PWMPCR = PWMPCR|(1<<PWMn)|(1<<(PWMn+8));	//����PWMn�����������PWM,��Ӱ������λ
	PWMMR0 = F_PWM;								//����PWM���Ƶ��	
	PWMLER = PWMLER|(3<<(PWMn-1))|0x01;			//PWM0��PWMn�ĸ�λ����λƥ������
	PWMTCR = 0x09;								//����PWM���
	return(1);
}
/*********************************************************************************************************
** �������ƣ�void	TimePWM_Init(uint32 PWM_time,uint32 PR_Data,uint8 TMODE)
** ����������PWM��ʼ��Ϊ32λ��ʱ��
** ��ڲ�����PWM_time	��ʱʱ��,��ֵ��ֱ��д�뵽PWMMR0�Ĵ�����
**			 PR_Data	Ԥ��Ƶ����ֵ
**			 TMODE		����ģʽ����λ����
**						bit0--�жϣ�Ϊ1ʱ�������ж�
**						bit1--��λ��Ϊ1ʱ��PWMTC��λ
**						bit2--ֹͣ��Ϊ1ʱ��PWMTCֹͣ
** ���ڲ�������
** ˵����PWMʹ��PWMMR0��ɶ�ʱ���ĳ�ʼ��������
********************************************************************************************************/
void	TimePWM_Init(uint32 PWM_time,uint32 PR_Data,uint8 TMODE)
{
	PWMTCR = 0x02;				//��λPWMTC
	PWMPR  = PR_Data;			//���÷�Ƶֵ
	PWMMCR = (TMODE & 0x07);	//����ƥ�䷢��ʱ�Ĺ���ģʽ
	PWMMR0 = PWM_time;			//���ö�ʱʱ��
	PWMTCR = 0x01;				//������ʱ��
}