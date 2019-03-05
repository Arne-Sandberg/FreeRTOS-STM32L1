#include "rtc.h"
#include "string.h"
#include "stdio.h"
#include "debug.h"
#include "FreeRTOS.h"
#include <stdbool.h>
#include "task.h"
#include "semphr.h"



/**************************************************
*Function: set RTC init
*Description:
*Input: NA
*Return: NA
**************************************************/

void RTC_Config(void)
{
	__IO uint32_t AsynchPrediv = 0,SynchPrediv = 0;
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef rtc_set_wakeup_time;	

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */

	PWR_RTCAccessCmd(ENABLE);


	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x0A0A)
	{

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
		/* The RTC Clock may varies due to LSI frequency dispersion. */
		//ʹ���ڲ�����ʱ��Դ
		RCC_LSICmd(ENABLE);

		//�ȴ�RTCʱ��Դ���óɹ�
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}

		//ѡ���ڲ�����ʱ����ΪRTCʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

		//����RTCʱ�ӷ�Ƶϵ��Ϊ0x7fff (32768)  
		SynchPrediv = 0xFF;
		AsynchPrediv = 0x7F;

//ѡ���ⲿ����ʱ����ΪRTCʱ��Դ
#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
		/* Enable the LSE OSC */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */	
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		SynchPrediv = 0xFF;
		AsynchPrediv = 0x7F;

#endif /* RTC_CLOCK_SOURCE_LSI */

		//ʹ��RTCʱ��
		RCC_RTCCLKCmd(ENABLE);

		//�ȴ�RTCʱ����APBʱ��ͬ��
		RTC_WaitForSynchro();
		
		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

		//����RTC��Ƶϵ���Լ�RTC������ʽ
		RTC_Init(&RTC_InitStructure);
		//�������ϵ��־��λ(д��0x0A0A)
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x0A0A);
		RTC_WaitForSynchro();

	}
	else
	{
		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF); 
	}
}





