/**********************************************************************
* $Id$		pca8581_test.c  				2010-05-21
*//**
* @file		pca8581_test.c
* @brief	An example of I2C using polling mode to test the I2C driver.
* 			Using EEPROM PCA8581 to transfer a number of data byte.
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/
#include "lpc17xx_i2c.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "lpc17xx_gpio.h"
#include "dataprocess.h"
#include "config.h"
#include "GUI.h"
#include "stdbool.h"

#include "config.h"

#include "hydbg.h"
#include "hy_instance.h"

#define HY_LOG_TAG    "main"
/*In <debug_frmwrk. H >file can choose to a serial port*/


void printf_logo()
{
	LOG_PRINT("HH      HH       YY        YY \r\n");
	LOG_PRINT("HH      HH        YY      YY  \r\n");
	LOG_PRINT("HH      HH         YY    YY   \r\n");
	LOG_PRINT("HH      HH          YY  YY    \r\n");
	LOG_PRINT("HHHHHHHHHH           YYYY     \r\n");
	LOG_PRINT("HH      HH            YY      \r\n");
	LOG_PRINT("HH      HH            YY      \r\n");
	LOG_PRINT("HH      HH            YY      \r\n");
	LOG_PRINT("HH      HH            YY      \r\n");
	
	LOG_PRINT("BUILD TIME: %s || %s\r\n",__DATE__,__TIME__);
}

/*
* warning: this funciont only call once!!
*/
int hy_instance_get_config(hy_instance *hy_handle,void* arg)
{
	int ret = -1;
	hy_config_init();
	ret = hy_config_datareading();
	if( ret != 0 ){
		LOG_INFO_TAG(HY_LOG_TAG,"hy instance get config failed!! ret [%d]",ret);
		return ret;
	}
	LOG_PRINT("==================\r\n");
	hy_handle->config.voltagerange = hy_config_readvoltagerange();
	hy_handle->config.currentrange = hy_config_readcurrentrange();
	hy_handle->config.controlstyle = hy_config_readctrlstyle();
	hy_handle->config.balancecurrent = hy_config_readbalancecurrent();
	
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom voltagerange   [%d] ",hy_handle->config.voltagerange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom currentrange   [%d] ",hy_handle->config.currentrange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom ctrlstyle      [%d] 0 for can 1 for local",hy_handle->config.controlstyle);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom balancecurrent [%d] ",hy_handle->config.balancecurrent);
	LOG_PRINT("==================\r\n");
	hy_handle->config.chargecurrent_1 = hy_config_readchargecurrent_1();
	hy_handle->config.limitvoltage_1  = hy_config_readlimitvoltage_1();
	hy_handle->config.chargetimeout_1_min = hy_config_readchargetimeout_1();
	hy_handle->config.switchvoltage_1 = hy_config_readswitchvoltage_1();
	
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargecurrent_1   [%d] ",hy_handle->config.chargecurrent_1);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitvoltage_1    [%d] ",hy_handle->config.limitvoltage_1);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_1   [%d] min",hy_handle->config.chargetimeout_1_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchvoltage_1   [%d] ",hy_handle->config.switchvoltage_1);
	LOG_PRINT("==================\r\n");
	hy_handle->config.chargecurrent_2 = hy_config_readchargecurrent_2();
	hy_handle->config.limitvoltage_2  = hy_config_readlimitvoltage_2();
	hy_handle->config.chargetimeout_2_min = hy_config_readchargetimeout_2();
	hy_handle->config.switchvoltage_2 = hy_config_readswitchvoltage_2();
	
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargecurrent_2   [%d] ",hy_handle->config.chargecurrent_2);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitvoltage_2    [%d] ",hy_handle->config.limitvoltage_2);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_2   [%d] min",hy_handle->config.chargetimeout_2_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchvoltage_2   [%d] ",hy_handle->config.switchvoltage_2);
  LOG_PRINT("==================\r\n");
	
	hy_handle->config.chargevoltage_3 = hy_config_readchargevoltage_3();
	hy_handle->config.limitcurrent_3  = hy_config_readlimitcurrent_3();
	hy_handle->config.chargetimeout_3_min = hy_config_readchargetimeout_3();
	hy_handle->config.switchcurrent_3 = hy_config_readswitchcurrent_3();
	
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargevoltage_3   [%d] ",hy_handle->config.limitcurrent_3);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitcurrent_3    [%d] ",hy_handle->config.limitcurrent_3);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_3   [%d] min",hy_handle->config.chargetimeout_3_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchcurrent_3   [%d] ",hy_handle->config.switchcurrent_3);
  LOG_PRINT("==================\r\n");
	
	return 0;
}
#define delay_power
int main(void)
{
	int ret = 0;
	
	hy_instance s_hy_instance;
	
	#ifdef delay_power
		long long int i = 50000000;
		while(i--);
	#endif

	/*debug init*/
	debug_frmwrk_init();
	LOG_INFO_TAG(HY_LOG_TAG,"HY system start %s\r\n");
	printf_logo();
	/*rom config init*/
	ret = hy_instance_get_config(&s_hy_instance,NULL);
	if(ret == 0){
	  s_hy_instance.configed_flag = true;
	}else{
		s_hy_instance.configed_flag = false;
	}
	/*todo input init*/
	
	/*todo can init if in can control mode*/
	
	
//	MainProcess();


  while(1);
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
	
}
#endif

/*
 * @}
 */
