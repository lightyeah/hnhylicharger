/****************************************  File Info  ****************************************************
** File name:               can.h
** Descriptions:            
** Author                    luoyang
*********************************************************************************************************/

#ifndef  __CAN_H__
#define  __CAN_H__
#include "stdint.h"
#include "can.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "dataprocess.h"
#include "can.h"
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




extern CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
extern uint32_t CANRxCount, CANTxCount;
void BMS_CAN_Init(void );
void can_sendbackmsg(uint32_t vol,uint32_t cur,uint8_t statu);
uint8_t canstop();
uint8_t _canconnectedcheck();
#endif
