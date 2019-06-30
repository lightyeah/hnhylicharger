#include "stdint.h"
#include "string.h"

#include "hy_instance.h"
#include "hy_dbg.h"

#define HY_LOG_TAG  "chargetask"

static hy_instance_t* hy_instance=NULL;

static hy_chargetask_t* s_chargetask=NULL;


int hy_chargetask_init(void* hy_instance_handle)
{
	int ret = HY_OK;
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_chargetask = &(hy_instance->chargetask);
	memset(s_chargetask,0,sizeof(hy_chargetask_t));
	memset(&(hy_instance->hy_data),0,HY_CHARGETASK_DATA_NUM*sizeof(hy_chargetask_data));
	

	LOG_INFO_TAG(HY_LOG_TAG,"chargetask init done....");

	return ret;
}

int hy_chargetask_start(int controltype, void* ctx)
{
	int ret = HY_OK;
	if(s_chargetask == NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"***chargetask not init!!!");
		ret = HY_ERROR;
		goto err_exit;
	}
	
	if(s_chargetask->start_flag == HY_TRUE){
		return HY_OK;
	}
	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	LOG_INFO_TAG(HY_LOG_TAG,"chargetask start....");
	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	
	s_chargetask->start_time_ms = hy_time_now_ms();
	s_chargetask->lastcontrol_time_ms = hy_time_now_ms();
	
	switch(controltype){
		case HY_CONTROLSTYLE_CAN:
			s_chargetask->state = CHARGETASK_CAN;
			s_chargetask->max_chargetimeout_ms = 600*60*1000;/*10 hours*/
			s_chargetask->statestarttime_ms = hy_time_now_ms();
			break;
		case HY_CONTROLSTYLE_LOCAL:
			s_chargetask->state = CHARGETASK_LOCAL_ONE;
			hy_chargetask_local_turntostate(CHARGETASK_LOCAL_ONE);
			// hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_1);
			// hy_chargetask_setmaxvoltage_x10V(hy_instance->config.limitvoltage_1*10);
			// hy_chargetask_setmaxvoltage_x10V(hy_instance->config.currentrange*10);
			break;
		default:
			LOG_ERROR_TAG(HY_LOG_TAG,"***chargetask start controltype error!!");
			goto err_exit;
	}
		
	/*data init*/
	if(s_chargetask->aim_type == 0){
		LOG_WARN_TAG(HY_LOG_TAG,"!!!chargetask no set aim!!!");
		s_chargetask->aim_type = BMS_OBC_BCL_MODE_VOL;
		s_chargetask->aim_voltage_x10V = 0;/*for safe consideration*/
	}
	if(s_chargetask->max_voltage_x10V == 0){
		LOG_WARN_TAG(HY_LOG_TAG,"!!!chargetask no set max voltage!!!");
		s_chargetask->max_voltage_x10V  = hy_instance->config.voltagerange * 10;
	}
	if(s_chargetask->max_current_x10A == 0){
		LOG_WARN_TAG(HY_LOG_TAG,"!!!chargetask no set max voltage!!!");
		s_chargetask->max_voltage_x10V  = hy_instance->config.currentrange * 10;
	}
	
	s_chargetask->output_current_x10A = 0;
	s_chargetask->output_voltage_x10V = 0;
	s_chargetask->output_dac_value = 0 ;
	s_chargetask->total_chargepower_x10kwh = 0;
	s_chargetask->total_chargepower_mj = 0;
	s_chargetask->average_powerrate_x10w = 0;

	
	s_chargetask->start_flag = HY_TRUE;
	s_chargetask->end_flag = HY_FALSE;
	
	return ret;
	err_exit:
	s_chargetask->start_flag = HY_FALSE;
	s_chargetask->end_flag = HY_TRUE;
	s_chargetask->state = CHARGETASK_IDLE;
	return ret=HY_ERROR;
}

int hy_chargetask_stop(int stop_code,void* ctx)
{
	
	if(s_chargetask->end_flag  == HY_TRUE){
		return HY_OK;
	}
	LOG_INFO_TAG(HY_LOG_TAG,"**********************");
	LOG_INFO_TAG(HY_LOG_TAG,"chargetask stop.......");
	LOG_INFO_TAG(HY_LOG_TAG,"**********************");
	s_chargetask->start_flag = HY_FALSE;
	s_chargetask->end_flag = HY_TRUE;
	s_chargetask->state = CHARGETASK_IDLE;
	
	s_chargetask->end_time_ms = hy_time_now_ms();
	s_chargetask->total_charge_time_ms = 
		systime_elapse_ms(s_chargetask->start_time_ms);
	
	
	return HY_OK;
}


int hy_chargetask_emergencystop()
{
	LOG_ERROR_TAG(HY_LOG_TAG,"!!!!!!!!!!!!!!!!!!");
	LOG_ERROR_TAG(HY_LOG_TAG,"emrgecystop!!!!!!!");
	LOG_ERROR_TAG(HY_LOG_TAG,"!!!!!!emrgencystop");
	LOG_ERROR_TAG(HY_LOG_TAG,"!!!!!!!!!!!!!!!!!!");
	s_chargetask->start_flag = HY_FALSE;
	s_chargetask->end_flag = HY_TRUE;
	hy_set_output(0);
	s_chargetask->state = CHARGETASK_IDLE;
	return 0;
}



int hy_chargetask_setmaxvoltage_x10V(uint32_t voltage)
{
	s_chargetask->max_voltage_x10V = voltage;
	LOG_INFO_TAG(HY_LOG_TAG,"chargettask set max voltage limit = [%d]x0.1V",
								s_chargetask->max_voltage_x10V);
	return HY_OK;
}

int hy_chargetask_setmaxcurrent_x10A(uint32_t current)
{
	s_chargetask->max_current_x10A = current;
	LOG_INFO_TAG(HY_LOG_TAG,"chargettask set max current limit = [%d]x0.1A",
								s_chargetask->max_current_x10A);
	return HY_OK;
}

int hy_chargetask_setaim(uint8_t type, uint32_t value)
{
	LOG_INFO_TAG(HY_LOG_TAG,"chargettask set aim type [%d] value [%d]",type,value);
	switch (type){
		case BMS_OBC_BCL_MODE_VOL:
			s_chargetask->aim_type = type;
		  if(value > hy_instance->config.voltagerange){
				s_chargetask->aim_voltage_x10V = (hy_instance->config.voltagerange*10);
			}else{
				s_chargetask->aim_voltage_x10V = value*10;
			}
			break;
		case BMS_OBC_BCL_MODE_CUR:
			s_chargetask->aim_type = type;
			if(value > hy_instance->config.currentrange){
				s_chargetask->aim_current_x10A = (hy_instance->config.currentrange*10);
			}else{
				s_chargetask->aim_current_x10A = value*10;
			}
			break;
		default:
			s_chargetask->aim_type = 0;
			s_chargetask->aim_current_x10A = 0;
		    s_chargetask->aim_voltage_x10V = 0;
			break;
	}
	
	return HY_OK;
}

uint32_t hy_chargetask_getoutputvol_x10V()
{
	return s_chargetask->output_voltage_x10V;
}

uint32_t hy_chargetask_getoutputcur_x10A()
{
	return s_chargetask->output_current_x10A;
}

uint32_t hy_chargetask_getchargetime_min()
{
	uint32_t interval_ms;
	uint32_t interval_min;
	
	if(s_chargetask->start_flag == HY_FALSE){
		return 0;
	}		
	
	interval_ms = systime_elapse_ms(s_chargetask->start_time_ms);
	interval_min = interval_ms/(1000*60);
	
	return interval_min;
}

uint32_t hy_chargetask_gettotalpower_x10kwh()
{
		return s_chargetask->total_chargepower_x10kwh;
}

void hy_chargetask_set_output(uint32_t currentfb_x10A,uint32_t voltagefb_x10V,uint32_t aimtype)
{
	LOG_INFO_TAG(HY_LOG_TAG,"aimtype [%d] fbvol[%d] fbcur[%d]",aimtype,voltagefb_x10V,currentfb_x10A);
	LOG_INFO_TAG(HY_LOG_TAG,"aimcur [%d] aimvol [%d] outputvalue [%d]",
		s_chargetask->aim_current_x10A,s_chargetask->aim_voltage_x10V,s_chargetask->output_dac_value);

	if(s_chargetask->max_voltage_x10V == 0){
		s_chargetask->max_voltage_x10V = hy_instance->config.voltagerange;
	}
	if(voltagefb_x10V >= s_chargetask->max_voltage_x10V){
		LOG_WARN_TAG(HY_LOG_TAG,"output voltaga reach max [%d]x0.1V !!!",s_chargetask->max_voltage_x10V);
		s_chargetask->output_dac_value--;
		if(s_chargetask->output_dac_value <= 0)
			s_chargetask->output_dac_value = 0;
		hy_set_output(s_chargetask->output_dac_value);
		return;
	}

	if(s_chargetask->max_current_x10A == 0){
		s_chargetask->max_current_x10A = hy_instance->config.currentrange;
	}
	if(currentfb_x10A >= s_chargetask->max_current_x10A){
		LOG_WARN_TAG(HY_LOG_TAG,"output current reach max [%d]x0.1A !!!",s_chargetask->max_current_x10A);
		s_chargetask->output_dac_value--;
		if(s_chargetask->output_dac_value <= 0)
			s_chargetask->output_dac_value = 0;
		hy_set_output(s_chargetask->output_dac_value);
		return;
	}
	

	switch(aimtype){
		case BMS_OBC_BCL_MODE_VOL:
			if(voltagefb_x10V > s_chargetask->aim_voltage_x10V){
				s_chargetask->output_dac_value--;
				if(s_chargetask->output_dac_value<=0)
					s_chargetask->output_dac_value = 0;
			}else{
				s_chargetask->output_dac_value++;
				if(s_chargetask->output_dac_value>=1024)
					s_chargetask->output_dac_value = 1023;
			}
			break;
		case BMS_OBC_BCL_MODE_CUR:
			if(currentfb_x10A > s_chargetask->aim_current_x10A){
				s_chargetask->output_dac_value--;
				if(s_chargetask->output_dac_value <= 0)
					s_chargetask->output_dac_value = 0;
			}else{
				s_chargetask->output_dac_value++;
				if(s_chargetask->output_dac_value>=1024)
					s_chargetask->output_dac_value = 1023;
			}
			break;
	}
	hy_set_output(s_chargetask->output_dac_value);
}

void hy_chargetask_local_turntostate(hy_chargetask_state state)
{
	switch (state){
		case CHARGETASK_LOCAL_ONE:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_1);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_1*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_1_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		  LOG_INFO_TAG(HY_LOG_TAG,"set max voltage [%d] x0.1V",s_chargetask->max_voltage_x10V);
			LOG_INFO_TAG(HY_LOG_TAG,"set max current [%d]");
			LOG_INFO_TAG(HY_LOG_TAG,"");
			LOG_INFO_TAG(HY_LOG_TAG,"");
		break;
		case CHARGETASK_LOCAL_TWO:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_2);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_2*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_2_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_THREE:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargevoltage_3);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitcurrent_3*10);
			s_chargetask->max_current_x10A = (hy_instance->config.voltagerange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_3_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		default:
		break;
	}
	LOG_INFO_TAG(HY_LOG_TAG,"set max voltage [%d] x0.1V",s_chargetask->max_voltage_x10V);
	LOG_INFO_TAG(HY_LOG_TAG,"set max current [%d] x0.1A",s_chargetask->max_current_x10A);
	LOG_INFO_TAG(HY_LOG_TAG,"set max chargetimeout [%d] ms",s_chargetask->max_chargetimeout_ms);
	LOG_INFO_TAG(HY_LOG_TAG,"set state start time [%d] ms",s_chargetask->statestarttime_ms);
}
void hy_chargetask_main()
{
	static uint32_t monitortime_ms;
	static uint32_t aimtype;
	static uint32_t currentfb_x10A;
	static uint32_t voltagefb_x10V; 
	static chargetask_gui_msg gui_msg;
	
	aimtype = s_chargetask->aim_type;
	/*use CHARGETASK_CONTROL_INTERVAL to control chargetask regulate*/
	if((systime_elapse_ms(s_chargetask->lastcontrol_time_ms)%CHARGETASK_CONTROL_INTERVAL)){
		return;
	}
	s_chargetask->lastcontrol_time_ms = hy_time_now_ms();
	
	currentfb_x10A = hy_get_currentfb_x10A();
	voltagefb_x10V = hy_get_voltagefb_x10V(); 
	s_chargetask->output_current_x10A = currentfb_x10A;
	s_chargetask->output_voltage_x10V = voltagefb_x10V;
	
	if(voltagefb_x10V>=50){/*通过电池电压判断电池是否接入*/
		gui_msg.state |= HY_GUI_BATTERY_ON_MASK;
		if(s_chargetask->batter_flag == HY_BATTERY_DISCONNECT){
			if(hy_instance->config.controlstyle == HY_CONTROLSTYLE_LOCAL){
				hy_chargetask_start(HY_CONTROLSTYLE_LOCAL,NULL);
			}
		}
		s_chargetask->batter_flag = HY_BATTERY_CONNECT;
	}else{
		gui_msg.state &= ~HY_GUI_BATTERY_ON_MASK;
		if(s_chargetask->batter_flag == HY_BATTERY_CONNECT){
			hy_chargetask_stop(0,NULL);
		}		
		s_chargetask->state = CHARGETASK_IDLE;
		s_chargetask->batter_flag = HY_BATTERY_DISCONNECT;
		
	}

	if(s_chargetask->start_flag){
		s_chargetask->total_chargepower_mj +=currentfb_x10A*voltagefb_x10V*CHARGETASK_CONTROL_INTERVAL/100;
		s_chargetask->total_chargepower_x10kwh = s_chargetask->total_chargepower_mj/(1000*360);
	}
	
	
	switch (s_chargetask->state){
		case 	CHARGETASK_IDLE:
			gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
			if(s_chargetask->output_dac_value != 0){
				s_chargetask->output_dac_value--;
				if(s_chargetask->output_dac_value<=0)
					s_chargetask->output_dac_value = 0;
				hy_set_output(s_chargetask->output_dac_value);
			}
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask idle state... \r\n******get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			break;

	  case CHARGETASK_LOCAL_ONE:
			gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
			hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask local [one] started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			if(voltagefb_x10V >= (hy_instance->config.switchvoltage_1 *10)
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){
				/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [two]");
				s_chargetask->state = CHARGETASK_LOCAL_TWO;
				hy_chargetask_local_turntostate(CHARGETASK_LOCAL_TWO);
			}
			break;

	  case CHARGETASK_LOCAL_TWO:
			gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
	  		hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask local [two] started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			if(voltagefb_x10V >= (hy_instance->config.switchvoltage_2 *10)
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [three]");
				s_chargetask->state = CHARGETASK_LOCAL_THREE;
				hy_chargetask_local_turntostate(CHARGETASK_LOCAL_THREE);
			}
			break;

		case CHARGETASK_LOCAL_THREE:
			gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
			hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask local [three] started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			/*attention !!! 
			* local_state_three switch condition is smaller!!!
			*/
			if(currentfb_x10A <= (hy_instance->config.switchcurrent_3 *10)
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"[three is end ,change to idle]");
				hy_chargetask_stop(0,NULL);
				s_chargetask->state = CHARGETASK_IDLE;
			}
			break;

		case CHARGETASK_CAN:
			gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
			hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask can started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			if(systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){
				LOG_WARN_TAG(HY_LOG_TAG,"***can chargetask is timeout !!!");
				hy_chargetask_stop(0,NULL);
				s_chargetask->state = CHARGETASK_IDLE;
			}
			break;	

		case CHARGETASK_ERR:
			break;

		default:
			gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
			if(s_chargetask->output_dac_value != 0){
				s_chargetask->output_dac_value--;
				if(s_chargetask->output_dac_value<=0)
					s_chargetask->output_dac_value=0;
				hy_set_output(s_chargetask->output_dac_value);
			}
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_WARN_TAG(HY_LOG_TAG,
				"***chargetask default state!!! get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			break;
	}
	gui_msg.chargetime_min = hy_chargetask_getchargetime_min();
	gui_msg.currentx10A = hy_chargetask_getoutputcur_x10A();
	gui_msg.voltagex10V = hy_chargetask_getoutputvol_x10V();
	hy_emit_gui_msg(CHARGETASK_MSG,&gui_msg);
	
}


