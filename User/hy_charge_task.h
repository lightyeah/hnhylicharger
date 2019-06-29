#ifndef __HY_CHARGE_TASK_H__
#define __HY_CHARGE_TASK_H__

#include "stdint.h"

#define CHARGETASK_MONITOR_INTERVAL 3000/*unit ms*/
#define CHARGETASK_CONTROL_INTERVAL 100/*unit ms*/

#define CHARGETASK_CAN_STOP_CODE    1
#define CHARGETASK_LOCAL_STOP_CODE  2

#define HY_BATTERY_CONNECT          1
#define HY_BATTERY_DISCONNECT       0
typedef enum Hy_Chargetask_State{
	CHARGETASK_IDLE = 0,
	CHARGETASK_LOCAL_ONE,
	CHARGETASK_LOCAL_TWO,
	CHARGETASK_LOCAL_THREE,
	CHARGETASK_CAN,
	CHARGETASK_ERR
}hy_chargetask_state;


typedef struct HY_ChargeTask_T{
	uint8_t start_flag;
	uint8_t end_flag;
	
	uint32_t start_time_ms;
	uint32_t lastcontrol_time_ms;
	uint32_t end_time_ms;
	uint32_t total_charge_time_ms;
	
	uint32_t aim_type;
	uint32_t aim_voltage_x10V;
	uint32_t aim_current_x10A;
	uint32_t output_voltage_x10V;
	uint32_t output_current_x10A;
	uint32_t output_dac_value;
	uint32_t max_voltage_x10V;
	
	uint32_t total_chargepower_x10kwh;
	uint32_t total_chargepower_mj;
	uint32_t average_powerrate_x10w;

	hy_chargetask_state state;
	int8_t batter_flag;
	
	
}hy_chargetask_t;

typedef struct HY_ChargeTask_Data{
	uint32_t start_time_ms;
	uint32_t total_chargepower_kwh;
	uint32_t average_powerrate_w;
	uint32_t battery_voltage_x10V;
	uint32_t total_charge_time_min;
	uint32_t error_code;
	char msg[50];
}hy_chargetask_data;

int hy_chargetask_init(void* hy_instance_handle);
int hy_chargetask_start(int controltype, void* ctx);
int hy_chargetask_stop(int stop_code,void* ctx);
int hy_chargetask_setmaxvoltage_x10V(uint32_t voltage);
int hy_chargetask_setaim(uint8_t type, uint32_t value);

uint32_t hy_chargetask_getoutputvol_x10V(void);
uint32_t hy_chargetask_getoutputcur_x10A(void);
uint32_t hy_chargetask_getchargetime_min(void);
uint32_t hy_chargetask_gettotalpower_x10kwh(void);

int hy_chargetask_end(void);
int hy_chargetask_emergencystop(void);

void hy_chargetask_main(void);
#endif


