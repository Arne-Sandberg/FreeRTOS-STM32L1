#ifndef __SYS_H
#define __SYS_H	
#include "stm32l1xx.h"

#include "Debug.h"



#define BOOTLOADER_SIZE		(8*1024)		/*8K bootloader*/


#define EEPROM_BASE_ADDR			0x08080000
#define BOOT_ADDR_COUNT				10

#define RESET_COUNT					50

#define BOOT_INFO_LEN				4


#define FIRMWARE_START_ADDR (FLASH_BASE + BOOTLOADER_SIZE)

#define RESET_APP			1

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(uint32_t addr);	//���ö�ջ��ַ



#endif
