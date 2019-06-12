#ifndef __HY_SYSTIME_H__
#define __HY_SYSTIME_H__

#include "stdint.h"

#define HY_SYSTIME_INTERVAL  10/*ms*/


typedef struct SystemTimeStruct
{
    uint32_t now_ms;
    uint32_t lastcount_ms;
    uint32_t second_60;
    uint32_t minute_60;
    uint32_t hour_24;
    uint32_t day_30;
    uint32_t month_12;
    uint32_t year;

    uint32_t timeoffset_hour;
}hy_systime_t;


hy_systime_t* hy_get_systime_handle(void);
int hy_systime_init(void* hy_instance_handle);	

#endif