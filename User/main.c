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

#include "config.h"
//#include "GUI.h"
#include "stdbool.h"

#include "config.h"


#include "hy_instance.h"
#include "hy_dbg.h"
#include "hy_can.h"
#include "hy_systime.h"

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
int hy_instance_get_config(hy_instance_t *hy_handle,void* arg)
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
	hy_handle->config.communicaterate = hy_config_readcommunicaterate();
	hy_handle->config.balancecurrent = hy_config_readbalancecurrent();
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom voltagerange   [%d] ",hy_handle->config.voltagerange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom currentrange   [%d] ",hy_handle->config.currentrange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom ctrlstyle      [%d] 0 for can 1 for local",hy_handle->config.controlstyle);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom canrate      [%d] ",hy_handle->config.communicaterate);
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
	static hy_instance_t s_hy_instance;
	#ifdef delay_power
		long long int i = 50000000;
		while(i--);
	#endif
	

	s_hy_instance.initdone = HY_INSTANCE_NOT_INITDONE;

	/*init*/
	/*debug init*/
	debug_frmwrk_init();
	/*systime init*/
	hy_systime_init(&s_hy_instance);
	LOG_INFO_TAG(HY_LOG_TAG,"HY system start\r\n");
	printf_logo();
	

	
	/*rom config init*/
	ret = hy_instance_get_config(&s_hy_instance,NULL);
	if(ret == 0){
	  s_hy_instance.configed_flag = true;
	}else{
		s_hy_instance.configed_flag = false;
		LOG_ERROR_TAG(HY_LOG_TAG,"***get config failed!!!");
	}
	/*can init if in can control mode*/
	if(s_hy_instance.config.controlstyle == HY_CONTROLSTYLE_CAN &&
			s_hy_instance.configed_flag){
				hy_can_init(&s_hy_instance);
	}
	
	/*for test api*/
	s_hy_instance.config.controlstyle = HY_CONTROLSTYLE_LOCAL;
	
	s_hy_instance.config.chargecurrent_1 = 30;
	s_hy_instance.config.limitvoltage_1 = 60;
	s_hy_instance.config.chargetimeout_1_min = 15;
	s_hy_instance.config.switchvoltage_1 = 30;
	
	s_hy_instance.config.chargecurrent_2 = 90;
	s_hy_instance.config.limitvoltage_2 = 60;
	s_hy_instance.config.chargetimeout_2_min = 15; 
	s_hy_instance.config.switchvoltage_2 = 56;
	
	s_hy_instance.config.chargevoltage_3 = 59;
	s_hy_instance.config.limitcurrent_3 = 60;
	s_hy_instance.config.chargetimeout_3_min = 15; 
	s_hy_instance.config.switchcurrent_3 = 10;
	
	s_hy_instance.config.voltagerange = 68;
	s_hy_instance.config.currentrange = 100;
	
	LOG_PRINT("==================test\r\n");
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom voltagerange   [%d] ",s_hy_instance.config.voltagerange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom currentrange   [%d] ",s_hy_instance.config.currentrange);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom ctrlstyle      [%d] 0 for can 1 for local",s_hy_instance.config.controlstyle);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom canrate      [%d] ",s_hy_instance.config.communicaterate);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom balancecurrent [%d] ",s_hy_instance.config.balancecurrent);
	LOG_PRINT("==================tset\r\n");
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargecurrent_1   [%d] ",s_hy_instance.config.chargecurrent_1);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitvoltage_1    [%d] ",s_hy_instance.config.limitvoltage_1);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_1   [%d] min",s_hy_instance.config.chargetimeout_1_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchvoltage_1   [%d] ",s_hy_instance.config.switchvoltage_1);
	LOG_PRINT("==================test\r\n");
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargecurrent_2   [%d] ",s_hy_instance.config.chargecurrent_2);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitvoltage_2    [%d] ",s_hy_instance.config.limitvoltage_2);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_2   [%d] min",s_hy_instance.config.chargetimeout_2_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchvoltage_2   [%d] ",s_hy_instance.config.switchvoltage_2);
  LOG_PRINT("==================test\r\n");
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargevoltage_3   [%d] ",s_hy_instance.config.limitcurrent_3);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom limitcurrent_3    [%d] ",s_hy_instance.config.limitcurrent_3);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom chargetimeout_3   [%d] min",s_hy_instance.config.chargetimeout_3_min);
	LOG_INFO_TAG(HY_LOG_TAG,"instance get config in rom switchcurrent_3   [%d] ",s_hy_instance.config.switchcurrent_3);
  LOG_PRINT("==================\r\n");
	
	/*input init*/
	hy_input_init(&s_hy_instance);
	/*output init*/
	hy_output_init(&s_hy_instance);
//		while(1){
//		hy_set_output(100);
//		hy_can_send();
//		i=50000000;
//		while(i--);
//		hy_get_voltagefb_x10V();
//		hy_get_currentfb_x10A();
//		LOG_PRINT("==================\r\n");
//	}
	
		hy_chargetask_init(&s_hy_instance);
		/*gui_init*/
		hy_gui_init(&s_hy_instance);
		delay_ms(1000);
	
		
		s_hy_instance.initdone = HY_INSTANCE_INITDONE;

		MainProcess();


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
