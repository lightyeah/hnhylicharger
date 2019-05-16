/****************************************  File Info  ****************************************************
** File name:               output.h
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Vraliens
** Created date:            2018-3-15
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef  __OUTPUT_H__
#define  __OUTPUT_H__

#define  SW_1             1
#define  SW_2             2
#define  SW_3             3
#define  SW_4             4
#define	 SW_5             5
#define  SW_6             6
typedef enum
{
	SCR1, 
	SCR2, 
	SCR3, 
	SCR4,
	SCR5,
	SCR6
}TRIGGLE_PORT;

void Output_Init(void);

void Output_Triggle_Set(uint8_t sw);
void Output_Triggle_Clear(uint8_t sw);

void Output_Run_Relay(uint8_t status);
void Output_Fault_Relay(uint8_t status);

void Output_RIT_Init(void);
void Output_Open_RIT(void);
void Output_Open_RIT_REAL(void);
void Output_Close_RIT(void);
void Output_Close_RIT_REAL(void);
void RIT_IRQHandler(void);

#endif
