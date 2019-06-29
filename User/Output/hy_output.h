#ifndef __HY_OUTPUT_H__
#define __HY_OUTPUT_H__

#include "stdint.h"

#define HY_OUTPUT_TYPE_CURRENT   0
#define HY_OUTPUT_TYPE_VOLTAGE   1

typedef struct HYOutputSignalStruct
{
    uint32_t output_time_ms;
	uint32_t total_power_joule;/*unit x1 */
	uint32_t total_power_kwh;/*unit x10*/
	
	uint32_t powerrate_microtime_w;/*unit x10*/
	uint32_t powerrate_average_w;/*unit x10*/
	
	uint32_t output_dac_value;/*range 0~1023*/
	uint32_t output_aim_voltage_x10V;/*unit x10*/
	uint32_t output_aim_current_x10A;/*unit x10*/
 	uint32_t output_fb_voltage_x10V;
	uint32_t output_fb_current_x10A;
	
}hy_output_t;


int hy_output_init(void* hy_instance_handle);
int hy_set_output(uint32_t value);

#endif
