#include "stdint.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_pinsel.h"


#include "systen_delay.h"
#include "hy_instance.h"
#include "hy_dbg.h"
#include "hy_input.h"
#include "hy_systime.h"

#define  HY_LOG_TAG    "input"

static hy_instance_t *hy_instance = NULL;

hy_inputsignal_t *s_inputsignal;

int hy_input_init(void* hy_instance_handle)
{
	int ret = HY_OK;
	PINSEL_CFG_Type PinCfg;

	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_inputsignal = &(hy_instance->inputsignal);
	/*software init*/
	s_inputsignal->currentfb = 0;
	s_inputsignal->currentfb_x10A = 0;
	s_inputsignal->voltagefb = 0;
	s_inputsignal->voltagefb_x10V = 0;
	s_inputsignal->heatwarn = 0;/*todo*/
	s_inputsignal->resettrigger = 0;
	s_inputsignal->embtrigger = 0;
	
	/*hardware init*/  
    //Init the input control message 
    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;

    //No.79 P0[6] EMB input
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
    GPIO_SetDir(0, (1<<6),0);
    GPIO_SetDir(1, (1<<17)|(1<<16),0);
	LOG_INFO_TAG(HY_LOG_TAG,"hy init gpio done!");
		/*ADC INIT*/
		/*
     * Init ADC pin connect
     * current 7 P0.25 AD0.2  FUNCIONT 1
     * voltage 21 P1.30 AD0.4 FUNCTION3
     */
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Funcnum = 1;
    PinCfg.Pinnum = 25;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Funcnum = 3;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 30;
    PINSEL_ConfigPin(&PinCfg);

    /* Configuration for ADC:
     *  select: ADC channel 2 
     *          ADC channel 4 
     *  ADC conversion rate = 200KHz
     */
    ADC_Init(LPC_ADC, 200000);
		ADC_IntConfig(LPC_ADC,ADC_ADINTEN4,ENABLE);
		ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,ENABLE);

		LOG_INFO_TAG(HY_LOG_TAG,"hy init adc done!");
	
	return ret;
}


uint8_t hy_get_heatwarn(void){// 1 for normal 0 for heat
		
	uint32_t ret = ((GPIO_ReadValue(1)&(1<<16))>>16);
	hy_instance->inputsignal.heatwarn = ret;
	// LOG_INFO_TAG(HY_LOG_TAG,"hy get heatwarn [%d]",ret);
	if(ret){	
		return 0;
	}else{
		return 1;
	}
};

uint8_t hy_get_embtrigger(void){
	uint32_t ret = GPIO_ReadValue(0)&(1<<6);
		if(hy_instance==NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"hy input not init!!");
	}
	hy_instance->inputsignal.embtrigger = ret;
	LOG_INFO_TAG(HY_LOG_TAG,"hy get embtrigger [%d]",ret);
	if(ret){	
		return ret;
	}else{
		return ret;
	}
}

uint8_t hy_get_resettrigger(void){
	uint32_t ret = GPIO_ReadValue(1)&(1<<17);
	if(hy_instance==NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"hy input not init!!");
	}
	hy_instance->inputsignal.resettrigger=ret;
	LOG_INFO_TAG(HY_LOG_TAG,"hy get rettrigger [%d]",ret);
	if(ret){	
		return ret;
	}else{
		return ret;
	}
}

void hy_set_voltagefb_x10V(uint16_t value){
	   LOG_INFO_TAG(HY_LOG_TAG,"get voltage [%d]x0.1",value);
    hy_instance->inputsignal.voltagefb_x10V = value;
}

void hy_set_currentfb_x10A(uint16_t value){

    hy_instance->inputsignal.currentfb_x10A = value;
}

void hy_set_voltagefb1_x10V(uint16_t value){
	   LOG_INFO_TAG(HY_LOG_TAG,"get voltage [%d]x0.1",value);
    hy_instance->inputsignal.voltagefb1_x10V = value;
}

void hy_set_currentfb1_x10A(uint16_t value){

    hy_instance->inputsignal.currentfb1_x10A = value;
}

void hy_set_voltagefb2_x10V(uint16_t value){
	   LOG_INFO_TAG(HY_LOG_TAG,"get voltage [%d]x0.1",value);
    hy_instance->inputsignal.voltagefb2_x10V = value;
}

void hy_set_currentfb2_x10A(uint16_t value){

    hy_instance->inputsignal.currentfb2_x10A = value;
}
/*ADC TODO use more effecient method !!!!*/
uint16_t hy_get_voltagefb_x10V(void){
	  //LOG_INFO_TAG(HY_LOG_TAG,"get voltage [%d]x0.1 ",hy_instance->inputsignal.voltagefb_x10V);
		hy_can_query_YRKcharger();
    return hy_instance->inputsignal.voltagefb_x10V;
}

uint16_t hy_get_currentfb_x10A(void){

	  //hy_instance->inputsignal.currentfb_x10A = hy_instance->inputsignal.currentfb1_x10A+hy_instance->inputsignal.currentfb2_x10A;
    return hy_instance->inputsignal.currentfb_x10A;
}



