//#include "GUI_page.h"
//#include "GUI_driver.h"
//#include "debug_frmwrk.h"
//#include "config.h"
//#include "systen_delay.h"
//#include "dataprocess.h"



//#ifndef DEBUG_PRINT
//#define DEBUG_PRINT
//#define DEBUG_PRINT(format, ...) _printf (format,##__VA_ARGS__)
//#endif




//void GUIInit(){
//	LCD_Configuration();
//	Button_Init();
//	lcdreset();             
//}

//void DataInit(){
//	uint8_t ret;
//	Config_Init();
//	ret = Config_DataReading();
//	if(ret == 0){
//		DEBUG_PRINT("hhhhhhhhhhhhhhhhhhhhhh\r\n");	
//		Config_EraseConfig();
//		Config_DataReading();
//	}
//}

//void MainProcess(void){	
//	PAGE PageState;
//	__CMsg *guicmsg;
//	uint32_t guifeedbackvoltage,guifeedbackcurrent,guichargetime;
//	uint16_t ctrlstyle;
//	uint8_t guilocalstate,guierr,overheatFlag;
//	uint8_t CSwitch,CStatus;
//	uint8_t CANStatus,CANSwitch,CANConnectedflag;
//	
//	
//	overheatFlag = 0;
//	guifeedbackvoltage = 0;
//	guifeedbackcurrent=0;
//	guichargetime = 0;
//	guilocalstate = 0;
//	guierr = 0;
//	CSwitch = 0;
//	CStatus = 0;
//	CANStatus = 1;
//	CANSwitch = 0;
//	CANConnectedflag = 0;
// 
//	GUIInit();
//	DataInit();
//	delay_ms(1000);
//	PageState = WelcomePage;
//	while(1){
//// 		chargehandler();
//		switch(PageState){
//			case WelcomePage:
//				PageState = welcomepage();
//				break;
//			case DisplayPage1:
//				ctrlstyle = Config_ReadCtrlStyle();//can for 0 local for 1
//				//PAGE displaypage1(unsigned char status,unsigned int vol,unsigned int cur,unsigned int time,unsigned char * flag);
//				if(ctrlstyle == 1){
//					if((CSwitch == 0)&&(CStatus == 1)){
//						DEBUG_PRINT("[Y]Charge stop\r\n");
//						chargestop();
//						CStatus = 0;
//					}
//					else if((CSwitch == 1)&&(CStatus == 0)){
//						DEBUG_PRINT("[Y]Charge start\r\n");
//						chargestart();
//						CStatus = 1;
//					}
//					guicmsg = chargemsg();
//					if(guicmsg->updateflag == 0x01){
//						guifeedbackvoltage = guicmsg->feedbackvoltage;
//						guifeedbackcurrent = guicmsg->feedbackcurrent;
//						guichargetime = guicmsg->chargetime;
//						guilocalstate = guicmsg->localstate;
//						guierr = guicmsg->err;
//						guicmsg->updateflag = 0x00;
//						if(guierr != 0){
//	// 						chargestop();
//							DEBUG_PRINT("[Y]Receive error,enter errrorpage\r\n");
//							PageState = errorpage(guierr);
//							CSwitch = 0;
//							CStatus = 0;
//							DEBUG_PRINT("[Y]Charge error\r\n");
//							break;
//						}
//						if(guilocalstate == 24){
//							CSwitch = 0;
//							CStatus = 0;
//							PageState = finishpage(guifeedbackvoltage,guichargetime);
//							DEBUG_PRINT("[Y]charge end\r\n");
//							guifeedbackvoltage = 0;
//							guifeedbackcurrent = 0;
//							guichargetime = 0;
//						}
//					}
//					PageState = displaypage1(guierr,guifeedbackvoltage,guifeedbackcurrent,guichargetime,&CSwitch);//conmunication state unknown
//				}
//				else if(ctrlstyle == 0){			//CAN通讯充电
//					//Canstatus状态，在界面内按下ON会将其置为1,进入待机模式，按下OFF会将其置为0，进入停机模式
//					if((CANSwitch == 0)&&(CANStatus == 1)){					//待机状态下检查通讯重新建立的情况，开始充电
//						CANConnectedflag = canconnectedcheck();
//						if((CANConnectedflag == 1)&&(guierr != ERR_OVERHEAT)&&(guierr != ERR_OVERHEAT)){
//							guifeedbackvoltage = 0;
//							guifeedbackcurrent=0;
//							guichargetime = 0;
//							guilocalstate = 0;
//							DEBUG_PRINT("[Y]Charge startuuuyuu\r\n");
//							chargestart();
//							DEBUG_PRINT("[Y]Charge startffffff\r\n");
//							CANSwitch = 1;
//						}
//					}
//					else if((CANStatus == 0)&&(CANSwitch == 1)){//充电进行中，按下了OFF键
//						chargestop();
//						CANSwitch = 0;
//					}

//					
//					//更新msg
//					guicmsg = chargemsg();
//					if(guicmsg->updateflag == 0x01){
//						guifeedbackvoltage = guicmsg->feedbackvoltage;
//						guifeedbackcurrent = guicmsg->feedbackcurrent;
//						guichargetime = guicmsg->chargetime;
//						guilocalstate = guicmsg->localstate;
//						guierr = guicmsg->err;
//						guicmsg->updateflag = 0x00;
//						if(guilocalstate == CAN_DISCONNECTED){
//							CANSwitch = 0;
//						}
//						if(guierr == ERR_OVERHEAT){
//							//如果有错误，进入停机模式，需要按下ESC 和 ON之后才能正常工作
//							PageState = errorpage(guierr);
//							CANStatus = 0;
//							CANSwitch = 0;
//						}
//						else if(guierr == ERR_LACKPHASE){
//							//如果有错误，进入停机模式，需要按下ESC 和 ON之后才能正常工作
//							PageState = errorpage(guierr);
//							CANStatus = 0;
//							CANSwitch = 0;
//						}
//					}
//					
//					if(guilocalstate == CAN_FINISH){
//						//充电结束，提示充电结束 直到拔下插头
//						PageState = finishpage2(guifeedbackvoltage,guichargetime,&CANStatus);
//					}
//					else{
//						//平时显示对应状态
//						PageState = displaypage1(guierr,guifeedbackvoltage,guifeedbackcurrent,guichargetime,&CANStatus);//conmunication state unknown
//					}
//				}
//				break;
//				
//			case DisplayPage2:
//				//PAGE displaypage2(unsigned char mode,unsigned int no,unsigned int vol,unsigned char status,unsigned char * flag);
//				guicmsg = chargemsg();
//				if(guicmsg->updateflag == 0x01){
//					guifeedbackvoltage = guicmsg->feedbackvoltage;
//					guifeedbackcurrent = guicmsg->feedbackcurrent;
//					guichargetime = guicmsg->chargetime;
//					guilocalstate = guicmsg->localstate;
//					guierr = guicmsg->err;
//					guicmsg->updateflag = 0x00;

//					if(guierr != 0){
//						DEBUG_PRINT("[Y]Receive error,enter errrorpage\r\n");
//						PageState = errorpage(guierr);
//						CSwitch = 0;
//						CStatus = 0;
//						DEBUG_PRINT("[Y]Charge error\r\n");
//					}
//					if(guilocalstate == 12 ||guilocalstate == 24){
//						CSwitch = 0;
//						CStatus = 0;
//						PageState = finishpage(guifeedbackvoltage,guichargetime);
//						guifeedbackvoltage = 0;
//						guifeedbackcurrent = 0;
//						guichargetime = 0;

//					}
//				}
//				if(ctrlstyle == 1){
//					PageState = displaypage2(guilocalstate,1234,guifeedbackvoltage,guierr,&CSwitch);//battery number unknown
//				}
//				else if(ctrlstyle == 0){
//					PageState = displaypage2(guilocalstate,1234,guifeedbackvoltage,guierr,&CANStatus);//battery number unknown
//				}
//				break;
//			case PassportPage:
//				PageState = passportpage();
//				break;
//			case SettingMainPage1:
//				PageState = settingmainpage(0,0);
//				break;
//			case SettingMainPage2:
//				PageState = settingmainpage(1,0);
//				break;
//			case SettingPage11:
//				PageState = settingpage1(0);
//				break;
//			case SettingPage21:
//				PageState = settingpage2(0);
//				break;
//			case SettingPage31:
//				PageState = settingpage3(0);
//				break;
//			case SettingPage4:
//				PageState = settingpage4();
//				break;
//			case SettingPage5:
//				guifeedbackvoltage = 0;
//				guifeedbackcurrent=0;
//				guichargetime = 0;
//				guilocalstate = 0;
//				guierr = 0;
//				PageState = settingpage5();
//				break;
//			case SettingPage6:
//				PageState = settingpage6();
//				break;
//			case SettingPage7:
//				PageState = settingpage7();
//				break;
//			case SettingPage12:
//				PageState = settingpage1(1);
//				break;
//			case SettingPage22:
//				PageState = settingpage2(1);
//				break;
//			
//			case SettingPage32:
//				PageState = settingpage3(1);
//				break;
//			
//			default:
//				PageState = welcomepage();
//				break;
//				
//		}
//	}	


//	while(1);
//}

