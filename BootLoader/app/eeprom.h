#ifndef __EEPROM_H
#define __EEPROM_H
#include "sys.h"



#define EEPROM_BYTE_SIZE			0x0FFF


#define EN_INT						__enable_irq()					//ʹ���ж�
#define DIS_INT						__disable_irq()					//�ر��ж�

void EEPROM_ReadBytes(uint32_t Addr,uint8_t *Buffer,uint16_t Length);

void EEPROM_WriteBytes(uint32_t Addr,uint8_t *Buffer,uint16_t Length);
uint32_t STMFLASH_ReadWord(uint32_t faddr);
void STMFLASH_WriteBootInfo(uint32_t WriteAddr,uint32_t *pBuffer);

#endif
