/*
 *      File name:      debug.h
 *
 *      Created on:     Aug 20, 2018
 *      Author:         manhbt
 *      Brief:          Debug functions ....
 */


#ifndef DEBUG_H_
#define DEBUG_H_
#include "modem_wrapper.h"

//void DEBUG(const char* msg);

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define DEBUG 	PRINTF

#define MODEM_DEBUG(s, args...)		PRINTF("\r\n" KYEL "[UC15] " s "\r\n" KNRM, ##args)

//#define MODEM_DEBUG 	LREF_WARNING

//#define LREF_RAW(s, args...)     	debug_print(s, ##args)
//#define LREF(s, args...)         	debug_print(KGRN "[Firmware] " s "\n" KNRM, ##args)
//#define LREF_CRITICAL(s, args...)   debug_print(KRED "[Firmware][ERROR] :%d:%s " s "\n" KNRM, __FILE__, __LINE__, ##args)
//#define LREF_ERROR(s, args...)   	debug_print(KMAG "[Firmware][ERROR] : " s "\n" KNRM, ##args)
//#define LREF_WARNING(s, args...) 	PRINTF(KYEL "[Firmware][WARNING] " s "\n" KNRM, ##args)
//#define LREF_LINE(s, args...) 		debug_print(KNRM "[%s:%d] " s "\n" KNRM, __FILE__, __LINE__, ##args)

#endif /* DEBUG_H_ */
