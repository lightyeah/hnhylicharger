#ifndef __LANGPUDA_PROTOCOL_H__
#define __LANGPUDA_PROTOCOL_H__

#define LPD_BMS_BROADCAST_ID1  0x111

#define CHARGER_TO_LPD_ID     0x112

#define LPD_BMS_BROADCAST_ID2  0x115

#define LPD_INTERVAL_TIME      1000//ms

#define LPD_TIMEOUT_TIME           5000//

typedef struct LANGPUDA_MSG{
	uint16_t bms_max_voltage_x10V;
	uint16_t bms_max_current_x10A;
	uint8_t control_byte;
	uint8_t statu;
	uint8_t mode;
	uint16_t single_battery_max_x1000mV;
	uint16_t single_battery_min_x1000mA;
	uint8_t soc_byte;
	uint16_t temperatue_x1degree;
	uint16_t battery_voltage_x10V;
}lpd_msg;

typedef struct CHARGE_TO_LANGUDA_MSG{
	uint16_t charge_output_voltage_x10V;
	uint16_t charge_output_current_x10A;
	uint8_t status;
}charge_to_langpuda_msg;


#endif /*__LANGPUDA_PROTOCOL_H__*/
