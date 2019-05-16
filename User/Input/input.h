/****************************************  File Info  ****************************************************
** File name:               input.h
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Vraliens
** Created date:            2018-3-16
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

#ifndef  __INPUT_H__
#define  __INPUT_H__

void Input_Init_ENTx(void);
void Input_Open_ENTx(void);
void Input_Close_ENTx(void);
uint8_t Input_Get_ENTx(void);
uint8_t Input_Get_ENT_1_UV(void);
uint8_t Input_Get_ENT_2_WU(void);
uint8_t Input_Get_ENT_3_VW(void);
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

void Input_Init_CTRL(void);
uint8_t Input_Start_Relay(void);
uint8_t Input_Stop_Relay(void);
uint8_t Input_Is_Emergency(void);


void Input_Init_Sample(void);
uint16_t Input_Get_SampleV(void);
uint16_t Input_Get_SampleI(void);



#endif
