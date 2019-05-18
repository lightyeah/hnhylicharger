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
#define BMS_CAN_PIN_FUNCNUM   1                   
#define BMS_CAN_PIN_PORTNUM   0
#define BMS_CAN_H_PINNUM      0
#define BMS_CAN_L_PINNUM      1
#define BMS_CAN_PIN_OPENDRAIN  0
#define BMS_CAN_PIN_MODE       0
// #define BMS_BAUD_RATE          250000

//更改对应IP和帧合格式，以适应不同充电机通讯版本。目前是扩展帧
#define BMS_CAN_FRAME_ID       0x1806E5F4
#define CCS_CAN_FRAME_ID       0x18FF50E5
#define MY_ID_FORMAT					 EXT_ID_FORMAT
//标准帧例子
// #define BMS_CAN_FRAME_ID       0x180
// #define CCS_CAN_FRAME_ID       0x18F
// #define MY_ID_FORMAT					 STD_ID_FORMAT



#define BMS_CAN_CONTROL_MASK   0x1
#define BMS_CONTROL_STOP       0x1
#define BMS_CONTROL_START      0x0


#define CALLBACK_HARDWARE_ERR        0x1
#define CALLBACK_OVERHEAT            0x2
#define CALLBACK_INPUT_ERR           0x4
#define CALLBACK_START_STATUS        0x8
#define CALLBACK_COM_TIMEOUT_STATU   0x10


/**************************************************/
/********************协议内容 protocol v1.0********/
/**************************************************/

#define  BMS_OBC_BHM_FRAME_ID                   0x182756F4 /*handshake BMS-->OBC*/
#define  BMS_OBC_BHM_MAX_VOL_MASK               0x1fff     /*handshake data MAX charge Voltage unit:0.1 Volt*/
#define  BMS_OBC_BHM_INTERVAL                   250        /*unit: ms*/

#define  BMS_OBC_BCL_FRAME_ID                   0x181056F4 /*charge req BMS-->OBC*/
#define  BMS_OBC_BCL_VOL_REQ_MASK               0x1fff   
#define  BMS_OBC_BCL_CUR_REQ_MASK               ((0x1fff)<<16)
#define  BMS_OBC_BCL_MODE_REQ_MASK              0xff
#define  BMS_OBC_BCL_INTERVAL                   50

#define  BMS_OBC_BST_FRAME_ID                   0x181956F4 /*BMS stop charge BMS --> OBC*/
#define  BMS_OBC_BST_REACH_SOC_GOAL_MASK        0x03 /*0 for normal 1 for higher 2 3for uncertainty */               
#define  BMS_OBC_BST_REACH_TOTAL_VOL_MASK       ((0x03)<<2)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  BMS_OBC_BST_REACH_SINGLE_VOL_MASK      ((0x03)<<4)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  BMS_OBC_BST_OBC_STOP_MASK              ((0x03)<<6)/*0 for normal 1 for stoped 2 3for uncertainty*/
#define  BMS_OBC_BST_INSULATION_ERR_MASK        ((0x03)<<8)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_CONNECTOR_HEAT_MASK        ((0x03)<<10)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_BMS_HEAT_MASK              ((0x03)<<12)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_C_CONNECTOR_HEAT_MASK      ((0x03)<<14)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_BATTERY_HEAT_MASK          ((0x03)<<16)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_RELAY_ERR_MASK             ((0x03)<<18)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_POINT_2_ERR_MASK           ((0x03)<<20)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_OTHER_ERR_MASK             ((0x03)<<22)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_CUR_OVERFLOW_MASK          ((0x03)<<24)/*0 for normal 1 for error 2 3for uncertainty*/
#define  BMS_OBC_BST_VOL_ERR_MASK               ((0x03)<<26)/*0 for normal 1 for error 2 3for uncertainty*/ 
#define  BMS_OBC_BST_INTERVAL                   10

#define  OBC_BMS_CHM_FRAME_ID                   0x1826F456/*OBC handshake OBC-->BMS*/
#define  OBC_BMS_PROTOCOL_VERSION_BYTE1         0x00
#define  OBC_BMS_PROTOCOL_VERSION_BYTE2         ((0x01)<<8)
#define  OBC_BMS_PROTOCOL_VERSION_BYTE3         ((0x01)<<16)
#define  OBC_BMS_CHM_INTERVAL                   250

#define  OBC_BMS_CML_FRAME_ID                    0x1808F456/*OBC MAX output OBC-->BMS*/
#define  OBC_BMS_CML_MAX_OUTPUT_VOL(x)           (x)       /*unit 0.1 */
#define  OBC_BMS_CML_MIN_OUTPUT_VOL(x)           ((x)<<16)
#define  OBC_BMS_CML_MAX_OUTPUT_CUR(x)           (x)
#define  OBC_BMS_CML_MIN_OUTPUT_CUR(x)           ((x)<<16)
#define  OBC_BMS_CML_INTERVAL                    250

#define  OBC_BMS_CCS_FRAME_ID                    0x1812F456/*OBC charge control state OBC-->BMS*/
#define  OBC_BMS_CCS_OUTPUT_VOL(x)           	 (x)   
#define  OBC_BMS_CCS_OUTPUT_CUR(x)           	 ((x)<<16)
#define  OBC_BMS_CCS_OUTPUT_TIME(x)          	 (x) /*min*/
#define  OBC_BMS_CCS_INTERVAL                    50

#define  OBC_BMS_CST_FRAME_ID                    0x181AF456/*OBC stop charge OBC-->BMS*/
#define  OBC_BMS_BST_REACH_GOAL(x)               ((x)&0x03) /*0 for normal 1 for higher 2 3for uncertainty */               
#define  OBC_BMS_BST_MANUAL_STOP(x)              (((x)&0x03)<<2)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  OBC_BMS_BST_ERR_STOP(x)                 (((x)&0x03)<<4)/*0 for unreach 1 for reached 2 3for uncertainty*/
#define  OBC_BMS_BST_BMS_STOP(x)                 (((x)&0x03)<<6)/*0 for normal 1 for stoped 2 3for uncertainty*/
#define  OBC_BMS_BST_HEAT_ERR(x)                 (((x)&0x03)<<8)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_CONNECTOR_ERR(x)            (((x)&0x03)<<10)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_OBC_HEAT_ERR(x)             (((x)&0x03)<<12)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_BEYOND_CAPACITY(x)          (((x)&0x03)<<14)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_EMERGENCY_STOP(x)           (((x)&0x03)<<16)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_OHTER_ERR(x)                (((x)&0x03)<<18)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_CUR_NOT_MATCH(x)            (((x)&0x03)<<24)/*0 for normal 1 for error 2 3for uncertainty*/
#define  OBC_BMS_BST_VOL_ERR(x)                  (((x)&0x03)<<26)/*0 for normal 1 for error 2 3for uncertainty*/ 
#define  OBC_BMS_BST_INTERVAL                    10 

#define  OBC_BMS_CSD_FRAME_ID                    0x181DF459/*OBC charge static data OBC-->BMS*/
#define  OBC_BMS_CSD_TOTAL_TIME(x)               (x)/*unit: min*/
#define  OBC_BMS_CSD_OUTPUT_POWER(x)             ((x)<<16)/*unit: kW*h*/
#define  OBC_BMS_CSD_OBC_NUMBER(x)               (x)
#define  OBC_BMS_CSD_INTERVAL                    250

#define  FRAME_ID(x)               x##_FRAME_ID 
#define  INTERVAL(x)               x##_INTERVAL

/*************************************/
/********************协议内容 ********/
/*************************************/

typedef enum HY_CANCONNECT{
	HY_CAN_DISCONNECTED=0,
	HY_CAN_CONNECTED
}hy_canconnect;

typedef struct HY_CANMSG{
	uint32_t frame_id;
	uint8_t databyte[4];
}hy_canmsg;

typedef struct CanComStrcut{
	uint8_t cannum;
	hy_canconnect canconnected;
	hy_canmsg canmsg;
}cancom_t;

int hy_can_init(void* hy_instance);

#endif
