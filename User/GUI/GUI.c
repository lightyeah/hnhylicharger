 #include "GUI_page.h"
 #include "GUI_driver.h"
 #include "string.h"
// #include "debug_frmwrk.h"
// #include "config.h"
// #include "systen_delay.h"
// #include "dataprocess.h"

#include "stdint.h"
#include "string.h"


#include "hy_instance.h"
#include "hy_dbg.h"

#define HY_LOG_TAG    "gui"

hy_instance_t* hy_instance = NULL;
hy_gui_t* s_gui = NULL;

int hy_gui_init(void* hy_instance_handle)
{
	
	hy_instance = (hy_instance_t*)hy_instance_handle;
	s_gui = &(hy_instance->gui);
	
	hy_button_init(s_gui);
	hy_led_init(s_gui);
	
	s_gui->chargetask_flag = NO_MSG;
	s_gui->button_flag = NO_MSG;
	s_gui->end_flag = NO_MSG;
	s_gui->err_flag = NO_MSG;
	
	s_gui->controlstyle = HY_CONTROLSTYLE_CAN;//hy_instance->config.controlstyle;
	s_gui->lastfreshtime_ms = 0;
	s_gui->set_in_flash = HY_FALSE;
	s_gui->machine_stop_flag = HY_FALSE;

	hy_gui_page_init(s_gui,&(hy_instance->config));
	
	LCD_Configuration();
	
	lcdreset();   

	LOG_INFO_TAG(HY_LOG_TAG,"hy gui init done!!");
	return HY_OK;
}


void hy_gui_refresh(void)
{
	while(systime_elapse_ms(s_gui->lastfreshtime_ms)<HY_GUI_REFRESH_INTERVAL);
	s_gui->lastfreshtime_ms = hy_time_now_ms();
	
}

void hy_button_task(void)
{
	static button_gui_msg msg;
	switch(s_gui->button.state){
		case pressed_none:
			if (hy_button_check(button_off)){
				s_gui->button.state = off_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_off;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [off] pressed!");

			}else if(hy_button_check(button_on)){
				s_gui->button.state = on_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_on;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [on] pressed!");

			}else if(hy_button_check(button_esc)){
				s_gui->button.state = esc_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_esc;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [esc] pressed!");
				
			}else if(hy_button_check(button_down)){
			
				s_gui->button.state = down_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_down;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [down] pressed!");
				
			}else if(hy_button_check(button_up)){
		
				s_gui->button.state = up_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_up;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [up] pressed!");
				
			}else if(hy_button_check(button_set)){
				s_gui->button.state = set_pressed;
				s_gui->button.pressed_time_ms = hy_time_now_ms();
				s_gui->button.work_state = HY_BUTTON_NOWORK;
				s_gui->button.name = button_set;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [set] pressed!");
				
			}
		break;

		case off_pressed:
			if(hy_button_check(button_off)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_off;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [off] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [off] loosen!");
			}
		break;

		case on_pressed:
			if(hy_button_check(button_on)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_on;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [on] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [on] loosen!");
			}
		break;

		case esc_pressed:
			if(hy_button_check(button_esc)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_esc;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [esc] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [esc] loosen!");
			}
		break;

		case down_pressed:
			if(hy_button_check(button_down)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_down;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [down] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [down] loosen!");
			}
		break;

		case up_pressed:
			if(hy_button_check(button_up)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_up;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [up] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [up] loosen!");
			}
		break;

		case set_pressed:
			if(hy_button_check(button_set)){
				if((systime_elapse_ms(s_gui->button.pressed_time_ms) >= HY_PRESSED_VALID_TIME_MS)
					&& (s_gui->button.work_state == HY_BUTTON_NOWORK)){
					s_gui->button.work_state = HY_BUTTON_WORKED;
					msg.button_name = button_set;
					msg.work_state = HY_BUTTON_NOWORK;	
					hy_emit_gui_msg(BUTTON_MSG,&msg);
					LOG_INFO_TAG(HY_LOG_TAG,"gui button [set] worked!");
				}
			}else{/*loosen button */
				s_gui->button.state = pressed_none;
				s_gui->button.name = button_default;
				LOG_INFO_TAG(HY_LOG_TAG,"gui button [set] loosen!");
			}
		break;

	}

}

int hy_gui_handle_button_msg(button_gui_msg* msg)
{
	int idx;
	for(idx = (HY_BUTTON_QUEUE_MAX-1);idx > 0;idx --){
		s_gui->button_msg_queue[idx].button_name = s_gui->button_msg_queue[idx-1].button_name;
	}
	s_gui->button_msg_queue[0].button_name = ((button_gui_msg*)msg)->button_name;
	s_gui->button_msg_queue[0].work_state = HY_BUTTON_NOWORK;
	return HY_OK;
}

int hy_emit_gui_msg(hy_gui_msg_type type, void* msg)
{
	switch (type){
		case NO_MSG:
			break;
		case CHARGETASK_MSG:
			s_gui->chargetask_flag = CHARGETASK_MSG;
			s_gui->charge2gui_msg.currentx10A = 
						((chargetask_gui_msg*)msg)->currentx10A;
			s_gui->charge2gui_msg.voltagex10V = 
						((chargetask_gui_msg*)msg)->voltagex10V;
			s_gui->charge2gui_msg.chargetime_min = 
						((chargetask_gui_msg*)msg)->chargetime_min;
		
		    s_gui->charge2gui_msg.charger_statu1 = 
		        ((chargetask_gui_msg*)msg)->charger_statu1;
		    s_gui->charge2gui_msg.charger_statu2 = 
		        ((chargetask_gui_msg*)msg)->charger_statu2;

			s_gui->charge2gui_msg.charge_module_connected = 
		        ((chargetask_gui_msg*)msg)->charge_module_connected;
		    s_gui->charge2gui_msg.battery_connected = 
		        ((chargetask_gui_msg*)msg)->battery_connected;
			s_gui->charge2gui_msg.bms_connected = 
		        ((chargetask_gui_msg*)msg)->bms_connected;

			
			s_gui->charge2gui_msg.errorstate = 
						   ((chargetask_gui_msg*)msg)->errorstate;
			s_gui->charge2gui_msg.workstate = 
						   ((chargetask_gui_msg*)msg)->workstate;

				
			break;
		case BUTTON_MSG:
			s_gui->button_flag = BUTTON_MSG;
			hy_gui_handle_button_msg((button_gui_msg*)msg);
			break;
		case ERR_MSG:
			
			break;
		default:
			break;
	}
	
	return HY_OK;
}


int hy_handle_gui_msg(void)
{
	return 0;
}

void MainProcess(void){	
	PAGE PageState;
  
	static uint32_t guivoltagex10V = 0;
	static uint32_t guicurrentx10A = 0;
  	static uint32_t guichargetime_min = 0;
	static uint32_t guichargetask_state = 0;
	
	
	if (hy_instance->configed_flag){
		PageState = WelcomePage;
	}else{
		PageState = ErrorPage2;
	}
	while(1){
// 		chargehandler();
		switch(PageState){
			case WelcomePage:
				PageState = welcomepage();
				break;
			case DisplayPage1:
				if (s_gui->set_in_flash){
					PageState = DisplayPage1_3;
					break;
				}
				if (s_gui->machine_stop_flag){
					PageState = DisplayPage1_1;
					break;
				}
				if(s_gui->chargetask_flag == CHARGETASK_MSG){
					guivoltagex10V = s_gui->charge2gui_msg.voltagex10V;
					guicurrentx10A = s_gui->charge2gui_msg.currentx10A;
					guichargetime_min = s_gui->charge2gui_msg.chargetime_min;
					set_gwcharger_status(s_gui->charge2gui_msg.charger_statu1,
										 s_gui->charge2gui_msg.charger_statu2);
					guichargetask_state = s_gui->charge2gui_msg.state;
					s_gui->chargetask_flag = NO_MSG;
				}

				PageState = displaypage1(&s_gui->charge2gui_msg,guivoltagex10V,guicurrentx10A,guichargetime_min);//conmunication state unknown

				hy_gui_refresh();
				break;
			case DisplayPage1_1://停機中
				if(s_gui->chargetask_flag == CHARGETASK_MSG){
					guivoltagex10V = s_gui->charge2gui_msg.voltagex10V;
					guicurrentx10A = s_gui->charge2gui_msg.currentx10A;
					guichargetime_min = s_gui->charge2gui_msg.chargetime_min;
					guichargetask_state = s_gui->charge2gui_msg.state;
					s_gui->chargetask_flag = NO_MSG;
				}

				PageState = displaypage1_1(&s_gui->charge2gui_msg,guivoltagex10V,guicurrentx10A,guichargetime_min);//conmunication state unknown

				hy_gui_refresh();
				break;
			case DisplayPage1_2://開始充電
				PageState = displaypage1_2(DisplayPage1);
				break;
			case DisplayPage1_3:
				PageState = displaypage1_3(DisplayPage1);
				break;
			case PassportPage:
				PageState = passportpage();
				break;			

			case PassportPage1:
				PageState = passportpage1();
				break;

			case SettingMainPage1:
				PageState = settingmainpage(0,0);
				break;

			case SettingMainPage2:
				PageState = settingmainpage(1,0);
				break;

			case SettingPage11:
				PageState = settingpage11(SettingMainPage1);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage11 next PageState [%d]",PageState);
				break;

			case SettingPage21:
				PageState = settingpage21(SettingMainPage1);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage21 next PageState [%d]",PageState);
				break;

			case SettingPage31:
				PageState = settingpage31(SettingMainPage1);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage31 next PageState [%d]",PageState);
				break;
			case SettingPage4:
				PageState = settingpage4(SettingMainPage1);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage4 next PageState [%d]",PageState);
				break;
			case SettingPage5:
				PageState = settingpage5(SettingMainPage2);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage5 next PageState [%d]",PageState);
				break;
			case SettingPage6:
				PageState = settingpage6(SettingMainPage2);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage6 next PageState [%d]",PageState);
				break;
			case SettingPage7:
				PageState = settingpage7(SettingMainPage2);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage7 next PageState [%d]",PageState);
				break;
			case SettingPage12:
				PageState = settingpage12(SettingMainPage1);
				LOG_INFO_TAG(HY_LOG_TAG,"SettingPage12 next PageState [%d]",PageState);
				break;

			case SettingPage22:
				PageState = settingpage22(SettingMainPage1);
				break;
			
			case SettingPage32:
				PageState = settingpage32(SettingMainPage1);
				break;
			
			case ErrorPage1://未知错误
				PageState = errorpage1();
				break;

			case ErrorPage2://参数配置错误
				PageState = errorpage2();
				break;
			case ErrorPage4://高温状态
				errorpage4(DisplayPage1);
				break;
			case ErrorPage5://电流错误
				errorpage5(ErrorPage5);
				break;
			case ErrorPage6://电压错误
				errorpage6(ErrorPage6);
				break;
			default:
				PageState = errorpage1();
				break;
				
		}
	}	

}



