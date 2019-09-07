#include "stdint.h"
#include "string.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_gpdma.h"
#include "debug_frmwrk.h"
#include "config.h"

#include "hy_instance.h"


#define HY_LOG_TAG "output"

/** DMA size of transfer */
#define DMA_SIZE		1
// Terminal Counter flag for Channel 0
uint32_t Channel0_TC;

static hy_instance_t *hy_instance = NULL;
hy_output_t *s_hy_output;

GPDMA_Channel_CFG_Type GPDMACfg;

/************************** PRIVATE FUNCTION *************************/
void DMA_IRQHandler (void);


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		GPDMA interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void DMA_IRQHandler (void)
{
	// check GPDMA interrupt on channel 0
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0)){
		// Check counter terminal status
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)){
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
			Channel0_TC++;
		}
		// Check error terminal status
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)){
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);

		}
	}
}

int hy_output_init(void* hy_instance_handle)
{
	int ret = HY_OK;
	PINSEL_CFG_Type PinCfg;
	DAC_CONVERTER_CFG_Type DAC_ConverterConfigStruct;
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_hy_output = &(hy_instance->output);
	
	/*software init*/
	Channel0_TC = 0;
	memset(s_hy_output,0,sizeof(hy_output_t));
	s_hy_output->output_dac_value = 0;
	
	/*hardware init*/  
	/*
	demand:
1. relay 1 - 57_rd1_P0[21]  default down, work in up
2. relay 2 - 56_td1_p0[22]  default down, work in up
	*/
	PinCfg.Funcnum = 0;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_PULLDOWN;

  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 21;
  PINSEL_ConfigPin(&PinCfg);
	PinCfg.Portnum = 0;
  PinCfg.Pinnum = 22;
  PINSEL_ConfigPin(&PinCfg);
	
	GPIO_SetDir(0,(1<<21)|(1<<22),1);
	hy_output_set_charge_relay(HY_OUTPUT_RELAY_OFF);
	hy_output_set_error_relay(HY_OUTPUT_RELAY_OFF);
	
  /*dac gpio init*/
  PinCfg.Funcnum = 2;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_PULLDOWN;
  //No.6 P0[26] DAC AOUT
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 26;
  PINSEL_ConfigPin(&PinCfg);
    
	NVIC_DisableIRQ(DMA_IRQn);
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
	
	DAC_ConverterConfigStruct.CNT_ENA =SET;
	DAC_ConverterConfigStruct.DMA_ENA = SET;
	DAC_Init(LPC_DAC);
	/* set time out for DAC*/
	DAC_SetDMATimeOut(LPC_DAC,0x00);
	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_ConverterConfigStruct);
	/* Initialize GPDMA controller */
	GPDMA_Init();
	// Setup GPDMA channel --------------------------------
	// channel 0
	GPDMACfg.ChannelNum = 0;
	// Source memory
	GPDMACfg.SrcMemAddr = (uint32_t)(&(s_hy_output->output_dac_value));
	// Destination memory - unused
	GPDMACfg.DstMemAddr = 0;
	// Transfer size
	GPDMACfg.TransferSize = DMA_SIZE;
	// Transfer width - unused
	GPDMACfg.TransferWidth = 0;
	// Transfer type
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
	// Source connection - unused
	GPDMACfg.SrcConn = 0;
	// Destination connection
	GPDMACfg.DstConn = GPDMA_CONN_DAC;
	// Linker List Item - unused
	GPDMACfg.DMALLI = 0;
	// Setup channel with given parameter
	GPDMA_Setup(&GPDMACfg);
	
	/* Enable GPDMA interrupt */
	NVIC_EnableIRQ(DMA_IRQn);
	LOG_INFO_TAG(HY_LOG_TAG,"hy init dac done!");
	
	return ret;
}


int hy_set_output(uint32_t value)
{
	int ret = HY_OK;
	int timeout = 505000;
	if(s_hy_output == NULL){
		LOG_ERROR_TAG(HY_LOG_TAG,"***output not init!!");
		return HY_ERROR;
	}
	if(value <= 0){
		value = 0;
	}
	if (value >= CHARGETASK_MAX_DAC_OUTPUT_VALUE)
	{
		value = CHARGETASK_MAX_DAC_OUTPUT_VALUE;
	}
	
	GPDMA_ChannelCmd(0, ENABLE);	
	while(Channel0_TC == 0){
		timeout--;
		if(timeout==0){
//			LOG_ERROR_TAG(HY_LOG_TAG,"***set dac dma blocked!!!");
			break;
		}
	}
	GPDMA_ChannelCmd(0, DISABLE);

	s_hy_output->output_dac_value=value;
	//	s_hy_output->output_dac_value=1;
//	LOG_INFO_TAG(HY_LOG_TAG,"set output voltage [%d]",value);

	Channel0_TC = 0;
	GPDMA_Setup(&GPDMACfg);
//	LOG_INFO_TAG(HY_LOG_TAG,"set output voltage [%d] value [%d]",s_hy_output->output_dac_value,value);

	return ret;
}

/****add for relay 20190831 start*****/
/**
demand:
1. relay 1 - 57_rd1_P0[21]  for charge,default down, work in up
2. relay 2 - 56_td1_p0[22]  for error,default down, work in up

init in <hy_output_init>

*/

int hy_output_set_charge_relay(int state)
{
	if(state == HY_OUTPUT_RELAY_ON){
		GPIO_SetValue(0,(1<<21));
	}else if(state == HY_OUTPUT_RELAY_OFF){
		GPIO_ClearValue(0,(1<<21));
	}
	return 0;
}

int hy_output_set_error_relay(int state)
{
	if(state == HY_OUTPUT_RELAY_ON){
		GPIO_SetValue(0,(1<<22));
	}else if(state == HY_OUTPUT_RELAY_OFF){
		GPIO_ClearValue(0,(1<<22));
	}
	return 0;
}

/****add for relay 20190831 end*****/


