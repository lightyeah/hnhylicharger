#ifndef __GUIPAGE_H__
#define __GUIPAGE_H__

#include "stdint.h"


typedef enum PAGE
{
	WelcomePage = 1,
	/*预备界面*/
	DisplayPage0,/*初始化中*/
	DisplayPage0_1,/*读取配置错误*/
	/*main page
	*  充电电压：xxxV
	*  充电电流：xxxA
	*  充电时间：xxxmin
	*  状态： ********
	*/
	DisplayPage1,
	DisplayPage1_1,//stop page  停机中
	DisplayPage1_2,//start page 开始充电
	DisplayPage1_3,//设置完出厂参数请重启
	DisplayPage1_4,//查询上一次充电成功 状态->时间
	DisplayPage2,//3
	PassportPage,//4
	PassportPage1,//请先停止充电再进行设置
	SettingMainPage1,//5
	SettingMainPage2,//6
	SettingPage11,//7
	SettingPage21,//8
	SettingPage31,//9
	SettingPage4,//10
	SettingPage5,//11
	SettingPage6,//12
	SettingPage7,//13
	SettingPage12,//14
	SettingPage22,//15
	SettingPage32,//16
	ErrorPage,//17
	ErrorPage1,
	ErrorPage2,//没有进行配置
} PAGE;


typedef enum Data_Name
{
    voltagerange = 0,
	currentrange,
	balancecurrent,
	controlstyle,
	chargecurrent_1,
	limitvoltage_1,
	chargetimeout_1_min,
	switchvoltage_1,	
	chargecurrent_2,
	limitvoltage_2,
	chargetimeout_2_min,
	switchvoltage_2,
	chargevoltage_3,
	limitcurrent_3,
	chargetimeout_3_min,
	switchcurrent_3,
	communicaterate,
} data_name;

int hy_gui_page_init(void* gui_handle, void* config_handle);

void show(void);

PAGE welcomepage(void);

PAGE displaypage1(uint32_t state,
	uint32_t vol,
	uint32_t cur,
	uint32_t time);

PAGE displaypage1_1(uint32_t state,
	uint32_t vol,
	uint32_t cur,
	uint32_t time);
PAGE displaypage1_2(PAGE father_page);
PAGE displaypage1_3(PAGE father_page);

PAGE displaypage2(uint8_t mode,uint16_t no,uint16_t vol,uint8_t status,uint8_t * flag);
	

PAGE passportpage(void); 
PAGE passportpage1(void);

PAGE settingmainpage(uint8_t page,uint8_t cursor);

PAGE settingpage11(PAGE father_page);
PAGE settingpage12(PAGE father_page);

PAGE settingpage21(PAGE father_page);
PAGE settingpage22(PAGE father_page);

PAGE settingpage31(PAGE father_page);
PAGE settingpage32(PAGE father_page);

PAGE settingpage4(PAGE father_page);
PAGE settingpage5(PAGE father_page);
PAGE settingpage6(PAGE father_page);
PAGE settingpage7(PAGE father_page);

PAGE datasettingpage(data_name name,PAGE page);

PAGE modesettingpage(void);
PAGE communicationratesettingpage(void);
PAGE balancesettingpage(void);

PAGE errorpage(uint8_t err);
PAGE errorpage1(void);
PAGE errorpage2(void);
PAGE errorpage3(PAGE father_page);

PAGE finishpage(uint32_t vol,uint32_t time);
PAGE finishpage2(uint32_t vol,uint32_t time,uint8_t * flag);
#endif
