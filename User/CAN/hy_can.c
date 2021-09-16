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

PINSEL_CFG_Type PinCfg;

int hy_can_init(void* hy_instance_handle)
{
		int ret = HY_OK;
	  
	
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

CAN_MSG_Type RXMsgQueue[16];
int canReciveMsgCount=0;
int canReadMsgCount=0;
#define updatemodule(x) s_cancom->##x##_module_canconnected=HY_TRUE;s_cancom->##x##_module_timeout=hy_time_now_ms();

void hy_can_task_main()
{
	static hy_canmsg msg = {0};
	static uint32_t lastsendtime_ms = 0;
	static uint32_t canmonitortime_ms = 0;
	
	lastsendtime_ms = lastsendtime_ms;

	while(canReadMsgCount < canReciveMsgCount){
		switch (RXMsgQueue[canReadMsgCount%16].id){
				//gw模块
				case GW_MODULE_1_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output1_current_x10A = INT8TO16(RXMsgQueue[canReadMsgCount%16].dataB[2],RXMsgQueue[canReadMsgCount%16].dataB[3]);
					s_cancom->get_charger_msg.output1_voltage_x10V = INT8TO16(RXMsgQueue[canReadMsgCount%16].dataB[0],RXMsgQueue[canReadMsgCount%16].dataB[1]);
					s_cancom->get_charger_msg.statu1_1 = RXMsgQueue[canReadMsgCount%16].dataA[0];
					s_cancom->get_charger_msg.statu1_2 = RXMsgQueue[canReadMsgCount%16].dataA[1];
					s_cancom->get_charger_msg.temperature1_x1degree = RXMsgQueue[canReadMsgCount%16].dataA[3]-40;
					
					break;
				case GW_MODULE_1_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery1_voltage = INT8TO16(RXMsgQueue[canReadMsgCount%16].dataA[0], RXMsgQueue[canReadMsgCount%16].dataA[1]);
					s_cancom->get_charger_msg.input1_AC_voltage_x10V = INT8TO16(RXMsgQueue[canReadMsgCount%16].dataA[2], RXMsgQueue[canReadMsgCount%16].dataA[3]);

					break;
					

				//千航
				case 0x180215f4:
				case 0x190515f4:
					updatemodule(bms);
					break;
				case QIANHANG_BMS_ID:
					updatemodule(bms);
					s_cancom->bms_msg.bms_max_voltage_x10V=INT8TO16(RXMsgQueue[canReadMsgCount%16].dataA[0], RXMsgQueue[canReadMsgCount%16].dataA[1]);
					s_cancom->bms_msg.bms_max_current_x10A=INT8TO16(RXMsgQueue[canReadMsgCount%16].dataA[2], RXMsgQueue[canReadMsgCount%16].dataA[3]);
					s_cancom->bms_msg.control_byte=RXMsgQueue[canReadMsgCount%16].dataB[0];
					s_cancom->bms_msg.soc=RXMsgQueue[canReadMsgCount%16].dataB[1];
					//hy_can_broadcast_to_bms();
					break;

				//end 千航

				 
		}			
		canReadMsgCount++;
	}
//	hy_can_getmsg();
////	if(systime_elapse_ms(canmonitortime_ms)>=HY_CAN_TASK_MONITOR_INTERVAL){
////		canmonitortime_ms = hy_time_now_ms();
////		LOG_INFO_TAG(HY_LOG_TAG,"can monitor on cancom.state = [%d]",s_cancom->state);
////	}
//	
//	switch (s_cancom->state){
//		case HY_CANTASK_IDLE:
//			break;
//		case HY_CANTASK_NORMAL:
//			break;
//		case HY_CANTASK_ERR:
//			break;
//		default:
//			break;
//	}

}



int canprotect = 0;
uint32_t irqtimeout = 0;

void CAN_IRQHandler()
{
    uint8_t IntStatus;
    /* get interrupt status
     * Note that: Interrupt register CANICR will be reset after read.
     * So function "CAN_IntGetStatus" should be call only one time
     */
     canprotect = 1;
	irqtimeout = hy_time_now_ms();
    IntStatus = CAN_IntGetStatus(BMS_CAN_TUNNEL_X);
	//CAN_FullCANIntGetStatus(LPC_CANAF);
    //check receive interrupt
    //LOG_DEBUG_TAG("HY_CAN", "irq [%x]", IntStatus);
    if((IntStatus>>0)&0x01)
    {
			CAN_ReceiveMsg(BMS_CAN_TUNNEL_X,&RXMsg);

			memcpy((char*)&RXMsgQueue[(canReciveMsgCount++)%16],(const char*)&RXMsg,sizeof(CAN_MSG_Type));
			//todo
			//LOG_DEBUG_TAG("HY_CAN", "irq [%x]", RXMsg.id);
//			switch (RXMsg.id){
//				//gw模块
//				case GW_MODULE_1_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
//					updatemodule(charger);
//				
//					s_cancom->get_charger_msg.output1_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
//					s_cancom->get_charger_msg.output1_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
//					s_cancom->get_charger_msg.statu1_1 = RXMsg.dataA[0];
//					s_cancom->get_charger_msg.statu1_2 = RXMsg.dataA[1];
//					s_cancom->get_charger_msg.temperature1_x1degree = RXMsg.dataA[3]-40;
//					
//					break;
//				case GW_MODULE_1_MSG_500MS_MSG_FRAME_ID:
//					updatemodule(charger);
//
//					s_cancom->get_charger_msg.battery1_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
//					s_cancom->get_charger_msg.input1_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);
//
//					break;
//					
//
//				//千航
//				case 0x180215f4:
//				case 0x190515f4:
//					updatemodule(bms);
//					break;
//				case QIANHANG_BMS_ID:
//					updatemodule(bms);
//					s_cancom->bms_msg.bms_max_voltage_x10V=INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
//					s_cancom->bms_msg.bms_max_current_x10A=INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);
//					s_cancom->bms_msg.control_byte=RXMsg.dataB[0];
//					s_cancom->bms_msg.soc=RXMsg.dataB[1];
//					//hy_can_broadcast_to_bms();
//					break;
//
//				//end 千航
//
//				 
//			}
			canprotect = 0;
			// LOG_DEBUG_TAG(HY_LOG_TAG,"get can msg");
    }
}



////////////////
///yrk
///////////////////


//YRK
//报文控制开始充电机输出, 
int hy_can_start_charger(void){
	int i = 0;
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

	while(canprotect==1);
	i = CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
	//LOG_DEBUG_TAG("HY_CAN", "sendres [%x]", i);
    return 0;	
}


//YRK 报文控制关闭充电机输出, 
int hy_can_stop_charger(void){
	int i = 0;	
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

	while(canprotect==1);
	i = CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
	//LOG_DEBUG_TAG("HY_CAN", "sendres [%x]", i);
    return 0;	
}


//设置充电器
int hy_can_control_set_charger(uint32_t voltage_x10V, uint32_t current_x10A){

	int i = 0;

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

	while(canprotect==1);
	i = CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
	//LOG_DEBUG_TAG("HY_CAN", "sendres [%x]", i);
		
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



		return s_cancom->get_charger_msg.output1_current_x10A;//+
//		s_cancom->get_charger_msg.output2_current_x10A+
//		s_cancom->get_charger_msg.output3_current_x10A+
//		s_cancom->get_charger_msg.output4_current_x10A;


}

uint32_t hy_can_get_output_voltage_x10V(void){
	return s_cancom->get_charger_msg.output1_voltage_x10V;
}

int hy_can_get_intput_voltage_x10V(uint32_t * vol1, uint32_t * vol2, uint32_t * vol3){
//todo
	return s_cancom->get_charger_msg.input1_AC_voltage_x10V;
}

uint32_t hy_can_get_charger_module_temperature_x10degree(void){
	return s_cancom->get_charger_msg.temperature1_x1degree;
}	


uint8_t hy_can_get_charger_module_statu1(void){


			return s_cancom->get_charger_msg.statu1_1;
//				|
//					s_cancom->get_charger_msg.statu2_1|
//					s_cancom->get_charger_msg.statu3_1|
//					s_cancom->get_charger_msg.statu4_1;



}

uint8_t hy_can_get_charger_module_statu2(void){

				return s_cancom->get_charger_msg.statu1_2;
//					|
//						s_cancom->get_charger_msg.statu2_2|
//						s_cancom->get_charger_msg.statu3_2|
//						s_cancom->get_charger_msg.statu4_2;
	

}


uint8_t hy_can_get_charger_module_connected(void)
{
	if(systime_elapse_ms(irqtimeout)>=(2000)){
		irqtimeout=hy_time_now_ms();
		CAN_DeInit(BMS_CAN_TUNNEL_X);
		PinCfg.Funcnum = 1;
	    PinCfg.OpenDrain = 0;
	    PinCfg.Pinmode = 0;    
	    PinCfg.Portnum = 0;
	    PinCfg.Pinnum = 0;
	    PINSEL_ConfigPin(&PinCfg);
	    PinCfg.Pinnum = 1;
	    PINSEL_ConfigPin(&PinCfg);  
		delay_ms(10);
		CAN_Init(BMS_CAN_TUNNEL_X,250000);
	    LOG_INFO_TAG(HY_LOG_TAG,"REset can  ");
	    delay_ms(10);
	    CAN_IRQCmd(BMS_CAN_TUNNEL_X,CANINT_RIE, ENABLE);

	    NVIC_EnableIRQ(CAN_IRQn);
	    //NVIC_SetPriority(CAN_IRQn, 10);
	    CAN_SetAFMode(LPC_CANAF,CAN_AccBP);	
		delay_ms(10);
//		hy_can_broadcast_to_bms();
	}
	if(systime_elapse_ms(s_cancom->charger_module_timeout)>=QH_TIMEOUT_TIME){
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
	return 0;
}

int hy_can_get_bms_control(void){
	return s_cancom->bms_msg.control_byte;
}

int hy_can_get_bms_soc(void){
	return s_cancom->bms_msg.soc;
}

int hy_can_get_bms_status(void)
{
	return 0;//_cancom->bms_msg.statu;
}

int hy_can_get_bms_mode(void)
{
	return 0;//s_cancom->bms_msg.mode;
}

int hy_can_get_bms_battery_voltage_x10V(void)
{
	return 0;//s_cancom->bms_msg.battery_voltage_x10V;
}

int hy_can_set_output_msg(uint16_t voltage_x10V, uint16_t current_x10A)
{
	s_cancom->charge_to_bms_msg.output_current_x10A=current_x10A;
	s_cancom->charge_to_bms_msg.output_voltage_x10V=voltage_x10V;
	return 0;
}


int hy_can_set_status_msg(uint8_t status)
{
	s_cancom->charge_to_bms_msg.status=status;
	return 0;
}





int hy_can_broadcast_to_bms(void){
	int i = 0;
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.id = QIANHANG_CHARGER_ID;
	

	
	*((uint8_t *) &TXMsg.dataA[0])= INT16TO8_2(s_cancom->charge_to_bms_msg.output_voltage_x10V);
	*((uint8_t *) &TXMsg.dataA[1])= INT16TO8_1(s_cancom->charge_to_bms_msg.output_voltage_x10V);
	*((uint8_t *) &TXMsg.dataA[2])= INT16TO8_2(s_cancom->charge_to_bms_msg.output_current_x10A);
	*((uint8_t *) &TXMsg.dataA[3])= INT16TO8_1(s_cancom->charge_to_bms_msg.output_current_x10A);
	*((uint8_t *) &TXMsg.dataB[0])= s_cancom->charge_to_bms_msg.status;
	*((uint8_t *) &TXMsg.dataB[1])= 0x00;
	*((uint8_t *) &TXMsg.dataB[2])= 0x00;
	*((uint8_t *) &TXMsg.dataB[3])= 0x00;

	while(canprotect==1);
	i = CAN_SendMsg(CHARGER_CAN_TUNNEL_X, &TXMsg);
	//LOG_DEBUG_TAG("HY_CAN", "sendres [%x]", i);
		
    return 0;		


}


int hy_can_detect_bms(void){//tood
	hy_can_broadcast_to_bms();
	return 0;
}


//检测电池BMS是否连接
uint8_t hy_can_get_bms_connected(void){
	if(systime_elapse_ms(s_cancom->bms_module_timeout)>=100000){
		s_cancom->bms_module_canconnected=HY_FALSE;
	}else{
		s_cancom->bms_module_canconnected=HY_TRUE;
	}
	return s_cancom->bms_module_canconnected;
}




