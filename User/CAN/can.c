#include "stdint.h"
#include "can.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "dataprocess.h"
#include "can.h"
#include "debug_frmwrk.h"
#include "config.h"

CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
uint32_t CANRxCount, CANTxCount = 0;
extern uint16_t gcfg_can_baudrate_index ;
extern uint8_t gcfg_mode;
extern uint8_t chargestartflag;
uint8_t caninitflag;
const uint32_t baudrate[5] = {50000,125000,250000,500000,800000};

void BMS_CAN_Init(void)
{

    PINSEL_CFG_Type PinCfg;
    
//     Pincfg.Funcnum = BMS_CAN_PIN_FUNCNUM;
//     Pincfg.OpenDrain = BMS_CAN_PIN_OPENDRAIN;
//     Pincfg.Pinmode = BMS_CAN_PIN_MODE;
//     Pincfg.Portnum = BMS_CAN_PIN_PORTNUM;
//     Pincfg.Pinnum = BMS_CAN_H_PINNUM;
//     PINSEL_ConfigPin(&Pincfg);
//     Pincfg.Pinnum = BMS_CAN_L_PINNUM;
//     PINSEL_ConfigPin(&Pincfg);
		if(caninitflag == FLAGREADY){
		return;
	}
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;    
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);    
    CAN_Init(BMS_CAN_TUNNEL_X,baudrate[gcfg_can_baudrate_index]);
    DEBUG_PRINT(">>>>>>>baud 2342342 %d<<<<<<<<<\r\n",baudrate[gcfg_can_baudrate_index]);
    
    CAN_IRQCmd(BMS_CAN_TUNNEL_X,CANINT_RIE, ENABLE);

    NVIC_EnableIRQ(CAN_IRQn);
    //NVIC_SetPriority(CAN_IRQn, 10);
    CAN_SetAFMode(LPC_CANAF,CAN_AccBP);
    caninitflag = FLAGREADY;
}

uint8_t canstop()
{    
    if(caninitflag == FLAGREADY){
        caninitflag = FLAGNOTREADY;
        gBMSMessage.BMSFlag = FLAGNOTREADY;
        NVIC_DisableIRQ(CAN_IRQn);
        //DEBUG_PRINT("111111111111111111111111\r\n");
        CAN_IRQCmd(BMS_CAN_TUNNEL_X,CANINT_RIE, DISABLE);
        //DEBUG_PRINT("222222222222222222222222\r\n");
        CAN_DeInit(BMS_CAN_TUNNEL_X);
    }
    //DEBUG_PRINT("333333333333333333333333\r\n");
    return 0;
}

uint8_t Hex2Num(char hex)
{
    return hex;
}
uint8_t Num2Hex(uint8_t num)
{
    return num;
}
void PrintMessage(CAN_MSG_Type* CAN_Msg)
{
    uint32_t data;
    DEBUG_PRINT("Message ID:  %x   \r\n",CAN_Msg->id);
    DEBUG_PRINT("Message length: %d  \r\n",CAN_Msg->len);
    DEBUG_PRINT("Message type:   ");
    if(CAN_Msg->type==DATA_FRAME)
    {
        DEBUG_PRINT("DATA FRAME \r\n");
    }
    else
    {
        DEBUG_PRINT("REMOTE FRAME \r\n");
    }
    DEBUG_PRINT("Message format: ");
    if(CAN_Msg->format==STD_ID_FORMAT)
    {
        DEBUG_PRINT("STANDARD ID FRAME FORMAT\r\n");
    }
    else
    {
        DEBUG_PRINT("EXTENDED ID FRAME FORMAT\r\n");
    }
    data = (CAN_Msg->dataA[3])|(CAN_Msg->dataA[2]<<8)|(CAN_Msg->dataA[1]<<16)|(CAN_Msg->dataA[0]<<24);
    DEBUG_PRINT("Message dataA:  %08x \r\n",data);
    data = (CAN_Msg->dataB[3])|(CAN_Msg->dataB[2]<<8)|(CAN_Msg->dataB[1]<<16)|(CAN_Msg->dataB[0]<<24);
    DEBUG_PRINT("Message dataB:  %08x \r\n",data);
}


void can_sendbackmsg(uint32_t vol,uint32_t cur,uint8_t statu)
{
    uint32_t data;
    if(gcfg_mode != CONFIG_MODE_CAN || caninitflag != FLAGREADY){
        return;
    }

    TXMsg.format = MY_ID_FORMAT;
    TXMsg.len = 8;
    TXMsg.type = DATA_FRAME;
    if(vol>=9999)vol=0;
    if(cur>=9999)cur=0;
    TXMsg.id = CCS_CAN_FRAME_ID;
    TXMsg.dataA[0] = Num2Hex(vol/256);
    TXMsg.dataA[1] = Num2Hex(vol%256);
    TXMsg.dataA[2] = Num2Hex(cur/256);
    TXMsg.dataA[3] = Num2Hex(cur%256);
    TXMsg.dataB[0] = statu;
    TXMsg.dataB[1] = 0;
    TXMsg.dataB[2] = 0;
    TXMsg.dataB[3] = 0;
//     data = (TXMsg.dataA[3])|(TXMsg.dataA[2]<<8)|(TXMsg.dataA[1]<<16)|(TXMsg.dataA[0]<<24);
//      *((uint8_t *) &TXMsg.dataA[0])= (data & 0xFF000000)>>24;
//      *((uint8_t *) &TXMsg.dataA[1])= (data & 0x00FF0000)>>16;;
//      *((uint8_t *) &TXMsg.dataA[2])= (data & 0x0000FF00)>>8;
//      *((uint8_t *) &TXMsg.dataA[3])= (data & 0x000000FF);
//     data = (TXMsg.dataB[3])|(TXMsg.dataB[2]<<8)|(TXMsg.dataB[1]<<16)|(TXMsg.dataB[0]<<24);
//     *((uint8_t *) &TXMsg.dataB[0])= (data & 0xFF000000)>>24;
//      *((uint8_t *) &TXMsg.dataB[1])= (data & 0x00FF0000)>>16;;
//      *((uint8_t *) &TXMsg.dataB[2])= (data & 0x0000FF00)>>8;
//      *((uint8_t *) &TXMsg.dataB[3])= (data & 0x000000FF);
//   PrintMessage(&TXMsg);
    
    CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    DEBUG_PRINT("CAN SEND DONE \r\n");
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
    IntStatus = CAN_IntGetStatus(BMS_CAN_TUNNEL_X);
    //check receive interrupt
    if((IntStatus>>0)&0x01)
    {
        CAN_ReceiveMsg(BMS_CAN_TUNNEL_X,&RXMsg);
        if(RXMsg.id == BMS_CAN_FRAME_ID)
        {///?????
            //DEBUG_PRINT("[BBBBB]*****get bms msg\r\n");
            gBMSMessage.BMSVoltage = (uint32_t)RXMsg.dataA[0]*256+RXMsg.dataA[1];
            gBMSMessage.BMSCurrent = (uint32_t)RXMsg.dataA[2]*256+RXMsg.dataA[3];
            gBMSMessage.BMSControl = RXMsg.dataB[0]&BMS_CAN_CONTROL_MASK;
            
            gBMSMessage.BMSFlag = FLAGREADY;
        }
        //PrintMessage(&RXMsg);
        //DEBUG_PRINT("[B]receive can msg \r\n");
        //DEBUG_PRINT("[B]bms get vol: %d \r\n",gBMSMessage.BMSVoltage);
        //DEBUG_PRINT("[B]bms get cur: %d \r\n",gBMSMessage.BMSCurrent);
        //DEBUG_PRINT("[B]bms get ctr: %d \r\n",gBMSMessage.BMSControl);
    }
}

uint8_t CAN_TESTTX()
{
    uint8_t ret = ERR_OK;
    uint32_t data1 = 0x12345678;
    BMS_CAN_Init();
    while(1){
        delay_ms(1000);
        DEBUG_PRINT("system alive!!\r\n");

        TXMsg.format = MY_ID_FORMAT;
        TXMsg.len = 8;
        TXMsg.id = 0x11112222;
        *((uint8_t *) &TXMsg.dataA[0])= *((uint8_t *) &TXMsg.dataB[0])= data1 & 0x000000FF;
        *((uint8_t *) &TXMsg.dataA[1])= *((uint8_t *) &TXMsg.dataB[1])=(data1 & 0x0000FF00)>>8;
        *((uint8_t *) &TXMsg.dataA[2])= *((uint8_t *) &TXMsg.dataB[2])=(data1 & 0x00FF0000)>>16;
        *((uint8_t *) &TXMsg.dataA[3])= *((uint8_t *) &TXMsg.dataB[3])=(data1 & 0xFF000000)>>24;

    //CAN_SendMsg(BMS_CAN_TUNNEL_X, &TXMsg);
    }

    return 0;
}


uint8_t _canconnectedcheck(){
    if(caninitflag == FLAGNOTREADY){
			  gcfg_can_baudrate_index = Config_ReadCommunicationRate();
        BMS_CAN_Init();
			  return 0;
    }
    if(gBMSMessage.BMSFlag == FLAGREADY){
    	return 1;
    }else{
    	return 0;
    }

}
