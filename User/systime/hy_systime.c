
#include "stdint.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_systick.h"
#include "string.h"

#include "hy_instance.h"
#include "hy_dbg.h"

#define HY_LOG_TAG "systime"

static hy_instance_t* hy_instance=NULL;

static hy_systime_t* s_systime=NULL;

hy_systime_t* hy_get_systime_handle(void)
{
	return s_systime;
}

int hy_systime_init(void* hy_instance_handle)
{
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_systime = &(hy_instance->systime);
	/*software init*/
	memset(s_systime,0,sizeof(hy_systime_t));
	/*todo rtc init time*/
	/*hardware init*/
	//Initialize System Tick with 10ms time interval
	SYSTICK_InternalInit(10);
	//Enable System Tick interrupt
	SYSTICK_IntCmd(ENABLE);
	//Enable System Tick Counter
	SYSTICK_Cmd(ENABLE);

	return HY_OK;
}

uint32_t hy_time_now_ms()
{
	return s_systime->now_ms;
}

uint32_t systime_elapse_ms(uint32_t last_ms)
{
		uint32_t now_ms = s_systime->now_ms;
    if(last_ms  < now_ms)
        return now_ms - last_ms;
    else       
        return 0xFFFFFFFF - last_ms + now_ms + 1;
}

static void timecounting(void){
	
		if(s_systime == NULL){
			LOG_ERROR_TAG(HY_LOG_TAG,"systime not init!!!");
			return;			
		}
    if(systime_elapse_ms(s_systime->lastcount_ms) >= 1000){
			  s_systime->lastcount_ms = s_systime->now_ms;
        s_systime->second_60 += 1;
        if(s_systime->second_60 >= 60){
            s_systime->second_60 = 0;
            s_systime->minute_60 += 1;
            if(s_systime->minute_60 >= 60){
                s_systime->minute_60 = 0;
                s_systime->hour_24 += 1;
                if (s_systime->hour_24 >= 24){
                    s_systime->hour_24 = 0;
                    s_systime->day_30+=1;
                    if(s_systime->day_30>=30){
                        s_systime->day_30=0;
                        s_systime->month_12+=1;
                        if(s_systime->month_12>=12){
                            s_systime->month_12=0;
                            s_systime->year+=1;
                        }
                    }
                }
            }
        }
    }
    s_systime->now_ms += HY_SYSTIME_INTERVAL;
}

void SysTick_Handler(void)
{
	static int ctx;
	static uint32_t overheat_time_ms;
	//Clear System Tick counter flag
	SYSTICK_ClearCounterFlag();
	
	timecounting();
	
	if(!(hy_instance->initdone)){/*not initdone*/
		return;
	}

	/*main task*/
	/*sensor task*/
	// LOG_INFO_TAG(HY_LOG_TAG,"hy_get_heatwarn [%d]",hy_get_heatwarn());
	if (hy_get_heatwarn() && !hy_chargetask_getoverheat()){
		LOG_ERROR_TAG(HY_LOG_TAG,"machine is over heat!!!");
		ctx = OBC_BMS_CST_HEAT_ERR(1); 
		hy_chargetask_stop(CHARGETASK_ERR_STOP_CODE,&ctx);
		hy_chargetask_setoverheat();
		overheat_time_ms = hy_time_now_ms();
	}else if(!hy_get_heatwarn() && hy_chargetask_getoverheat()){
		if(systime_elapse_ms(overheat_time_ms) >= HY_CHARGETASK_COLDTIME){//cold for 1 min
			hy_chargetask_clearoverheat();
			ctx = CHARGETASK_COLDDONW_START_CODE;
			hy_chargetask_start(0,&ctx);
		}
	}

	/*GUI button task*/
	hy_button_task();

	/*can task*/
	if(hy_instance->config.controlstyle == HY_CONTROLSTYLE_CAN){
		hy_can_getmsg();
		hy_can_task_main();
	}
	
	/*chargetask*/
	
	hy_chargetask_main();

}

void hy_delay_ms (unsigned long tick) 
{
	unsigned long long systickcnt = tick*12000;
	uint32_t updatetime_ms;
	updatetime_ms = hy_time_now_ms();
	if (tick >= 10)
	{
		while(systime_elapse_ms(updatetime_ms)<tick);
	}else{
		while(systickcnt--);
	}

}
