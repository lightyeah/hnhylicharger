#ifndef __QIANHANG_PROTOCOL_H__
#define __QIANHANG_PROTOCOL_H__

#define QH_TIMEOUT_TIME 5000//ms

#define QIANHANG_BMS_ID  0x1806e5f4

#define QIANHANG_CHARGER_ID 0x18ff50e5


typedef struct{
	uint32_t bms_max_voltage_x10V;
	uint32_t bms_max_current_x10A;
	uint8_t control_byte;
	uint8_t soc;
}qianhang_bms_msg;

typedef struct{
	uint32_t output_voltage_x10V;
	uint32_t output_current_x10A;
	uint8_t status;
}qianhang_charger_msg;



#endif 

