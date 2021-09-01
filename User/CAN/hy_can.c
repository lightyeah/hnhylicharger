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
		s_cancom->charger_addr = (uint32_t)DEFAULT_CHARGER_ADDR;
		s_cancom->charge_to_msg_msg.machine_type = 10024;//100V24V
		
		s_cancom->charge_to_msg_msg.control_status = 0x02;//默认不上锁，执行配置参数
		
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
		//LOG_INFO_TAG(HY_LOG_TAG,"can monitor on cancom.state = [%d]",s_cancom->state);
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
			//LOG_DEBUG_TAG("CAN IRQ", "rc [%x] [%x]",RXMsg.id,RXMsg.dataB[2]);
			switch (RXMsg.id){
				//gw模块
				case GW_MODULE_1_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output1_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
					s_cancom->get_charger_msg.output1_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					s_cancom->get_charger_msg.statu1_1 = RXMsg.dataA[0];
					s_cancom->get_charger_msg.statu1_2 = RXMsg.dataA[1];
					s_cancom->get_charger_msg.temperature1_x1degree = RXMsg.dataA[3]-40;
					s_cancom->charge_to_msg_msg.charger_temperature = s_cancom->get_charger_msg.temperature1_x1degree;
						
					break;
				case GW_MODULE_1_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery1_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->get_charger_msg.input1_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3]);
					s_cancom->charge_to_msg_msg.input_220V_voltage_x100V = s_cancom->get_charger_msg.input1_AC_voltage_x10V*10;
					break;
#if (GW_MOUDLE_NUM >= 2)					
				case GW_MODULE_2_MSG_100MS_MSG_FRAME_ID://返回电压电流和状态
					updatemodule(charger);
				
					s_cancom->get_charger_msg.output2_current_x10A = INT8TO16(RXMsg.dataB[2],RXMsg.dataB[3]);
					s_cancom->get_charger_msg.output2_voltage_x10V = INT8TO16(RXMsg.dataB[0],RXMsg.dataB[1]);
					s_cancom->get_charger_msg.statu2_1 = RXMsg.dataA[0];
					s_cancom->get_charger_msg.statu2_2 = RXMsg.dataA[1];
					s_cancom->get_charger_msg.temperature2_x1degree = RXMsg.dataA[3]-40;
					
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
					s_cancom->get_charger_msg.temperature3_x1degree = RXMsg.dataA[3]-40;
					
					break;
				case GW_MODULE_3_MSG_500MS_MSG_FRAME_ID:
					updatemodule(charger);

					s_cancom->get_charger_msg.battery3_voltage = INT8TO16(RXMsg.dataA[0], RXMsg.dataA[1]);
					s_cancom->get_charger_msg.input3_AC_voltage_x10V = INT8TO16(RXMsg.dataA[2], RXMsg.dataA[3])

					break;

#endif

				//圣阳协议
				case TP_BAM_CONTROLER_ID:
					if (RXMsg.dataB[2]==0xf1){//CONTROL_REQUEST_PGN
						
						switch (RXMsg.dataB[1]){
							case 0x01:
								//updatemodule(bms);
								s_cancom->sy_bam = CONTROL_REQUEST_PGN;
								break;
							case 0x02:
								//updatemodule(bms);
								s_cancom->sy_bam = CONFIG_REQUEST_PGN;
								break;
							case 0x03:
								//updatemodule(bms);
								s_cancom->sy_bam = EXIT_REQUEST_PGN;
								break;
							case 0x04:
								//updatemodule(bms);
								s_cancom->sy_bam = BROADCAST_REQUEST_PGN;
								break;
							case 0x05:
								//updatemodule(bms);
								s_cancom->sy_bam = ADDRESS_RESET_REQUEST_PGN;
								break;
							case 0x06:
								//updatemodule(bms);
								s_cancom->sy_bam = CONTROL_BROADCAST_PGN;
								break;
							default:
							s_cancom->sy_bam = 0xff;
								break;
						}

					}
					break;
				case TP_DP_CONTROLER_ID:
					updatemodule(bms);
					switch(s_cancom->sy_bam){
						case CONTROL_REQUEST_PGN:
							hy_sy_control_response(s_cancom->charger_addr);
							s_cancom->charge_to_msg_msg.control_status = 0x02;//执行配置参数
							break;
						case CONFIG_REQUEST_PGN:
							s_cancom->bms_msg.aim_voltage_x100V = (uint32_t)INT8TO16(RXMsg.dataA[2], RXMsg.dataA[1]);
							s_cancom->bms_msg.aim_current_x100A = (uint32_t)INT8TO16(RXMsg.dataB[0], RXMsg.dataA[3]);
							s_cancom->bms_msg.charger_percent = RXMsg.dataB[1];
							s_cancom->charge_to_msg_msg.control_status = 0x02;//执行配置参数
							hy_sy_config_response(s_cancom->charger_addr);
							break;
						case EXIT_REQUEST_PGN:
							hy_sy_exit_response(s_cancom->charger_addr);
							s_cancom->charge_to_msg_msg.control_status = 0x03;//锁定配置参数
							break;
						case BROADCAST_REQUEST_PGN:
							hy_sy_broadcast_response(s_cancom->charger_addr);
							break;
						case ADDRESS_RESET_REQUEST_PGN:
							s_cancom->charger_addr = (0xffffffff&RXMsg.dataA[1]);
							hy_sy_address_reset_response(s_cancom->charger_addr);
							break;
						case CONTROL_BROADCAST_PGN:
							s_cancom->bms_msg.battery_temperature = RXMsg.dataA[1]-10;
							s_cancom->charge_to_msg_msg.battery_temperature = s_cancom->bms_msg.battery_temperature;
							s_cancom->bms_msg.soc = RXMsg.dataA[2];
							s_cancom->bms_msg.soh = RXMsg.dataA[3];
							s_cancom->bms_msg.battery_status = RXMsg.dataB[0];
							s_cancom->bms_msg.battery_warnning = RXMsg.dataB[1];
							break;
						}
					break;
				case ADDRESS_QUERY_FRAME_ID:
					hy_sy_address_reset_response(s_cancom->charger_addr);
					break;
				//测试
				case HY_CHARGE_MSG_TEST_FRAME_ID:
					switch (RXMsg.dataA[0]){
		
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





///圣阳

/**
* 控制请求应答
**/
int hy_sy_control_response(uint32_t charger_addr){
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;
	if(charger_addr == 0){
    	TXMsg.id = TP_BAM_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_BAM_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x20;
    *((uint8_t *) &TXMsg.dataA[1])= 0x02;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x01;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;//CONTROL_RESPONSE_PGN
    *((uint8_t *) &TXMsg.dataB[2])= 0xf1;//
    *((uint8_t *) &TXMsg.dataB[3])= 0x01;//

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	if(charger_addr == 0){
    	TXMsg.id = TP_DP_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_DP_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x01;
    *((uint8_t *) &TXMsg.dataA[1])= 0xF1;//
    *((uint8_t *) &TXMsg.dataA[2])= 0x01;//CONTROL_RESPONSE_2_BYTE
    *((uint8_t *) &TXMsg.dataA[3])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[2])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[3])= 0xFF;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    return 0;	


}


/**
* 配置参数应答
**/
int hy_sy_config_response(uint32_t charger_addr){
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;
	if(charger_addr == 0){
    	TXMsg.id = TP_BAM_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_BAM_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x20;
    *((uint8_t *) &TXMsg.dataA[1])= 0x05;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x01;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;//CONFIG_RESPONSE_PGN
    *((uint8_t *) &TXMsg.dataB[2])= 0xf1;//
    *((uint8_t *) &TXMsg.dataB[3])= 0x02;//

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	if(charger_addr == 0){
    	TXMsg.id = TP_DP_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_DP_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x01;
    *((uint8_t *) &TXMsg.dataA[1])= INT32TO8_1(s_cancom->charge_to_msg_msg.output_voltage_x100V);//LSB
    *((uint8_t *) &TXMsg.dataA[2])= INT32TO8_2(s_cancom->charge_to_msg_msg.output_voltage_x100V);//MSB
    *((uint8_t *) &TXMsg.dataA[3])= INT32TO8_1(s_cancom->charge_to_msg_msg.output_current_x100A);//LSB
    *((uint8_t *) &TXMsg.dataB[0])= INT32TO8_2(s_cancom->charge_to_msg_msg.output_current_x100A);//MSB
    *((uint8_t *) &TXMsg.dataB[1])= s_cancom->charge_to_msg_msg.machine_status;
    *((uint8_t *) &TXMsg.dataB[2])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[3])= 0xFF;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    return 0;	


}


/**
* 退出请求应答
**/
int hy_sy_exit_response(uint32_t charger_addr){
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;
	if(charger_addr == 0){
    	TXMsg.id = TP_BAM_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_BAM_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x20;
    *((uint8_t *) &TXMsg.dataA[1])= 0x02;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x01;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;//EXIT_RESPONSE_PGN
    *((uint8_t *) &TXMsg.dataB[2])= 0xf1;//
    *((uint8_t *) &TXMsg.dataB[3])= 0x03;//

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	if(charger_addr == 0){
    	TXMsg.id = TP_DP_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_DP_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x01;
    *((uint8_t *) &TXMsg.dataA[1])= 0xf1;//EXIT_REQUEST_2_BYTE
    *((uint8_t *) &TXMsg.dataA[2])= 0x03;
    *((uint8_t *) &TXMsg.dataA[3])= 0xff;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0xff;
    *((uint8_t *) &TXMsg.dataB[2])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[3])= 0xFF;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    return 0;	


}


/**
* 广播请求应答 TODO
**/
int hy_sy_broadcast_response(uint32_t charger_addr){
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;
	if(charger_addr == 0){
    	TXMsg.id = TP_BAM_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_BAM_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x20;
    *((uint8_t *) &TXMsg.dataA[1])= 0x10;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x03;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;//
    *((uint8_t *) &TXMsg.dataB[2])= 0xf1;//
    *((uint8_t *) &TXMsg.dataB[3])= 0x04;//

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	if(charger_addr == 0){
    	TXMsg.id = TP_DP_CHARGER_DEFAULT_ID;
	}else{
		TXMsg.id = (TP_DP_CHARGER_MASK&charger_addr);
	}

    *((uint8_t *) &TXMsg.dataA[0])= 0x01;
    *((uint8_t *) &TXMsg.dataA[1])= INT32TO8_1(s_cancom->charge_to_msg_msg.output_voltage_x100V);//
    *((uint8_t *) &TXMsg.dataA[2])= INT32TO8_2(s_cancom->charge_to_msg_msg.output_voltage_x100V);
    *((uint8_t *) &TXMsg.dataA[3])= INT32TO8_1(s_cancom->charge_to_msg_msg.output_current_x100A);
    *((uint8_t *) &TXMsg.dataB[0])= INT32TO8_2(s_cancom->charge_to_msg_msg.output_current_x100A);
    *((uint8_t *) &TXMsg.dataB[1])= INT32TO8_1(s_cancom->charge_to_msg_msg.output_energy_x10Ah);
    *((uint8_t *) &TXMsg.dataB[2])= INT32TO8_2(s_cancom->charge_to_msg_msg.output_energy_x10Ah);
    *((uint8_t *) &TXMsg.dataB[3])= s_cancom->charge_to_msg_msg.error_code;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	*((uint8_t *) &TXMsg.dataA[0])= 0x02;
    *((uint8_t *) &TXMsg.dataA[1])= s_cancom->charge_to_msg_msg.charge_stage;
    *((uint8_t *) &TXMsg.dataA[2])= s_cancom->charge_to_msg_msg.battery_temperature+10;
    *((uint8_t *) &TXMsg.dataA[3])= s_cancom->charge_to_msg_msg.charger_temperature+10;
    *((uint8_t *) &TXMsg.dataB[0])= INT32TO8_1(s_cancom->charge_to_msg_msg.input_220V_voltage_x100V);
    *((uint8_t *) &TXMsg.dataB[1])= INT32TO8_2(s_cancom->charge_to_msg_msg.input_220V_voltage_x100V);
    *((uint8_t *) &TXMsg.dataB[2])= 0x32;//50hz
    *((uint8_t *) &TXMsg.dataB[3])= INT32TO8_1(s_cancom->charge_to_msg_msg.machine_type);

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);

	*((uint8_t *) &TXMsg.dataA[0])= 0x03;
    *((uint8_t *) &TXMsg.dataA[1])= INT32TO8_2(s_cancom->charge_to_msg_msg.machine_type);
    *((uint8_t *) &TXMsg.dataA[2])= s_cancom->charge_to_msg_msg.control_status;
    *((uint8_t *) &TXMsg.dataA[3])= 0xff;
    *((uint8_t *) &TXMsg.dataB[0])= 0xff;
    *((uint8_t *) &TXMsg.dataB[1])= 0xff;
    *((uint8_t *) &TXMsg.dataB[2])= 0xFF;
    *((uint8_t *) &TXMsg.dataB[3])= 0xFF;

	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    return 0;	


}


/**
* 地址重置	请求
**/
int hy_sy_address_reset_response(uint32_t charger_addr){
    TXMsg.format = EXT_ID_FORMAT;
    TXMsg.len = 8;

	TXMsg.id = (ADDRESS_CLAIM_MASK&(0xffffff00|charger_addr));


    *((uint8_t *) &TXMsg.dataA[0])= 0x32;
    *((uint8_t *) &TXMsg.dataA[1])= 0x00;
    *((uint8_t *) &TXMsg.dataA[2])= 0x00;
    *((uint8_t *) &TXMsg.dataA[3])= 0x00;
    *((uint8_t *) &TXMsg.dataB[0])= 0x00;
    *((uint8_t *) &TXMsg.dataB[1])= 0x00;//EXIT_RESPONSE_PGN
    *((uint8_t *) &TXMsg.dataB[2])= 0x00;//
    *((uint8_t *) &TXMsg.dataB[3])= 0x00;//


	CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    return 0;	

}


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
	return s_cancom->get_charger_msg.temperature1_x1degree*10;
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
	if(systime_elapse_ms(s_cancom->charger_module_timeout)>=SHENGYANG_TIMEOUT_TIME){
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
	return s_cancom->bms_msg.aim_voltage_x100V/10;
}
int hy_can_get_bms_set_current_10A(void)
{
	return s_cancom->bms_msg.aim_current_x100A/10;
}

int hy_can_get_bms_temperature(void)
{
	return s_cancom->bms_msg.battery_temperature;
}


int hy_can_get_bms_battery_status(void)
{
	return s_cancom->bms_msg.battery_status;//
}

int hy_can_get_bms_soc(void)
{
	return s_cancom->bms_msg.soc;
}

int hy_can_get_bms_soh(void)
{
	return s_cancom->bms_msg.soh;
}

int hy_can_get_bms_warnning(void)
{
	return s_cancom->bms_msg.battery_warnning;
}

int hy_can_get_bms_charge_percent(void)
{
	return s_cancom->bms_msg.charger_percent;
}



int hy_can_set_output_msg(uint16_t voltage_x10V, uint16_t current_x10A)
{
	s_cancom->charge_to_msg_msg.output_current_x100A=current_x10A*10;
	s_cancom->charge_to_msg_msg.output_voltage_x100V=voltage_x10V*10;
	return 0;
}

int hy_can_set_output_energy(uint16_t energy_x10Ah)
{
	s_cancom->charge_to_msg_msg.output_energy_x10Ah = energy_x10Ah;
	return 0;
}

int hy_can_set_charger_errorcode(uint8_t error)
{
	s_cancom->charge_to_msg_msg.error_code = error;
	return 0;
}

int hy_can_set_charger_stage(uint8_t stage)
{
	s_cancom->charge_to_msg_msg.charge_stage = stage;
	return 0;
	
}


int hy_can_set_charger_AC_voltage(uint32_t voltage_x10V)
{
	s_cancom->charge_to_msg_msg.input_220V_voltage_x100V = voltage_x10V*10;
	return 0;
}


int hy_can_set_charger_machine_type(uint16_t machine_type)
{
	s_cancom->charge_to_msg_msg.machine_type = machine_type;
	return 0;
}


int hy_can_set_charger_control_status(uint8_t status)
{
	s_cancom->charge_to_msg_msg.control_status = status;
	return 0;
}


uint8_t hy_can_get_bms_connected(void){
	if(systime_elapse_ms(s_cancom->bms_module_timeout)>=SHENGYANG_TIMEOUT_TIME){
		s_cancom->bms_module_canconnected=HY_FALSE;
	}else{
		s_cancom->bms_module_canconnected=HY_TRUE;
	}
	return s_cancom->bms_module_canconnected;
}




