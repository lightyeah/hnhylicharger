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

/*config can*/
#define CHARGER_CAN_TUNNEL_X      LPC_CAN1
#define BMS_CAN_TUNNEL_X          LPC_CAN1  



#define  HY_CAN_CONNECT_TIMEOUT                  30000/*unit ms*/

/*************************************/
/********************协议********/
/*************************************/
#define HY_CHARGE_ID_FORMAT					 STD_ID_FORMAT
#define HY_CHARGE_ID_EXT_FORMAT              EXT_ID_FORMAT


#define HY_CHARGE_MSG_TEST_FRAME_ID 0x100//12078081

// 英瑞可
//控制指令ID
/*
* 1. 开机 CMD=2
* 2. 关机 CMD=2
* 3. 设置电压电流 CMD=0
* 4. 读取模块信息 电流电压状态 CMD=1
*/
#define HY_YRK_CONTROL_FRAME_ID          0x1307C081
#define HY_YRK_CONTROL_BACK_FRAME_ID     0x1207C081

//读取监控设定电压
#define HY_YRK_READ_SETTING_VOLTAGE_FRAMD_ID      0x13010081
#define HY_YRK_READ_SETTING_VOLTAGE_BACK_FRAMD_ID 0x12010081

//读取监控设定电流
#define HY_YRK_READ_SETTING_CURRENT_FRAMD_ID      0x13010881
#define HY_YRK_READ_SETTING_CURRENT_BACK_FRAMD_ID 0x12010881

//读取输入电压
#define HY_YRK_READ_INPUT_VOLTAGE_FRAMD_ID        0x1307a081
#define HY_YRK_READ_INPUT_VOLTAGE_BACK_FRAMD_ID   0x1207a081

//读取环境温度
#define HY_YRK_READ_TEMPERATURE_FRAMD_ID          0x13008081
#define HY_YRK_READ_TEMPERATURE_BACK_FRAMD_ID     0x12008081



/*start*********增加充电器控制协议*********/
/********************/



/*end*********增加充电器控制协议*******/
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

typedef struct YRK_MSG{
	uint32_t output_current_x10A;
	uint32_t output_voltage_x10V;
	uint8_t statu1;
	uint8_t statu2;
	uint32_t setting_voltage_x10V;
	uint32_t setting_current_10A;
	uint32_t temperature_x10degree;	
}yrk_msg;

typedef struct BMS_MSG{
	int dddd;
}bms_msg;

typedef struct CanComStrcut{

	uint32_t charger_module_timeout;//刷新连接时间
	uint32_t bms_module_timeout;
	
	hy_canmsg charger_module_canmsg;//充电模块信息
	hy_canmsg bms_module_canmsg;//bms充电机信息
	
	char charger_module_canconnected;//充电模块连接状态
	char bms_module_connected;//电池bms模块连接状态

	yrk_msg charger_msg;

	bms_msg no_msg;
	
	hy_cantask_state state;
}hy_cancom_t;





int hy_can_init(void* hy_instance);
int hy_can_getmsg(void);
void hy_can_task_main(void);


// YRK控制
int hy_can_start_YRKcharger(void);
int hy_can_stop_YRKcharger(void);
int hy_can_control_set_yrkcharger(uint32_t current_x1000mA,uint32_t voltage_x1000mV);
int hy_can_control_query_YRKcharger(void);//YRK 获取充电器状态数据

//YRK 询问信息 异步处理
int hy_can_query_setting_voltage_yrkcharger(void);
int hy_can_query_setting_current_yrkcharger(void);
int hy_can_query_temperature_yrkcharger(void);
int hy_can_query_220V_yrkcharger(void);



//信息获取
int hy_can_get_batterystate(void);//电池连接状态
uint32_t hy_can_get_battery_voltage(void);//电池电压
	
uint32_t hy_can_get_output_voltage_x10V(void);//输出电压
uint32_t hy_can_get_output_current_x10A(void);//输出电流

int hy_can_get_intput_voltage_x10V(uint32_t* vol1,uint32_t* vol2, uint32_t* vol3);//输入三相电压

uint32_t hy_can_get_charger_module_temperature_x10degree(void);//yrk充电模块温度

uint8_t hy_can_charger_module_connected(void);
uint8_t hy_can_bms_connected(void);

uint8_t hy_can_get_charger_module_statu1(void);

uint8_t hy_can_get_charger_module_statu2(void);


int hy_can_detect_yrkcharger(void);
int hy_can_detect_bms(void);


#endif
