/************************
***********************/
#include "lpc17xx_libcfg.h"
#include "lpc17xx_systick.h"
#include "dataprocess.h"
#include "batterycharge.h"
//#include <includes.h>
/**wait for yym**/
#include "input.h"
#include "cos_map.h"
#include "input.h"
#include "output.h"
#include "hy_can.h"
#include "systen_delay.h"
#include "stdlib.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
//#include "app_cfg.h"
/****variable**********/
/****global*****/
#define BATTERYCHARGE_GLOBAL
#define DATAPROCESS_GLOBAL
uint8_t gPulseOutputStatu;
uint8_t softstartcontrol;
uint8_t softstopcontrol;
uint8_t gswstartflag = FLAGNOTREADY;
enum CHARGESTATU gChargeStatu = CHARGESTATU_IDEL;
struct ENT_IN gEnt_In;
__ENTSTATU  gentstatu = ENT_IDEL;
__OC gCurrentAll ;
__OV gVoltageAll ;
__Aerf gAerfDegree;
__CMsg gchargemsg;
uint32_t gChargeSystemTimeCount_10ms;
uint32_t gChargeLastStageSysTime_10ms; 
uint32_t gSystemTime = 0;
uint32_t gchargetime_ms_count = 0;
uint32_t gchargehandler_time = 0;
uint32_t gchargetime_interval_ms_count = 0;
uint32_t gchargetime_softstart_ms_count = 0;
uint32_t gchargetime_softstop_ms_count = 0;
uint32_t gchargehandler_softstart_count = 0;
uint32_t gchargehandler_softstop_count = 0;
uint32_t localstatu_one_timeouttimer = 0;
uint32_t localstatu_two_timeouttimer = 0;
uint32_t localstatu_three_timeouttimer = 0;    
uint32_t can_timeouttimer = 0;
uint32_t gchargetime_bms_can_timeout_count = 0;
uint32_t gchargetime_us_count = 0;
uint32_t geintwatchtime_count[3] = {0,0,0};
uint8_t gSWtimeFlag = FLAGNOTREADY;
uint8_t localchargestatu = 0;
uint8_t canchargestatu = 0;
uint8_t chargestartflag = FLAGNOTREADY;
uint8_t chargestopflag = FLAGNOTREADY;
uint8_t cancallbackstatus = 0;
     //public
__BMSMsg  gBMSMessage = {FLAGNOTREADY,0,0,0,ERR_OK};
//     static  OS_STK         App_Task_LocalBatteryCharge_Stk   [APP_TASK_LocalBatteryCharge_STK_SIZE];

const uint8_t chargeOrder_UVW[6] = {6,1,2,3,4,5};
const uint8_t chargeOrder_UWV[6] = {1,6,5,4,3,2};

__SWQUEUE gswitchopenqueue[6];    


    /*
 * hardware init
*/
#define SERIAL_DEBUG
uint8_t HW_BSP_init()
{
    NVIC_SetPriorityGrouping(0x08);
    NVIC_SetPriority(RIT_IRQn, 0);
    NVIC_SetPriority(CAN_IRQn, 10);
    
    Input_Init_ENTx();//
    Input_Init_Sample();//
     Input_Init_CTRL();//
//     
     Output_Init();//
    Output_RIT_Init();//
    

// 25 ms systemtic
    //SYSTICK_InternalInit(SYSTEMTICK_INTERVAL);//100ms interval
    //Enable System Tick interrupt
    //SYSTICK_IntCmd(ENABLE);
    //Enable System Tick Counter
    //SYSTICK_Cmd(ENABLE);
    

#ifdef SERIAL_DEBUG
    //UART0_Init();
#endif
    return 0;
}

void paraminit()
{
    gchargemsg.err = ERR_OK;
    
    gswstartflag = FLAGNOTREADY;
    
    gChargeStatu = CHARGESTATU_IDEL;
    
    gentstatu = ENT_IDEL;
    
    localstatu_one_timeouttimer = 0;
    localstatu_two_timeouttimer = 0;
    localstatu_three_timeouttimer = 0;    
    
    can_timeouttimer = 0;
    cancallbackstatus = 0;

    gCurrentAll.CurrentTarget_BMS = 0;
    gVoltageAll.VoltageTarget_BMS = 0;    
    gCurrentAll.CurrentFeedback = 0;
    gVoltageAll.VoltageFeedback = 0;    
}
void queueinit(void)
{
        //openqueue = (__SWQUEUE *)malloc(sizeof(__SWQUEUE));
     //gswitchclosequeue = (__SWQUEUE *)malloc(sizeof(__SWQUEUE));
    int i = 0;
    for (i = 0; i < 6; ++i)
    {
         /* code */
        gswitchopenqueue[i].staticstart = 0;
        gswitchopenqueue[i].staticend = QUEUE_NUM_MAX-1;
        gswitchopenqueue[i].queuehead = 0;
        gswitchopenqueue[i].queuetail = 0;
        gswitchopenqueue[i].headtailstatu = HEAD_TAIL;
        gswitchopenqueue[i].num = 0;
    }
    gAerfDegree.AerfThis = MIN_OUTPUT_AERF;
    gAerfDegree.updateflag = FLAGNOTREADY;
    DEBUG_PRINT("*******param*******\r\n");
    DEBUG_PRINT("[P]:SW_CLOSE_TIME_INTERVAL_COUNT %d\r\n",SW_CLOSE_TIME_INTERVAL_COUNT);
    DEBUG_PRINT("[P]:AERF_DEGREE_INTERVAL %d\r\n",AERF_DEGREE_INTERVAL);
    DEBUG_PRINT("[P]:BMS_CAN_TIMEOUT_COUNT %d\r\n",BMS_CAN_TIMEOUT_COUNT);
    DEBUG_PRINT("[P]:SW_CLOSE_TIME_INTERVAL %d\r\n",SW_CLOSE_TIME_INTERVAL);     
    DEBUG_PRINT("*******param end*******\r\n");
}

/**
** emergency stop close all 
****/
uint8_t Output_Close_All(void)
{
    int i = 0;
    for(i=0;i<6;i++)
    {
        Output_Triggle_Clear(chargeOrder_UVW[i]);
    }
    return ERR_OK;
}

void localchargestart(void)
{
    chargestartflag = FLAGREADY;
    chargestopflag = FLAGNOTREADY;
    gchargetime_ms_count = 0;
    DEBUG_PRINT("[I]:local charge start\r\n");
}
void bmscanchargestart(void)
{
    chargestartflag = FLAGREADY;
    chargestopflag = FLAGNOTREADY;
    gchargetime_bms_can_timeout_count = 0;
    gchargetime_ms_count = 0;
    DEBUG_PRINT("[I]:can charge start\r\n");
}
/*********
  *
  *  chargestart
  *******************/
uint8_t gcfg_mode = 0;


uint16_t gcfg_voltage_limit_1 = 0;
uint16_t gcfg_voltage_switch_1 = 0;
uint16_t gcfg_current_1 = 0;
uint32_t gcfg_timeout_1 = 0;
uint16_t gcfg_voltage_limit_2 = 0;
uint16_t gcfg_voltage_switch_2 = 0;
uint16_t gcfg_current_2 = 0;
uint32_t gcfg_timeout_2 = 0;
uint16_t gcfg_current_limit_3 = 0;
uint16_t gcfg_voltage_switch_3 = 0;
uint16_t gcfg_current_3 = 0;
uint32_t gcfg_timeout_3 = 0;
uint16_t gcfg_current_range = 0;
uint16_t gcfg_voltage_range = 0;
uint16_t gcfg_current_balance = 0;
uint16_t gcfg_can_baudrate_index = 0;

uint8_t chargestart(void)
{
    uint8_t ret = ERR_OK;
    gSystemTime = 0;
    gchargetime_bms_can_timeout_count = 0;
    gchargetime_us_count = 0;
    gchargehandler_time = 0;
    gchargehandler_softstart_count = 0;
    gchargehandler_softstop_count = 0;
    
    queueinit();
    paraminit();
    /**read config**/
    gcfg_mode = Config_ReadCtrlStyle();//local for 0 local for 1
    gcfg_current_range = Config_ReadCurrentRange();
    gcfg_voltage_range = Config_ReadVoltageRange();
    DEBUG_PRINT("[I]:set config mode\r\n");
    DEBUG_PRINT("[I]:current range: %d\r\n",gcfg_current_range);
    DEBUG_PRINT("[I]:voltage range: %d\r\n",gcfg_voltage_range);
    switch (gcfg_mode){
        case CONFIG_MODE_LOCAL:
        canstop();
        DEBUG_PRINT("[I]:config set local mode\r\n");
        
        gcfg_voltage_limit_1 = Config_ReadVolLimit_1()*10;
        gVoltageAll.VoltageTarget_sw_one = Config_ReadSwitchVol_1()*10;
        DEBUG_PRINT("[I]:config||vol one sw: %d\r\n",gVoltageAll.VoltageTarget_sw_one);
        gcfg_timeout_1 = Config_ReadChargeTime_1()*RIT_1_MIN_COUNT;
        gCurrentAll.CurrentTarget_one = Config_ReadCurrent_1()*10;
        DEBUG_PRINT("[I]:config||cur one t: %d\r\n",gCurrentAll.CurrentTarget_one);
        
        gcfg_voltage_limit_2 = Config_ReadVolLimit_2()*10;
        gVoltageAll.VoltageTarget_sw_two = Config_ReadSwitchVol_2()*10;
        DEBUG_PRINT("[I]:config||vol sw two: %d\r\n",gVoltageAll.VoltageTarget_sw_two);
        gcfg_timeout_2 = Config_ReadChargeTime_2()*RIT_1_MIN_COUNT;
        gCurrentAll.CurrentTarget_two = Config_ReadCurrent_2()*10;
        DEBUG_PRINT("[I]:config||cur two t: %d\r\n",gCurrentAll.CurrentTarget_two);
        
        gcfg_current_limit_3 = Config_ReadCurLimit_3()*10;
        gVoltageAll.VoltageTarget_three = Config_ReadVoltage_3()*10;
        DEBUG_PRINT("[I]:config||vol three t: %d\r\n",gVoltageAll.VoltageTarget_three);
        gcfg_timeout_3 = Config_ReadChargeTime_3()*RIT_1_MIN_COUNT;
        gCurrentAll.CurrentTarget_sw_three = Config_ReadSwitchCur_3()*10;
        DEBUG_PRINT("[I]:config||cur sw three: %d\r\n",gCurrentAll.CurrentTarget_sw_three);
        
        localchargestart();
        break;
        case CONFIG_MODE_CAN:
        DEBUG_PRINT("[I]:config set can mode\r\n");
        gCurrentAll.CurrentTarget_two = Config_ReadCurrent_2()*10;
        gcfg_can_baudrate_index = Config_ReadCommunicationRate();
        BMS_CAN_Init();
          //gcfg_current_balance = Config_ReadCurrentBalance();
        bmscanchargestart();
        break;
        default:
        DEBUG_PRINT("[E]:config mode err!! \r\n");
        setchargemsgerr(ERR_UNKNOW);
        return ret = ERR_UNKNOW;
        break;
    }
    /*soft start stop init*/
    softstartcontrol = SOFT_START_NULL;
    softstopcontrol = SOFT_STOP_NULL;
    /**hw init**/
    HW_BSP_init();
    DEBUG_PRINT("[I]:Hardware init\r\n");
    /**input three phase check**/
    ret = ThreePhaseCheck( PHASECHECK_TIMEOUT_COUNT , PHASECHECK_COUNT);
    return ret;
}


uint8_t chargestop(void)
{
    DEBUG_PRINT("luoyang stop*****\r\n");
    if(gChargeStatu==CHARGESTATU_ERR||softstopcontrol == SOFT_STOP_END){
        if(chargestopflag == FLAGNOTREADY){
            Input_Close_ENTx();
            Output_Close_RIT_REAL();
            Output_Close_All();
            Output_Run_Relay(RUN_OFF);
            chargestartflag = FLAGNOTREADY;
            chargestopflag = FLAGREADY;
            switch (gcfg_mode){
                case CONFIG_MODE_CAN:                    
                setchargemsg(gVoltageAll.VoltageFeedback,0,canchargestatu);
                //canstop();
                DEBUG_PRINT("[[[]]]]CAN stop \r\n");
                break;
                case CONFIG_MODE_LOCAL:
                setchargemsg(gVoltageAll.VoltageFeedback,0,LOCAL_STATE_END);
                break;
            }
        }
    }
    if(gChargeStatu!=CHARGESTATU_END||gChargeStatu!=CHARGESTATU_SOFT_STOP){
        gChargeStatu = CHARGESTATU_END;
    }
    if(softstopcontrol != SOFT_STOP_ON){
        softstopcontrol = SOFT_STOP_ON;
        localchargestatu = LOCAL_STATE_END;
        gChargeStatu = CHARGESTATU_SOFT_STOP;
    }
    //Enable System Tick interrupt
    //SYSTICK_IntCmd(DISABLE);
  //Enable System Tick Counter
    //SYSTICK_Cmd(DISABLE);
    
}
/***
 * localcharge control
 * system tick handler 25ms
 *
**/
uint32_t OSTimeGet()
{
    return gchargetime_ms_count;
}

void aerfcontrol(uint32_t target,uint32_t feedback)
{
    if((feedback * 100) < (target * 98))
    {
        UpdateAerf(gAerfDegree.AerfThis-1);
    }
    else if((feedback * 100) >= (target * 102))
    {
        UpdateAerf(gAerfDegree.AerfThis+1);
    }
        //DEBUG_PRINT("[I]:aerd: %d \r\n",gAerfDegree.AerfUpdate);

}

/******
*
*   chargehandler
*************/

extern volatile unsigned long SysTickCnt;
void chargehandler(void)
{
    uint32_t temptime = 0;
    uint32_t tempinterval = 0;
    uint32_t temptimeouttimer = 0;
    //uint8_t IntStatus = 0;
    /*safe check*/
    if(Input_Is_Emergency()!=NO_EMERGENCY)
    {
        DEBUG_PRINT("[E]:emergency handler!!\r\n");
        Output_Close_All();
        Input_Close_ENTx();
        Output_Fault_Relay(FAULT_OUTPUT);
        gChargeStatu = CHARGESTATU_ERR;
        can_sendbackmsg(gVoltageAll.VoltageFeedback,0,(cancallbackstatus|=CALLBACK_OVERHEAT));
        setchargemsgerr(ERR_OVERHEAT);
            /*BMS todo*/

    }else{
        if(gchargemsg.err == ERR_OVERHEAT){
            cancallbackstatus &= ~CALLBACK_OVERHEAT;
            setchargemsgerr(ERR_OK);
        }
    }

    if(chargestartflag != FLAGREADY){
        return;
    }

//         if(gcfg_mode == CONFIG_MODE_CAN){//luxun can msg
//             //IntStatus = CAN_IntGetStatus(BMS_CAN_TUNNEL_X);
//             if((BMS_CAN_TUNNEL_X->SR &0x00000001))
//             {
//                 CAN_ReceiveMsg(BMS_CAN_TUNNEL_X,&RXMsg);
//                 if(RXMsg.id == BMS_CAN_FRAME_ID)
//                 {///?????
//                     //DEBUG_PRINT("[BBBBB]*****get bms msg\r\n");
//                     gBMSMessage.BMSVoltage = Hex2Num(RXMsg.dataA[0])*100+Hex2Num(RXMsg.dataA[1]);
//                     gBMSMessage.BMSCurrent = Hex2Num(RXMsg.dataA[2])*100+Hex2Num(RXMsg.dataA[3]);
//                     gBMSMessage.BMSControl = RXMsg.dataB[0]&BMS_CAN_CONTROL_MASK;
//                     gBMSMessage.BMSFlag = FLAGREADY;
//                 }
//             }
//         }
    temptime = gchargetime_us_count;
    tempinterval = temptime - gchargehandler_time;
    temptimeouttimer = 0;
      if(tempinterval <= CHARGEHANDLER_INTERVAL_COUNT){//100ms
          return;
      }
      DEBUG_PRINT("[I]:chargehandler at %u\r\n",temptime);

      switch (gcfg_mode)
      {
          case CONFIG_MODE_CAN :
          switch (gChargeStatu){
              case CHARGESTATU_IDEL:
              break;
                case CHARGESTATU_START://checking
/*                    if(gBMSMessage.BMSFlag == FLAGREADY)
                    {
                                                DEBUG_PRINT("[B]get bms msg\r\n");    
                        can_timeouttimer = temptime;
                        gBMSMessage.BMSFlag = FLAGNOTREADY;
                        if(gBMSMessage.err != ERR_OK)
                        {
                            DEBUG_PRINT("[E]:BMS CAN err!!\r\n");
                            Output_Fault_Relay(FAULT_OUTPUT );
                                                      setchargemsgerr(ERR_UNKNOW);
                            gChargeStatu = CHARGESTATU_ERR;
                                                      chargestop();
                        }
                        if(gBMSMessage.BMSControl == BMS_CONTROL_STOP)
                        {
                                                      canchargestatu = CAN_FINISH;
                            chargestop();
                        }
                        else if(gBMSMessage.BMSControl == BMS_CONTROL_START)
                        {
                            gCurrentAll.CurrentTarget_BMS = gBMSMessage.BMSCurrent;
                            gVoltageAll.VoltageTarget_BMS = gBMSMessage.BMSVoltage;                         
                        }
                    }
                    else//not receive BMS can msg
                    {
                                                temptimeouttimer = temptime - can_timeouttimer;
                        if(temptimeouttimer >= BMS_CAN_TIMEOUT_COUNT)
                        {
                            DEBUG_PRINT("[E]:BMS CAN rec timeout!!\r\n");
                           // Output_Fault_Relay(FAULT_OUTPUT);
                            //Output_Run_Relay(0);
                            gChargeStatu = CHARGESTATU_ERR;
                                                        setchargemsgerr(ERR_TIMEOUT);
                                                }
                    }*/
                break;
                case CHARGESTATU_WORK:
                eintwatchdagwow();
                if(gBMSMessage.BMSFlag == FLAGREADY)
                {
                                                      //DEBUG_PRINT("[B]--------get bms msg\r\n");
                                                      //can_sendbackmsg(0,0,0);
                    can_timeouttimer = temptime;
                    gBMSMessage.BMSFlag = FLAGNOTREADY;
                    if(gBMSMessage.err != ERR_OK)
                    {
                                //DEBUG_PRINT("[E]:BMS CAN err!!\r\n");
                        Output_Fault_Relay(FAULT_OUTPUT );
                        Output_Run_Relay(0);
                        gChargeStatu = CHARGESTATU_ERR;
                    }
                    if(gBMSMessage.BMSControl == BMS_CONTROL_STOP)
                    {
                                                              //DEBUG_PRINT("<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>!!\r\n");
                        canchargestatu = CAN_FINISH;
                        gChargeStatu = CHARGESTATU_END;
                        break;
                    }
                    else if(gBMSMessage.BMSControl == BMS_CONTROL_START)
                    {
                        gCurrentAll.CurrentTarget_BMS = gBMSMessage.BMSCurrent;
                        if(gCurrentAll.CurrentTarget_BMS >= gCurrentAll.CurrentTarget_two){
                            gCurrentAll.CurrentTarget_BMS = gCurrentAll.CurrentTarget_two;
                        }
                        gVoltageAll.VoltageTarget_BMS = gBMSMessage.BMSVoltage;
                        DEBUG_PRINT("[B]: get bms msg: vol %d cur %d\r\n",gVoltageAll.VoltageTarget_BMS ,gCurrentAll.CurrentTarget_BMS);
                    }
                }
                        else//not receive BMS can msg
                        {
                            temptimeouttimer = temptime - can_timeouttimer;
                            if(temptimeouttimer >= BMS_CAN_TIMEOUT_COUNT)
                            {
                                DEBUG_PRINT("[E]:BMS CAN rec timeout!! %u %u %u %u\r\n",temptimeouttimer,temptime,can_timeouttimer,BMS_CAN_TIMEOUT_COUNT);
                                gChargeStatu = CHARGESTATU_ERR;
                                setchargemsgerr(ERR_TIMEOUT);
                                canchargestatu = CAN_STOP;
                                can_sendbackmsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,(cancallbackstatus|=CALLBACK_COM_TIMEOUT_STATU));
                                break;
                                                              //chargestop();
                            }
                        }                    
                        /*get feedback data*/
                                                    //DEBUG_PRINT("[CC]:timeout %d %d %d \r\n",temptime,can_timeouttimer,BMS_CAN_TIMEOUT_COUNT);
                        gCurrentAll.CurrentFeedback = GetFeedbackCurrent();
                        gVoltageAll.VoltageFeedback = GetFeedbackVoltage();
                        if(gVoltageAll.VoltageFeedback){
                            cancallbackstatus &= ~CALLBACK_START_STATUS;
                            can_sendbackmsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,cancallbackstatus);
                            DEBUG_PRINT("[C]:can ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_BMS,gCurrentAll.CurrentFeedback);
                            DEBUG_PRINT("[C]:can vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_BMS,gVoltageAll.VoltageFeedback);
                            setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,CAN_STATE);
                            aerfcontrol(gCurrentAll.CurrentTarget_BMS,gCurrentAll.CurrentFeedback);  
                            if(gVoltageAll.VoltageFeedback>gVoltageAll.VoltageTarget_BMS){
                                UpdateAerf(gAerfDegree.AerfThis+1);
                            }
                            DEBUG_PRINT("[111111B]bms can aerf : %d\r\n",gAerfDegree.AerfThis);
                        }else{
                            cancallbackstatus |= CALLBACK_START_STATUS;
                            can_sendbackmsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,cancallbackstatus);
                            setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,CAN_STATE);
                            DEBUG_PRINT("[000000B]bms can aerf : %d\r\n",gAerfDegree.AerfThis);
                        }
                        break;
                        case  CHARGESTATU_SOFT_STOP:
                        if(softstopcontrol == SOFT_STOP_ON)
                        {
                            localchargestatu = LOCAL_STATE_END;
                            gchargehandler_softstop_count++;
                            if(gchargehandler_time >= SOFT_STOP_INTERVAL_COUNT)
                            {
                                gchargehandler_softstop_count = 0;
                                DEBUG_PRINT("[I]:soft stop ...\r\n");
                                gchargetime_softstop_ms_count = 0;
                                gCurrentAll.CurrentFeedback = GetFeedbackCurrent();
                                gVoltageAll.VoltageFeedback = GetFeedbackVoltage();
                                DEBUG_PRINT("[I]:soft stop ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_sw_three,gCurrentAll.CurrentFeedback);
                                DEBUG_PRINT("[I]:soft stop vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_three,gVoltageAll.VoltageFeedback);
                                UpdateAerf(gAerfDegree.AerfThis+5);
                                DEBUG_PRINT("[I]:soft stop aerf: %d\r\n",gAerfDegree.AerfUpdate);
                                if(gAerfDegree.AerfUpdate>=AERFA_MAX||gCurrentAll.CurrentFeedback <= CLOSE_CURRENT)
                                {
                                    softstopcontrol = SOFT_STOP_END;
                                    DEBUG_PRINT("[I]:soft stop aerf ***MAX***: %d\r\n",gAerfDegree.AerfUpdate);
                                    chargestop();
                                    gChargeStatu = CHARGESTATU_END;

                                }                   
                            }
                        }
                        break;
                        case CHARGESTATU_ERR:
                        DEBUG_PRINT("[E]:can charge happen something wrong!!\r\n");
                        chargestop();                                        
                        break;
                        case CHARGESTATU_END:
                      /*softstop*/
                            DEBUG_PRINT("[E]:can charge endddddddddddddddddddd!!\r\n");
                            if(softstopcontrol == SOFT_STOP_NULL){
                                softstopcontrol=SOFT_STOP_ON;
                                gChargeStatu = CHARGESTATU_SOFT_STOP;
                            }
                            if(softstopcontrol == SOFT_STOP_END){
                                chargestop();
                            }
                        break;                                
                        default:
                        break;
                    }            
                    break;
                    case CONFIG_MODE_LOCAL:
                    switch (gChargeStatu){
                        case CHARGESTATU_IDEL:
                        DEBUG_PRINT("[I]:local idel\r\n");
                        break;
                        case CHARGESTATU_START:
                        DEBUG_PRINT("[I]:local start\r\n");
                        localchargestatu = LOCAL_STATE_ONE;
                        break;
                        case CHARGESTATU_SOFT_START:
                        if(softstartcontrol == SOFT_START_ON)
                        {
                            gchargehandler_softstart_count++;
                            if(gchargehandler_softstart_count >= SOFT_START_INTERVAL_COUNT)
                            {
                                gchargehandler_softstart_count = 0;
                                DEBUG_PRINT("[I]:soft start ...\r\n");
                                                        //Output_Close_RIT_REAL();
                                gCurrentAll.CurrentFeedback = GetFeedbackCurrent();
                                gVoltageAll.VoltageFeedback = GetFeedbackVoltage();
                                                        //Output_Open_RIT_REAL();
                                                        //DEBUG_PRINT("[I]:soft start 2\r\n");
                                DEBUG_PRINT("[I]:soft start ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_one,gCurrentAll.CurrentFeedback);
                                DEBUG_PRINT("[I]:soft start vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_sw_one,gVoltageAll.VoltageFeedback);
                                if((gCurrentAll.CurrentFeedback * 100) >= (gCurrentAll.CurrentTarget_one * 90))
                                {
                                    softstartcontrol = SOFT_START_END;
                                    localchargestatu = LOCAL_STATE_ONE;
                                    gChargeStatu = CHARGESTATU_WORK;
                                    DEBUG_PRINT("[I]:soft start end cur %d %d %dr\n",gCurrentAll.CurrentFeedback,gCurrentAll.CurrentTarget_one,gAerfDegree.AerfUpdate);
                                }
                                if((gVoltageAll.VoltageFeedback * 100) >= (gVoltageAll.VoltageTarget_sw_one * 90))
                                {
                                    softstartcontrol = SOFT_START_END;
                                    localchargestatu = LOCAL_STATE_ONE;
                                    localstatu_one_timeouttimer = temptime;
                                    gChargeStatu = CHARGESTATU_WORK;
                                    DEBUG_PRINT("[I]:soft start end vol %d %d %d\r\n",gVoltageAll.VoltageFeedback,gVoltageAll.VoltageTarget_sw_one,gAerfDegree.AerfUpdate);
                                }
                                setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,LOCAL_STATE_ONE);
                                                        //UpdateAerf(gAerfDegree.AerfThis-1);
                                UpdateAerf(gAerfDegree.AerfThis-1);            
                                DEBUG_PRINT("[I]:soft start aerf: %d\r\n",gAerfDegree.AerfUpdate);
                                                        //DEBUG_PRINT("[I]:rit time: %d\r\n",gchargetime_us_count);
                                if(gAerfDegree.AerfUpdate<=AERFA_MIN)
                                {
                                    UpdateAerf(AERFA_MIN);
                                                            /*max limit todo*/
                                    DEBUG_PRINT("[I]:soft start aerf ***MAX***: %d\r\n",gAerfDegree.AerfUpdate);
                                }                   
                            }
                        }                                    
                        break;
                        case CHARGESTATU_WORK:
                    /*three phase watch dog*/
                        eintwatchdagwow();
                    /*softstart*/
                        if(softstartcontrol == SOFT_START_NULL){
                            DEBUG_PRINT("[I]:local soft start\r\n");
                            softstartcontrol=SOFT_START_ON; 
                            gChargeStatu = CHARGESTATU_SOFT_START;
                            UpdateAerf(90);
                        }
                        if(softstartcontrol == SOFT_START_END)
                        {
                        /*get feedback data*/
                            gCurrentAll.CurrentFeedback = GetFeedbackCurrent();
                            gVoltageAll.VoltageFeedback = GetFeedbackVoltage();
                            /*local statu change*/
                            switch(localchargestatu)
                            {
                                case LOCAL_STATE_ONE:
                                    temptimeouttimer = temptime - localstatu_one_timeouttimer;
                                    DEBUG_PRINT("[I-1]:timeout %u %u %u ",temptime,temptimeouttimer,gcfg_timeout_1);
                                    if(temptimeouttimer >= gcfg_timeout_1){
                                        DEBUG_PRINT("****charge state change two\r\n");
                                        localchargestatu = LOCAL_STATE_TWO;
                                        localstatu_two_timeouttimer = temptime;
                                        break;
                                    }
                                    DEBUG_PRINT("[I-1]:local work state one\r\n");
                                    if(gVoltageAll.VoltageFeedback >= gVoltageAll.VoltageTarget_sw_one){
                                        DEBUG_PRINT("****charge state change two\r\n");
                                        localchargestatu = LOCAL_STATE_TWO;
                                        localstatu_two_timeouttimer = temptime;
                                        break;
                                    }
                                    aerfcontrol(gCurrentAll.CurrentTarget_one,gCurrentAll.CurrentFeedback);
                                    DEBUG_PRINT("[I-1]:local statu one ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_one,gCurrentAll.CurrentFeedback);
                                    DEBUG_PRINT("[I-1]:local statu vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_sw_one,gVoltageAll.VoltageFeedback);
                                    setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,localchargestatu);                                                                        
                                break;
                                case LOCAL_STATE_TWO:
                                    temptimeouttimer = temptime - localstatu_two_timeouttimer;
                                    DEBUG_PRINT("[I-2]:timeout %u %u %u ",temptime,temptimeouttimer,gcfg_timeout_2);
                                    if(temptimeouttimer >= gcfg_timeout_2){
                                        DEBUG_PRINT("****charge state change three\r\n");
                                        localchargestatu = LOCAL_STATE_THREE;
                                        localstatu_three_timeouttimer = temptime;
                                        break;
                                    }                                                                    
                                    DEBUG_PRINT("[I-2]:local work two\r\n");
                                    if(gVoltageAll.VoltageFeedback >= gVoltageAll.VoltageTarget_sw_two){
                                        DEBUG_PRINT("****charge state change three\r\n");
                                        localchargestatu = LOCAL_STATE_THREE;
                                        localstatu_three_timeouttimer = temptime;
                                        break;
                                    }
                                    aerfcontrol(gCurrentAll.CurrentTarget_two,gCurrentAll.CurrentFeedback);
                                    DEBUG_PRINT("[I-2]:local statu two ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_two,gCurrentAll.CurrentFeedback);
                                    DEBUG_PRINT("[I-2]:lcoal statu two vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_sw_two,gVoltageAll.VoltageFeedback);
                                    setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,localchargestatu);    
                                break;
                                case LOCAL_STATE_THREE:
                                    temptimeouttimer = temptime - localstatu_three_timeouttimer;
                                    DEBUG_PRINT("[I-3]:timeout %u %u %u ",temptime,temptimeouttimer,gcfg_timeout_3);
                                    if(temptimeouttimer >= gcfg_timeout_3){
                                        DEBUG_PRINT("****charge state change end\r\n");
                                        localchargestatu = LOCAL_STATE_END;
                                        gChargeStatu = CHARGESTATU_END;
                                        break;
                                    }                                            
                                    DEBUG_PRINT("[I-3]:local work three\r\n");
                                    if(gCurrentAll.CurrentFeedback <= gCurrentAll.CurrentTarget_sw_three){
                                        DEBUG_PRINT("****charge state change end\r\n");
                                        localchargestatu = LOCAL_STATE_END;
                                        gChargeStatu = CHARGESTATU_END;
                                        break;
                                    }
                                    aerfcontrol(gVoltageAll.VoltageTarget_three,gVoltageAll.VoltageFeedback);   
                                    if(gCurrentAll.CurrentFeedback>=gcfg_current_limit_3){
                                        UpdateAerf(gAerfDegree.AerfThis+1);
                                    }
                                    DEBUG_PRINT("[I-3]:local statu three ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_sw_three,gCurrentAll.CurrentFeedback);
                                    DEBUG_PRINT("[I-3]:local statu vt: %d vf %d\r\n",gVoltageAll.VoltageTarget_three,gVoltageAll.VoltageFeedback);
                                    setchargemsg(gVoltageAll.VoltageFeedback,gCurrentAll.CurrentFeedback,localchargestatu);    
                                break;

                                default:
                                break;
                            }
                        }
                        break;
                        case  CHARGESTATU_SOFT_STOP:
                        if(softstopcontrol == SOFT_STOP_ON)
                        {
                            localchargestatu = LOCAL_STATE_END;
                            gchargehandler_softstop_count++;
                            if(gchargehandler_time >= SOFT_STOP_INTERVAL_COUNT)
                            {
                                gchargehandler_softstop_count = 0;
                                DEBUG_PRINT("[I]:soft stop ...\r\n");
                                gchargetime_softstop_ms_count = 0;
                                gCurrentAll.CurrentFeedback = GetFeedbackCurrent();
                                gVoltageAll.VoltageFeedback = GetFeedbackVoltage();
                                DEBUG_PRINT("[I]:soft stop ct: %d cf : %d\r\n",gCurrentAll.CurrentTarget_sw_three,gCurrentAll.CurrentFeedback);
                                DEBUG_PRINT("[I]:soft stop vt: %d vf : %d\r\n",gVoltageAll.VoltageTarget_three,gVoltageAll.VoltageFeedback);
                                UpdateAerf(gAerfDegree.AerfThis+5);
                                DEBUG_PRINT("[I]:soft stop aerf: %d\r\n",gAerfDegree.AerfUpdate);
                                if(gAerfDegree.AerfUpdate>=AERFA_MAX||gCurrentAll.CurrentFeedback <= CLOSE_CURRENT)
                                {
                                    softstopcontrol = SOFT_STOP_END;
                                    DEBUG_PRINT("[I]:soft stop aerf ***MAX***: %d\r\n",gAerfDegree.AerfUpdate);
                                    chargestop();
                                    gChargeStatu = CHARGESTATU_END;
                                }                   
                            }
                        }
                        break;
                        case    CHARGESTATU_STOP:
                            DEBUG_PRINT("[I]:CHARGE STOP !!\r\n");
                            gChargeStatu = CHARGESTATU_END;
                            break;
                            case CHARGESTATU_END:
                      /*softstop*/
                            if(softstopcontrol == SOFT_STOP_NULL){
                                softstopcontrol=SOFT_STOP_ON;
                                gChargeStatu = CHARGESTATU_SOFT_STOP;
                            }
                            if(softstopcontrol == SOFT_STOP_END){
                                chargestop();
                            }
                            break;
                            case CHARGESTATU_ERR:
                            Output_Fault_Relay(FAULT_OUTPUT);
                            setchargemsgerr(ERR_UNKNOW);
                            chargestop();
                            gChargeStatu = CHARGESTATU_END;
                            DEBUG_PRINT("[E]:local charge happen something wrong!!\r\n");
                        break;
//                         case CHARGESTATU_END:
//                       /*softstop*/
//                             DEBUG_PRINT("[E]:can charge endddddddddddddddddddd!!\r\n");
//                             if(softstopcontrol == SOFT_STOP_NULL){
//                                 softstopcontrol=SOFT_STOP_ON;
//                                 gChargeStatu = CHARGESTATU_SOFT_STOP;
//                             }
//                             if(softstopcontrol == SOFT_STOP_END){
//                                 chargestop();
//                             }
//                         break;
                        default://gchargestatu
                            Output_Close_All();
                            Output_Close_RIT_REAL();
                            Input_Close_ENTx();
                            Output_Fault_Relay(FAULT_OUTPUT);
                                 // Output_Run_Relay(0);
                            DEBUG_PRINT("[E]:err mode!!\r\n");
                        break;
            }
        default://gcfg_mode
        break;
    }
    gchargehandler_time =  gchargetime_us_count;

}

void SysTick_Handler(void){


}
__CMsg* chargemsg(void)
{
    return &gchargemsg;
}

void setchargemsgerr(uint8_t err)
{
    gchargemsg.err = err;
    gchargemsg.updateflag = FLAGREADY;
}
void setchargemsg(uint32_t vol,uint32_t cur, uint8_t state)
{
    uint32_t timetemp;
    gchargemsg.feedbackvoltage = vol;
    gchargemsg.feedbackcurrent = cur;
    timetemp = gchargetime_us_count/(SYSTEMTICK_ONE_SECOND_COUNT*60);
    if(timetemp <= 0)timetemp = 0;
    gchargemsg.chargetime  = timetemp;
      //FDEBUG_PRINT("[I]:charge time :%d min \r\n");
    gchargemsg.localstate = state;
    gchargemsg.updateflag = FLAGREADY;
    DEBUG_PRINT("setchargemsg:%d %d %d\r\n",vol,cur,state);
}

/**
*
*  ent input enable
**/
void entphase_input_enable()
{
    Input_Open_ENTx();
}
/*********
  * @function check three phase
    * @return 0 for normal  1 for err
    * @para   timeout : 
              count : 
*********/

uint8_t ThreePhaseCheck(uint32_t timeout,uint32_t count)
{
    uint8_t ret = ERR_OK;
    uint32_t starttime = 0;
    uint32_t VU_WVsequence = 0;
    uint32_t VU_UWsequence = 0;
    uint32_t WV_UWsequence = 0;
    uint32_t WV_VUsequence = 0;
    uint32_t UW_VUsequence = 0;
    uint32_t UW_WVsequence = 0;
    gChargeStatu = CHARGESTATU_START;
    DEBUG_PRINT("[I]:check start\r\n");
    //enable input ent #################### 
    entphase_input_enable();
    gentstatu = ENT_CHECKPHASE;
    starttime = OSTimeGet();

    gEnt_In.readyflag = FLAGNOTREADY;
    gEnt_In.phase = 0;
    gEnt_In.phase_order = 0;
    gEnt_In.VU_voltage = 0;
    gEnt_In.WV_voltage = 0;
    gEnt_In.UW_voltage = 0;
    gEnt_In.ccount = 0;
    gEnt_In.err = ERR_OK;
    gEnt_In.tcount;
    while(1)
    {
        if((OSTimeGet()-starttime)>=timeout)
        {
            gChargeStatu = CHARGESTATU_ERR;
            Input_Close_ENTx();
            Output_Fault_Relay(FAULT_OUTPUT );
            Output_Run_Relay(0);
            DEBUG_PRINT("[E]:three check time out !!\r\n");
            can_sendbackmsg(gVoltageAll.VoltageFeedback,0,(cancallbackstatus|=CALLBACK_HARDWARE_ERR));
            return  ERR_LACKPHASE;
        }
        if(gEnt_In.readyflag == FLAGREADY)
        {
            gEnt_In.readyflag = FLAGNOTREADY;
            switch(gEnt_In.phase)
            {
                case PHASE_VU:
                    VU_WVsequence = VU_WVsequence + gEnt_In.WV_voltage;//1
                  VU_UWsequence = VU_UWsequence + gEnt_In.UW_voltage;//0
                  break;
                  case PHASE_WV:
                    WV_UWsequence = WV_UWsequence + gEnt_In.UW_voltage;//1
                  WV_VUsequence = WV_VUsequence + gEnt_In.VU_voltage;//0
                  break;
                  case PHASE_UW:
                    UW_VUsequence = UW_VUsequence + gEnt_In.VU_voltage;//1
                  UW_WVsequence = UW_WVsequence + gEnt_In.WV_voltage;//0
                  break;
                  default:
                  gChargeStatu = CHARGESTATU_ERR;
                  Input_Close_ENTx();
                    // Output_Fault_Relay(FAULT_OUTPUT );
                    // Output_Run_Relay(0);
                  return ERR_UNKNOW;
                }
                if(gEnt_In.ccount >= (count*3))
                {
                    gentstatu = ENT_IDEL;
                    if(((VU_WVsequence+UW_VUsequence+WV_UWsequence)>=(gEnt_In.ccount-PHASECHECK_DEVIATION*3))&&((VU_UWsequence+WV_VUsequence+UW_WVsequence)<(PHASECHECK_DEVIATION*3)))
                    {

                        gEnt_In.phase_order = PHASE_ORDER_UVW;
                        DEBUG_PRINT("[I]:order: %d UVW\r\n",gEnt_In.phase_order );
                    gentstatu = ENT_PREPARE;//entstatu change

                    can_timeouttimer = gchargetime_us_count;
                    gChargeStatu = CHARGESTATU_WORK;
                    return ERR_OK;
                }
                else if((VU_WVsequence+UW_VUsequence+WV_UWsequence)<(PHASECHECK_DEVIATION*3)&&((VU_UWsequence+WV_VUsequence+UW_WVsequence)>=(gEnt_In.ccount-PHASECHECK_DEVIATION*3)))
                {
                    gEnt_In.phase_order = PHASE_ORDER_UWV;
                    DEBUG_PRINT("[I]:order: %d UWV\r\n",gEnt_In.phase_order );
                    gentstatu = ENT_PREPARE;//ent statu change

                    can_timeouttimer = gchargetime_us_count;
                    gChargeStatu = CHARGESTATU_WORK;
                    return ERR_OK;
                }
                else
                {
                    DEBUG_PRINT("[E]:check failed! ret: %d,%d\r\n",(VU_WVsequence+UW_VUsequence+WV_UWsequence),(VU_UWsequence+WV_VUsequence+UW_WVsequence));
                    gChargeStatu = CHARGESTATU_ERR;
                    //entstatu change
                    Input_Close_ENTx();
                    gentstatu = ENT_IDEL;
                    setchargemsgerr(ERR_LACKPHASE);
                    can_sendbackmsg(gVoltageAll.VoltageFeedback,0,(cancallbackstatus|=CALLBACK_HARDWARE_ERR));
                    // Output_Fault_Relay(FAULT_OUTPUT );
                    // Output_Run_Relay(0);
                    return ERR_LACKPHASE;
                }
            }
        }        
    }
}

void eintfeedwatchdag(uint8_t eintx)
{
    geintwatchtime_count[eintx]=0;
}
void eintwatchdagwow()
{
    int i;
    DEBUG_PRINT("feed doggggggggggggggggggggg! %d %d\r\n",geintwatchtime_count[1],SYSTEMTICK_ONE_SECOND_COUNT_H);

    for(i = 0;i<3;i++)
    {
        geintwatchtime_count[i]++;
        if(geintwatchtime_count[i] >= SYSTEMTICK_ONE_SECOND_COUNT_H)
        {
            DEBUG_PRINT("[E]:dagwow: lack %d phase!! \r\n",(i+1));
            gChargeStatu = CHARGESTATU_ERR;
            setchargemsgerr(ERR_LACKPHASE);
            can_sendbackmsg(gVoltageAll.VoltageFeedback,0,(cancallbackstatus|=CALLBACK_HARDWARE_ERR));
        }
    }
}

void localchargefeedwatchdag(uint8_t statudog)
{
    
}
void localchargewatchdagwow(uint8_t statudog)
{
    
}

//50ms
void UpdateAerf(uint32_t aerf)
{
    if(aerf<=AERFA_MIN)aerf=AERFA_MIN;//AERFA_MIN                           1*AERF_DEGREE_INTERVAL 
    if(aerf>=AERFA_MAX)aerf=AERFA_MAX;//AERFA_MAX                           120*AERF_DEGREE_INTERVAL //degree
    gAerfDegree.AerfUpdate = aerf;
    gAerfDegree.updateflag = FLAGREADY;
    return;
}
uint32_t GetFeedbackCurrent(void)
{
    uint32_t temp;
    uint32_t tempcurrent;
    //DEBUG_PRINT("           ");
    delay_ms(1);
    temp = Input_Get_SampleI();    
    //tempcurrent = ((Input_Get_SampleI()+1)*gcfg_current_range/1024);//0~1024
    tempcurrent = ((temp+1)*gcfg_current_range*10/4096);//
    //tempcurrent = ((temp+1)*1500/4096);
    return tempcurrent;
}
uint32_t GetFeedbackVoltage(void)
{
    uint32_t temp;
    uint32_t tempvoltage;
    //DEBUG_PRINT("           \r\n");
    delay_ms(1);
    temp = Input_Get_SampleV();
    //tempvoltage = ((Input_Get_SampleV()+1)*gcfg_voltage_range/1024);//0~1024    
    //DEBUG_PRINT("[I]:*** adc get vol %d\r\n",temp);
    tempvoltage = ((temp+1)*gcfg_voltage_range*10/4096);//
    //tempvoltage = ((temp+1)*630/4096);
    return tempvoltage;
}

/********queue*****/
/**return ERR_OK ERR_UNKNOW**/    
uint8_t switchopenqueueappend(__SWOPEN swopen, __SWQUEUE *openqueue)
{
    int i = 0;
    //DEBUG_PRINT("open que %d\r\n",openqueue->num);
    if(openqueue->num>=QUEUE_NUM_MAX){
        return ERR_QUEUE_FULL;
    }
    /*sort time*/
    //DEBUG_PRINT("fuck\r\n");
    //DEBUG_PRINT("%d %d %d %d\r\n",openqueue->pstaticstart,openqueue->pstaticend,openqueue->pqueuehead,openqueue->pqueuetail);
    openqueue->sw[openqueue->queuetail] = swopen;
    //DEBUG_PRINT("%d %d %d\r\n",openqueue->pqueuetail->swnum,openqueue->pqueuetail->swopentime,openqueue->pqueuetail->swclosetime);
    openqueue->num++;
    if(openqueue->queuetail==openqueue->staticend){
        openqueue->queuetail = openqueue->staticstart;
        openqueue->headtailstatu = TAIL_HEAD;
    }else{
        openqueue->queuetail++;
    }
    return ERR_OK;
}
/**return ERR_OK ERR_UNKNOW**/
uint8_t switchopenqueuepop(__SWQUEUE *openqueue)
{
    if(openqueue->num==0)
    {
        return ERR_QUEUE_EMPTY;
    }else if(openqueue->queuehead==openqueue->staticend){
        openqueue->queuehead = openqueue->staticstart;
        openqueue->headtailstatu = HEAD_TAIL;
    }else{
        openqueue->queuehead++;
    }
    openqueue->num--;
    return ERR_OK;
}

uint8_t genswitchqueue(uint8_t order,uint32_t time,uint32_t aerf){
    __SWOPEN swtemp;
    int i = 0;

    switch(order){
        case PHASE_ORDER_UVW:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
        for (i = 0; i < 6; ++i)
        {
            swtemp.swnum = chargeOrder_UVW[i];
      swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
      swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*i+AERFA_OFFSET + i*WUCHA_XIUZHENG;
      switchopenqueueappend(swtemp,&gswitchopenqueue[i]);
      swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*i + DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET + i*WUCHA_XIUZHENG;
      switchopenqueueappend(swtemp,&gswitchopenqueue[i]);
  }
  break;
  case PHASE_ORDER_UWV:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
  for (i = 0; i < 6; ++i)
  {
      swtemp.swnum = chargeOrder_UWV[i];
      swtemp.swcount = SW_OPEN_CLOSE_COUNT;
      swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*i+AERFA_OFFSET + i*WUCHA_XIUZHENG;
      switchopenqueueappend(swtemp,&gswitchopenqueue[i]);
      swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*i + DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET + i*WUCHA_XIUZHENG;
      switchopenqueueappend(swtemp,&gswitchopenqueue[i]);
  }
  break;
  default:
  break;
}

}

uint8_t genswitchqueue_eint1_vu(uint8_t order,uint32_t time,uint32_t aerf){
    __SWOPEN swtemp;
    int i = 0;
    switch(order){
        case PHASE_ORDER_UVW:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
        swtemp.swnum = chargeOrder_UVW[0];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*0+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[0]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*0 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[0]);
                swtemp.swnum = chargeOrder_UVW[3];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*3+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[3]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*3 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[3]);


                break;
                case PHASE_ORDER_UWV:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
                swtemp.swnum = chargeOrder_UWV[0];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*0;
                switchopenqueueappend(swtemp,&gswitchopenqueue[0]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*0 + DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1;
                switchopenqueueappend(swtemp,&gswitchopenqueue[0]);
                swtemp.swnum = chargeOrder_UWV[3];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*3+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[3]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*3 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[3]);
                break;
                default:
                break;
            }
            return 0;
        } 

        uint8_t genswitchqueue_eint2_uw(uint8_t order,uint32_t time,uint32_t aerf){
            __SWOPEN swtemp;
            int i = 0;
            switch(order){
                case PHASE_ORDER_UVW:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
                swtemp.swnum = chargeOrder_UVW[1];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[1]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*1 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[1]);
                swtemp.swnum = chargeOrder_UVW[4];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*4+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[4]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*4 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[4]);


                break;
                case PHASE_ORDER_UWV:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
                swtemp.swnum = chargeOrder_UWV[1];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*1;
                switchopenqueueappend(swtemp,&gswitchopenqueue[1]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*1 + DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1;
                switchopenqueueappend(swtemp,&gswitchopenqueue[1]);
                swtemp.swnum = chargeOrder_UWV[4];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*4+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[4]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*4 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[4]);
                break;
                default:
                break;
            }
            return 0;
        }
        uint8_t genswitchqueue_eint3_wv(uint8_t order,uint32_t time,uint32_t aerf){
            __SWOPEN swtemp;
            int i = 0;
            switch(order){
                case PHASE_ORDER_UVW:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
                swtemp.swnum = chargeOrder_UVW[2];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*2+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[2]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*2 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[2]);
                swtemp.swnum = chargeOrder_UVW[5];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*5+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[5]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*5 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[5]);


                break;
                case PHASE_ORDER_UWV:
            //DEBUG_PRINT("PHASE_ORDER_UWV\r\n",);
                swtemp.swnum = chargeOrder_UWV[2];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*2;
                switchopenqueueappend(swtemp,&gswitchopenqueue[2]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*2 + DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1;
                switchopenqueueappend(swtemp,&gswitchopenqueue[2]);
                swtemp.swnum = chargeOrder_UWV[5];
                swtemp.swcount = SW_OPEN_CLOSE_COUNT;//count 10 time 
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*5+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[5]);
                swtemp.swopentime = time + aerf + SW_DEGREE_INTERVAL*5 +DOUBLE_SW_STIMULATE_INTERVAL*RIT_1_MS_COUNT+1+AERFA_OFFSET;
                switchopenqueueappend(swtemp,&gswitchopenqueue[5]);
                break;
                default:
                break;
            }
            return 0;
        }
        void printqueue(__SWQUEUE que)
        {
            int i = 0;
            int tempaddr = 0;
    //DEBUG_PRINT("*****que*****\r\n");
    //DEBUG_PRINT("num: %d  \r\n",que.num);
    //DEBUG_PRINT("addr: ss %d\r\n",que.staticstart);
    //DEBUG_PRINT("addr: se %d\r\n",que.staticend);
    //DEBUG_PRINT("addr: h %d\r\n",que.queuehead);
    //DEBUG_PRINT("addr: t %d\r\n",que.queuetail);
    //DEBUG_PRINT("data: \r\n",que.staticstart);
            tempaddr = (int)que.queuehead;
    //for(i = 0;i<que.num;i++){
            DEBUG_PRINT("[q]%d  %d  \r\n",que.sw[que.queuehead].swnum,que.sw[que.queuehead].swopentime);
        //que.queuehead++;
        //if(que.queuehead>que.staticend)
        //{
        //        que.queuehead = que.staticstart + que.queuehead - que.staticend - 1;
        //}
    //}
    //que.queuehead = tempaddr;
    //DEBUG_PRINT("***end****\r\n");
        }



uint8_t canconnectedcheck(){
	
	uint8_t ret  = _canconnectedcheck();
	if(ret){
		return ret;
	}else{
		setchargemsgerr(ERR_TIMEOUT);
		return ret;
	}
	
}

