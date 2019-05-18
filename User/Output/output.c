//#include "stdint.h"
//#include "output.h"
//#include "lpc17xx_gpio.h"
//#include "lpc17xx_pinsel.h"
////luoyang
//#include "dataprocess.h"


///*********************************************************************
// * @Function    Output_Init
// * @brief       Init the SCRx output and the run relay output and fault 
//                relay output
// * @param[in]   None
// * @return      None
// **********************************************************************/
//void Output_Init(void){
//    PINSEL_CFG_Type PinCfg;
//    
//    //Init the SCRx output
//    PinCfg.Funcnum = 0;
//    PinCfg.OpenDrain = 0;
//    PinCfg.Pinmode = 0;

//    //No.59 SCR5 P0[19]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 19;
//    PINSEL_ConfigPin(&PinCfg);
//    
//    //No.60 SCR3 P0[18]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 18;
//    PINSEL_ConfigPin(&PinCfg);

//    //No.61 SCR1 P0[17]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 17;
//    PINSEL_ConfigPin(&PinCfg);

//    //No.62 SCR2 P0[15]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 15;
//    PINSEL_ConfigPin(&PinCfg);
//    
//    //No.63 SCR6 P0[16]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 16;
//    PINSEL_ConfigPin(&PinCfg);
//    
//    //No.64 SCR4 P2[9]
//    PinCfg.Portnum = 2;
//    PinCfg.Pinnum = 9;
//    PINSEL_ConfigPin(&PinCfg);
//		
//    
//    //Defined as output
//    GPIO_SetDir(0, (1<<16)|(1<<17)|(1<<15)|(1<<18)|(1<<19),1);
//    GPIO_SetDir(2, (1<<9),1);
//    
//    //Init the value
//    GPIO_SetValue(0, (1<<16)|(1<<17)|(1<<15)|(1<<18)|(1<<19));
//		GPIO_SetValue(2, (1<<9));
//    
//    //Init the Run-Relay output
//    //No.57 P0[21]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 21;
//    PINSEL_ConfigPin(&PinCfg);

//    //Init the Fault-Relay output
//    //No.56 P0[22]
//    PinCfg.Portnum = 0;
//    PinCfg.Pinnum = 22;
//    PINSEL_ConfigPin(&PinCfg);
//    
//    //Defined as output
//    GPIO_SetDir(0, (1<<21)|(1<<22),1);
//    
//    //Init the value
//    GPIO_ClearValue(0, (1<<21)|(1<<22));
//		
//}

///*********************************************************************
// * @Function    Output_Triggle_Set
// * @brief       Set the SCRx output as low level, 
//								and it will pass a 'not' logic.
// * @param[in]   SCRx: Should be SCR1/SCR2/SCR3/SCR4/SCR5/SCR6
// * @return      None
// **********************************************************************/
//void Output_Triggle_Set(uint8_t sw){
//    switch (sw){
//       case SW_1:
//         GPIO_ClearValue(0,(1<<17));
//         break;
//       case SW_2:
//         GPIO_ClearValue(0,(1<<15));
//         break;
//       case SW_3:
//         GPIO_ClearValue(0,(1<<18));
//         break;
//       case SW_4:
//         GPIO_ClearValue(2,(1<<9));
//         break;
//       case SW_5:
//         GPIO_ClearValue(0,(1<<19));
//         break;
//       case SW_6:
//         GPIO_ClearValue(0,(1<<16));
//         break;
//       default:
//         break;
//    }
//}

///*********************************************************************
// * @Function    Output_Triggle_Clear
// * @brief       Set the SCRx output as high level, 
//								and it will pass a 'not' logic.
// * @param[in]   SCRx: Should be SCR1/SCR2/SCR3/SCR4/SCR5/SCR6
// * @return      None
// **********************************************************************/
//void Output_Triggle_Clear(uint8_t sw){
//		switch (sw){
//       case SW_1:
//         GPIO_SetValue(0,(1<<17));
//         break;
//       case SW_2:
//         GPIO_SetValue(0,(1<<15));
//         break;
//       case SW_3:
//         GPIO_SetValue(0,(1<<18));
//         break;
//       case SW_4:
//         GPIO_SetValue(2,(1<<9));
//         break;
//       case SW_5:
//         GPIO_SetValue(0,(1<<19));
//         break;
//       case SW_6:
//         GPIO_SetValue(0,(1<<16));
//         break;
//       default:
//         break;
//    }
//}   

///*********************************************************************
// * @Function    Output_Run_Relay
// * @brief       Set the Run_Relay output level
// * @param[in]   status: 1 : output high level
//                        0 : output low level
// * @return      None 
// **********************************************************************/
//void Output_Run_Relay(uint8_t status){
//    if (status){    
//        GPIO_SetValue(0, (1<<22));
//    }
//    else{
//        GPIO_ClearValue(0, (1<<22));
//    }
//}

///*********************************************************************
// * @Function    Output_Fault_Relay
// * @brief       Set the Fault_Relay output level
// * @param[in]   status: 1 : output high level
//                        0 : output low level
// * @return      None 
// **********************************************************************/
//void Output_Fault_Relay(uint8_t status){
//    if (status){    
//        GPIO_SetValue(0, (1<<21));

//    }
//    else{
//        GPIO_ClearValue(0, (1<<21));			
//    }
//		
//		//Output_Run_Relay(1);
//}


///*********************************************************************
// * @Function    Output_RIT_Init
// * @brief       Init 25us RIT, but not open it 
// * @param[in]   None
// * @return      None 
// **********************************************************************/
//void Output_RIT_Init(void){
//    RIT_Init(LPC_RIT);
//    RIT_Cmd(LPC_RIT,DISABLE);
//    RIT_TimerConfig(LPC_RIT, 10); //5*5us = 25us
//    //NVIC_SetPriorityGrouping(4);  
//    //NVIC_SetPriority(RIT_IRQn, 1);
//    NVIC_EnableIRQ(RIT_IRQn);
//}

///*********************************************************************
// * @Function    Output_Open_RIT
// * @brief       Close the 50us RIT 
// * @param[in]   None
// * @return      None 
// **********************************************************************/
//void Output_Open_RIT(void){
//	  gSWtimeFlag = FLAGREADY;
//}
//void Output_Open_RIT_REAL(void){
//    RIT_Cmd(LPC_RIT,ENABLE);
//}


///*********************************************************************
// * @Function    Output_Close_RIT
// * @brief       Close the 50us RIT 
// * @param[in]   None
// * @return      None 
// **********************************************************************/
//void Output_Close_RIT(void){
//	  gSWtimeFlag = FLAGNOTREADY;
//}
//void Output_Close_RIT_REAL(void){
//    RIT_Cmd(LPC_RIT,DISABLE);
//}

///*********************************************************************
// * @Function    RIT_IRQHandler
// * @brief       50us interrupt process 
// * @param[in]   None
// * @return      None 
// **********************************************************************/
//int tempcount;
//void RIT_IRQHandler(void){
//    int i = 0;
//	//gSystemTime++;
//    gchargetime_us_count++;
//  //if(gSWtimeFlag!=FLAGREADY)return;

//	if(gswstartflag!=FLAGREADY){
//        return;
//    }
//    for (i = 0; i < 6; ++i)
//    {
//        tempcount = gchargetime_us_count - gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swopentime;
//        if(gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swopentime == gchargetime_us_count){//into count
//            Output_Triggle_Set(gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swnum);
//        }else if ((gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swopentime + 2 * RIT_1_MS_COUNT - 1) < gchargetime_us_count){//leave count
//            switchopenqueuepop(&gswitchopenqueue[i]);
//        }else if(tempcount > 0){//counting
//            if(tempcount % 2){
//                Output_Triggle_Clear(gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swnum);
//                if(gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swcount){
//                    gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swcount--;
//                }else{
//                    switchopenqueuepop(&gswitchopenqueue[i]);
//                }
//            }else{
//                Output_Triggle_Set(gswitchopenqueue[i].sw[gswitchopenqueue[i].queuehead].swnum);
//            }
//        }
//    }
//		
//			RIT_GetIntStatus(LPC_RIT);//clean IRQ flag
//// 	if(gchargetime_us_count==98)
//// 	{
//// 		Output_Triggle_Set(1);
//// 		Output_Triggle_Set(2);
//// 	}
//// 	else if(gchargetime_us_count==100)
//// 	{
//// 		Output_Triggle_Clear(1);
//// 		Output_Triggle_Clear(2);
//// 		gchargetime_us_count = 0;
//// 	}

//    //@luoyang
//// 		pulseclose(gSwgroupControl_1);
//// 	  pulseclose(gSwgroupControl_2);
//// 		//switch
//// 	  switchcontrol(gSwgroupControl_1);
//// 	  switchcontrol(gSwgroupControl_2);
//	  
//}
