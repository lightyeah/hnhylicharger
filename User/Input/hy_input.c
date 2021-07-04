#include "stdint.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_pinsel.h"


#include "systen_delay.h"
#include "hy_instance.h"
#include "hy_dbg.h"
#include "hy_input.h"
#include "hy_systime.h"

#define  HY_LOG_TAG    "input"

static hy_instance_t *hy_instance = NULL;

hy_inputsignal_t *s_inputsignal;

int hy_input_init(void* hy_instance_handle)
{
	int ret = HY_OK;

	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_inputsignal = &(hy_instance->inputsignal);
	/*software init*/
	s_inputsignal->output_current_x10A = 0;
	s_inputsignal->output_voltage_x10V = 0;
	s_inputsignal->battery_voltage_x10V = 0;
	s_inputsignal->charger_module_temperature_x10degree = 0;
	s_inputsignal->charger_module_statu1 = 0;/*todo*/
	s_inputsignal->charger_module_statu2 = 0;
	s_inputsignal->charger_module_connected = HY_FALSE;
	s_inputsignal->bms_module_connectd = HY_FALSE;
	s_inputsignal->battery_module_connected = HY_FALSE;
	


	LOG_INFO_TAG(HY_LOG_TAG,"hy init adc done!");
	
	return ret;
}


//YRK模块首先获取电压
uint16_t hy_get_output_voltage_x10V(void){
	hy_can_control_query_YRKcharger();//需要先获取
	s_inputsignal->output_voltage_x10V = hy_can_get_output_voltage_x10V();
	s_inputsignal->battery_voltage_x10V = s_inputsignal->output_voltage_x10V;
	return s_inputsignal->output_voltage_x10V;
}

uint16_t hy_get_output_current_x10A(void){
	s_inputsignal->output_current_x10A = hy_can_get_output_current_x10A();
	return s_inputsignal->output_current_x10A;
}

uint8_t hy_get_charger_module_statu1(void){
	s_inputsignal->charger_module_statu1 = hy_can_get_charger_module_statu1();
	return s_inputsignal->charger_module_statu1;
}

uint8_t hy_get_charger_module_statu2(void){
	s_inputsignal->charger_module_statu2 = hy_can_get_charger_module_statu2();
	return s_inputsignal->charger_module_statu2;

}



uint16_t hy_get_output_battery_voltage_x10V(void){
	s_inputsignal->battery_voltage_x10V = s_inputsignal->output_voltage_x10V;
	return s_inputsignal->output_voltage_x10V;
}


//todo
//uint16_t hy_get_input_voltage_x10V(void);

uint16_t hy_get_charger_module_temperatur_x10degree(void){
	hy_can_query_temperature_yrkcharger();
	s_inputsignal->charger_module_temperature_x10degree = hy_can_get_charger_module_temperature_x10degree();
	return s_inputsignal->charger_module_temperature_x10degree;
}


int16_t battery_voltage_last=0;
int16_t battery_voltage_diff=0;
uint32_t battery_check_time = 0;
uint32_t battery_check_flag = 0;
uint8_t hy_get_battery_connected(void){
	//LOG_DEBUG_TAG(HY_LOG_TAG, "GET BATTERY [%d]\r\n",s_inputsignal->battery_voltage_x10V);

	if(s_inputsignal->battery_module_connected==HY_FALSE&&s_inputsignal->battery_voltage_x10V>150){
		s_inputsignal->battery_module_connected = HY_TRUE;
	}
	if (s_inputsignal->battery_module_connected==HY_TRUE&&s_inputsignal->battery_voltage_x10V<=150){
		s_inputsignal->battery_module_connected = HY_FALSE;
	}
	return s_inputsignal->battery_module_connected;



//	if(s_inputsignal->battery_module_connected==HY_FALSE&&s_inputsignal->battery_voltage_x10V>50){
//		if(battery_check_flag==0){
//			battery_check_flag=1;
//			battery_check_time=hy_time_now_ms();
//			battery_voltage_diff = 0;
//			battery_voltage_last = (int16_t)s_inputsignal->battery_voltage_x10V;
//		}
//		if(battery_check_flag==1){
//			battery_voltage_diff += (int16_t)s_inputsignal->battery_voltage_x10V-battery_voltage_last;
//			battery_voltage_last = (int16_t)s_inputsignal->battery_voltage_x10V;
//			LOG_DEBUG_TAG(HY_LOG_TAG, "check battery [%d]", battery_voltage_diff);
//			if (systime_elapse_ms(battery_check_time)>=5000){
//				if(battery_voltage_diff>=-5){
//					s_inputsignal->battery_module_connected=HY_TRUE;
//					battery_check_flag=0;
//					return HY_TRUE;
//				}else{
//					battery_check_flag=0;
//				}
//			}
//		}
//	}
//	if (s_inputsignal->battery_module_connected==HY_TRUE){
//		if(battery_check_flag==0){
//			battery_check_flag=1;
//			battery_check_time=hy_time_now_ms();
//			battery_voltage_diff = 0;
//			battery_voltage_last = (int16_t)s_inputsignal->battery_voltage_x10V;
//		}
//		if(battery_check_flag==1){
//			battery_voltage_diff += (int16_t)s_inputsignal->battery_voltage_x10V-battery_voltage_last;
//			battery_voltage_last = (int16_t)s_inputsignal->battery_voltage_x10V;
//			if (systime_elapse_ms(battery_check_time)>=5000){
//				if(battery_voltage_diff>=-5){
//					s_inputsignal->battery_module_connected=HY_TRUE;
//					battery_check_flag=0;
//					return HY_TRUE;
//				}else{
//					battery_check_flag=0;
//					s_inputsignal->battery_module_connected=HY_FALSE;
//					return HY_FALSE;
//				}
//			}
//		}
//	}
//	return s_inputsignal->battery_module_connected;
}








