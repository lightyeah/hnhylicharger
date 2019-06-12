
#include "stdint.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_systick.h"
#include "string.h"

#include "hy_instance.h"
#include "hy_dbg.h"

static hy_instance_t* hy_instance=NULL;

static hy_systime_t* s_systime_t=NULL;

hy_systime_t* hy_get_systime_handle(void)
{
	return s_systime_t;
}

int hy_systime_init(void* hy_instance_handle)
{
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_systime_t = &(hy_instance->systime);
	/*software init*/
	memset(s_systime_t,0,sizeof(hy_systime_t));
	/*hardware init*/
	//Initialize System Tick with 10ms time interval
	SYSTICK_InternalInit(10);
	//Enable System Tick interrupt
	SYSTICK_IntCmd(ENABLE);
	//Enable System Tick Counter
	SYSTICK_Cmd(ENABLE);

}

uint32_t systime_elapse_ms(uint32_t last_ms)
{
		uint32_t now_ms = s_systime_t->now_ms;
    if(last_ms  < now_ms)
        return now_ms - last_ms;
    else       
        return 0xFFFFFFFF - last_ms + now_ms + 1;
}

static void timecounting(void){
    if(systime_elapse_ms(s_systime_t->lastcount_ms) >= 1000){
			  s_systime_t->lastcount_ms = s_systime_t->now_ms;
        s_systime_t->second_60 += 1;
        if(s_systime_t->second_60 >= 60){
            s_systime_t->second_60 = 0;
            s_systime_t->minute_60 += 1;
            if(s_systime_t->minute_60 >= 60){
                s_systime_t->minute_60 = 0;
                s_systime_t->hour_24 += 1;
                if (s_systime_t->hour_24 >= 24){
                    s_systime_t->hour_24 = 0;
                    s_systime_t->day_30+=1;
                    if(s_systime_t->day_30>=30){
                        s_systime_t->day_30=0;
                        s_systime_t->month_12+=1;
                        if(s_systime_t->month_12>=12){
                            s_systime_t->month_12=0;
                            s_systime_t->year+=1;
                        }
                    }
                }
            }
        }
    }
    s_systime_t->now_ms += HY_SYSTIME_INTERVAL;
}

void SysTick_Handler(void)
{
	//Clear System Tick counter flag
	SYSTICK_ClearCounterFlag();
	
	timecounting();

}