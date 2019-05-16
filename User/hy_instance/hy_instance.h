#ifndef __HY_INSTANCE_H__
#define __HY_INSTANCE_H__

#include "stdint.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "lpc17xx_gpio.h"

#define HY_CONTROLSTYLE_CAN                         0
#define HY_CONTROLSTYLE_LOCAL                       1

/*error define*/


typedef struct Hy_Config{
	/*for all*/
	uint16_t voltagerange;
	uint16_t currentrange;
	uint16_t balancecurrent;
	uint8_t controlstyle;/*can for o local for 1*/
	
	/*for local*/
	uint16_t  chargecurrent_1;/*unit  1 V/A */
	uint16_t  limitvoltage_1;
	uint16_t  chargetimeout_1_min;
	uint16_t  switchvoltage_1;
	
	uint16_t  chargecurrent_2;
	uint16_t  limitvoltage_2;
	uint16_t  chargetimeout_2_min;
	uint16_t  switchvoltage_2;
	
	uint16_t chargevoltage_3;
	uint16_t limitcurrent_3;
	uint16_t chargetimeout_3_min;
	uint16_t switchcurrent_3;

	/*for can */	
	uint16_t ommunicationrate;
	
}hy_config;

typedef struct Hy_Instance
{
	hy_config config;
	int configed_flag;
}hy_instance;

#endif