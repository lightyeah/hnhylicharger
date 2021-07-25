/****************************************  File Info  ****************************************************
** File name:               input.h
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Vraliens
** Created date:            2018-3-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef  __INPUT_H__
#define  __INPUT_H__

#include "stdint.h"




typedef struct HYInputSignalStruct{
	uint16_t output_voltage_x10V;
	uint16_t output_current_x10A;
	
	uint16_t battery_voltage_x10V;
	
	uint16_t input_voltage1_x10V;
	uint16_t input_voltage2_x10V;
	uint16_t input_voltage3_x10V;

	uint16_t charger_module_temperature_x10degree;

	uint8_t charger_module_statu1;
	uint8_t charger_module_statu2;

	uint8_t charger_module_connected;
	uint8_t bms_module_connectd;
	uint8_t battery_module_connected;
	
}hy_inputsignal_t;


int hy_input_init(void* hy_instance_handle);




uint16_t hy_get_output_voltage_x10V(void);
uint16_t hy_get_output_current_x10A(void);

uint16_t hy_get_output_battery_voltage_x10V(void);


//todo
//uint16_t hy_get_input_voltage_x10V(void);

uint16_t hy_get_charger_module_temperatur_x10degree(void);

uint8_t hy_get_charger_module_statu1(void);
uint8_t hy_get_charger_module_statu2(void);

uint8_t hy_get_battery_connected(void);

uint8_t hy_get_charger_module_connected(void);


uint16_t hy_get_bms_request_voltage_x10V(void);

uint16_t hy_get_bms_request_current_x10A(void);

uint8_t hy_get_bms_status(void);

uint8_t hy_get_bms_control(void);

uint8_t hy_get_bms_soc(void);

uint8_t hy_get_bms_connected(void);



#endif
