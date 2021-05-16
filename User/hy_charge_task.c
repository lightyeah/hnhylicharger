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

	s_chargetask->controltype = hy_instance->config.controlstyle;
	s_chargetask->machine_start_flag = HY_TRUE;

	LOG_INFO_TAG(HY_LOG_TAG,"chargetask init done....");

	return ret;
}

int hy_chargetask_start(int controltype, void* ctx)
{
	int ret = HY_OK;

	if (ctx != NULL){
		if (*(uint32_t*)ctx == CHARGETASK_BUTTON_START_CODE){
			s_chargetask->machine_start_flag = HY_TRUE;
			controltype = hy_instance->config.controlstyle;
		}else if (*(uint32_t*)ctx == CHARGETASK_COLDDONW_START_CODE){
			controltype = hy_instance->config.controlstyle;
		}
	}

	if (s_chargetask->machine_start_flag != HY_TRUE){
		LOG_WARN_TAG(HY_LOG_TAG,"not ready");
		return HY_ERROR;
	}

	if (s_chargetask->gui_msg.state & HY_GUI_ERR_OVERHEAT_MASK){
		LOG_WARN_TAG(HY_LOG_TAG,"overheat!!");
		return HY_ERROR;
	}

	if (s_chargetask->gui_msg.state & HY_GUI_CHARGETASK_END_MASK){
		return HY_OK;
	}

	if(s_chargetask->start_flag == HY_TRUE){
		return HY_OK;
	}

	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	LOG_INFO_TAG(HY_LOG_TAG,"chargetask start....");
	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	
	s_chargetask->start_time_ms = hy_time_now_ms();
	s_chargetask->lastcontrol_time_ms = hy_time_now_ms();

	hy_output_set_charge_relay(HY_OUTPUT_RELAY_ON);
	
	switch(controltype){
		case HY_CONTROLSTYLE_CAN:
			s_chargetask->state = CHARGETASK_CAN;
			s_chargetask->max_chargetimeout_ms = 600*60*1000;/*10 hours*/
			s_chargetask->statestarttime_ms = hy_time_now_ms();
			s_chargetask->controltype = HY_CONTROLSTYLE_CAN;
			s_chargetask->gui_msg.state |= HY_GUI_CAN_ON_MASK;
			break;
		case HY_CONTROLSTYLE_LOCAL:
			s_chargetask->state = CHARGETASK_LOCAL_ONE;
			hy_chargetask_local_turntostate(CHARGETASK_LOCAL_ONE);
			s_chargetask->controltype = HY_CONTROLSTYLE_LOCAL;
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

//停止充电
int hy_chargetask_stop(int stop_code,void* ctx)
{
	
	if (s_chargetask->end_flag == HY_TRUE){
		return HY_OK;
	}

	LOG_INFO_TAG(HY_LOG_TAG,"**********************");
	LOG_INFO_TAG(HY_LOG_TAG,"chargetask stop...[%d]",stop_code);
	LOG_INFO_TAG(HY_LOG_TAG,"**********************");

	switch (stop_code){
		case CHARGETASK_CAN_STOP_CODE:
			if (ctx != NULL){
				if (*(int*)ctx == HY_CAN_OBC_STOP_CANTIMEOUT){
					s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
					s_chargetask->gui_msg.state &= ~HY_GUI_CAN_ON_MASK;
				}else if(*(int*)ctx == HY_CAN_BMS_STOP){
					/*todo ???*/
					s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
					s_chargetask->gui_msg.state |= HY_GUI_CAN_ON_MASK;
					s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_END_MASK;
				}/*todo can timeout 600min*/
			}
			break;
		case CHARGETASK_LOCAL_NORMAL_STOP_CODE:
			s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
			s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_END_MASK;		
			break;
		case CHARGETASK_BATTERY_DISCONNECT_STOP_CODE:
			s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
			break;
		case CHARGETASK_ERR_STOP_CODE:
			if (ctx != NULL){
				if (*(int*)ctx == OBC_BMS_CST_HEAT_ERR(1)){//overheat error
					if (s_chargetask->controltype == HY_GUI_CAN_ON_MASK){
						hy_can_stop(OBC_BMS_CST_HEAT_ERR(1),NULL);
					}	
					s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
					s_chargetask->gui_msg.state |= HY_GUI_ERR_OVERHEAT_MASK;
				}
			}
			break;
		case CHARGETASK_BUTTON_STOP_CODE:
			s_chargetask->machine_start_flag = HY_FALSE;
			s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
			if (s_chargetask->controltype == HY_GUI_CAN_ON_MASK){
				hy_can_stop(OBC_BMS_CST_MANUAL_STOP(1),NULL);
			}
			 
			break;
		default:
			break;
	}

	s_chargetask->start_flag = HY_FALSE;
	s_chargetask->end_flag = HY_TRUE;
	s_chargetask->state = CHARGETASK_IDLE;
	
	s_chargetask->end_time_ms = hy_time_now_ms();
	s_chargetask->total_charge_time_ms = 
		systime_elapse_ms(s_chargetask->start_time_ms);
	
	hy_output_set_charge_relay(HY_OUTPUT_RELAY_OFF);
	
	return HY_OK;
}

//紧急停止充电
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
		  if(value > (hy_instance->config.voltagerange*10)){
				s_chargetask->aim_voltage_x10V = (hy_instance->config.voltagerange*10);
			}else{
				s_chargetask->aim_voltage_x10V = value;
			}
			break;
		case BMS_OBC_BCL_MODE_CUR:
			s_chargetask->aim_type = type;
			if(value > (hy_instance->config.currentrange*10)){
				s_chargetask->aim_current_x10A = (hy_instance->config.currentrange*10);
			}else{
				s_chargetask->aim_current_x10A = value;
			}
			break;
		default:
			s_chargetask->aim_type = 0;
			s_chargetask->aim_current_x10A = 0;
		    s_chargetask->aim_voltage_x10V = 0;
			break;
	}
	LOG_INFO_TAG(HY_LOG_TAG,"chargettask set aim type [%d] set value [%d] [%d]",type,s_chargetask->aim_voltage_x10V,s_chargetask->aim_current_x10A);
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

//设置输出电压电流
void hy_chargetask_set_output(uint32_t currentfb_x10A,uint32_t voltagefb_x10V,uint32_t aimtype)
{

	uint32_t aimvalue;
	uint32_t fbvalue;
	uint32_t diffvalue;
	uint32_t outputvalue_increment = 0;
	uint32_t outputvalue = s_chargetask->output_dac_value;
	static uint32_t stablecontroltime_ms = 0;

	LOG_INFO_TAG(HY_LOG_TAG,"fbvol[%d] fbcur[%d]",voltagefb_x10V,currentfb_x10A);
	LOG_INFO_TAG(HY_LOG_TAG,"aimtype [%d]  aimcur [%d] aimvol [%d] outputvalue [%d]",aimtype,
		s_chargetask->aim_current_x10A,s_chargetask->aim_voltage_x10V,s_chargetask->output_dac_value);
	
	switch (s_chargetask->state){
		case CHARGETASK_LOCAL_ONE:
			if((voltagefb_x10V+100)<480){
				hy_can_control_GWcharger(480, hy_instance->config.chargecurrent_1*10);
			}else{
				hy_can_control_GWcharger(voltagefb_x10V+100, hy_instance->config.chargecurrent_1*10);
			}
// 			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_1*10);
// 			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_1*10);
// 			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
// 			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_1_min*60*1000);
// 			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_TWO:
			hy_can_control_GWcharger(hy_instance->config.limitvoltage_2*10, hy_instance->config.chargecurrent_2*10);
// 			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_2*10);
// 			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_2*10);
// 			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
// 			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_2_min*60*1000);
// 			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_THREE:
			hy_can_control_GWcharger(hy_instance->config.chargevoltage_3*10, hy_instance->config.limitcurrent_3*10);
// 			hy_chargetask_setaim(BMS_OBC_BCL_MODE_VOL,hy_instance->config.chargevoltage_3*10);
// 			s_chargetask->max_voltage_x10V = (hy_instance->config.limitcurrent_3*10);
// 			s_chargetask->max_current_x10A = (hy_instance->config.voltagerange*10);
// 			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_3_min*60*1000);
// 			s_chargetask->statestarttime_ms = hy_time_now_ms();
		default:
		break;
	}

}


void hy_chargetask_local_turntostate(hy_chargetask_state state)
{
	switch (state){
		case CHARGETASK_LOCAL_ONE:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_1*10);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_1*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_1_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_TWO:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,hy_instance->config.chargecurrent_2*10);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_2*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_2_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_THREE:
			hy_chargetask_setaim(BMS_OBC_BCL_MODE_VOL,hy_instance->config.chargevoltage_3*10);
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

//充电控制主循环
void hy_chargetask_main()
{
	static uint32_t monitortime_ms;
	static uint32_t aimtype;
	static uint32_t currentfb_x10A;
	static uint32_t voltagefb_x10V; 
	static uint32_t battery_on_time_ms;
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
	
	if(!hy_can_GWcharger_batteryoff()){/*通过电池电压判断电池是否接入*/
		s_chargetask->gui_msg.state |= HY_GUI_BATTERY_ON_MASK;
		if(s_chargetask->battery_flag == HY_BATTERY_DISCONNECT){
			if(hy_instance->config.controlstyle == HY_CONTROLSTYLE_LOCAL){
				hy_chargetask_start(HY_CONTROLSTYLE_LOCAL,NULL);
			}else if(hy_instance->config.controlstyle == HY_CONTROLSTYLE_CAN){
				hy_can_restart(0,NULL);
			}
		}
		s_chargetask->battery_flag = HY_BATTERY_CONNECT;
		battery_on_time_ms = hy_time_now_ms();
	}else{
		if ( systime_elapse_ms(battery_on_time_ms) >= 500 ){/*battery disconnected*/				
			s_chargetask->gui_msg.state &= ~HY_GUI_BATTERY_ON_MASK;
			if(s_chargetask->battery_flag == HY_BATTERY_CONNECT){
				hy_chargetask_stop(CHARGETASK_BATTERY_DISCONNECT_STOP_CODE,NULL);
				s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_END_MASK;
			}		
			s_chargetask->state = CHARGETASK_IDLE;
			s_chargetask->battery_flag = HY_BATTERY_DISCONNECT;
		}
	}

	if(s_chargetask->start_flag){
		s_chargetask->total_chargepower_mj +=currentfb_x10A*voltagefb_x10V*CHARGETASK_CONTROL_INTERVAL/100;
		s_chargetask->total_chargepower_x10kwh = s_chargetask->total_chargepower_mj/(1000*360);
	}
	
	
	switch (s_chargetask->state){
		case 	CHARGETASK_IDLE://空闲状态
			s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;

			if(s_chargetask->output_dac_value != 0){
				s_chargetask->output_dac_value = s_chargetask->output_dac_value - 20;
				if(s_chargetask->output_dac_value <= 0 )
					s_chargetask->output_dac_value = 0;		
			}
			LOG_ERROR_TAG(HY_LOG_TAG,"idletest [%d]",s_chargetask->output_dac_value);
			hy_set_output(s_chargetask->output_dac_value);
			
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask idle state... \r\n******get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				if(s_chargetask->controltype == HY_CONTROLSTYLE_CAN){
					if (!hy_can_connected()){/*can disconnected*/
						s_chargetask->gui_msg.state &= ~HY_GUI_CAN_ON_MASK;
					}else{
						s_chargetask->gui_msg.state |= HY_GUI_CAN_ON_MASK;
					}
					LOG_INFO_TAG(HY_LOG_TAG,"can connected status = [%d]",hy_can_connected());
				}
				monitortime_ms = hy_time_now_ms();
			}

			break;

	  case CHARGETASK_LOCAL_ONE://第一阶段
			s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
			hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask local [one] started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			if(voltagefb_x10V >= (hy_instance->config.switchvoltage_1 *10)//跳转电压
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){
				/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [two]");
				s_chargetask->state = CHARGETASK_LOCAL_TWO;
				hy_chargetask_local_turntostate(CHARGETASK_LOCAL_TWO);
			}
			break;

	  case CHARGETASK_LOCAL_TWO://第二阶段
			s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
	  		hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);
			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask local [two] started ... get voltage [%d]x0.1V current [%d]x0.1A",
				s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
				monitortime_ms = hy_time_now_ms();
			}
			if(voltagefb_x10V >= (hy_instance->config.switchvoltage_2 *10)//跳转电压
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [three]");
				s_chargetask->state = CHARGETASK_LOCAL_THREE;
				hy_chargetask_local_turntostate(CHARGETASK_LOCAL_THREE);
			}
			break;

		case CHARGETASK_LOCAL_THREE://第三阶段
			s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
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
			if(currentfb_x10A <= (hy_instance->config.switchcurrent_3 *10)//跳转电流
				|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
				LOG_INFO_TAG(HY_LOG_TAG,"[three is end ,change to idle]");
				hy_chargetask_stop(CHARGETASK_LOCAL_NORMAL_STOP_CODE,NULL);
				s_chargetask->state = CHARGETASK_IDLE;
			}
			break;

		case CHARGETASK_CAN://通讯控制模式

			s_chargetask->gui_msg.state |= HY_GUI_CHARGETASK_ON_MASK;
			s_chargetask->gui_msg.state |= HY_GUI_CAN_ON_MASK;

			hy_chargetask_set_output(currentfb_x10A,voltagefb_x10V,aimtype);

			if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
				LOG_INFO_TAG(HY_LOG_TAG,
				"***chargetask can start ... get voltage [%d]x0.1V current [%d]x0.1A GUI STATE [%d]",
				s_chargetask->output_voltage_x10V,
				s_chargetask->output_current_x10A,
				s_chargetask->gui_msg.state);

				monitortime_ms = hy_time_now_ms();
			}
			if(systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){
				LOG_WARN_TAG(HY_LOG_TAG,"***can chargetask is timeout !!!");
				hy_chargetask_stop(CHARGETASK_CAN_STOP_CODE,NULL);//todo there can not be NULL!!!!!
				s_chargetask->state = CHARGETASK_IDLE;
			}
			break;	

		case CHARGETASK_ERR:
			break;

		default:
			s_chargetask->gui_msg.state &= ~HY_GUI_CHARGETASK_ON_MASK;
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
	s_chargetask->gui_msg.chargetime_min = hy_chargetask_getchargetime_min();
	s_chargetask->gui_msg.currentx10A = hy_chargetask_getoutputcur_x10A();
	s_chargetask->gui_msg.voltagex10V = hy_chargetask_getoutputvol_x10V();
	hy_emit_gui_msg(CHARGETASK_MSG,&s_chargetask->gui_msg);

	
}

void hy_chargetask_setoverheat(void)
{
	s_chargetask->gui_msg.state |= HY_GUI_ERR_OVERHEAT_MASK;
}

void hy_chargetask_clearoverheat(void)
{
	s_chargetask->gui_msg.state &= ~HY_GUI_ERR_OVERHEAT_MASK;
}

int hy_chargetask_getoverheat(void)
{
	if (s_chargetask->gui_msg.state & HY_GUI_ERR_OVERHEAT_MASK){
		return 1;
	}else{
		return 0;
	}
}

void hy_test_key_up(void)
{
//	s_chargetask->output_dac_value += 10;
//	hy_set_output(s_chargetask->output_dac_value);
//	LOG_WARN_TAG(HY_LOG_TAG,"hy_test_key_up test s_chargetask->output_dac_value[%d]",s_chargetask->output_dac_value);
}


void hy_test_key_down(void)
{
//	s_chargetask->output_dac_value -= 10;
//	hy_set_output(s_chargetask->output_dac_value);
//	LOG_WARN_TAG(HY_LOG_TAG,"hy_test_key_up test s_chargetask->output_dac_value[%d]",s_chargetask->output_dac_value);
}


