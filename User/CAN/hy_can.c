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

int hy_can_send(hy_canmsg* msg)
{
		if(msg->resendcounts > HY_CAN_MSG_NEED_RESEND){
			msg->resendcounts--;
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

int hy_can_getmsg()
{
	int ret = HY_OK;
	if(s_cancom == NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"***hy can not init!!!");
		ret = HY_ERROR;
		goto err_exit;
	}
	/*todo receive timeout!!!*/
	if(s_cancom->msgupdate_flag){
		LOG_DEBUG_TAG(HY_LOG_TAG, "get can new msg");
		s_cancom->msgupdate_flag = HY_FALSE;/*clear flag*/
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
						/*todo timeout*/
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
			default:
				break;
		}
	}else{
	}
	
	err_exit:
	if(s_cancom){
		s_cancom->msgupdate_flag = HY_FALSE;
	}
	return ret;
}

void hy_can_task_main()
{
	static hy_canmsg msg = {0};
	static uint32_t lastsendtime_ms = 0;
	static uint32_t canmonitortime_ms = 0;
	if(s_cancom == NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"***hy can not init!!!");
		goto err_exit;
	}
	
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
				hy_can_send(&msg);
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
				msg.databyte[0] = INT16TO8_1((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
				msg.databyte[1] = INT16TO8_2((hy_config_readvoltagerange()*10)&HY_MAX_VOLTAGE);
				msg.databyte[4] = INT16TO8_1((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
				msg.databyte[5] = INT16TO8_2((hy_config_readcurrentrange()*10)&HY_MAX_CURRENT);
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
				msg.databyte[0] = INT16TO8_1(hy_chargetask_getoutputvol_x10V());
				msg.databyte[1] = INT16TO8_2(hy_chargetask_getoutputvol_x10V());
				msg.databyte[2] = INT16TO8_1(hy_chargetask_getoutputcur_x10A());
				msg.databyte[3] = INT16TO8_2(hy_chargetask_getoutputcur_x10A());
				msg.databyte[4] = INT16TO8_1(hy_chargetask_getchargetime_min());
				msg.databyte[5] = INT16TO8_2(hy_chargetask_getchargetime_min());
				msg.databyte[6] = OBC_BMS_CCS_ALWAYS_ON;
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				hy_can_send(&msg);
				lastsendtime_ms = hy_time_now_ms();
			}
			break;
			
			
		case HY_CANTASK_BMS_STOP:
			s_cancom->bms_stop_code = hy_can_msg_stop_handle();
			hy_chargetask_stop(CHARGETASK_CAN_STOP_CODE,NULL);
			if(msg.resendcounts == HY_CAN_MSG_RESEND_DONE){
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				s_cancom->state = HY_CANTASK_END;
				break;
			}else if(msg.resendcounts > HY_CAN_MSG_NEED_RESEND){
				if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CST_INTERVAL){
					hy_can_send(&msg);
					lastsendtime_ms = hy_time_now_ms();
				}	
				break;
			}
			msg.frame_id = OBC_BMS_CST_FRAME_ID;
			memset(msg.databyte,0,8);
			s_cancom->obc_stop_code =  OBC_BMS_CST_REACH_GOAL(1);
			msg.databyte[0] = INT32TO8_1(s_cancom->obc_stop_code);
			msg.databyte[1] = INT32TO8_2(s_cancom->obc_stop_code);
			msg.databyte[2] = INT32TO8_3(s_cancom->obc_stop_code);
			msg.databyte[3] = INT32TO8_4(s_cancom->obc_stop_code);
			msg.resendcounts = HY_CAN_MSG_RESEND(5);
			hy_can_send(&msg);
			break;
			
			
		case HY_CANTASK_OBC_STOP:
			break;
		
		
		case HY_CANTASK_END:
			if(msg.resendcounts == HY_CAN_MSG_RESEND_DONE){
				msg.resendcounts = HY_CAN_MSG_NO_RESEND;
				s_cancom->state = HY_CANTASK_IDLE;
				break;
			}else if(msg.resendcounts > HY_CAN_MSG_NEED_RESEND){
				if(systime_elapse_ms(lastsendtime_ms)>=OBC_BMS_CSD_INTERVAL){
					hy_can_send(&msg);
					lastsendtime_ms = hy_time_now_ms();
				}	
				break;
			}
			msg.frame_id = OBC_BMS_CSD_FRAME_ID;
			memset(msg.databyte,0,8);
			msg.databyte[0] = INT16TO8_1(hy_chargetask_getchargetime_min());
			msg.databyte[1] = INT16TO8_2(hy_chargetask_getchargetime_min());
			msg.databyte[2] = INT16TO8_1(hy_chargetask_gettotalpower_x10kwh());
			msg.databyte[3] = INT16TO8_2(hy_chargetask_gettotalpower_x10kwh());
			msg.resendcounts = HY_CAN_MSG_RESEND(5);
			hy_can_send(&msg);
			break;
			
			
		case HY_CANTASK_ERR:
			break;
		default:
			break;
	}
	err_exit:
	return;
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
			LOG_DEBUG_TAG(HY_LOG_TAG,"get can msg");
    }
}


