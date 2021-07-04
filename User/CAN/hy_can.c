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


int hy_can_init(void* hy_instance_handle)
{
		int ret = HY_OK;
	  PINSEL_CFG_Type PinCfg;
	
		/*software init*/
		hy_instance_t *hy_instance=(hy_instance_t*)hy_instance_handle;
		s_cancom = &(hy_instance->cancom);
		memset(s_cancom,0,sizeof(hy_cancom_t));
		s_cancom->state = HY_CANTASK_IDLE;
		s_cancom->charger_module_canconnected = HY_FALSE;
		s_cancom->bms_module_connected = HY_FALSE;
		s_cancom->bms_module_timeout = 0;
		s_cancom->charger_module_timeout=0;
		
    
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;    
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);   
		
    CAN_Init(BMS_CAN_TUNNEL_X,hy_instance->config.communicaterate);
    LOG_INFO_TAG(HY_LOG_TAG,"set can baudrate %d",hy_instance->config.communicaterate);
    
    CAN_IRQCmd(BMS_CAN_TUNNEL_X,CANINT_RIE, ENABLE);

    NVIC_EnableIRQ(CAN_IRQn);
    //NVIC_SetPriority(CAN_IRQn, 10);
    CAN_SetAFMode(LPC_CANAF,CAN_AccBP);	
		LOG_INFO_TAG(HY_LOG_TAG,"hy can init done!!");
		return ret;
}




int hy_can_getmsg()
{
	int ret = HY_OK;

	if(s_cancom->charger_module_canmsg.updateflag){//更新充电模块信息
		LOG_DEBUG_TAG(HY_LOG_TAG, "can update new msg ");
		s_cancom->charger_module_canmsg.updateflag = HY_FALSE;/*clear flag*/
		s_cancom->charger_module_timeout = 0;//计时清零
	
		switch(s_cancom->charger_module_canmsg.frame_id){

			//YRK 
			case HY_YRK_CONTROL_BACK_FRAME_ID:
//				s_cancom->state = HY_CANTASK_NORMAL;
//				if(RXMsg.dataA[0]==0x01){//返回模块信息
//					LOG_ERROR_TAG(HY_LOG_TAG,"======GET voltage [%d] current [%d]",INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]),INT8TO16(RXMsg.dataA[2],RXMsg.dataA[3]));	
//					hy_set_currentfb_x10A(INT8TO16(RXMsg.dataA[2],RXMsg.dataA[3]));//YRK 返回电流 byte2,3
//					hy_set_voltagefb_x10V(INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]));//YRK 返回电压 byte4 5
//					
//					if(ghy_can_GWcharger_batteryoff_flag==1&&INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1])>=150){//电池未连接的时候，电压不等于0
//						ghy_can_GWcharger_batteryoff_flag=0;
//						
//					}else if(ghy_can_GWcharger_batteryoff_flag==0&&INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1])<=150){//电池连接的时候，电压等于0
//						ghy_can_GWcharger_batteryoff_flag=1;
//					}
//				}
				break;
			case HY_CHARGE_MSG_TEST_FRAME_ID:
				break;
			//YRK
		
			default:

				break;
		}
	}

	if(s_cancom->bms_module_canmsg.updateflag){/*bms 信息*/
		
//		if (systime_elapse_ms(s_cancom->connectupdate_time_ms) >= HY_CAN_CONNECT_TIMEOUT){
//			s_cancom->state = HY_CANTASK_IDLE;
//			s_cancom->canconnected = HY_FALSE;
//			s_cancom->obc_stop_code = HY_CAN_OBC_STOP_CANTIMEOUT;
//			hy_chargetask_stop(CHARGETASK_CAN_STOP_CODE,&(s_cancom->obc_stop_code));
//		}

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
			break;
		case HY_CANTASK_NORMAL:
			break;
		case HY_CANTASK_ERR:
			break;
		default:
			break;
	}

}

#define updatemodule(x) s_cancom->##x##_module_canconnected=HY_TRUE;s_cancom->##x##_module_timeout=0

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
			//todo
			switch (RXMsg.id){
				//YRK 模块
				case HY_YRK_CONTROL_BACK_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
					switch (RXMsg.dataA[0]){
						case 1://模块返回状态信息
							s_cancom->charger_msg.output_current_x10A = INT8TO16(RXMsg.dataA[2],RXMsg.dataA[3]);
							s_cancom->charger_msg.output_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
							s_cancom->charger_msg.statu1 = RXMsg.dataB[2];
							s_cancom->charger_msg.statu2 = RXMsg.dataB[3];
							break;
					}
					break;
					
				case HY_YRK_READ_SETTING_CURRENT_BACK_FRAMD_ID://TODO
					updatemodule(charger);
					break;

				case HY_YRK_READ_SETTING_VOLTAGE_BACK_FRAMD_ID://TODO
					updatemodule(charger);
					break;

				case HY_YRK_READ_INPUT_VOLTAGE_BACK_FRAMD_ID://TODO
					updatemodule(charger);
					break;
				case HY_YRK_READ_TEMPERATURE_BACK_FRAMD_ID:
					updatemodule(charger);
					s_cancom->charger_msg.temperature_x10degree = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					break;
				case HY_CHARGE_MSG_TEST_FRAME_ID:
					switch (RXMsg.dataA[0]){
						case 0:
							hy_can_query_220V_yrkcharger();
							break;
						case 1:
							hy_can_control_query_YRKcharger();
							break;
						case 2:
							hy_can_query_temperature_yrkcharger();
							break;
						case 3:
							hy_can_query_setting_voltage_yrkcharger();
							break;
						case 4:
							hy_can_query_setting_current_yrkcharger();
							break;
						case 5:
							hy_can_control_set_yrkcharger(50000, 50000);
							break;
						case 6:
							hy_can_start_YRKcharger();
							break;
						case 7:
							hy_can_stop_YRKcharger();
							break;
					}
					break;
				//end YRK
				default:
					break;
				 
			}
			// LOG_DEBUG_TAG(HY_LOG_TAG,"get can msg");
    }
}



////////////////
///yrk
///////////////////


//YRK
//报文控制开始充电机输出, 
int hy_can_start_YRKcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_YRK_CONTROL_FRAME_ID;
	
    *((uint8_t *) &TXMsg.dataA[0])= 0x02;//控制指令2 
    *((uint8_t *) &TXMsg.dataA[1])= 0x00;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x55;//YRK 开机

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}


//YRK 报文控制关闭充电机输出, 
int hy_can_stop_YRKcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_YRK_CONTROL_FRAME_ID;
					
    *((uint8_t *) &TXMsg.dataA[0])= 0x02;//YRK 控制指令2 
    *((uint8_t *) &TXMsg.dataA[1])= 0x00;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0xAA;//YRK 关机

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}


//询问YRK充电器信息
int hy_can_control_set_yrkcharger(uint32_t current_x1000mA, uint32_t voltage_x1000mV){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_YRK_CONTROL_FRAME_ID;
	
	*((uint8_t *) &TXMsg.dataA[0])= 0x00;//YRK 设定输出
	*((uint8_t *) &TXMsg.dataA[1])= INT32TO8_3((current_x1000mA));//YRK 电流
	*((uint8_t *) &TXMsg.dataA[2])= INT32TO8_2((current_x1000mA));//YRK 电流
	*((uint8_t *) &TXMsg.dataA[3])= INT32TO8_1((current_x1000mA));//YRK 电流
	*((uint8_t *) &TXMsg.dataB[0])= INT32TO8_4((voltage_x1000mV));//YRK 电压
	*((uint8_t *) &TXMsg.dataB[1])= INT32TO8_3((voltage_x1000mV));//YRK 电压
	*((uint8_t *) &TXMsg.dataB[2])= INT32TO8_2((voltage_x1000mV));//YRK 电压
	*((uint8_t *) &TXMsg.dataB[3])= INT32TO8_1((voltage_x1000mV));//YRK 电压


	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}

//YRK 报文控制询问英瑞可充电机输出, 
int hy_can_control_query_YRKcharger(void){// for YRK
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = HY_YRK_CONTROL_FRAME_ID;
	
    *((uint8_t *) &TXMsg.dataA[0])= 0x01;
    *((uint8_t *) &TXMsg.dataA[1])= 0x00;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;//关机

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}

//获取设置电压
int hy_can_query_setting_voltage_yrkcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 0;
    TXMsg.id = HY_YRK_READ_SETTING_VOLTAGE_FRAMD_ID;
	

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	


}
//获取设置电流
int hy_can_query_setting_current_yrkcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 0;
    TXMsg.id = HY_YRK_READ_SETTING_CURRENT_FRAMD_ID;
	

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;

}

//获取温度
int hy_can_query_temperature_yrkcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 0;
    TXMsg.id = HY_YRK_READ_TEMPERATURE_FRAMD_ID;
	

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;
}

//获取输入电压
int hy_can_query_220V_yrkcharger(void){
    TXMsg.format = HY_CHARGE_ID_EXT_FORMAT;
    TXMsg.len = 0;
    TXMsg.id = HY_YRK_READ_INPUT_VOLTAGE_FRAMD_ID;
	

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;

}



int hy_can_get_batterystate(void){
	//todo
	return 0;
}

uint32_t hy_can_get_output_current_x10A(void){
	return s_cancom->charger_msg.output_current_x10A;
}

uint32_t hy_can_get_output_voltage_x10V(void){
	return s_cancom->charger_msg.output_voltage_x10V;
}

int hy_can_get_intput_voltage_x10V(uint32_t * vol1, uint32_t * vol2, uint32_t * vol3){
//todo
	return 0;
}

uint32_t hy_can_get_charger_module_temperature_x10degree(void){
	return s_cancom->charger_msg.temperature_x10degree;
}	


uint8_t hy_can_get_charger_module_statu1(void){
	return s_cancom->charger_msg.statu1;
}

uint8_t hy_can_get_charger_module_statu2(void){
	return s_cancom->charger_msg.statu2;
}


uint8_t hy_can_charger_module_connected(void){
	return s_cancom->charger_module_canconnected;
}

uint8_t hy_can_bms_connected(void){
	return s_cancom->bms_module_connected;
}


int hy_can_detect_yrkcharger(void){
	hy_can_stop_YRKcharger();
	return 0;
}

int hy_can_detect_bms(void){//tood
	return 0;
}






