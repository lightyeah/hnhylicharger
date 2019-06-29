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
	DisplayPage1_1,//stop page  状态：停止充电
	DisplayPage1_2,//start page 状态：开始充电
	DisplayPage1_3,//end page 状态：充电完成
	DisplayPage1_4,//查询上一次充电成功 状态->时间
	DisplayPage2,//3
	PassportPage,//4
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
} PAGE;


int hy_gui_page_init(void* gui_handle);

void show(void);

PAGE welcomepage(void);

PAGE displaypage1(uint32_t state,
	uint32_t vol,
	uint32_t cur,
	uint32_t time);

PAGE displaypage2(uint8_t mode,uint16_t no,uint16_t vol,uint8_t status,uint8_t * flag);
	
PAGE passportpage(void); 

PAGE settingmainpage(uint8_t page,uint8_t cursor);
PAGE settingpage1(uint8_t page);
PAGE settingpage2(uint8_t page);
PAGE settingpage3(uint8_t page);
PAGE settingpage4(void);
PAGE settingpage5(void);
PAGE settingpage6(void);
PAGE settingpage7(void);
PAGE datasettingpage(uint8_t ID);
PAGE modesettingpage(void);
PAGE communicationratesettingpage(void);
PAGE balancesettingpage(void);
PAGE errorpage(uint8_t err);
PAGE finishpage(uint32_t vol,uint32_t time);
PAGE finishpage2(uint32_t vol,uint32_t time,uint8_t * flag);
#endif
