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

	s_chargetask->controltype = HY_CONTROLSTYLE_CAN;//hy_instance->config.controlstyle;
	s_chargetask->machine_start_flag = HY_TRUE;

	LOG_INFO_TAG(HY_LOG_TAG,"chargetask init done....");

	return ret;
}

int hy_chargetask_start(int controltype, void* ctx)
{
	int ret = HY_OK;

//	if (s_chargetask->gui_msg.state & HY_GUI_CHARGETASK_END_MASK){//充电完成，不能启动
//		return HY_OK;
//	}
//
//	if(s_chargetask->start_flag == HY_TRUE){//充电中不能启动
//		return HY_OK;
//	}

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
			s_chargetask->controltype = HY_CONTROLSTYLE_CAN;
			s_chargetask->gui_msg.workstate = HY_GUI_CAN_ON_MASK;
			break;
		case HY_CONTROLSTYLE_LOCAL:
			s_chargetask->state = CHARGETASK_LOCAL_ONE;
			hy_chargetask_local_turntostate(CHARGETASK_LOCAL_ONE);
			s_chargetask->controltype = HY_CONTROLSTYLE_LOCAL;

			break;
		default:
			break;
	}
		
	/*data init*/
	if(s_chargetask->aim_type == 0){
		LOG_WARN_TAG(HY_LOG_TAG,"!!!chargetask no set aim!!!");
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

	s_chargetask->total_chargepower_x10kwh = 0;
	s_chargetask->total_chargepower_mj = 0;
	s_chargetask->average_powerrate_x10w = 0;

	
	s_chargetask->start_flag = HY_TRUE;
	s_chargetask->end_flag = HY_FALSE;

	hy_set_start_output();
	
	return ret;
	
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
	
	hy_set_stop_output();
	
	switch (stop_code){
		case CHARGETASK_CAN_STOP_CODE:
			break;
				
		case CHARGETASK_LOCAL_NORMAL_STOP_CODE:
			s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_END_MASK;		
			break;
		case CHARGETASK_BUTTON_STOP_CODE:
			s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_STOP_MASK;			 
			break;
		default:
			break;
	}

	s_chargetask->start_flag = HY_FALSE;
	s_chargetask->end_flag = HY_TRUE;
	s_chargetask->state = CHARGETASK_IDLE;
	
	s_chargetask->end_time_ms = hy_time_now_ms();
	s_chargetask->total_charge_time_ms = systime_elapse_ms(s_chargetask->start_time_ms);
	
	
	return HY_OK;
}

int hy_chargetask_pause(int stop_code,void* ctx){
	s_chargetask->pause_state = s_chargetask->state;
	s_chargetask->state = CHARGETASK_PAUSE;
	return 0;
}


int hy_chargetask_continue(int stop_code,void* ctx){
	hy_set_start_output();
	s_chargetask->state = s_chargetask->pause_state;
	s_chargetask->pause_state = CHARGETASK_IDLE;
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

int hy_chargetask_setaim(uint16_t current_x10A, uint16_t voltage_x10V)
{
	LOG_INFO_TAG(HY_LOG_TAG,"chargettask set current [%d] voltage [%d]",current_x10A,voltage_x10V);
	
	hy_set_charger_output(current_x10A, voltage_x10V);
	
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


void hy_chargetask_local_turntostate(hy_chargetask_state state)
{
	switch (state){
		case CHARGETASK_LOCAL_ONE:
			hy_chargetask_setaim(hy_instance->config.chargecurrent_1*10,hy_instance->config.limitvoltage_1*10);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_1*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_1_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_TWO:
			hy_chargetask_setaim(hy_instance->config.chargecurrent_2*10,s_chargetask->output_voltage_x10V+100);
			s_chargetask->max_voltage_x10V = (hy_instance->config.limitvoltage_2*10);
			s_chargetask->max_current_x10A = (hy_instance->config.currentrange*10);
			s_chargetask->max_chargetimeout_ms = (hy_instance->config.chargetimeout_2_min*60*1000);
			s_chargetask->statestarttime_ms = hy_time_now_ms();
		break;
		case CHARGETASK_LOCAL_THREE:
			hy_chargetask_setaim(hy_instance->config.limitcurrent_3*10,hy_instance->config.chargevoltage_3*10);
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
	static uint32_t state_two_aim_voltage=0;
	
	/*use CHARGETASK_CONTROL_INTERVAL to control chargetask regulate*/
	if((systime_elapse_ms(s_chargetask->lastcontrol_time_ms)%CHARGETASK_CONTROL_INTERVAL)){
		return;
	}
	s_chargetask->lastcontrol_time_ms = hy_time_now_ms();
	
	s_chargetask->output_voltage_x10V = hy_get_output_voltage_x10V();
	s_chargetask->output_current_x10A = hy_get_output_current_x10A();

	//LOG_DEBUG_TAG(HY_LOG_TAG, "chargertask battercheck [%d] [%d]\r\n",hy_get_battery_connected(),s_chargetask->battery_flag);
	//LOG_DEBUG_TAG(HY_LOG_TAG, "chargertask sytle [%d]\r\n",s_chargetask->controltype);



	//LOCAL
	if(s_chargetask->controltype == HY_CONTROLSTYLE_LOCAL){

		if (hy_get_charger_module_connected()==HY_FALSE)
			{
				goto chargetask_exit;
			}
	
		if(hy_get_battery_connected()&&s_chargetask->battery_flag == HY_BATTERY_DISCONNECT){//电池未连接状态下检测到电池
			hy_chargetask_start(HY_CONTROLSTYLE_LOCAL,NULL);//本地充电
			s_chargetask->battery_flag = HY_BATTERY_CONNECT;
		}else if(!hy_get_battery_connected()&&s_chargetask->battery_flag == HY_BATTERY_CONNECT){//电池连接状态下检测到电池断开
			hy_chargetask_stop(CHARGETASK_BATTERY_DISCONNECT_STOP_CODE,NULL);
			s_chargetask->battery_flag = HY_BATTERY_DISCONNECT;
		}
		

		if(s_chargetask->start_flag){
			s_chargetask->total_chargepower_mj +=s_chargetask->output_current_x10A*s_chargetask->output_voltage_x10V*CHARGETASK_CONTROL_INTERVAL/100;
			s_chargetask->total_chargepower_x10kwh = s_chargetask->total_chargepower_mj/(1000*360);
		}
		
		
		switch (s_chargetask->state){
			case 	CHARGETASK_IDLE://空闲状态
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_IDLE;
				LOG_DEBUG_TAG(HY_LOG_TAG,"idletest");
				hy_set_stop_output();
				
				if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
					LOG_INFO_TAG(HY_LOG_TAG,
					"***chargetask idle state [%d]******get voltage [%d]x0.1V current [%d]x0.1A",
					s_chargetask->battery_flag,s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
					monitortime_ms = hy_time_now_ms();
				}
				
				break;

		  case CHARGETASK_LOCAL_ONE://第一阶段
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_ON_MASK;
				hy_chargetask_setaim(hy_instance->config.chargecurrent_1*10,hy_instance->config.limitvoltage_1*10);
				if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
					LOG_INFO_TAG(HY_LOG_TAG,
					"***chargetask local [one] get voltage [%d]x0.1V current [%d]x0.1A",
					s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
					monitortime_ms = hy_time_now_ms();
				}
				if(s_chargetask->output_voltage_x10V >= (hy_instance->config.switchvoltage_1 *10)//跳转电压
					|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){
					/*change to local [two]*/
					LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [two]");
					s_chargetask->state = CHARGETASK_LOCAL_TWO;
					hy_chargetask_local_turntostate(CHARGETASK_LOCAL_TWO);
				}
				break;

		  case CHARGETASK_LOCAL_TWO://第二阶段
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_ON_MASK;
				if (s_chargetask->output_voltage_x10V <= hy_instance->config.limitvoltage_2 - 50)
				{
					state_two_aim_voltage = s_chargetask->output_voltage_x10V+40;
				}else{
					state_two_aim_voltage = hy_instance->config.limitvoltage_2*10;
				}
		  		hy_chargetask_setaim(hy_instance->config.chargecurrent_2*10,state_two_aim_voltage);
				if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
					LOG_INFO_TAG(HY_LOG_TAG,
					"***chargetask local [two] get voltage [%d]x0.1V current [%d]x0.1A",
					s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
					monitortime_ms = hy_time_now_ms();
				}
				if(s_chargetask->output_voltage_x10V >= (hy_instance->config.switchvoltage_2 *10)//跳转电压
					|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
					LOG_INFO_TAG(HY_LOG_TAG,"switch to local state [three]");
					s_chargetask->state = CHARGETASK_LOCAL_THREE;
					hy_chargetask_local_turntostate(CHARGETASK_LOCAL_THREE);
				}
				
				break;

			case CHARGETASK_LOCAL_THREE://第三阶段
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_ON_MASK;
				hy_chargetask_setaim(hy_instance->config.limitcurrent_3*10,hy_instance->config.chargevoltage_3*10);
				if(systime_elapse_ms(monitortime_ms)>=CHARGETASK_MONITOR_INTERVAL){
					LOG_INFO_TAG(HY_LOG_TAG,
					"***chargetask local [three]  get voltage [%d]x0.1V current [%d]x0.1A",
					s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);
					monitortime_ms = hy_time_now_ms();
				}
				/*attention !!! 
				* local_state_three switch condition is smaller!!!
				*/
				if(s_chargetask->output_current_x10A <= (hy_instance->config.switchcurrent_3 *10)//跳转电流
					|| systime_elapse_ms(s_chargetask->statestarttime_ms) >= s_chargetask->max_chargetimeout_ms){/*change to local [two]*/
					LOG_INFO_TAG(HY_LOG_TAG,"[three is end ,change to idle]");
					hy_chargetask_stop(CHARGETASK_LOCAL_NORMAL_STOP_CODE,NULL);
					s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_END_MASK;
					s_chargetask->state = CHARGETASK_IDLE;
				}
				break;

			case CHARGETASK_CAN://通讯控制模式
				break;	
			case CHARGETASK_PAUSE://暂停状态
				hy_set_stop_output();
				break;
			case CHARGETASK_ERR:
				break;

			default:
				break;
		}


		
	}else if(s_chargetask->controltype == HY_CONTROLSTYLE_CAN){


		// can control
		if(hy_get_charger_module_connected()==HY_FALSE){
			goto chargetask_exit;
			}
		if(hy_get_battery_connected()==HY_FALSE){
			goto chargetask_exit;
			}
		if(hy_get_bms_connected()==HY_FALSE){
			goto chargetask_exit;
			}
		
		LOG_DEBUG_TAG(HY_LOG_TAG, "bms control [%d]  status [%d]\r\n",hy_get_bms_control(),hy_get_bms_status());
		if (hy_get_bms_control()==1 && 
				hy_get_bms_status() == 0 ){//充电完成
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_END_MASK;
				hy_set_stop_output();
		}else if(hy_get_bms_status() != 0){//bms 充电异常
				s_chargetask->gui_msg.workstate = HY_GUI_CHARGETASK_STOP_MASK;
				hy_set_stop_output();
				goto chargetask_exit;
		}else if(hy_get_bms_status()==0 && hy_get_bms_control()==0){//正常通信充电
			s_chargetask->gui_msg.workstate = HY_GUI_CAN_ON_MASK;
			LOG_DEBUG_TAG(HY_LOG_TAG, "bms control [%d]  status [%d]\r\n",hy_get_bms_request_voltage_x10V(),hy_get_bms_request_current_x10A());
			hy_set_charger_output(hy_get_bms_request_voltage_x10V(), hy_get_bms_request_current_x10A());
		}
		hy_set_data_broadcast_to_bms(s_chargetask->output_voltage_x10V,s_chargetask->output_current_x10A);

	}


chargetask_exit:
	s_chargetask->gui_msg.charge_module_connected = hy_get_charger_module_connected();
	s_chargetask->gui_msg.battery_connected = hy_get_battery_connected();
	s_chargetask->gui_msg.bms_connected = hy_get_bms_connected();
	
	s_chargetask->gui_msg.chargetime_min = hy_chargetask_getchargetime_min();
	s_chargetask->gui_msg.currentx10A = hy_chargetask_getoutputcur_x10A();
	s_chargetask->gui_msg.voltagex10V = hy_chargetask_getoutputvol_x10V();
	
	s_chargetask->gui_msg.charger_statu1 = hy_get_charger_module_statu1();
	s_chargetask->gui_msg.charger_statu2 = hy_get_charger_module_statu2();
	
	s_chargetask->gui_msg.bms_status = hy_get_bms_status();
	if(s_chargetask->gui_msg.charger_statu2&0xfa
		||s_chargetask->gui_msg.charger_statu1&0x03
		||s_chargetask->gui_msg.bms_status != 0 ){
		LOG_DEBUG_TAG(HY_LOG_TAG, "EMIT GUI ERROR");
		s_chargetask->gui_msg.errorstate = HY_GUI_ERR_MASK;
		hy_set_stop_output();
	}else{
		s_chargetask->gui_msg.errorstate = 0;
		//hy_set_start_output();
	}
	hy_emit_gui_msg(CHARGETASK_MSG,&s_chargetask->gui_msg);

	
}



