/****************************************************
*������ʹ�õ�BootLoader˼·����
*
*ʹ��STM32L151RCT6���ڴ�(RAM) 32k(0x20000000 start)��Flash 256k(0x08000000~0x0803FFFF)��eeprom 8k(0x08080000~0X08081FFF)
*
*
  --------------------------------------------------------------------------------------------------------------
 |     start     |     end        |      size     |                               
 |---------------|----------------|---------------|-------------------------------------------------------------
 |  0x08000000   |   0x08001FFF   |     8K		  |   bootloader �������		  
 |---------------|----------------|---------------|-------------------------------------------------------------
 |  0x08002000   |   0x08002FFF   |     4K        |   ϵͳ��������һ������ȷ��ʹ���ĸ���ַ��ΪAPP����ʼ��ַ
 |---------------|----------------|---------------|-------------------------------------------------------------
 |  0x08003000   |   0x08014FFF   |     72k       |   ��һ������ռ�  
 |--------------------------------------------------------------------------------------------------------------
 |  0x08015000   |   0x08015FFF   |     4k        |   ����
 |--------------------------------------------------------------------------------------------------------------
 |  0x08016000   |   0x08027FFF   |     72k       |   �ڶ�������ռ�  
 |--------------------------------------------------------------------------------------------------------------
 |  0x08028000   |   0x08028FFF   |     4k        |   ����
 |--------------------------------------------------------------------------------------------------------------
 |  0x08029000   |   0x0803AFFF   |     72k       |   ����������ռ�  
 |--------------------------------------------------------------------------------------------------------------
 |  0x0803B000   |   0x0803FFFF   |     20k       |  ����
 |--------------------------------------------------------------------------------------------------------------

ϵͳ�����Ĳ���Ӧ�ó�����flash��λΪ0x08002000��ֵȷ������ֵ(appoffset)ֻ��Ϊ(1, 2, 3),�����Ϊ����ֵ��Ĭ��ʹ��1.
��ȡ����ֵ����0x08002000�Ļ����ϣ�����ƫ��������ȡ��Ӧ�ó���ĵ�ַ��ƫ����Ϊoffset = (appoffset * 8),ÿ��ֵ��4�ֽ���ɣ��м��4�ֽ�Ϊ����
��EEPROM�У�0x0808000A�������ϵͳ������ʧ�ܴ������ﵽһ����ֵ��ϵͳ�Զ��ָ���������
(FOTAʱ�����ܵ���Ӧ�ó�������ʧ�ܣ����¿��Ź�����ϵͳ���ڽ���Ӧ�ó������Ҫ��������ֵ����Ϊ1����ʾ�Ѿ�����Ӧ�ó���ɹ�)
��FTOAʱ����һ�����򲻽��鸲�ǣ�����������������ڵڶ��͵������ռ�ʹ�á����������󣬿������·������ռ䣬��Ҫ�Լ���֤FOTA����У��ɹ�
��Ӧ�ó����У�ϵͳ�����ɹ�����Ҫ��0x0808000Aֵ����Ϊ1����BootLoader�У�ϵͳ���ʧ����������Ϊ50�Ρ�
���Խ��ָ��������ô���ע�͵����޸�sys.h�в���RESET_APPΪ0������
*
*
*
*
*/

#include "sys.h"
#include "stdio.h"
#include "iap.h"
#include "Debug.h"
#include "eeprom.h"

iapfun jump2app; 
uint16_t iapbuf[1024];


uint32_t bootInfo[BOOT_INFO_LEN] = {1,0x08003000,0x08016000,0x08029000};

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(uint32_t appxaddr)
{
	if(((*(uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{
		jump2app=(iapfun)*(uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(uint32_t*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		for(int i = 0; i < 8; i++)
		{
			NVIC->ICER[i] = 0xFFFFFFFF;	/* �ر��ж�*/
			NVIC->ICPR[i] = 0xFFFFFFFF;	/* ����жϱ�־λ */
		}
		
		Debug("\n\rget the right addr");
		jump2app();									//��ת��APP.
	}
}		 


void iap_load_addr(uint32_t appOffsetAddr)
{
	uint32_t offset = 1;
	uint32_t appxaddr = 0;
	uint8_t bootCount = 0;	
	
	if( ((*(uint32_t*)appOffsetAddr)&0x00000003)==0x00000000
		|| ((*(uint32_t*)appOffsetAddr)&0xFFFEFFFF)==0xFFFFFFFF)
	{
		offset = 1;
		Debug("\n\r the offset default");
	}
	else
	{
		offset = *(uint32_t*)appOffsetAddr;
		Debug("\n\rget the offset %d",offset);
	}
#if (RESET_APP == 1)	
	EEPROM_ReadBytes(BOOT_ADDR_COUNT,&bootCount, 1);
	
	Debug("\n\rreboot count %d",bootCount);
	
	if(bootCount != 0xFF)
	{
		bootCount++;
	}
	else
	{
		bootCount = 1;
	}
	
	EEPROM_WriteBytes(BOOT_ADDR_COUNT,&bootCount, 1);
	
	if(bootCount > RESET_COUNT)
	{
		if(offset != 1)
		{
			offset = 1;
			bootInfo[0] = 1;
			bootInfo[1] = STMFLASH_ReadWord(appOffsetAddr + 8);
			bootInfo[2] = STMFLASH_ReadWord(appOffsetAddr + 2*8);
			bootInfo[3] = STMFLASH_ReadWord(appOffsetAddr + 3*8);
			
			STMFLASH_WriteBootInfo(appOffsetAddr,bootInfo);
			
			bootCount = 1;
			
		}

		Debug("\n\rreboot too much, the sys is err, reset to factory setting");
	}
#endif	
	appxaddr = *(uint32_t*)(appOffsetAddr + offset * 8);
	Debug("\n\rthe app addr is: %d",appxaddr);
	
	
	iap_load_app(appxaddr);
	
	
}
