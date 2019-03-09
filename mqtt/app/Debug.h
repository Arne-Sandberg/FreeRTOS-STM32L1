#ifndef __DEBUG_H
#define __DEBUG_H

//������Ϣ���أ��ر�ֱ��ע��
/*************/



#define TDDEBUG 1
#define TDERROR 1
#define TDFINFO 1

#ifdef TDDEBUG
#define Debug(...) 	printf(__VA_ARGS__)
#else
#define Debug(...)
#endif


#ifdef TDERROR
#define Error(...) 	printf(__VA_ARGS__)
#else
#define Error(...)
#endif


#ifdef TDFINFO
#define Info(...) 	printf(__VA_ARGS__)
#else
#define Info(...)
#endif


#endif



