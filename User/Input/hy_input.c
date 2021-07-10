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


//模块首先获取电压
uint16_t hy_get_output_voltage_x10V(void){
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
	hy_can_query_temperature_charger();
	s_inputsignal->charger_module_temperature_x10degree = hy_can_get_charger_module_temperature_x10degree();
	return s_inputsignal->charger_module_temperature_x10degree;
}


int16_t battery_voltage_last=0;
int16_t battery_voltage_diff=0;
uint32_t battery_check_time = 0;
uint32_t battery_check_flag = 0;

uint8_t hy_get_battery_connected(void){
	return hy_can_get_battery_connected();
}


uint8_t hy_get_charger_module_connected(void)
{
	return hy_can_get_charger_module_connected();
}

uint16_t hy_get_bms_request_voltage_x10V(void)
{
	return hy_can_get_bms_set_voltage_10V();
}

uint16_t hy_get_bms_request_current_x10A(void)
{
	return hy_can_get_bms_set_current_10A();
}

uint8_t hy_get_bms_status(void)
{	
	uint8_t status;
	if(hy_can_get_bms_mode()!=0)//加热模式
		{
			status = hy_can_get_bms_status()&~(uint8_t)(1<<2);//忽略温度过低
		}else{
			status = hy_can_get_bms_status();
			}
	return status;
}



uint8_t hy_get_bms_control(void)
{
	return hy_can_get_bms_control();
}


uint8_t hy_get_bms_connected(void)
{
	return hy_can_get_bms_connected();
}






