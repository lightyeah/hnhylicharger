#ifndef __HY_INSTANCE_H__
#define __HY_INSTANCE_H__

#include "stdint.h"
#include "stdbool.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "lpc17xx_gpio.h"

#include "hy_can.h"
#include "hy_input.h"
#include "hy_systime.h"
#include "hy_output.h"
#include "hy_charge_task.h"
#include "GUI.h"

#define INT8TO32(a,b,c,d)         (uint32_t)(d|(((c)<<8)&0xff00)|(((b)<<16)&0xff0000)|(((a)<<24)&0xff000000))  
#define INT8TO16(a,b)             INT8TO32(0,0,a,b)
#define INT32TO8_1(a)             (uint8_t)((a)&0xff)
#define INT32TO8_2(a)	 		  (uint8_t)(((a)&0xff00)>>8)
#define INT32TO8_3(a)             (uint8_t)(((a)&0xff0000)>>16)
#define INT32TO8_4(a)             (uint8_t)(((a)&0xff000000)>>24)
#define INT16TO8_1(a)             INT32TO8_1(a)
#define INT16TO8_2(a)             INT32TO8_2(a)
#define HY_32NTOH(a)              (((a&0xff)<<24)|((a&0xff00)<<8)|((a&0xff0000)>>8)|((a&0xff000000)>>24))




#define HY_CONTROLSTYLE_CAN                         0
#define HY_CONTROLSTYLE_LOCAL                       1


/*error code define*/
#define HY_OK                                       0
#define HY_ERROR                                    -1

#define HY_TRUE                                     1
#define HY_FALSE                                    0

#define HY_TRIGGERED                                1
#define HY_NOTRIGGERED                              0    

#define HY_CHARGETASK_DATA_NUM                      3

#define HY_INSTANCE_INITDONE                        1
#define HY_INSTANCE_NOT_INITDONE                    0 

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
	uint32_t communicaterate;
	
}hy_config;


typedef struct Hy_Instance
{
	int initdone; 

	hy_config config;
	int configed_flag;
	
	hy_cancom_t cancom;
	
	hy_inputsignal_t inputsignal;
	hy_output_t output;
	
	hy_systime_t systime;
	
	hy_chargetask_t chargetask;
	hy_chargetask_data hy_data[HY_CHARGETASK_DATA_NUM];
	

	hy_gui_t gui;
	
}hy_instance_t;

#endif


