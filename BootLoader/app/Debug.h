#ifndef __DEBUG_H
#define __DEBUG_H

//������Ϣ���أ��ر�ֱ��ע��
/*************/
#define DEBUG 
/*************/

#ifdef DEBUG
#define Debug(...) 	printf(__VA_ARGS__)
#else
#define Debug(...)
#endif



#endif



