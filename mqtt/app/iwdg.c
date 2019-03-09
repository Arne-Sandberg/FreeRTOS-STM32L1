#include "iwdg.h"

/**************************************************
*�������ܣ����Ź���ʼ��
*����˵������
*�� �� ֵ����
**************************************************/
void IWDG_Init(uint8_t prer,uint16_t rlr)
{
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		
		IWDG_SetPrescaler(prer);	
		IWDG_SetReload(rlr);		
		IWDG_ReloadCounter();		
		IWDG_Enable();	
}

/**************************************************
*�������ܣ�ι��
*����˵������
*�� �� ֵ����
**************************************************/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}


void IWDG_DeInit(uint8_t prer,uint16_t rlr)
{
	DBGMCU_Config(DBGMCU_IWDG_STOP, ENABLE);
}
