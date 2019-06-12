#include "stdint.h"

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
	
		hy_instance_t *hy_instance=(hy_instance_t*)hy_instance_handle;
		if(s_cancom==NULL){
			s_cancom = &(hy_instance->cancom);
		}
		
    
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
		return 0;
}


void CAN_IRQHandler()
{
    uint8_t IntStatus;
//    uint32_t data1;
    /* get interrupt status
     * Note that: Interrupt register CANICR will be reset after read.
     * So function "CAN_IntGetStatus" should be call only one time
     */
    //DEBUG_PRINT("------------GET CAN MSG \r\n");
//    IntStatus = CAN_IntGetStatus(BMS_CAN_TUNNEL_X);
//    //check receive interrupt
//    if((IntStatus>>0)&0x01)
//    {
//        CAN_ReceiveMsg(BMS_CAN_TUNNEL_X,&RXMsg);
//        if(RXMsg.id == BMS_CAN_FRAME_ID)
//        {///?????
//            //DEBUG_PRINT("[BBBBB]*****get bms msg\r\n");
//            gBMSMessage.BMSVoltage = (uint32_t)RXMsg.dataA[0]*256+RXMsg.dataA[1];
//            gBMSMessage.BMSCurrent = (uint32_t)RXMsg.dataA[2]*256+RXMsg.dataA[3];
//            gBMSMessage.BMSControl = RXMsg.dataB[0]&BMS_CAN_CONTROL_MASK;
//            
//            gBMSMessage.BMSFlag = FLAGREADY;
//        }
//    }
}





