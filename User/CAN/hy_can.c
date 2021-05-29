#include "stdint.h"
#include "string.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "debug_frmwrk.h"
#include "config.h"

#include "hy_instance.h"

#define HY_LOG_TAG   "can"

CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
uint32_t CANRxCount, CANTxCount = 0;

hy_cancom_t *s_cancom = NULL;

char ghy_can_GWcharger_batteryoff_flag=1;


char ghy_can_GWcharger_status1 = 0;
char ghy_can_GWcharger_status2 = 0;
// 第二台充电机
char ghy_can_GWcharger_status3 = 0;
char ghy_can_GWcharger_status4 = 0;
// char ghy_can_gwcharger_jiaoliuqianya=0;
// char ghy_can_gwcharger_jiaoliuguoya=0;
// char ghy_can_gwcharger_shuchuqianya=0;
// char ghy_can_gwcharger_shuchuguoya=0;
// char ghy_can_gwcharger_shuchuguoliu=0;
// char ghy_can_gwcharger_shuchuduanlu=0;
// char ghy_can_gwcharger_guowenguanji=0;
// char ghy_can_gwcharger_yingjianguzhang=0;

// char ghy_can_gwcharger_shuchufanjie=0;
// char ghy_can_gwcharger_guowenjiange=0;
// char ghy_can_gwcharger_fengshanguzhang=0;
// char ghy_can_gwcharger_guowenguzhang=0;

int hy_can_init(void* hy_instance_handle)
{
		int ret = HY_OK;
	  PINSEL_CFG_Type PinCfg;
	
		/*software init*/
		hy_instance_t *hy_instance=(hy_instance_t*)hy_instance_handle;
		s_cancom = &(hy_instance->cancom);
		memset(s_cancom,0,sizeof(hy_cancom_t));
		s_cancom->state = HY_CANTASK_IDLE;
		s_cancom->msgupdate_flag = HY_FALSE;
		s_cancom->bms_stop_code = 0;
		s_cancom->obc_stop_code = 0;
		s_cancom->canconnected = HY_FALSE;
		s_cancom->connectupdate_time_ms = 0;
    
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;    
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);   
		
    CAN_Init(LPC_CAN1,hy_instance->config.communicaterate);
    LOG_INFO_TAG(HY_LOG_TAG,"set can baudrate %d",hy_instance->config.communicaterate);
    
    CAN_IRQCmd(BMS_CAN_TUNNEL_X,CANINT_RIE, ENABLE);

    NVIC_EnableIRQ(CAN_IRQn);
    //NVIC_SetPriority(CAN_IRQn, 10);
    CAN_SetAFMode(LPC_CANAF,CAN_AccBP);	
		LOG_INFO_TAG(HY_LOG_TAG,"hy can init done!!");
		return ret;
}

int hy_can_msg_stop_handle()
{
	int stop_code;
	stop_code =INT8TO32(s_cancom->canmsg.databyte[0],
											s_cancom->canmsg.databyte[1],
											s_cancom->canmsg.databyte[2],
											s_cancom->canmsg.databyte[3]);
	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	LOG_INFO_TAG(HY_LOG_TAG,"*********************");
	LOG_INFO_TAG(HY_LOG_TAG,"bms stop code : [%d]",stop_code);
	LOG_INFO_TAG(HY_LOG_TAG,"**********************");
	return stop_code;
}

int hy_can_send(hy_canmsg* msg)
{
		if(msg->resendcounts > HY_CAN_MSG_NEED_RESEND){
			msg->resendcounts--;
			if (msg->resendcounts <= HY_CAN_MSG_RESEND_DONE){
				msg->resendcounts = HY_CAN_MSG_RESEND_DONE;
			}
		}
	
    TXMsg.format = HY_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = msg->frame_id;
	
    *((uint8_t *) &TXMsg.dataA[0])= msg->databyte[0];
    *((uint8_t *) &TXMsg.dataA[1])= msg->databyte[1];
    *((uint8_t *) &TXMsg.dataA[2])= msg->databyte[2];
    *((uint8_t *) &TXMsg.dataA[3])= msg->databyte[3];
    *((uint8_t *) &TXMsg.dataB[0])= msg->databyte[4];
    *((uint8_t *) &TXMsg.dataB[1])= msg->databyte[5];
    *((uint8_t *) &TXMsg.dataB[2])= msg->databyte[6];
    *((uint8_t *) &TXMsg.dataB[3])= msg->databyte[7];

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
		
    return 0;
}

/*测试接口*/
int hy_can_send_test(void)
{

	
//     TXMsg.format = HY_CHARGE_ID_FORMAT;
//     TXMsg.len = 8;
//     TXMsg.id = HY_CHARGE_CONTROL_FRAME_ID;
// 	
//     *((uint8_t *) &TXMsg.dataA[0])= 0xff;
//     *((uint8_t *) &TXMsg.dataA[1])= 0x03;
//     *((uint8_t *) &TXMsg.dataA[2])= 0x01;
//     *((uint8_t *) &TXMsg.dataA[3])= 0xf4;
//     *((uint8_t *) &TXMsg.dataB[0])= 0x00;
//     *((uint8_t *) &TXMsg.dataB[1])= 0x10;
//     *((uint8_t *) &TXMsg.dataB[2])= 0x00;
//     *((uint8_t *) &TXMsg.dataB[3])= 0x00;

//     CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
		
    return 0;
}

//报文控制充电机输出, 
int hy_can_control_GWcharger(uint16_t vol_x10v, uint16_t cur_x10a){
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_CHARGE_CONTROL_FRAME_ID;
// 	   LOG_DEBUG_TAG(HY_LOG_TAG, "====control [%d]v [%d]a",vol_x10v,cur_x10a);
    *((uint8_t *) &TXMsg.dataA[0])= 0xff;//广播地址
    *((uint8_t *) &TXMsg.dataA[1])= 0x03;//充电机正常工作，充电继电器打开
    *((uint8_t *) &TXMsg.dataA[2])= INT16TO8_2((vol_x10v));
    *((uint8_t *) &TXMsg.dataA[3])= INT16TO8_1((vol_x10v));
    *((uint8_t *) &TXMsg.dataB[0])= INT16TO8_2((cur_x10a>>1));
    *((uint8_t *) &TXMsg.dataB[1])= INT16TO8_1((cur_x10a>>1));
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;

	  CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}

int hy_can_GWcharger_batteryoff(void){
	return ghy_can_GWcharger_batteryoff_flag;
}


int hy_can_GWcharger_status1(void){
	return (ghy_can_GWcharger_status1|ghy_can_GWcharger_status3);
}

int hy_can_GWcharger_status2(void){
	return (ghy_can_GWcharger_status2|ghy_can_GWcharger_status4);
}
//报文控制关闭充电机输出, 
int hy_can_stop_GWcharger(void){
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_CHARGE_CONTROL_FRAME_ID;
	
    *((uint8_t *) &TXMsg.dataA[0])= 0xff;//广播地址
    *((uint8_t *) &TXMsg.dataA[1])= 0x01;//充电机正常工作，充电继电器打开
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}

int hy_can_getmsg()
{
	int ret = HY_OK;

	if(s_cancom->msgupdate_flag){
		// LOG_DEBUG_TAG(HY_LOG_TAG, "can update new msg ");
		s_cancom->msgupdate_flag = HY_FALSE;/*clear flag*/

		s_cancom->canconnected = HY_TRUE;
		s_cancom->connectupdate_time_ms = hy_time_now_ms();

		switch(RXMsg.id){
			case BMS_OBC_BHM_FRAME_ID:/*handshake*/
				s_cancom->state = HY_CANTASK_HANDSHAKE;
				s_cancom->canmsg.frame_id = BMS_OBC_BHM_FRAME_ID;
				s_cancom->canmsg.databyte[0] = RXMsg.dataA[0];
				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
				hy_chargetask_setmaxvoltage_x10V(INT8TO16(s_cancom->canmsg.databyte[0],
																							s_cancom->canmsg.databyte[1])&BMS_OBC_BHM_MAX_VOL_MASK);	
				break;
			case BMS_OBC_BCL_FRAME_ID:/*req*/
				s_cancom->state = HY_CANTASK_CHARGE;
				s_cancom->canmsg.frame_id = BMS_OBC_BCL_FRAME_ID;
				s_cancom->canmsg.databyte[0] = RXMsg.dataA[0];
				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
				s_cancom->canmsg.databyte[2] = RXMsg.dataA[2];
				s_cancom->canmsg.databyte[3] = RXMsg.dataA[3];
				s_cancom->canmsg.databyte[4] = RXMsg.dataB[0];
				switch (s_cancom->canmsg.databyte[4]){
					case BMS_OBC_BCL_MODE_VOL:
						hy_chargetask_setaim(BMS_OBC_BCL_MODE_VOL,
						INT8TO16(s_cancom->canmsg.databyte[0],s_cancom->canmsg.databyte[1]));
						
						break;
					case BMS_OBC_BCL_MODE_CUR:
						hy_chargetask_setaim(BMS_OBC_BCL_MODE_CUR,
						INT8TO16(s_cancom->canmsg.databyte[2],s_cancom->canmsg.databyte[3]));
						
						break;
					default:
						LOG_ERROR_TAG(HY_LOG_TAG,"bms_obc_bcl can msg is wrong!!!");						
						break;
				}
				break;
			case BMS_OBC_BST_FRAME_ID:/*stop*/
				s_cancom->state = HY_CANTASK_BMS_STOP;
				s_cancom->canmsg.frame_id = BMS_OBC_BST_FRAME_ID;
				s_cancom->canmsg.databyte[0] = RXMsg.dataA[0];
				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
				s_cancom->canmsg.databyte[2] = RXMsg.dataA[2];
				s_cancom->canmsg.databyte[3] = RXMsg.dataA[3];	

				break;
			//第一台
			case HY_CHARGE_MSG_100MS_FRAME_ID://100ms 充电器上报数据处理
    
				s_cancom->state = HY_CANTASK_CHARGE_MSG_100MS;
				s_cancom->canmsg.frame_id = HY_CHARGE_MSG_100MS_FRAME_ID;
			  	ghy_can_GWcharger_status1 = RXMsg.dataA[0];
				ghy_can_GWcharger_status2 = RXMsg.dataA[1];

			    ghy_can_GWcharger_batteryoff_flag=ghy_can_GWcharger_status2&0x01;//电池未连接
			    LOG_DEBUG_TAG(HY_LOG_TAG,"======batteryoff [%d]",ghy_can_GWcharger_batteryoff_flag);
// 				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
// 				s_cancom->canmsg.databyte[2] = RXMsg.dataA[2];
// 				s_cancom->canmsg.databyte[3] = RXMsg.dataA[3];	
			  //s_cancom->canmsg.databyte[4] = RXMsg.dataB[0];	
			  //s_cancom->canmsg.databyte[5] = RXMsg.dataB[1];	
				s_cancom->canmsg.databyte[6] = RXMsg.dataB[2];	
				s_cancom->canmsg.databyte[7] = RXMsg.dataB[3];	
			 //hy_set_voltagefb_x10V(INT8TO16(s_cancom->canmsg.databyte[4],s_cancom->canmsg.databyte[5]));
			 	hy_set_currentfb1_x10A(INT8TO16(s_cancom->canmsg.databyte[6],s_cancom->canmsg.databyte[7]));
				break;
			case HY_CHARGE_MSG_500MS_FRAME_ID://500ms 充电器上报数据处理
				s_cancom->state = HY_CANTASK_CHARGE_MSG_500MS;
				s_cancom->canmsg.frame_id = HY_CHARGE_MSG_500MS_FRAME_ID;
				s_cancom->canmsg.databyte[0] = RXMsg.dataA[0];
				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
// 				s_cancom->canmsg.databyte[2] = RXMsg.dataA[2];
// 				s_cancom->canmsg.databyte[3] = RXMsg.dataA[3];
			 	hy_set_voltagefb1_x10V(INT8TO16(s_cancom->canmsg.databyte[0],s_cancom->canmsg.databyte[1]));
				break;
			//第二台
			case HY_CHARGE_MSG_100MS_FRAME_ID2://100ms 第二台充电器上报数据处理
    
				s_cancom->state = HY_CANTASK_CHARGE_MSG_100MS;
				s_cancom->canmsg.frame_id = HY_CHARGE_MSG_100MS_FRAME_ID;
			  	ghy_can_GWcharger_status3 = RXMsg.dataA[0];
				ghy_can_GWcharger_status4 = RXMsg.dataA[1];

			    ghy_can_GWcharger_batteryoff_flag=ghy_can_GWcharger_status4&0x01;//电池未连接
			    LOG_DEBUG_TAG(HY_LOG_TAG,"======batteryoff [%d]",ghy_can_GWcharger_batteryoff_flag);

				s_cancom->canmsg.databyte[6] = RXMsg.dataB[2];	
				s_cancom->canmsg.databyte[7] = RXMsg.dataB[3];	
			
			 	hy_set_currentfb2_x10A(INT8TO16(s_cancom->canmsg.databyte[6],s_cancom->canmsg.databyte[7]));
				break;
			case HY_CHARGE_MSG_500MS_FRAME_ID2://500ms 第二台充电器上报数据处理
				s_cancom->state = HY_CANTASK_CHARGE_MSG_500MS;
				s_cancom->canmsg.frame_id = HY_CHARGE_MSG_500MS_FRAME_ID;
				s_cancom->canmsg.databyte[0] = RXMsg.dataA[0];
				s_cancom->canmsg.databyte[1] = RXMsg.dataA[1];
			 	hy_set_voltagefb2_x10V(INT8TO16(s_cancom->canmsg.databyte[0],s_cancom->canmsg.databyte[1]));
				break;			
			default:
				//s_cancom->state = HY_CANTASK_IDLE;
				LOG_ERROR_TAG(HY_LOG_TAG,"get can wrong msg!!!");
				break;
		}
	}else{/*receive timeout!!!*/
		
		if (systime_elapse_ms(s_cancom->connectupdate_time_ms) >= HY_CAN_CONNECT_TIMEOUT){
			s_cancom->state = HY_CANTASK_IDLE;
			s_cancom->canconnected = HY_FALSE;
			s_cancom->obc_stop_code = HY_CAN_OBC_STOP_CANTIMEOUT;
			hy_chargetask_stop(CHARGETASK_CAN_STOP_CODE,&(s_cancom->obc_stop_code));
		}

	}
	
	return ret;
}

void hy_can_task_main()
{
	static hy_canmsg msg = {0};
	static uint32_t lastsendtime_ms = 0;
	static uint32_t canmonitortime_ms = 0;
	
	hy_can_getmsg();
	if(systime_elapse_ms(canmonitortime_ms)>=HY_CAN_TASK_MONITOR_INTERVAL){
		canmonitortime_ms = hy_time_now_ms();
		LOG_INFO_TAG(HY_LOG_TAG,"can monitor on cancom.state = [%d]",s_cancom->state);
	}
	
	switch (s_cancom->state){
		case HY_CANTASK_IDLE:
//			msg.frame_id = OBC_BMS_CHM_FRAME_ID;
//			memset(msg.databyte,0,8);
//			msg.databyte[0] = OBC_BMS_PROTOCOL_VERSION_BYTE1;
//		  msg.databyte[1] = OBC_BMS_PROTOCOL_VERSION_BYTE2;
//			msg.databyte[2] = OBC_BMS_PROTOCOL_VERSION_BYTE3;	  
//			msg.resendcounts = HY_CAN_MSG_NO_RESEND;
			if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CHM_INTERVAL){
				msg.frame_id = OBC_BMS_CHM_FRAME_ID;
				memset(msg.databyte,0,8);
				msg.databyte[0] = OBC_BMS_PROTOCOL_VERSION_BYTE1;
				msg.databyte[1] = OBC_BMS_PROTOCOL_VERSION_BYTE2;
				msg.databyte[2] = OBC_BMS_PROTOCOL_VERSION_BYTE3;
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				// hy_can_send(&msg);
				lastsendtime_ms = hy_time_now_ms();
			}
			break;
			
			
		case HY_CANTASK_HANDSHAKE:
//			msg.frame_id = OBC_BMS_CML_FRAME_ID;
//			memset(msg.databyte,0,8);
//			msg.databyte[0] = INT16TO8_1((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
//			msg.databyte[1] = INT16TO8_2((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
//			msg.databyte[4] = INT16TO8_1((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
//			msg.databyte[5] = INT16TO8_2((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
//			msg.resendcounts = HY_CAN_MSG_NO_RESEND;
			if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CML_INTERVAL){
				msg.frame_id = OBC_BMS_CML_FRAME_ID;
				memset(msg.databyte,0,8);
				msg.databyte[0] = INT16TO8_2((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
				msg.databyte[1] = INT16TO8_1((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
				msg.databyte[4] = INT16TO8_2((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
				msg.databyte[5] = INT16TO8_1((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				hy_can_send(&msg);
				lastsendtime_ms = hy_time_now_ms();
			}
			break;
			
			
		case HY_CANTASK_CHARGE:
			hy_chargetask_start(HY_CONTROLSTYLE_CAN,NULL);
//			msg.frame_id = OBC_BMS_CCS_FRAME_ID;
//			memset(msg.databyte,0,8);
//			msg.databyte[0] = INT16TO8_1(hy_chargetask_getoutputvol_x10V());
//			msg.databyte[1] = INT16TO8_2(hy_chargetask_getoutputvol_x10V());
//			msg.databyte[2] = INT16TO8_1(hy_chargetask_getoutputcur_x10A());
//			msg.databyte[3] = INT16TO8_2(hy_chargetask_getoutputcur_x10A());
//			msg.databyte[4] = INT16TO8_1(hy_chargetask_getchargetime_min());
//			msg.databyte[5] = INT16TO8_2(hy_chargetask_getchargetime_min());
//			msg.databyte[6] = OBC_BMS_CCS_ALWAYS_ON;
//			msg.resendcounts = HY_CAN_MSG_NO_RESEND;
			if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CCS_INTERVAL){
					msg.frame_id = OBC_BMS_CCS_FRAME_ID;
				memset(msg.databyte,0,8);
				msg.databyte[0] = INT16TO8_2(hy_chargetask_getoutputvol_x10V());
				msg.databyte[1] = INT16TO8_1(hy_chargetask_getoutputvol_x10V());
				msg.databyte[2] = INT16TO8_2(hy_chargetask_getoutputcur_x10A());
				msg.databyte[3] = INT16TO8_1(hy_chargetask_getoutputcur_x10A());
				msg.databyte[4] = INT16TO8_2(hy_chargetask_getchargetime_min());
				msg.databyte[5] = INT16TO8_1(hy_chargetask_getchargetime_min());
				msg.databyte[6] = OBC_BMS_CCS_ALWAYS_ON;
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				hy_can_send(&msg);
				lastsendtime_ms = hy_time_now_ms();
			}
			break;
			
			
		case HY_CANTASK_BMS_STOP:
			s_cancom->bms_stop_code = HY_CAN_BMS_STOP;
			hy_chargetask_stop(CHARGETASK_CAN_STOP_CODE,&(s_cancom->bms_stop_code));
			if(msg.resendcounts == HY_CAN_MSG_RESEND_DONE){
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				s_cancom->state = HY_CANTASK_END;
				break;
			}else if(msg.resendcounts > HY_CAN_MSG_NEED_RESEND){
				if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CST_INTERVAL){
					hy_can_send(&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"can resend [bms stop] counts = [%d]",msg.resendcounts);
					lastsendtime_ms = hy_time_now_ms();
				}	
				break;
			}
			msg.frame_id = OBC_BMS_CST_FRAME_ID;
			memset(msg.databyte,0,8);
			s_cancom->obc_stop_code =  OBC_BMS_CST_REACH_GOAL(1);
			msg.databyte[0] = INT32TO8_4(s_cancom->obc_stop_code);
			msg.databyte[1] = INT32TO8_3(s_cancom->obc_stop_code);
			msg.databyte[2] = INT32TO8_2(s_cancom->obc_stop_code);
			msg.databyte[3] = INT32TO8_1(s_cancom->obc_stop_code);
			msg.resendcounts = HY_CAN_MSG_RESEND(5);
			hy_can_send(&msg);
			break;
			
			
		case HY_CANTASK_OBC_STOP:
			if(msg.resendcounts <= HY_CAN_MSG_RESEND_DONE){
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				s_cancom->state = HY_CANTASK_END;
				break;
			}else if(msg.resendcounts > HY_CAN_MSG_NEED_RESEND){
				if(systime_elapse_ms(lastsendtime_ms) >= OBC_BMS_CST_INTERVAL){
					hy_can_send(&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"can resend [cantask end] counts = [%d]",msg.resendcounts);
					lastsendtime_ms = hy_time_now_ms();
				}	
				break;
			}
			msg.frame_id = OBC_BMS_CST_FRAME_ID;
			memset(msg.databyte,0,8);
			msg.databyte[0] = INT32TO8_4(s_cancom->obc_stop_code);
			msg.databyte[1] = INT32TO8_3(s_cancom->obc_stop_code);
			msg.databyte[2] = INT32TO8_2(s_cancom->obc_stop_code);
			msg.databyte[3] = INT32TO8_1(s_cancom->obc_stop_code);
			msg.resendcounts = HY_CAN_MSG_RESEND(5);
			hy_can_send(&msg);
			break;
		
		
		case HY_CANTASK_END:
			if(msg.resendcounts == HY_CAN_MSG_RESEND_DONE){
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				s_cancom->state = HY_CANTASK_IDLE;
				break;
			}else if(msg.resendcounts > HY_CAN_MSG_NEED_RESEND){
				if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CSD_INTERVAL){
					hy_can_send(&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"can resend [cantask end] counts = [%d]",msg.resendcounts);
					lastsendtime_ms = hy_time_now_ms();
				}	
				break;
			}
			msg.frame_id = OBC_BMS_CSD_FRAME_ID;
			memset(msg.databyte,0,8);
			msg.databyte[0] = INT16TO8_2(hy_chargetask_getchargetime_min());
			msg.databyte[1] = INT16TO8_1(hy_chargetask_getchargetime_min());
			msg.databyte[2] = INT16TO8_2(hy_chargetask_gettotalpower_x10kwh());
			msg.databyte[3] = INT16TO8_1(hy_chargetask_gettotalpower_x10kwh());
			msg.resendcounts = HY_CAN_MSG_RESEND(600*1000/OBC_BMS_CSD_INTERVAL);//重复10分钟
			hy_can_send(&msg);
			break;
			
			
		case HY_CANTASK_CHARGE_MSG_100MS://100ms 充电器上报状态处理
			
			break;
		case HY_CANTASK_CHARGE_MSG_500MS://500ms 充电器上报状态处理
			
			break;
		
		case HY_CANTASK_ERR:
			break;
		default:
			break;
	}

}


void CAN_IRQHandler()
{
    uint8_t IntStatus;
    /* get interrupt status
     * Note that: Interrupt register CANICR will be reset after read.
     * So function "CAN_IntGetStatus" should be call only one time
     */
    IntStatus = CAN_IntGetStatus(BMS_CAN_TUNNEL_X);
    //check receive interrupt
    if((IntStatus>>0)&0x01)
    {
			CAN_ReceiveMsg(BMS_CAN_TUNNEL_X,&RXMsg);
			s_cancom->msgupdate_flag = HY_TRUE;
			// LOG_DEBUG_TAG(HY_LOG_TAG,"get can msg");
    }
}

int hy_can_connected(void)
{
	return s_cancom->canconnected;
}

int hy_can_restart(int start_code, void* ctx)
{
	s_cancom->state = HY_CANTASK_IDLE;
	return HY_OK;
}

int hy_can_stop(int stop_code, void* ctx)
{
	s_cancom->state = HY_CANTASK_OBC_STOP;
	s_cancom->obc_stop_code = stop_code;
	return HY_OK;
}
