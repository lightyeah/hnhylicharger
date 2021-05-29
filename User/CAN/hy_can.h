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
#define BMS_CAN_TUNNEL_X      LPC_CAN1

#define HY_CAN_MSG_NO_RESEND     0
#define HY_CAN_MSG_RESEND_DONE   1
#define HY_CAN_MSG_NEED_RESEND   1
#define HY_CAN_MSG_RESEND(x)     ((x)+1)
/**************************************************/
/********************Ð­ÒéÄÚÈÝ protocol v1.0********/
/**************************************************/
#define HY_ID_FORMAT					 EXT_ID_FORMAT

#define  BMS_OBC_BHM_FRAME_ID                   0x182756F4 /*handshake BMS-->OBC*/
#define  BMS_OBC_BHM_MAX_VOL_MASK               HY_MAX_VOLTAGE     /*handshake data MAX charge Voltage unit:0.1 Volt*/
#define  BMS_OBC_BHM_INTERVAL                   250 /*all interval unit is ms*/       /*unit: ms*/

#define  BMS_OBC_BCL_FRAME_ID                   0x181056F4 /*charge req BMS-->OBC*/
#define  BMS_OBC_BCL_VOL_REQ_MASK               HY_MAX_VOLTAGE   
#define  BMS_OBC_BCL_CUR_REQ_MASK               HY_MAX_CURRENT
#define  BMS_OBC_BCL_MODE_VOL                   0x01
#define  BMS_OBC_BCL_MODE_CUR                   0x02
#define  BMS_OBC_BCL_INTERVAL                   50

#define  BMS_OBC_BST_FRAME_ID                  	0x101956F4 /*BMS stop charge BMS --> OBC*/
#define  BMS_OBC_BST_REACH_SOC_GOAL_MASK        0x01 /*0 for normal 1 for higher 2 3for uncertainty */               
#define  BMS_OBC_BST_REACH_TOTAL_VOL_MASK       ((0x01)<<2)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  BMS_OBC_BST_REACH_SINGLE_VOL_MASK      ((0x01)<<4)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  BMS_OBC_BST_OBC_STOP_MASK              ((0x01)<<6)/*0 for normal 1 for stoped 2 3for uncertainty*/
#define  BMS_OBC_BST_INSULATION_ERR_MASK        ((0x01)<<8)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_CONNECTOR_HEAT_MASK        ((0x01)<<10)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_BMS_HEAT_MASK              ((0x01)<<12)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_C_CONNECTOR_HEAT_MASK      ((0x01)<<14)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_BATTERY_HEAT_MASK          ((0x01)<<16)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_RELAY_ERR_MASK             ((0x01)<<18)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_POINT_2_ERR_MASK           ((0x01)<<20)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_OTHER_ERR_MASK             ((0x01)<<22)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_CUR_OVERFLOW_MASK          ((0x01)<<24)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_VOL_ERR_MASK               ((0x01)<<26)/*0 for normal 1 for error 2 3for uncertainty*/ 
#define  BMS_OBC_BST_INTERVAL                   10

#define  OBC_BMS_CHM_FRAME_ID                   0x1826F456/*OBC handshake OBC-->BMS*/
#define  OBC_BMS_PROTOCOL_VERSION_BYTE1         0x00
#define  OBC_BMS_PROTOCOL_VERSION_BYTE2         0x01
#define  OBC_BMS_PROTOCOL_VERSION_BYTE3         0x01
#define  OBC_BMS_CHM_INTERVAL                   250

#define  OBC_BMS_CML_FRAME_ID                    0x1808F456/*OBC MAX output OBC-->BMS*/
#define  OBC_BMS_CML_INTERVAL                    250

#define  OBC_BMS_CCS_FRAME_ID                    0x1812F456/*OBC charge control state OBC-->BMS*/
#define  OBC_BMS_CCS_ALWAYS_ON                   0x01
#define  OBC_BMS_CCS_INTERVAL                    50

#define  OBC_BMS_CST_FRAME_ID                    0x101AF456/*OBC stop charge OBC-->BMS*/
#define  OBC_BMS_CST_REACH_GOAL(x)               ((x)&0x01) /*0 for normal 1 for higher 2 3for uncertainty */               
#define  OBC_BMS_CST_MANUAL_STOP(x)              (((x)&0x01)<<2)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  OBC_BMS_CST_ERR_STOP(x)                 (((x)&0x01)<<4)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  OBC_BMS_CST_BMS_STOP(x)                 (((x)&0x01)<<6)/*0 for normal 1 for stoped 2 3for uncertainty*/
#define  OBC_BMS_CST_HEAT_ERR(x)                 (((x)&0x01)<<8)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_CONNECTOR_ERR(x)            (((x)&0x01)<<10)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_OBC_HEAT_ERR(x)             (((x)&0x01)<<12)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_BEYOND_CAPACITY(x)          (((x)&0x01)<<14)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_EMERGENCY_STOP(x)           (((x)&0x01)<<16)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_OHTER_ERR(x)                (((x)&0x01)<<18)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_CUR_NOT_MATCH(x)            (((x)&0x01)<<24)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_CST_VOL_ERR(x)                  (((x)&0x01)<<26)/*0 for normal 1 for error 2 3for uncertainty*/ 
#define  OBC_BMS_CST_INTERVAL                    10 

#define  OBC_BMS_CSD_FRAME_ID                    0x181DF459/*OBC charge static data OBC-->BMS*/
#define  OBC_BMS_CSD_INTERVAL                    250

#define  FRAME_ID(x)               x##_FRAME_ID 
#define  INTERVAL(x)               x##_INTERVAL

#define  HY_CAN_CONNECT_TIMEOUT                  30000/*unit ms*/

#define HY_CAN_BMS_STOP                          2/**/
#define HY_CAN_OBC_STOP_CANTIMEOUT               3
/*************************************/
/********************协议********/
/*************************************/
#define HY_CHARGE_ID_FORMAT					 STD_ID_FORMAT

#define HY_CHARGE_CONTROL_FRAME_ID   0x200
#define HY_CHARGE_MSG_100MS_FRAME_ID     0x201//100ms间隔上报帧
#define HY_CHARGE_MSG_500MS_FRAME_ID     0x301//500ms间隔上报帧

#define HY_CHARGE_MSG_100MS_FRAME_ID2     0x205//100ms间隔上报帧
#define HY_CHARGE_MSG_500MS_FRAME_ID2     0x305//500ms间隔上报帧

/*start*********增加充电器控制协议*********/
/********************/



/*end*********增加充电器控制协议*******/
#define HY_CAN_TASK_MONITOR_INTERVAL  2000/*unit ms*/
typedef enum HY_BMS_STOP_MSG{
	hy_bms_no_stop = 0,
	HY_BMS_REACH_SOC_GOAL,
	HY_BMS_REACH_TOTAL_VOL,


}hy_bms_stop_msg;

typedef enum HY_CANTASK_STATE{
	HY_CANTASK_IDLE = 0,
	HY_CANTASK_HANDSHAKE,
	HY_CANTASK_CHARGE,
	HY_CANTASK_BMS_STOP,
	HY_CANTASK_OBC_STOP,
	HY_CANTASK_END,
	
	HY_CANTASK_CHARGE_MSG_100MS,//充电器上报状态报文 100ms间隔
	HY_CANTASK_CHARGE_MSG_500MS,//充电器上报状态报文 500ms间隔
	
	HY_CANTASK_ERR
}hy_cantask_state;



typedef struct HY_CANMSG{
	uint32_t frame_id;
	uint8_t databyte[8];
	uint32_t resendcounts;
}hy_canmsg;

typedef struct CanComStrcut{
	uint8_t msgupdate_flag;
	int canconnected;
	uint32_t connectupdate_time_ms;
	hy_canmsg canmsg;
	/*注意：
	* bms_stop_code 都定义为偶数
	* obc_stop_code	都定义为奇数
	*/
	int bms_stop_code;
	int obc_stop_code;
	hy_cantask_state state;
}hy_cancom_t;

int hy_can_send(hy_canmsg* msg);
int hy_can_send_test(void);

//充电器控制
int hy_can_control_GWcharger(uint16_t vol_x10v, uint16_t cur_x10a);
int hy_can_stop_GWcharger(void);
int hy_can_GWcharger_batteryoff(void);

int hy_can_GWcharger_status1(void);
int hy_can_GWcharger_status2(void);

int hy_can_init(void* hy_instance);
int hy_can_getmsg(void);
void hy_can_task_main(void);

int hy_can_connected(void);
int hy_can_get_taskstate(void);

int hy_can_restart(int start_code, void* ctx);
int hy_can_stop(int stop_code, void* ctx);
#endif
