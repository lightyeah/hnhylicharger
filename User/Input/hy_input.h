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

#define HY_SAMPLE_INTERVAL 100  /*unit: ms*/
#define HY_SAMPLE_COUNT    5    /* 5 for average */

#define HY_ADC_OLD_PERCENT   3
#define HY_ADC_NEW_PERCENT   7
/*!!!! TOTAL PERCENT must be 10!!!!*/
#define HY_ADC_TOTAL_PERCENT (HY_ADC_OLD_PERCENT+HY_ADC_NEW_PERCENT)


int hy_input_init(void* hy_instance_handle);

uint16_t hy_get_voltagefb_x10V(void);

uint16_t hy_get_currentfb_x10A(void);

void hy_set_voltagefb_x10V(uint16_t value);

void hy_set_currentfb_x10A(uint16_t value);

void hy_set_input_voltage_x10V(uint16_t value);

void hy_set_battery_voltage_x10V(uint16_t value);


void hy_set_voltagefb1_x10V(uint16_t value);

void hy_set_currentfb1_x10A(uint16_t value);

void hy_set_voltagefb2_x10V(uint16_t value);

void hy_set_currentfb2_x10A(uint16_t value);

typedef struct HYInputSignalStruct{
	uint16_t voltagefb;
	uint16_t currentfb;
	uint16_t voltagefb_x10V;
	uint16_t currentfb_x10A;
	uint16_t voltagefb1_x10V;
	uint16_t currentfb1_x10A;
	uint16_t voltagefb2_x10V;
	uint16_t currentfb2_x10A;
	uint16_t voltagefb3_x10V;
	uint16_t currentfb3_x10A;
	uint16_t inputvoltage_x10V;
	uint16_t batteryvoltage_x10V;
	uint8_t gw_temperature;
	uint8_t gw_state1;
	uint8_t gw_state2;
	uint8_t gw_relay;
}hy_inputsignal_t;




#endif
