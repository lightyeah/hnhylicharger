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

	hy_can_stop_charger();
	
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
//		LOG_INFO_TAG(HY_LOG_TAG,"can monitor on cancom.state = [%d]",s_cancom->state);
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

#define updatemodule(x) s_cancom->##x##_module_canconnected=HY_TRUE;s_cancom->##x##_module_timeout=hy_time_now_ms();

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
				//gw模块
				case GW_MODULE_1_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output1_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
					s_cancom->get_charger_msg.output1_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					s_cancom->get_charger_msg.statu1_1 = RXMsg.dataA[0];
					s_cancom->get_charger_msg.statu1_2 = RXMsg.dataA[1];
					s_cancom->get_charger_msg.temperature1_x10degree = RXMsg.dataA[3]-40;
					
					break;
				case GW_MODULE_1_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery1_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->get_charger_msg.input1_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);

					break;
				#if (GW_MOUDLE_NUM >= 2)					
				case GW_MODULE_2_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output2_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
					s_cancom->get_charger_msg.output2_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					s_cancom->get_charger_msg.statu2_1 = RXMsg.dataA[0];
					s_cancom->get_charger_msg.statu2_2 = RXMsg.dataA[1];
					s_cancom->get_charger_msg.temperature2_x10degree = RXMsg.dataA[3]-40;
					
					break;
				case GW_MODULE_2_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery2_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->get_charger_msg.input2_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3])

					break;

				#endif

				#if (GW_MOUDLE_NUM >= 3)
				case GW_MODULE_3_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output3_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
					s_cancom->get_charger_msg.output3_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					s_cancom->get_charger_msg.statu3_1 = RXMsg.dataA[0];
					s_cancom->get_charger_msg.statu3_2 = RXMsg.dataA[1];
					s_cancom->get_charger_msg.temperature3_x10degree = RXMsg.dataA[3]-40;
					
					break;
				case GW_MODULE_3_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery3_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->get_charger_msg.input3_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3])

					break;

				#endif

				//朗普达协议
				case LPD_BMS_BROADCAST_ID1:
					updatemodule(bms);
					s_cancom->bms_msg.bms_max_voltage_x10V=INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->bms_msg.bms_max_current_x10A=INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);
					s_cancom->bms_msg.control_byte=RXMsg.dataB[0];
					s_cancom->bms_msg.statu=RXMsg.dataB[1];
					s_cancom->bms_msg.mode=RXMsg.dataB[2];
					hy_can_broadcast_to_bms();
					break;
				case LPD_BMS_BROADCAST_ID2:
					updatemodule(bms);
					s_cancom->bms_msg.single_battery_max_x1000mV=INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->bms_msg.single_battery_min_x1000mA=INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);
					s_cancom->bms_msg.soc_byte=RXMsg.dataB[0];
					s_cancom->bms_msg.temperatue_x1degree = RXMsg.dataB[1]-40;
					s_cancom->bms_msg.battery_voltage_x10V=INT8TO16(RXMsg.dataB[2], RXMsg.dataB[3]);
					break;
				//end 朗普达协议
				case HY_CHARGE_MSG_TEST_FRAME_ID:
					switch (RXMsg.dataA[0]){
						case 0:
							hy_can_query_220V_charger();
							break;
						case 1:
							hy_can_control_query_charger();
							break;
						case 2:
							hy_can_query_temperature_charger();
							break;
						case 3:
							hy_can_query_setting_voltage_charger();
							break;
						case 4:
							hy_can_query_setting_current_charger();
							break;
						case 5:
							hy_can_control_set_charger(50000, 50000);
							break;
						case 6:
							hy_can_start_charger();
							break;
						case 7:
							hy_can_stop_charger();
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
int hy_can_start_charger(void){
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = GW_CONTROL_FRAME_ID;
	
    *((uint8_t *) &TXMsg.dataA[0])= 0xff;//gw 广播地址 
    *((uint8_t *) &TXMsg.dataA[1])= 0x03;// 开机
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}


//YRK 报文控制关闭充电机输出, 
int hy_can_stop_charger(void){
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = GW_CONTROL_FRAME_ID;
					
    *((uint8_t *) &TXMsg.dataA[0])= 0xff;
    *((uint8_t *) &TXMsg.dataA[1])= 0x02;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;

	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}


//设置充电器
int hy_can_control_set_charger(uint32_t voltage_x1000mV, uint32_t current_x1000mA){
	
	uint32_t current_x10A = current_x1000mA/100;
	uint32_t voltage_x10V = voltage_x1000mV/100;
	
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = GW_CONTROL_FRAME_ID;
	

	
	*((uint8_t *) &TXMsg.dataA[0])= 0xff;
	*((uint8_t *) &TXMsg.dataA[1])= 0x03;
	*((uint8_t *) &TXMsg.dataA[2])= INT32TO8_2((voltage_x10V));
	*((uint8_t *) &TXMsg.dataA[3])= INT32TO8_1((voltage_x10V));
	*((uint8_t *) &TXMsg.dataB[0])= INT32TO8_2((current_x10A));
	*((uint8_t *) &TXMsg.dataB[1])= INT32TO8_1((current_x10A));
	*((uint8_t *) &TXMsg.dataB[2])= 0x00;
	*((uint8_t *) &TXMsg.dataB[3])= 0x00;


	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;	
}

//报文控制充电机输出, 
int hy_can_control_query_charger(void){// for YRK

		
    return 0;	
}

//获取设置电压
int hy_can_query_setting_voltage_charger(void){

		
    return 0;	


}
//获取设置电流
int hy_can_query_setting_current_charger(void){

		
    return 0;

}

//获取温度
int hy_can_query_temperature_charger(void){

		
    return 0;
}

//获取输入电压
int hy_can_query_220V_charger(void){

		
    return 0;

}


int hy_can_get_battery_connected(void){
	if(s_cancom->get_charger_msg.statu1_2&0x01){
		s_cancom->battery_module_canconnected=HY_FALSE;
	}else{
		s_cancom->battery_module_canconnected=HY_TRUE;
	}
	return s_cancom->battery_module_canconnected;
}

uint32_t hy_can_get_output_current_x10A(void){

#if (GW_MOUDLE_NUM == 1)
	return s_cancom->get_charger_msg.output1_current_x10A;
#endif

#if (GW_MOUDLE_NUM == 2)
		return s_cancom->get_charger_msg.output1_current_x10A+
		s_cancom->get_charger_msg.output2_current_x10A;
#endif

#if (GW_MOUDLE_NUM == 3)
		return s_cancom->get_charger_msg.output1_current_x10A+
		s_cancom->get_charger_msg.output2_current_x10A+
		s_cancom->get_charger_msg.output3_current_x10A;
#endif

}

uint32_t hy_can_get_output_voltage_x10V(void){
	return s_cancom->get_charger_msg.output1_voltage_x10V;
}

int hy_can_get_intput_voltage_x10V(uint32_t * vol1, uint32_t * vol2, uint32_t * vol3){
//todo
	return s_cancom->get_charger_msg.input1_AC_voltage_x10V;
}

uint32_t hy_can_get_charger_module_temperature_x10degree(void){
	return s_cancom->get_charger_msg.temperature1_x10degree;
}	


uint8_t hy_can_get_charger_module_statu1(void){

#if (GW_MOUDLE_NUM == 1)
		return s_cancom->get_charger_msg.statu1_1;

#endif
	
#if (GW_MOUDLE_NUM == 2)
			return s_cancom->get_charger_msg.statu1_1|
					s_cancom->get_charger_msg.statu2_1;


#endif
	
#if (GW_MOUDLE_NUM == 3)
			return s_cancom->get_charger_msg.statu1_1|
					s_cancom->get_charger_msg.statu2_1|
					s_cancom->get_charger_msg.statu3_1;

#endif


}

uint8_t hy_can_get_charger_module_statu2(void){
#if (GW_MOUDLE_NUM == 1)
			return s_cancom->get_charger_msg.statu1_2;
	
#endif
		
#if (GW_MOUDLE_NUM == 2)
				return s_cancom->get_charger_msg.statu1_2|
						s_cancom->get_charger_msg.statu2_2;
	
	
#endif
		
#if (GW_MOUDLE_NUM == 3)
				return s_cancom->get_charger_msg.statu1_2|
						s_cancom->get_charger_msg.statu2_2|
						s_cancom->get_charger_msg.statu3_2;
	
#endif

}


uint8_t hy_can_get_charger_module_connected(void)
{
	if(systime_elapse_ms(s_cancom->charger_module_timeout)>=LPD_TIMEOUT_TIME){
		s_cancom->charger_module_canconnected=HY_FALSE;
	}else{
		s_cancom->charger_module_canconnected=HY_TRUE;
	}
	//LOG_DEBUG_TAG("HY_CAN", "charger module connected [%d]", s_cancom->charger_module_canconnected);
	return s_cancom->charger_module_canconnected;

}


int hy_can_detect_charger(void){
	hy_can_stop_charger();
	return 0;
	
}


//bms*********************************


int hy_can_get_bms_set_voltage_10V(void)
{
	return s_cancom->bms_msg.bms_max_voltage_x10V;
}
int hy_can_get_bms_set_current_10A(void)
{
	return s_cancom->bms_msg.bms_max_current_x10A;
}

int hy_can_get_bms_temperature(void)
{
	return s_cancom->bms_msg.temperatue_x1degree;
}

int hy_can_get_bms_control(void){
	return s_cancom->bms_msg.control_byte;
}

int hy_can_get_bms_status(void)
{
	return s_cancom->bms_msg.statu;
}

int hy_can_get_bms_mode(void)
{
	return s_cancom->bms_msg.mode;
}

int hy_can_get_bms_battery_voltage_x10V(void)
{
	return s_cancom->bms_msg.battery_voltage_x10V;
}

int hy_can_set_output_msg(uint16_t voltage_x10V, uint16_t current_x10A)
{
	s_cancom->charge_to_msg_msg.charge_output_current_x10A=current_x10A;
	s_cancom->charge_to_msg_msg.charge_output_voltage_x10V=voltage_x10V;
	return 0;
}


int hy_can_set_status_msg(uint8_t status)
{
	s_cancom->charge_to_msg_msg.status=status;
	return 0;
}





int hy_can_broadcast_to_bms(void){
	
    TXMsg.format = HY_CHARGE_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = CHARGER_TO_LPD_ID;
	

	
	*((uint8_t *) &TXMsg.dataA[0])= INT16TO8_2(s_cancom->charge_to_msg_msg.charge_output_voltage_x10V);
	*((uint8_t *) &TXMsg.dataA[1])= INT16TO8_1(s_cancom->charge_to_msg_msg.charge_output_voltage_x10V);
	*((uint8_t *) &TXMsg.dataA[2])= INT16TO8_2(s_cancom->charge_to_msg_msg.charge_output_current_x10A);
	*((uint8_t *) &TXMsg.dataA[3])= INT16TO8_1(s_cancom->charge_to_msg_msg.charge_output_current_x10A);
	*((uint8_t *) &TXMsg.dataB[0])= s_cancom->charge_to_msg_msg.status;
	*((uint8_t *) &TXMsg.dataB[1])= 0x00;
	*((uint8_t *) &TXMsg.dataB[2])= 0x00;
	*((uint8_t *) &TXMsg.dataB[3])= 0x00;


	CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
		
    return 0;		


}


int hy_can_detect_bms(void){//tood
	hy_can_broadcast_to_bms();
	return 0;
}

uint8_t hy_can_get_bms_connected(void){
	if(systime_elapse_ms(s_cancom->bms_module_timeout)>=LPD_TIMEOUT_TIME){
		s_cancom->bms_module_canconnected=HY_FALSE;
	}else{
		s_cancom->bms_module_canconnected=HY_TRUE;
	}
	return s_cancom->bms_module_canconnected;
}




