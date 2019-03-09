#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32l1xx.h"


/* ��ʱ���ṹ�� */
typedef struct
{
	volatile uint32_t count;	/* ������*/
	volatile uint8_t flag;		/* ��ʱ�ﵽ��־ */
}SOFT_TMR;


/************��ʱ��ID**********************/
#define NET_SYSTICK_ID								1

#define MOTO_SYSTICK_ID								(NET_SYSTICK_ID + 1)

#define SILT_WAITING_ID								(MOTO_SYSTICK_ID + 1)

#define ONLINE_WAITING_ID							(SILT_WAITING_ID + 1)

/************��ʱ��ID**********************/
void StartNetTimer(uint32_t _period);
void StartWkModeTimer(uint32_t _period);

uint8_t CheckNetTimer(void);
uint8_t CheckWkModeTimer(void);

uint8_t CheckMotoTimer(void);
void StartMotoTimer(uint32_t _period);

uint8_t CheckOnlineTimer(void);
void StartOnlineTimer(uint32_t _period);

void Silt_Waiting_Delay(uint32_t _period);
uint8_t Check_Silt_Waiting_Timer(void);

void BT_Waiting_Delay(uint32_t _period);
uint8_t Check_BT_Waiting_Timer(void);

#endif
