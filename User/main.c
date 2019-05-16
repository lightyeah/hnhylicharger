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


void GPIO_Configuration(void);
/*In <debug_frmwrk. H >file can choose to a serial port*/

int main(void)
{
	#ifdef TEST_0623
		long long int i = 50000000;
		while(i--);
	#endif

	delay_init();
	debug_frmwrk_init();
	DEBUG_PRINT("[I]:system start \r\n");
	
	
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
