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
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_hy_output = &(hy_instance->output);
	
	/*software init*/

	memset(s_hy_output,0,sizeof(hy_output_t));
	

	LOG_INFO_TAG(HY_LOG_TAG,"hy init dac done!");
	
	return ret;
}

int hy_set_start_output(void){
	hy_can_start_charger();
	return 0;
}

int hy_set_stop_output(void)
{
	hy_can_stop_charger();
	return 0;
}

int hy_set_charger_output(uint16_t voltage_x10V, uint16_t current_x10A){
	hy_can_control_set_charger((uint32_t)voltage_x10V, (uint32_t)current_x10A/4);//4台设备
	return 0;
}

int hy_set_data_broadcast_to_bms(uint16_t voltage_x10V, uint16_t current_x10A)
{
	uint8_t status=0;
	hy_can_set_output_msg(voltage_x10V, current_x10A);
	if(hy_get_charger_module_statu1()&0x80){//硬件故障
		status |= (1<<0);
		}
	if(hy_get_charger_module_statu1()&(1<<6)){//过温关机
		status |= (1<<1);
		}
	if(hy_get_charger_module_statu1()&(1<<0)){//输入欠压
		status != (1<<2);
		}
	if(hy_get_charger_module_statu2()&(1<<1)){//电池接反
		status != (1<<3);
		}
	if(hy_get_bms_connected()==HY_FALSE){//BMS 通信超时
		status |= (1<<4);
		}
	hy_can_set_status_msg(status);
	return 0;
}



/****add for relay 20190831 end*****/


