#ifndef __GUI_H__
#define __GUI_H__

#include "stdint.h"

#define HY_BUTTON_PRESSED           1
#define HY_BUTTON_LOOSEN            0
#define HY_BUTTON_WORKED            0
#define HY_BUTTON_NOWORK            1
#define HY_BUTTON_QUEUE_MAX         6

#define HY_PRESSED_VALID_TIME_MS    50
#define HY_PRESSED_TIMEOUT_MS       1000

#define HY_GUI_REFRESH_INTERVAL     200/*unit ms*/

#define HY_GUI_BATTERY_ON_MASK          (1)


#define HY_GUI_CAN_ON_MASK              1//通讯充电中
#define HY_GUI_CHARGETASK_ON_MASK       2//本地充电中
#define HY_GUI_CHARGETASK_END_MASK      3//充电完成
#define HY_GUI_CHARGETASK_STOP_MASK     4//充电停止
#define HY_GUI_CHARGETASK_IDLE          5//空闲状态


#define HY_GUI_ERR_MASK                 12
#define HY_GUI_ERR_OVERHEAT_MASK        13
#define HY_GUI_ERR_CURRENT_MASK         14
#define HY_GUI_ERR_VOLTAGE_MASK         15

typedef enum Hy_Gui_Msg_Type{
	NO_MSG = 0,
	CHARGETASK_MSG,
	BUTTON_MSG,
	END_MSG,
	ERR_MSG
}hy_gui_msg_type;

typedef enum Hy_Button_Pressed_state
{
	pressed_none = 0,
	off_pressed,
	on_pressed,
	esc_pressed,
	down_pressed,
	up_pressed,
	set_pressed
}hy_pressed_state;

typedef enum Hy_Button_Name
{
	button_default = 0,
	button_off,
	button_on,
	button_esc,
	button_down,
	button_up,
	button_set
}hy_button_name;

typedef enum HY_LED_NAME
{
	led_running = 0,
	led_fullcharge,
	led_err,
	led_offall
}hy_led_name;

typedef struct Hy_Button_Struct{
	hy_pressed_state state;
	hy_button_name name;
	uint32_t pressed_time_ms;
	int work_state;
}hy_button_t;


typedef struct Chargetask_Gui_Msg{
	uint32_t voltagex10V;
	uint32_t currentx10A;
	uint32_t chargetime_min;
	
	uint32_t state;//deprecated

	uint8_t workstate;
	uint8_t errorstate;
	
	uint8_t charger_statu1;
	uint8_t charger_statu2;

	uint8_t bms_status;
	uint8_t bms_soc;
	
	uint8_t charge_module_connected;
	uint8_t battery_connected;
	uint8_t bms_connected;
}chargetask_gui_msg;

typedef struct Button_Gui_Msg
{
	hy_button_name button_name;
	int work_state;
}button_gui_msg;

typedef struct Hy_Gui_Struct{
	hy_button_t button;

	hy_gui_msg_type chargetask_flag;
	hy_gui_msg_type button_flag;
	hy_gui_msg_type end_flag;
	hy_gui_msg_type err_flag;

	chargetask_gui_msg charge2gui_msg;
	button_gui_msg button_msg_queue[HY_BUTTON_QUEUE_MAX];

	uint8_t controlstyle;

	uint32_t lastfreshtime_ms;

	int8_t set_in_flash;
	int8_t machine_stop_flag;
}hy_gui_t;




int hy_gui_init(void* hy_instance_handle);

void hy_gui_refresh(void);
void hy_button_task(void);

int hy_emit_gui_msg(hy_gui_msg_type type, void* msg);

int hy_gui_handle_button_msg(button_gui_msg* msg);


void MainProcess(void);
#endif
