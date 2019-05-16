#include "stdint.h"
#include "input.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_pinsel.h"
//#include <UART/UART.h>
//#include <includes.h>
#include "dataprocess.h"
#include "systen_delay.h"


/*********************************************************************
 * @Function    Input_Init_ENTx
 * @brief       Init the ENT1\2\3 Setting
 * @param[in]   None
 * @return      None 
 **********************************************************************/
void Input_Init_ENTx(void){
    
    EXTI_InitTypeDef EXTICfg;
    PINSEL_CFG_Type PinCfg;

    /* Setting P2.13 as EINT3 */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    PinCfg.Pinnum = 13;
    PinCfg.Portnum = 2;
    PINSEL_ConfigPin(&PinCfg);

    /* Setting P2.11 as EINT1 */
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg);

    /* Setting P2.12 as EINT2 */
    PinCfg.Pinnum = 12;
    PINSEL_ConfigPin(&PinCfg);

    /* Initialize External 1 interrupt */
    EXTI_Init();
    EXTICfg.EXTI_Line = EXTI_EINT1;
    /* edge sensitive */
    EXTICfg.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    EXTICfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
    EXTI_Config(&EXTICfg);

    /* Initialize External 2 interrupt */
    EXTICfg.EXTI_Line = EXTI_EINT2;
    /* edge sensitive */
    EXTICfg.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    EXTICfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
    EXTI_Config(&EXTICfg);

    /* Initialize External 2 interrupt */
    EXTICfg.EXTI_Line = EXTI_EINT3;
    /* edge sensitive */
    EXTICfg.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    EXTICfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
    EXTI_Config(&EXTICfg);

    NVIC_SetPriorityGrouping(4);  
    NVIC_SetPriority(EINT1_IRQn, 1);
    NVIC_SetPriority(EINT2_IRQn, 1);
    NVIC_SetPriority(EINT3_IRQn, 1);
}

/*********************************************************************
 * @Function    Input_Open_ENTx
 * @brief       Open the ENT1\2\3 
 * @param[in]   None
 * @return      None 
 **********************************************************************/
void Input_Open_ENTx(void){
    NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_EnableIRQ(EINT3_IRQn);
}

/*********************************************************************
 * @Function    Input_Close_ENTx
 * @brief       Close the ENT1\2\3 
 * @param[in]   None
 * @return      None 
 **********************************************************************/
void Input_Close_ENTx(void){
    NVIC_DisableIRQ(EINT1_IRQn);
    NVIC_DisableIRQ(EINT2_IRQn);
    NVIC_DisableIRQ(EINT3_IRQn);
}


/*********************************************************************
 * @Function    Input_Get_ENTx
 * @brief       Get the ENTx input level
 * @param[in]   None
 * @return      0:ENT1 is '0',ENT2 is '0',ENT3 is '0'
                1:ENT1 is '1',ENT2 is '0',ENT3 is '0'
                2:ENT1 is '0',ENT2 is '1',ENT3 is '0'
                3:ENT1 is '1',ENT2 is '1',ENT3 is '0'
                4:ENT1 is '0',ENT2 is '0',ENT3 is '1'
                5:ENT1 is '1',ENT2 is '0',ENT3 is '1'
                6:ENT1 is '0',ENT2 is '1',ENT3 is '1'
                7:ENT1 is '1',ENT2 is '1',ENT3 is '1'
 **********************************************************************/
uint8_t Input_Get_ENTx(void){
    return ((GPIO_ReadValue(2) & 0x3800)>>11);
}
uint8_t Input_Get_ENT_1_VU(void){
    return ((GPIO_ReadValue(2) & 0x800)>>11);
}
uint8_t Input_Get_ENT_2_UW(void){
		return ((GPIO_ReadValue(2) & 0x1000)>>12);
}
uint8_t Input_Get_ENT_3_WV(void){
		return ((GPIO_ReadValue(2) & 0x2000)>>13);
}


/*********************************************************************
 * @Function    EINT1_IRQHandler
 * @brief       External Interrupt 1 handler
 * @param[in]   None
 * @return      None
 **********************************************************************/
int printfcount = 10;
void EINT1_IRQHandler(void){//VU
		EXTI_ClearEXTIFlag(EXTI_EINT1);
	
    //@luoyang
	#ifdef SELFLOCK_MODE
	if(Input_Start_Relay()!=START_CHARGE_RELAY)
	{
		Output_Close_All();
		gChargeStatu = CHARGESTATU_STOP;
		while(1);
		return;
	}
	#endif
	
	switch(gentstatu)
	{
	case ENT_IDEL:
		return;
	case ENT_CHECKPHASE:
    /**status change**/
	  DEBUG_PRINT("[I]:eint1 %d\r\n",gEnt_In.ccount);
		gEnt_In.readyflag = FLAGREADY;
		gEnt_In.phase = PHASE_VU;
		gEnt_In.WV_voltage = Input_Get_ENT_3_WV();
	  //DEBUG_PRINT("[I]:eint1 WV: %d \r\n",gEnt_In.WV_voltage);
		gEnt_In.UW_voltage = Input_Get_ENT_2_UW();
	  //DEBUG_PRINT("[I]:eint1 UW: %d \r\n\r\n",gEnt_In.UW_voltage);
	  gEnt_In.err = ERR_OK;
		gEnt_In.tcount = 0;
		gEnt_In.ccount = 1 + gEnt_In.ccount;
		return;
	case ENT_PREPARE:
	  //  DEBUG_PRINT("[I]:oENT_PREPARErder: 2\r\n" );
		gentstatu = ENT_START_CHARGE;
	  Output_Open_RIT_REAL();
	  //DEBUG_PRINT("[I]:oENT_PREPARErder: 3\r\n" );
	  Output_Run_Relay(RUN_OUTPUT);

		DEBUG_PRINT("[I]:*******eint start charge!!\r\n");
		return;
	case ENT_START_CHARGE:
		//DEBUG_PRINT("[I]: eint live!!\r\n");
		eintfeedwatchdag(0);
		if(gAerfDegree.updateflag == FLAGREADY){
			gAerfDegree.AerfThis = gAerfDegree.AerfUpdate;
			gAerfDegree.updateflag = FLAGNOTREADY;
		}
		genswitchqueue(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
	//genswitchqueue_eint1_vu(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
		if(gswstartflag==FLAGNOTREADY){
			gswstartflag=FLAGREADY;
		}
		//printqueue(gswitchclosequeue);
		//printqueue(gswitchopenqueue);
	  //Output_Open_RIT_REAL();
		return;
	case ENT_STOP_CHARGE:
		return;
	case ENT_ERR:
		Output_Close_All();
		Input_Close_ENTx();
		gEnt_In.err = ERR_UNKNOW;
	  DEBUG_PRINT("[I]:eint err!!\r\n");
		return;
	default:
		gentstatu = CHARGESTATU_ERR;
		gEnt_In.err = ERR_UNKNOW;
	  Output_Close_All();
	  Input_Close_ENTx();
		return;
	}
}


/*********************************************************************
 * @Function    EINT2_IRQHandler
 * @brief       External Interrupt 2 handler
 * @param[in]   None
 * @return      None
 **********************************************************************/
void EINT2_IRQHandler(void){//UW
		EXTI_ClearEXTIFlag(EXTI_EINT2);
    //@luoyang
	#ifdef SELFLOCK_MODE
	if(Input_Start_Relay()!=START_CHARGE_RELAY)
	{
		Output_Close_All();
		gChargeStatu = CHARGESTATU_STOP;
		while(1);
		return;
	}
	#endif
		
	switch(gentstatu)
	{
	case ENT_IDEL:
		return;
	case ENT_CHECKPHASE:
		DEBUG_PRINT("[I]:eint2 %d\r\n",gEnt_In.ccount);
		gEnt_In.readyflag = FLAGREADY;
		gEnt_In.phase = PHASE_UW;
		gEnt_In.WV_voltage = Input_Get_ENT_3_WV();
	  //DEBUG_PRINT("[I]:eint2 WV: %d \r\n",gEnt_In.WV_voltage);
		gEnt_In.VU_voltage = Input_Get_ENT_1_VU();
	  //DEBUG_PRINT("[I]:eint2 UW: %d \r\n\r\n",gEnt_In.VU_voltage);
	  gEnt_In.err = ERR_OK;
		gEnt_In.tcount = 0;
		gEnt_In.ccount = 1 + gEnt_In.ccount;
		return;
	case ENT_PREPARE:
		return;
	case ENT_START_CHARGE:
		eintfeedwatchdag(1);	
	//genswitchqueue_eint3_wv(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
	//genswitchqueue_eint2_uw(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
		return;
	case ENT_STOP_CHARGE:
		return;
	case ENT_ERR:
		Input_Close_ENTx();
	  Output_Close_All();
		gEnt_In.err = ERR_UNKNOW;
		return;
	default:
		gentstatu = CHARGESTATU_ERR;
		gEnt_In.err = ERR_UNKNOW;
	  Output_Close_All();
	  Input_Close_ENTx();
		return;
	}
}

/*********************************************************************
 * @Function    EINT3_IRQHandler
 * @brief       External Interrupt 3 handler
 * @param[in]   None
 * @return      None
 **********************************************************************/
void EINT3_IRQHandler(void){//WV
		EXTI_ClearEXTIFlag(EXTI_EINT3);
    //@luoyang
	#ifdef SELFLOCK_MODE
	if(Input_Start_Relay()!=START_CHARGE_RELAY)
	{
		DEBUG_PRINT("[I]:SELFLOCK_MODE OFF\r\n");
		Output_Close_All();
		gChargeStatu = CHARGESTATU_STOP;
		while(1);
		return;
	}
	#endif

	switch(gentstatu)
	{
	case ENT_IDEL:
		return;
	case ENT_CHECKPHASE:
    /**status change**/
	  DEBUG_PRINT("[I]:eint3 %d\r\n",gEnt_In.ccount);
		gEnt_In.readyflag = FLAGREADY;
		gEnt_In.phase = PHASE_WV;
		gEnt_In.VU_voltage = Input_Get_ENT_1_VU();
	  //DEBUG_PRINT("[I]:eint3 VU: %d \r\n",gEnt_In.VU_voltage);
		gEnt_In.UW_voltage = Input_Get_ENT_2_UW();
	  //DEBUG_PRINT("[I]:eint3 WV: %d \r\n\r\n",gEnt_In.UW_voltage);
	  gEnt_In.err = ERR_OK;
		gEnt_In.tcount = 0;
		gEnt_In.ccount = 1 + gEnt_In.ccount;
		return;
	case ENT_PREPARE:
		return;
	case ENT_START_CHARGE:
		eintfeedwatchdag(2);
		//genswitchqueue_eint2_uw(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
		//genswitchqueue_eint3_wv(gEnt_In.phase_order,gchargetime_us_count,gAerfDegree.AerfThis);
		return;
	case ENT_STOP_CHARGE:
		return;
	case ENT_ERR:
		Input_Close_ENTx();
	  Output_Close_All();
		gEnt_In.err = ERR_UNKNOW;
		return;
	default:
		gentstatu = CHARGESTATU_ERR;
		gEnt_In.err = ERR_UNKNOW;
	  Output_Close_All();
	  Input_Close_ENTx();
		return;
  }
}


/*********************************************************************
 * @Function    Input_Init_CTRL
 * @brief       Init the run relay and stop relay input, and set the 3 
                quit interrupts for emergency affair.
 * @param[in]   None
 * @return      None 
 **********************************************************************/
void Input_Init_CTRL(void){
    PINSEL_CFG_Type PinCfg;
    
    //Init the input control message 
    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;

    //No.24 Start-Relay P0[28]
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 28;
    PINSEL_ConfigPin(&PinCfg);

    //No.25 Stop-Relay P0[27]
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 27;
    PINSEL_ConfigPin(&PinCfg);

    //No.79 P0[6] Quit input
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 6;
    PINSEL_ConfigPin(&PinCfg);
    
    //No.86 P1[17] Reset input
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 17;
    PINSEL_ConfigPin(&PinCfg);

    //No.87 P1[16] Overheat input
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 16;
    PINSEL_ConfigPin(&PinCfg);

     //Defined as input
    GPIO_SetDir(0, (1<<27)|(1<<28)|(1<<6),0);
    GPIO_SetDir(0, (1<<17)|(1<<16),0);
}


/*********************************************************************
 * @Function    Input_Start_Relay
 * @brief       Read the start relay input
 * @param[in]   None
 * @return      1 : Start cmd (Depend on the start mode) 
                0 : Stop cmd 
 **********************************************************************/
uint8_t Input_Start_Relay(void){
   if(!(GPIO_ReadValue(0) & (1<<28))){
		return 1;
	}
	else{
		return 0;
	}
}

/*********************************************************************
 * @Function    Input_Stop_Relay
 * @brief       Read the stop relay input
 * @param[in]   None
 * @return      1 : Stop cmd (Depend on the start mode)  
 **********************************************************************/
uint8_t Input_Stop_Relay(void){
   if(!(GPIO_ReadValue(0) & (1<<27))){
		return 1;
	}
	else{
		return 0;
	}
}

/*********************************************************************
 * @Function    Input_Is_Emergency
 * @brief       Read the 3 kinds of emergency affair input
 * @param[in]   None
 * @return      0: Nothing happen
                1: Quit cmd
                2: Reset cmd
                3: Overheat cmd
 **********************************************************************/
uint8_t Input_Is_Emergency(void){
	  //DEBUG_PRINT("____________%u %d %d _________\r\n",GPIO_ReadValue(1),(1<<16),(GPIO_ReadValue(1) & (1<<16)));
    if(!(GPIO_ReadValue(0) & (1<<6))){
        return 1;
    }
    else if(!(GPIO_ReadValue(1) & (1<<17))){
	      return 2;
    }
    else if(!(GPIO_ReadValue(1) & (1<<16))){
	      return 3;
    }
    else{
		    return 0;        
    }
}


/*********************************************************************
 * @Function    Input_Init_Sample
 * @brief       Init the voltage and current sampling adc 
 * @param[in]   None
 * @return      None
 **********************************************************************/
void Input_Init_Sample(void){
    
    PINSEL_CFG_Type PinCfg;
    /*
     * Init ADC pin connect
     * AD0.3 on P0.26 I
     * AD0.4 on P1.30 V
     */
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Funcnum = 1;
    PinCfg.Pinnum = 26;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Funcnum = 3;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 30;
    PINSEL_ConfigPin(&PinCfg);

    /* Configuration for ADC:
     *  select: ADC channel 3 
     *          ADC channel 4 
     *  ADC conversion rate = 200KHz
     */
    //ADC_Init(LPC_ADC, 200000);
}


/*********************************************************************
 * @Function    Input_Get_SampleV
 * @brief       Get average voltage of 10 times voltage samples
 * @param[in]   None
 * @return      uint32_t voltage data
 **********************************************************************/
uint16_t Input_Get_SampleV(void){
    uint16_t adc_value_average;
    uint8_t adc_cnt;
    adc_cnt = 0;
		adc_value_average = 0;
	  
    ADC_Init(LPC_ADC, 200000);
	  //DEBUG_PRINT("adc          \r\n");
    ADC_ChannelCmd(LPC_ADC,ADC_ADINTEN4,ENABLE);
    for (adc_cnt = 0; adc_cnt < 2; adc_cnt++){
        ADC_StartCmd(LPC_ADC,ADC_START_NOW);   
        while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_4,ADC_DATA_DONE)));
			  //DEBUG_PRINT("[I]:adc vol 3\r\n");
        adc_value_average += (ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_4));	
    }
		adc_value_average /= 2;
    ADC_ChannelCmd(LPC_ADC,ADC_ADINTEN4,DISABLE);
    return adc_value_average;
}


/*********************************************************************
 * @Function    Input_Get_SampleI
 * @brief       Get average current of 10 times current samples
 * @param[in]   None
 * @return      uint32_t current data 
 **********************************************************************/
uint16_t Input_Get_SampleI(void){
		uint16_t adc_value_average;
    uint8_t adc_cnt;
    adc_cnt = 0;
		adc_value_average = 0;
	
    ADC_Init(LPC_ADC, 200000);
	  //DEBUG_PRINT("adc          \r\n");
    ADC_ChannelCmd(LPC_ADC,ADC_ADINTEN3,ENABLE);
    for (adc_cnt = 0; adc_cnt < 2; adc_cnt++){
        ADC_StartCmd(LPC_ADC,ADC_START_NOW);
        while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_3,ADC_DATA_DONE)));
			  //DEBUG_PRINT("[I]:adc cur 3\r\n");
        adc_value_average += (ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3));	
    }
		adc_value_average /= 2;
    ADC_ChannelCmd(LPC_ADC,ADC_ADINTEN3,DISABLE);
    return adc_value_average;
}



