#ifndef _DATAPROCESS_H_
#define _DATAPROCESS_H_
#include "LPC17xx.h"

//#include <includes.h>

/*************
  * @parameters  local parameters
	*/
#define  SYSTEMTICK_INTERVAL                100//ms
#define  SYSTEMTICK_ONE_SECOND_COUNT_H        ((1000/SYSTEMTICK_INTERVAL))//
#define  PHASECHECK_TIMEOUT                 8000//ms
#define  PHASECHECK_TIMEOUT_COUNT           (PHASECHECK_TIMEOUT/SYSTEMTICK_INTERVAL)
#define  PHASECHECK_COUNT                   100
#define  PHASECHECK_DEVIATION               15
#define  CHARGEWOKR_MAXCURRENT              0
#define  CHARGEWORK_MAXVOLTAGE              0
#define  CHARGEWORK_TIMEOUT                 100//min h?
#define  FEEDBACK_UPDATE_INTERVAL           1000//ms
#define  FEEDBACK_UPDATE_INTERVAL_COUNT     (FEEDBACK_UPDATE_INTERVAL/SYSTEMTICK_INTERVAL)
#define  CONTROL_AERF_INTERVAL              500//ms
#define  CONTROL_AERF_INTERVAL_COUNT        (CONTROL_AERF_INTERVAL /SYSTEMTICK_INTERVAL)   

#define SOFT_START_NULL                     0
#define SOFT_START_ON                       1
#define SOFT_START_END                      2

#define SOFT_STOP_NULL                      0
#define SOFT_STOP_ON                       1
#define SOFT_STOP_END                      2

#define BMS_CAN_TIMEOUT                     5000//ms




#define RIT_TIME_INTERVAL                   50//us
#define RIT_1_MS_COUNT                      1000/RIT_TIME_INTERVAL
#define SYSTEMTICK_ONE_SECOND_COUNT         1000*RIT_1_MS_COUNT 
#define DEGREE_TIME_INTERVAL                50
#define AERF_DEGREE_INTERVAL                DEGREE_TIME_INTERVAL/RIT_TIME_INTERVAL
#define SW_CLOSE_TIME_INTERVAL              200
#define SW_CLOSE_TIME_INTERVAL_COUNT        ((SW_CLOSE_TIME_INTERVAL/RIT_TIME_INTERVAL)-1)
#define CHARGEHANDLER_INTERVAL_TIME        500//ms
#define CHARGEHANDLER_INTERVAL_COUNT       CHARGEHANDLER_INTERVAL_TIME*RIT_1_MS_COUNT
#define  SOFT_START_INTERVAL_TIME                500 //ms
#define  SOFT_START_INTERVAL_COUNT          SOFT_START_INTERVAL_TIME/CHARGEHANDLER_INTERVAL_TIME 
#define  SOFT_STOP_INTERVAL_TIME                 500 //ms
#define  SOFT_STOP_INTERVAL_COUNT          SOFT_STOP_INTERVAL_TIME/CHARGEHANDLER_INTERVAL_TIME

#define AERFA_MIN                           1*AERF_DEGREE_INTERVAL  //degree
#define AERFA_MAX                           120*AERF_DEGREE_INTERVAL //degree
#define MAX_OUTPUT_AERF                     0*AERF_DEGREE_INTERVAL//
#define MIN_OUTPUT_AERF                     120*AERF_DEGREE_INTERVAL//degree
#define SW_DEGREE_INTERVAL                  60*AERF_DEGREE_INTERVAL//degree
#define AERFA_OFFSET                        330*AERF_DEGREE_INTERVAL//degree
#define SW_OPEN_CLOSE_COUNT                 10
#define DOUBLE_SW_STIMULATE_INTERVAL        3
#define RIT_1_MIN_COUNT                     60*1000*RIT_1_MS_COUNT
#define SOFT_START_STEP                     3*AERF_DEGREE_INTERVAL
#define WUCHA_XIUZHENG                      7*AERF_DEGREE_INTERVAL

#define BMS_CAN_TIMEOUT_COUNT               BMS_CAN_TIMEOUT*RIT_1_MS_COUNT
/******@parameter   parameters ******************/

#define SINGEL_TIME_PULSE_WIDTH             55  //us
#define SINGEL_PULSE_DEGREE                 1   //degree
#define FEEDBACK_RATE_VOLTAGE               300  //V
#define FEEDBACK_RATE_CURRENT               60  //A
#define FISRTSTAGE_CONSTCURRENT             30  //A
#define FISRTSTAGE_LIMITVOLTAGE              40  //V
#define FISERSTAGE_TIMEOUT                  30*60*100//10ms
#define SECONDSTAGE_LIMITVOLTAGE            56 //
#define SECONDSTAGE_CONSTCURRENT           150//
#define SECONDSTAGE_TIMEOUT                120*60*100//10ms
#define THIRDSTAGE_CONSTVOLTAGE             59
#define THIRDSTAGE_LIMITVOLTAGE             60
#define THIRDSTAGE_LIMITCURRENT             80
#define THIRDSTAGE_TIMEOUT                  15*60*100
#define VOLTAGE_ACCURACY                    4//%
#define CURRENT_ACCURACY                    4//%
#define REFERENCE_VOLTAGE                   380//V
#define MULTIPLE_CONST                      10//
#define CLOSE_CURRENT                       200//20A


/****************
  * @common use define
	*/
#define  ERR_OK                             0
#define  ERR_TIMEOUT                        1
#define  ERR_LACKPHASE                      2
#define  ERR_OVERHEAT                       3
#define  ERR_BATTERYPROTECT									4
#define  ERR_UNKNOW                         5


#define  ERR_QUEUE_FULL                     6
#define  ERR_QUEUE_EMPTY                    7	

#define HIGHVOLTAGE                   1//front higher than the after UW : U > W for 1 , U < W for 0
#define LOWVOLTAGE                    0

#define FLAGREADY                     1
#define FLAGNOTREADY                  0
#define FLAGUSED                      1
#define FLAGNOTUSED                   0

#define PHASE_VU                      1
#define PHASE_UW                      2
#define PHASE_WV                      3
#define PHASE_ORDER_UVW               1
#define PHASE_ORDER_UWV               2

// 
#define PULSE_OFF                     0
#define PULSE_ON                      1
#define PULSE_ONE                     2

#define PULSE_OPENTIME                4//*27us = 108us

#define FAULT_OUTPUT                  1
#define NO_FAULT                      0
#define RUN_OUTPUT                    1
#define RUN_OFF                       0
#define NO_EMERGENCY                  0
#define START_CHARGE_RELAY            0
//
#define LOAD_MIN_60
//
#define SWITCH_ONE                    1
#define SWITCH_TWO                    2

#define STATE_IDEL 	                   			 00
#define CAN_STATE                            10
#define CAN_STOP 		                         11
#define CAN_FINISH 	                         12
#define CAN_DISCONNECTED                     13
#define LOCAL_STATE_CHECK                    20
#define LOCAL_STATE_ONE                      21
#define LOCAL_STATE_TWO                      22
#define LOCAL_STATE_THREE                    23
#define LOCAL_STATE_END                      24

#define CONFIG_MODE_LOCAL         1
#define CONFIG_MODE_CAN           0
//test work define 
//#define TEST_0406
#define TEST_0422

#ifndef DEBUG_PRINT
#define DEBUG_PRINT
#define DEBUG_PRINT(format, ...) _printf (format,##__VA_ARGS__)
#endif
#define swqueue(x)             gswitchopen_sw##x##_queue
/**
  * @struct charge mode  
	* @info two work mode one test mode 
	* @data 0- test
	*       1- local mode 
	*       2- communt mode
	*/
enum CHARGEMODE{
	CHARGEMODE_TEST = 0,
	CHARGEMODE_LOCAL,
	CHARGEMODE_COM
};


/**
  * @struct charge status 
	* @info  only for local mode
	* @data 0- idel
  *       1- prepare for start  :  three-phase voltage detection
  *       2- wait for start  : wait for sync time signal
  *       3- first stage  : const current
	*       4- second stage : const current
	*       5- third stage  : const voltage
  *       6- stop         : interrupt stop
  *       7- end          : normar end 
  *       8- err          : err
	*/
enum CHARGESTATU{
	CHARGESTATU_IDEL = 0,
	CHARGESTATU_START,
	CHARGESTATU_SOFT_START,
	//CHARGESTATU_PREPARE,
	CHARGESTATU_WORK,//both for local and bms_can
//	CHARGESTATU_WAITFOR_FIRST,
//	CHARGESTATU_FIRSTSTAGE,
//	CHARGESTATU_SECONDSTAGE,
//	CHARGESTATU_THIRDSTAGE,
	CHARGESTATU_STOP,
	CHARGESTATU_SOFT_STOP,
	CHARGESTATU_END,
	CHARGESTATU_ERR
};

enum ENTPHASESTATU{
	ENT_IDEL = 0,
	ENT_CHECKPHASE,
	ENT_PREPARE,
	ENT_START_CHARGE,
	ENT_STOP_CHARGE,
	ENT_ERR
};
typedef enum ENTPHASESTATU __ENTSTATU;
enum SWITCHGROUPSTATU{
	SWITCH_IDEL = 0,
	SWITCH_1,
	SWITCH_2,
	SWITCH_3,
	SWITCH_4,
	SWITCH_5,
	SWITCH_6,
	SWITCH_START,
	SWITCH_END,
	SWITCH_ERR
};
typedef enum SWITCHGROUPSTATU __SWSTATU;
/**input data**/
/**
  * @struct voltage ent input 
	*/
struct ENT_IN
{
	uint8_t readyflag;
	uint8_t phase;
	uint8_t phase_order;
	uint8_t VU_voltage;
	uint8_t WV_voltage;
	uint8_t UW_voltage;
	uint16_t ccount;
	uint8_t err;
	uint32_t tcount;
};
struct OutputVoltage
{
//real x 10
	uint32_t VoltageTarget_sw_one;
	uint32_t VoltageTarget_sw_two;
	uint32_t VoltageTarget_three;
	uint32_t VoltageTarget_BMS;
	uint32_t VoltageFeedback;
	uint32_t VoltageFeedbackTotal;
	uint8_t err;
};
typedef struct OutputVoltage __OV;
struct OutputCurrent
{
// real x 10
	uint32_t CurrentTarget_one;
	uint32_t CurrentTarget_two;
	uint32_t CurrentTarget_sw_three;
	uint32_t CurrentTarget_BMS;
	uint32_t CurrentFeedback;
	uint32_t CurrentFeedbackTotal;
	uint8_t err;
};
typedef struct OutputCurrent __OC;
struct AerfDegree
{
	uint32_t AerfNext;
	uint32_t AerfThis;
	uint32_t AerfLast;
	uint32_t AerfUpdate;
	uint32_t AerfTarget;
	uint8_t updateflag;
	uint8_t err;
};
typedef struct AerfDegree    __Aerf;
/***extern global varible***/
#ifdef DATAPROCESS_GLOBAL
#define DATAPROCESS_EXT
#else
#define DATAPROCESS_EXT extern
#endif
extern  const uint8_t chargeOrder_UVW[6];
extern  const uint8_t chargeOrder_UWV[6];
extern  uint8_t gPulseOutputStatu;
extern  uint8_t gswstartflag;
extern  enum CHARGESTATU gChargeStatu;
extern  __ENTSTATU  gentstatu;
extern  uint32_t gChargeSystemTimeCount_ms;
extern  uint32_t gChargeLastStageSysTime_ms;
extern  uint32_t gchargehandler_time; 
extern  struct ENT_IN gEnt_In;
extern  __OC gCurrentAll;
extern  __OV gVoltageAll;
extern __Aerf gAerfDegree;
extern  uint32_t gSystemTime;
extern  uint32_t gchargeh_ms_count;
extern  uint32_t gchargehandler_softstart_count;
extern  uint32_t gchargehandler_softstop_count;
extern  uint32_t gchargetime_us_count;
extern  uint8_t gSWtimeFlag;
extern  uint32_t geintwatchtime_count[3];

extern uint16_t gcfg_can_baudrate_index ;
//public 
struct BMSMessage{
	uint8_t BMSFlag;
	uint32_t BMSVoltage;
	uint32_t BMSCurrent;
	uint8_t BMSControl;
	uint8_t err;
};
typedef struct BMSMessage __BMSMsg;
extern __BMSMsg  gBMSMessage;
uint8_t Output_Close_All(void);
void bctsk_Local (void);
//void App_Local_BatteryCharge(void);

typedef struct ChargeMessage{
	uint8_t updateflag;
	uint32_t feedbackvoltage;
	uint32_t feedbackcurrent;
	uint32_t chargetime;
	uint8_t localstate;
	uint8_t err;
}__CMsg;
///typedef struct ChargeMessage __CMsg;
extern __CMsg gchargemsg;



//private
uint8_t ThreePhaseCheck(uint32_t timeout,uint32_t count);
void UpdateAerf(uint32_t aerf);
uint32_t GetFeedbackCurrent(void);
uint32_t GetFeedbackVoltage(void);

void eintfeedwatchdag(uint8_t eintx);
void eintwatchdagwow();
void localchargefeedwatchdag(uint8_t statudog);
void localchargewatchdagwow(uint8_t statudog);
void setchargemsg(uint32_t vol,uint32_t cur, uint8_t state);

uint32_t GetFeedbackVoltage(void);
uint32_t GetFeedbackCurrent(void);


/********/
#define QUEUE_NUM_MAX                   12


typedef struct SWITCHOPEN{
	uint8_t swnum;
	uint32_t swopentime;
	uint8_t swcount;
}__SWOPEN;

#define HEAD_TAIL         1
#define TAIL_HEAD         2

typedef struct SWITCHQUEUE{
	uint8_t num;
	uint8_t err;
	uint8_t headtailstatu;
	__SWOPEN sw[QUEUE_NUM_MAX];
	uint8_t staticstart;
	uint8_t staticend;
	uint8_t queuehead;
	uint8_t queuetail;
}__SWQUEUE;

extern 	__SWQUEUE gswitchopenqueue[6];	

//extern __SWQUEUE gswitchclosequeue;
/**return ERR_OK ERR_UNKNOW**/
uint8_t switchopenqueueappend(__SWOPEN swopen,__SWQUEUE *openqueue);
/**return ERR_OK ERR_UNKNOW**/
uint8_t switchopenqueuepop(__SWQUEUE *openqueue);

/**return ERR_OK ERR_UNKNOW**/
uint8_t genswitchqueue(uint8_t order,uint32_t time,uint32_t aerf);
uint8_t genswitchqueue_eint1_vu(uint8_t order,uint32_t time,uint32_t aerf);
uint8_t genswitchqueue_eint2_uw(uint8_t order,uint32_t time,uint32_t aerf);
uint8_t genswitchqueue_eint3_wv(uint8_t order,uint32_t time,uint32_t aerf);
void printqueue(__SWQUEUE que);

uint8_t chargestart(void);
uint8_t chargestop(void);
__CMsg* chargemsg(void);
void chargehandler(void);
void setchargemsgerr(uint8_t err);
uint8_t canconnectedcheck();
#endif

