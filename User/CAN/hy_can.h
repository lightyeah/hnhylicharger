/****************************************  File Info  ****************************************************
** File name:               can.h
** Descriptions:            
** Author                    luoyang
*********************************************************************************************************/

#ifndef  __CAN_H__
#define  __CAN_H__
#include "stdint.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "dataprocess.h"
#include "debug_frmwrk.h"

#include "gw_module.h"
#include "langpuda_protocol.h"

/*config can*/
#define CHARGER_CAN_TUNNEL_X      LPC_CAN1
#define BMS_CAN_TUNNEL_X          LPC_CAN1  



#define  HY_CAN_CONNECT_TIMEOUT                  30000/*unit ms*/

#define HY_CHARGE_ID_FORMAT					 STD_ID_FORMAT
#define HY_CHARGE_ID_EXT_FORMAT              EXT_ID_FORMAT

#define HY_CHARGE_MSG_TEST_FRAME_ID          0x401

#define HY_CAN_TASK_MONITOR_INTERVAL  2000/*unit ms*/

typedef enum HY_CANTASK_STATE{
	HY_CANTASK_IDLE = 0,
	
	HY_CANTASK_NORMAL,//正常工作
	
	HY_CANTASK_ERR_DISCONNECT,
	HY_CANTASK_ERR
}hy_cantask_state;



typedef struct HY_CANMSG{
	uint32_t frame_id;
	uint8_t databyte[8];
	uint8_t updateflag;
}hy_canmsg;

typedef struct CHARGER_MSG{
	
	uint32_t output1_current_x10A;
	uint32_t output1_voltage_x10V;
	uint8_t statu1_1;
	uint8_t statu1_2;
	uint32_t temperature1_x10degree;	
	uint32_t battery1_voltage;
	uint32_t input1_AC_voltage_x10V;
	
#if (GW_MOUDLE_NUM >= 2)
	uint32_t output2_current_x10A;
	uint32_t output2_voltage_x10V;
	uint8_t statu2_1;
	uint8_t statu2_2;
	uint32_t temperature2_x10degree;	
	uint32_t battery2_voltage;
	uint32_t input2_AC_voltage_x10V;
#endif

#if (GW_MOUDLE_NUM >= 3)
	uint32_t output3_current_x10A;
	uint32_t output3_voltage_x10V;
	uint8_t statu3_1;
	uint8_t statu3_2;
	uint32_t temperature3_x10degree;	
	uint32_t battery3_voltage;
	uint32_t input3_AC_voltage_x10V;
#endif

	uint32_t setting_voltage_x10V;
	uint32_t setting_current_10A;
	
	
}charger_msg;



typedef struct CanComStrcut{

	uint32_t charger_module_timeout;//刷新连接时间
	uint32_t bms_module_timeout;
	
	hy_canmsg charger_module_canmsg;//充电模块信息
	hy_canmsg bms_module_canmsg;//bms充电机信息
	
	char charger_module_canconnected;//充电模块连接状态
	char bms_module_canconnected;//电池bms模块连接状态

	charger_msg get_charger_msg;
	
	lpd_msg bms_msg;
	charge_to_langpuda_msg charge_to_msg_msg;
	hy_cantask_state state;
}hy_cancom_t;





int hy_can_init(void* hy_instance);
int hy_can_getmsg(void);
void hy_can_task_main(void);

//CHARGER**********************************

// 控制
int hy_can_start_charger(void);
int hy_can_stop_charger(void);
int hy_can_control_set_charger(uint32_t current_x1000mA,uint32_t voltage_x1000mV);


// 询问信息 异步处理
int hy_can_control_query_charger(void);// 获取充电器状态数据
int hy_can_query_setting_voltage_charger(void);
int hy_can_query_setting_current_charger(void);
int hy_can_query_temperature_charger(void);
int hy_can_query_220V_charger(void);



//信息获取
int hy_can_get_batterystate(void);//电池连接状态
uint32_t hy_can_get_battery_voltage(void);//电池电压
	
uint32_t hy_can_get_output_voltage_x10V(void);//输出电压
uint32_t hy_can_get_output_current_x10A(void);//输出电流

int hy_can_get_intput_voltage_x10V(uint32_t* vol1,uint32_t* vol2, uint32_t* vol3);//输入三相电压

uint32_t hy_can_get_charger_module_temperature_x10degree(void);//充电模块温度

uint8_t hy_can_charger_module_connected(void);
uint8_t hy_can_bms_connected(void);

uint8_t hy_can_get_charger_module_statu1(void);

uint8_t hy_can_get_charger_module_statu2(void);


int hy_can_detect_charger(void);


//BMS***************************************

int hy_can_get_bms_set_voltage_10V(void);
int hy_can_get_bms_set_current_10A(void);

int hy_can_get_bms_temperature(void);

int hy_can_get_bms_control(void);

int hy_can_get_bms_status(void);

int hy_can_get_bms_mode(void);

int hy_can_get_bms_battery_voltage_x10V(void);

int hy_can_set_output_msg(uint16_t voltage_x10V, uint16_t current_x10A);
int hy_can_set_status_msg(uint8_t status);

int hy_can_broadcast_to_bms(void);

int hy_can_detect_bms(void);




#endif
