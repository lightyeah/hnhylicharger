#ifndef __HY_OUTPUT_H__
#define __HY_OUTPUT_H__

#include "stdint.h"

#define HY_OUTPUT_TYPE_CURRENT   0
#define HY_OUTPUT_TYPE_VOLTAGE   1

#define HY_OUTPUT_RELAY_OFF      0
#define HY_OUTPUT_RELAY_ON       1

typedef struct HYOutputSignalStruct
{
    uint32_t output_time_ms;
	uint32_t total_power_joule;/*unit x1 */
	uint32_t total_power_kwh;/*unit x10*/
	
	uint32_t powerrate_microtime_w;/*unit x10*/
	uint32_t powerrate_average_w;/*unit x10*/
	
	uint32_t output_aim_voltage_x10V;/*unit x10*/
	uint32_t output_aim_current_x10A;/*unit x10*/

	
}hy_output_t;


int hy_output_init(void* hy_instance_handle);

int hy_set_start_output(void);
int hy_set_stop_output(void);

int hy_set_charger_output(uint16_t voltage_x10V, uint16_t current_x10A);

int hy_set_data_broadcast_to_bms(uint16_t voltage_x10V, uint16_t current_x10A, uint16_t energy_x10Ah);

#endif
