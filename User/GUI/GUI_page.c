#include "GUI_page.h"
#include "GUI_driver.h"
#include "GUI.h"
#include "config.h"
#include "hy_instance.h"

#include "stdint.h"

#include "hy_dbg.h"
#include "hy_systime.h"

#define HY_LOG_TAG "GUI_PAGE"

#define FIRST_ADDR 0  

#define key_time 700

static hy_gui_t* s_gui = NULL;
static hy_config* config_data = NULL;

uint8_t hysy[9] = {0xbb,0xb6,0xd3,0xad,0xca,0xb9,0xd3,0xc3,0x00};//欢迎使用
uint8_t hypcdj[13] = {0xea,0xbb,0xd1,0xc7,0xc5,0xc6,0xb3,0xe4,0xb5,0xe7,0xbb,0xfa,0x00};//昊亚牌充电机
uint8_t dian[3] = {0xb5,0xe7,0x00};//电
uint8_t liu[3] ={0xc1,0xf7,0x00};//流
uint8_t ya[3] = {0xd1,0xb9,0x00};//压
uint8_t sj[5] = {0xca,0xb1,0xbc,0xe4,0x00};//时间
uint8_t kz[5] = {0xbf,0xd8,0xd6,0xc6,0x00};//控制
uint8_t fs[5] = {0xb7,0xbd,0xca,0xbd,0x00};//方式
uint8_t tx[5] = {0xcd,0xa8,0xd0,0xc5,0x00};//通信
uint8_t bd[5] = {0xb1,0xbe,0xb5,0xd8,0x00};//本地
uint8_t zc[5] = {0xd5,0xfd,0xb3,0xa3,0x00};//正常
uint8_t cw[5] = {0xb4,0xed,0xce,0xf3,0x00};//错误
uint8_t yx[5] = {0xd4,0xcb,0xd0,0xd0,0x00};//运行
uint8_t ms[5] = {0xc4,0xa3,0xca,0xbd,0x00};//模式
uint8_t tj[5] = {0xcd,0xa3,0xbb,0xfa,0x00};//停机
uint8_t yu[3] = {0xd4,0xa4,0x00};//预
uint8_t chong[3] = {0xb3,0xe4,0x00};//充
uint8_t heng[3] = {0xba,0xe3,0x00};//恒
uint8_t chi[3] = {0xb3,0xd8,0x00};//池
uint8_t bianh[5] = {0xb1,0xe0,0xba,0xc5,0x00};//编号
uint8_t zt[5] = {0xd7,0xb4,0xcc,0xac,0x00};//状态
uint8_t qx[5] = {0xc8,0xb1,0xcf,0xe0,0x00};//缺相
uint8_t gw[5] = {0xb8,0xdf,0xce,0xc2,0x00};//高温
uint8_t bh[5] = {0xb1,0xa3,0xbb,0xa4,0x00};//保护
uint8_t bz[5] = {0xb2,0xbd,0xd6,0xe8,0x00};//步骤
uint8_t yi[3] = {0xd2,0xbb,0x00};//一
uint8_t er[3] = {0xb6,0xfe,0x00};//二
uint8_t san[3] ={0xc8,0xfd,0x00};//三
uint8_t jb[5] = {0xbb,0xf9,0xb1,0xbe,0x00};//基本
uint8_t sz[5] = {0xc9,0xe8,0xd6,0xc3,0x00};//设置
uint8_t hf[5] = {0xbb,0xd6,0xb8,0xb4,0x00};//恢复
uint8_t cc[5] = {0xb3,0xf6,0xb3,0xa7,0x00};//出厂
uint8_t cs[5] = {0xb2,0xce,0xca,0xfd,0x00};//参数
uint8_t sd[5] = {0xca,0xd6,0xb6,0xaf,0x00};//手动
uint8_t cp[5] = {0xb2,0xfa,0xc6,0xb7,0x00};//产品
uint8_t bb[5] = {0xb0,0xe6,0xb1,0xbe,0x00};//版本
uint8_t xx[5] = {0xd0,0xc5,0xcf,0xa2,0x00};//信息
uint8_t jd[5] = {0xbd,0xd7,0xb6,0xce,0x00};//阶段
uint8_t gd[5] = {0xb8,0xf8,0xb6,0xa8,0x00};//给定
uint8_t xz[5] = {0xcf,0xde,0xd6,0xc6,0x00};//限制
uint8_t tz[5] = {0xcc,0xf8,0xd7,0xaa,0x00};//跳转
uint8_t kuai[3] = {0xbf,0xec,0x00};//快
uint8_t man[3] = {0xc2,0xfd,0x00};//慢
uint8_t lc[5] = {0xc1,0xbf,0xb3,0xcc,0x00};//量程
uint8_t sl[5] = {0xcb,0xd9,0xc2,0xca,0x00};//速率
uint8_t ph[5] = {0xc6,0xbd,0xba,0xe2,0x00};//平衡
uint8_t xh[5] = {0xd0,0xcd,0xba,0xc5,0x00};//型号
uint8_t rj[5] = {0xc8,0xed,0xbc,0xfe,0x00};//软件
uint8_t cj[5] = {0xb3,0xa7,0xbc,0xd2,0x00};//厂家
uint8_t tianj[5] = {0xcc,0xec,0xbd,0xf2,0x00};//天津
uint8_t hy[5] = {0xea,0xbb,0xd1,0xc7,0x00};//昊亚
uint8_t dh[5] = {0xb5,0xe7,0xbb,0xb0,0x00};//电话
uint8_t gz[5] = {0xb9,0xca,0xd5,0xcf,0x00};//故障
uint8_t jc[5] = {0xbc,0xec,0xb2,0xe2,0x00};//检测
uint8_t	wei[3] = {0xce,0xb4,0x00};
uint8_t wz[5] = {0xce,0xb4,0xd6,0xaa};//未知
uint8_t js[5] = {0xbd,0xe1,0xca,0xf8};//结束
uint8_t mmcw[9] = {0xc3,0xdc,0xc2,0xeb,0xb4,0xed,0xce,0xf3,0x00};//密码错误
uint8_t qsmm[9] ={0xc7,0xeb,0xca,0xe4,0xc3,0xdc,0xc2,0xeb,0x00}; 
uint8_t	lianjie[5] = {0xc1,0xac,0xbd,0xd3,0x00};//连接
uint8_t qing[3] = {0xc7,0xeb,0x00};//请
uint8_t xian[3] = {0xcf,0xc8,0x00};//先
uint8_t tingzhi[5] = {0xcd,0xa3,0xd6,0xb9,0x00};//停止
uint8_t zai[3] = {0xd4,0xd9,0x00};//再
uint8_t jinxing[5] = {0xbd,0xf8,0xd0,0xd0,0x00};//进行
uint8_t zhengque[5] = {0xd5,0xfd,0xc8,0xb7,0x00};//正确
uint8_t mima[5] = {0xc3,0xdc,0xc2,0xeb,0x00};//密码
uint8_t chongqi[5] = {0xd6,0xd8,0xc6,0xf4,0x00};//重启
uint8_t botelv[7] = {0xb2,0xa8,0xcc,0xd8,0xc2,0xca,0x00};//波特率

uint8_t pass[6] ={2,2,2,3,3,3};
uint8_t unit[17] = {'A','V','M','V','A','V','M','V','V','A','M','A',' ','V','A','K','%'};
uint16_t max[17] = {300,300,999,300,300,300,999,300,300,300,999,300,1,300,300,4,60};
uint16_t min[17] = {  1,  1,  1,	1,	1,	1,	1,	1,	1,	1,	1,	1,0,	1,	1,0, 0};
uint16_t communicationrate[5] = {50,125,250,500,800};

int hy_gui_page_init(void* gui_handle, void* config_handle)
{
	if(gui_handle==NULL){

		return HY_ERROR;
	}
	s_gui = (hy_gui_t*)gui_handle;
	config_data = (hy_config*)config_handle;
	return HY_OK;
}

// void show(){
// 	datasettingpage(3);
// }

void showmainsettingitem(uint8_t no){
	write_data(no+0x31);
	hy_delay_ms(1);
	write_data(0x2e);
	hy_delay_ms(1);
	switch(no){
		case 0:
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(bz);
			lcd_display_chinese(yi);		
			break;
		case 1:
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(bz);
			lcd_display_chinese(er);		
			break;
		case 2:
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(bz);
			lcd_display_chinese(san);		
			break;
		case 3:
			lcd_display_chinese(jb);
			lcd_display_chinese(cs);
			lcd_display_chinese(sz);		
			break;
		case 4:
			lcd_display_chinese(hf);
			lcd_display_chinese(cc);
			lcd_display_chinese(cs);		
			break;
		case 5:
			lcd_display_chinese(dian);
			lcd_display_chinese(liu);
			lcd_display_chinese(sd);
			lcd_display_chinese(ph);		
			break;
		case 6:
			lcd_display_chinese(cp);
			lcd_display_chinese(bb);
			lcd_display_chinese(xx);
			break;
		default:
			break;
	}
}

void showsettingitem(uint8_t page,uint8_t no){
	uint16_t data;
	if(no != 0){
		data = ConfigReadFromAddr(page*4+no-1);
		write_data(no+0x30);
		hy_delay_ms(1);
		write_data(0x2e);
		hy_delay_ms(1);
	}
	switch(no){
		case 0:
			switch(page){
				case 0:
					lcd_display_space();
					lcd_display_chinese(yu);
					lcd_display_chinese(chong);
					lcd_display_space();
					lcd_display_chinese(heng);
					lcd_display_chinese(liu);
					break;
				case 1:
					lcd_display_space();
					lcd_display_chinese(kuai);
					lcd_display_chinese(chong);
					lcd_display_space();
					lcd_display_chinese(heng);
					lcd_display_chinese(liu);
					break;
				case 2:
					lcd_display_space();
					lcd_display_chinese(man);
					lcd_display_chinese(chong);
					lcd_display_space();
					lcd_display_chinese(heng);
					lcd_display_chinese(ya);
					break;
				default:
					break;
			}
			break;
		case 1:
			lcd_display_chinese(gd);
			lcd_display_chinese(dian);
			if(page == 2){
				lcd_display_chinese(ya);
				lcd_display_colon();
				lcd_display_num3(data,'V');
			}
			else{
				lcd_display_chinese(liu);			
				lcd_display_colon();
				lcd_display_num3(data,'A');
			}
			break;
		case 2:
			lcd_display_chinese(xz);
			lcd_display_chinese(dian);
			if(page == 2){
				lcd_display_chinese(liu);
				lcd_display_colon();
				lcd_display_num3(data,'A');
			}
			else{
				lcd_display_chinese(ya);
				lcd_display_colon();
				lcd_display_num3(data,'V');
			}
			break;
		case 3:
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);		
			lcd_display_colon();
			lcd_display_num3(data,'M');
			break;
		case 4:
			lcd_display_chinese(tz);
			lcd_display_chinese(dian);
			if(page == 2){
				lcd_display_chinese(liu);
				lcd_display_colon();
				lcd_display_num3(data,'A');
			}
			else{			
				lcd_display_chinese(ya);	
				lcd_display_colon();
				lcd_display_num3(data,'V');
			}
			break;
		default:
			break;
	}
}

PAGE welcomepage(){
	lcd_clear();
    lcd_display_chinese_at(2,1,hysy);
    lcd_display_chinese_at(1,2,hypcdj);
	hy_gui_delay_ms(2200);
	return DisplayPage1;
}

/*main page
*  充电电压：xxxV
*  充电电流：xxxA
*  充电时间：xxxmin
*  状态： 电池错误/通讯错误/其他错误/
		本地充电中/通讯充电中
**/
PAGE displaypage1(uint32_t state,
	uint32_t vol,
	uint32_t cur,
	uint32_t time)
{
//		lcd_clear();
	
		lcd_display_chinese_at(0,0,chong);
		lcd_display_chinese(dian);
		lcd_display_chinese(dian);
		lcd_display_chinese(ya);
		lcd_display_colon();
		lcd_display_num4(vol,'V');

		
		lcd_display_chinese_at(0,1,chong);
		lcd_display_chinese(dian);
		lcd_display_chinese(dian);
		lcd_display_chinese(liu);
		lcd_display_colon();
		lcd_display_num4(cur,'A');
		
		lcd_display_chinese_at(0,2,chong);
		lcd_display_chinese(dian);
		lcd_display_chinese(sj);
		lcd_display_colon();
		lcd_display_time4(time);

		lcd_display_chinese_at(0,3,zt);
		lcd_display_colon();

		if (!(state&HY_GUI_BATTERY_ON_MASK))/*no battery*/{
			lcd_display_chinese(dian);
			lcd_display_chinese(chi);
			lcd_display_chinese(wei);
			lcd_display_chinese(lianjie);
		}else if((state & HY_GUI_BATTERY_ON_MASK)
					&&(state & HY_GUI_CHARGETASK_ON_MASK)){/*normal*/
			if (s_gui->controlstyle == HY_CONTROLSTYLE_CAN)
			{
				lcd_display_chinese(tx);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
			}else{/*default / local*/
				lcd_display_chinese(bd);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
			}
		}else{/*err*/
			if (s_gui->controlstyle == HY_CONTROLSTYLE_CAN)
			{
				if(!(state&HY_GUI_CAN_ON_MASK))
				{
					lcd_display_chinese(tx);
					lcd_display_chinese(cw);
				}else{
					lcd_display_chinese(wz);
					lcd_display_chinese(cw);					
				}	
			}else{/*default / local*/
				lcd_display_chinese(bd);
				lcd_display_chinese(cw);
			}
		}
		/*todo errmsg display!!!*/
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					LOG_ERROR_TAG(HY_LOG_TAG,"DisplayPage1 get button_set [%d]",(state & HY_GUI_BATTERY_ON_MASK) && (state & HY_GUI_CHARGETASK_ON_MASK));
					if ((state & HY_GUI_BATTERY_ON_MASK) && (state & HY_GUI_CHARGETASK_ON_MASK)){
						return PassportPage1;
					}else{
						return PassportPage;
					}
				break;
				case button_off:/*stop charge*///0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
				break;
			}
		}	

/*				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					
				break;
				case button_off:stop charge//0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
				break;
			}
		}	*/	
		// while(time_cnt<100){
		// 	time_cnt++;
			
		// 	button = Button_Check();	
		// 	switch(button){
		// 		case 1://set
		// 			if(*flag == 0){
		// 				return PassportPage;
		// 			}
		// 			break;
		// 		case 4://esc
		// 			//查询
		// 			return DisplayPage2;
		// 		case 5://on
		// 			//开始充电/
		// 			*flag = 1;
		// 			return DisplayPage1;
		// 		case 6://off
		// 			//停止充电
		// 			*flag = 0;
		// 			return DisplayPage1;
		// 		default:
		// 			hy_delay_ms(10);
		// 			break;
		// 	}
		// }
		return DisplayPage1;
}

//PAGE displaypage2(uint8_t mode,uint16_t no,uint16_t vol,uint8_t status,uint8_t * flag){
//	uint8_t button;
//	uint16_t time_cnt;
//	time_cnt = 0;
//	lcd_clear();
//	lcd_display_chinese_at(0,0,ms);
//	lcd_display_colon();
//	switch(mode){
//// #define STATE_IDEL 	                   			00
//// #define CAN_STATE                            10
//// #define CAN_STOP 		                        11
//// #define LOCAL_STATE_CHECK                    20
//// #define LOCAL_STATE_ONE                      21
//// #define LOCAL_STATE_TWO                      22
//// #define LOCAL_STATE_THREE                    23
//// #define LOCAL_STATE_END                      24
//		case 0:
//			lcd_display_chinese(tj);
//			break;
//		case 24:
//			lcd_display_chinese(tj);
//			break;
//		case 10:
//			lcd_display_ascii("CAN ");
//			lcd_display_chinese(chong);			
//			lcd_display_chinese(dian);			
//			break;
//		case 11:
//		case 12:
//			lcd_display_ascii("CAN ");
//			lcd_display_chinese(tj);
//			break;
//		case 20:
//			lcd_display_chinese(jc);		
//			break;
//		case 21:
//			lcd_display_chinese(yu);
//			lcd_display_chinese(chong);			
//			lcd_display_chinese(dian);			
//			break;
//		case 22:
//			lcd_display_chinese(heng);
//			lcd_display_chinese(liu);
//			lcd_display_chinese(chong);			
//			lcd_display_chinese(dian);			
//			break;
//		case 23:
//			lcd_display_chinese(heng);
//			lcd_display_chinese(ya);
//			lcd_display_chinese(chong);			
//			lcd_display_chinese(dian);			
//			break;
//		default:
//			break;
//	}
//	lcd_display_chinese_at(0,1,dian);
//	lcd_display_chinese(chi);
//	lcd_display_chinese(bianh);
//	lcd_display_colon();
//	lcd_display_num4(no,' ');

//	lcd_display_chinese_at(0,2,dian);
//	lcd_display_chinese(chi);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(ya);
//	lcd_display_colon();
//	lcd_display_num4(vol,'V');

//// #define  ERR_OK                             0
//// #define  ERR_TIMEOUT                        1
//// #define  ERR_LACKPHASE                      2
//// #define  ERR_OVERHEAT                       3
//// #define  ERR_BATTERYPROTECT								 4
//// #define  ERR_UNKNOW                         5
//	lcd_display_chinese_at(0,3,zt);
//	lcd_display_colon();
//	switch(status){
//		case 0:
//			lcd_display_chinese(zc);
//			break;
//		case 1:
//			lcd_display_chinese(tx);
//			lcd_display_chinese(cw);			
//			break;
//		case 2:
//			lcd_display_chinese(qx);
//			lcd_display_chinese(bh);			
//			break;
//		case 3:
//			lcd_display_chinese(gw);
//			lcd_display_chinese(bh);
//			break;
//		case 4:
//			lcd_display_chinese(dian);			
//			lcd_display_chinese(chi);			
//			lcd_display_chinese(bh);			
//			break;
//		case 5:
//			lcd_display_chinese(wz);			
//			lcd_display_chinese(cw);		
//			break;

//		default:
//			break;
//	}
//	
//	while(time_cnt<100){
//		time_cnt++;		
//		chargehandler();
//		button = Button_Check();
//		switch(button){
//			case 4:
//				return DisplayPage1;
//				break;
//			case 6:
//				*flag = 0;
//				return DisplayPage1;
//				break;
//			default:
//				hy_delay_ms(10);
//				break;
//		}
//	}
//	return DisplayPage2;
//}



PAGE passportpage(void)
{
	uint8_t index,passport[6];
	uint32_t updatetime_ms;
	static uint32_t passright_time_ms = 0;
	index = 0;
	
	if (passright_time_ms != 0){
		if (systime_elapse_ms(passright_time_ms) <= 5*60*1000){
			return SettingMainPage1;
		}else{
			passright_time_ms = 0;
		}
	}
	lcd_clear();
	lcd_goto_pos(1,2);
	lcd_display_chinese(qsmm);
	lcd_goto_pos(2,1);	
	lcd_display_ascii(" ");
	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) <= 10000){
		if(s_gui->button_flag == BUTTON_MSG)
		{
			index++;
			lcd_display_ascii("*");
			lcd_display_ascii(" ");
			s_gui->button_flag = NO_MSG;
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
				case button_esc://0x04	
				case button_off://0x06
				case button_on://0x05
					lcd_clear();
					lcd_display_chinese_at(2,2,mmcw);
					hy_delay_ms(2000);
					return DisplayPage1;
				break;
				case button_up://0x02

					passport[index-1] = 2;
				break;
				case button_down://0x03
					passport[index-1] = 3;
				break;
			}
		}	
		if(index == 6){
			if( pass[0]==passport[0] && pass[1]==passport[1] &&
					pass[2]==passport[2] && pass[3]==passport[3] &&
					pass[4]==passport[4] && pass[5]==passport[5]){
				lcd_clear();
				lcd_display_chinese_at(2,2,mima);
				lcd_display_chinese(zhengque);
				hy_delay_ms(1500);
				passright_time_ms = hy_time_now_ms();
				return SettingMainPage1;
			}else{
				lcd_clear();
				lcd_display_chinese_at(2,2,mmcw);
				hy_delay_ms(2000);
				return DisplayPage1;					
			}
		}else if (index > 6){
				lcd_clear();
				lcd_display_chinese_at(2,2,mmcw);
				hy_delay_ms(2000);
				return DisplayPage1;	
		}	
		// hy_delay_ms(10);
		// button = Button_Check();
		// if(button != 0){
		// 	time_cnt = 0;
		// 	passport[index] = button;
		// 	index++;
		// 	lcd_display_ascii("*");
		// 	lcd_display_ascii(" ");
		// }
		// if(index == 6){
		// 	if( pass[0]==passport[0] && pass[1]==passport[1] &&
		// 			pass[2]==passport[2] && pass[3]==passport[3] &&
		// 			pass[4]==passport[4] && pass[5]==passport[5]){
		// 				DEBUG_PRINT("right\r\n");
		// 				return SettingMainPage1;
		// 	}
		// 	else{
		// 		lcd_clear();
		// 		lcd_display_chinese_at(2,2,mmcw);
		// 		hy_delay_ms(2000);
		// 		return DisplayPage1;
		// 	}
		// }
	}
	return DisplayPage1;
}

PAGE passportpage1(void)
{
	uint32_t updatetime_ms;

	lcd_clear();

    lcd_display_chinese_at(0,0,qing);
    lcd_display_chinese(xian);
    lcd_display_chinese(tingzhi);
    lcd_display_chinese(chong);
    lcd_display_chinese(dian);

    lcd_display_chinese_at(0,1,zai);
    lcd_display_chinese(jinxing);
    lcd_display_chinese(sz);

	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) <= 2200){
		if(s_gui->button_flag == BUTTON_MSG){
			s_gui->button_flag = NO_MSG;
			return DisplayPage1;
		}	
	}
	return DisplayPage1;

}

PAGE settingmainpage(uint8_t page,uint8_t cursor)
{//0,1;0,1,2,3
	
	uint8_t button,index;
	uint8_t pa;
	uint32_t updatetime_ms;
	
	index = 1;
	
	lcd_clear();
	for(pa = 0 ; pa < 4-page; pa++){
		lcd_goto_pos(pa,0);
		showmainsettingitem(pa+page*4);
	}
	
	lcd_cursor_goto(cursor,0);
	index = cursor+page*4;
	
	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) < 30000){

		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					if(index > 7 || index < 0){
						break;
					}
					lcd_cursor_close();
					return (PAGE)(index+SettingPage11);					
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					if(index <= 0){
						index = 1;
					}
					index = index-1;
					if(page == 1 && index == 3){
	  					lcd_cursor_close();
						return SettingMainPage1;//next page
					}					
				break;
				case button_down://0x03
					index = index + 1;
					if (index >= 7){
						index = 7;
					}
					if(index == 4){
						lcd_cursor_close();
						return SettingMainPage2;//next page
					}
				break;
				case button_esc://0x04
					lcd_cursor_close();
					return DisplayPage1;	
				break;
			}
		}		
		lcd_cursor_goto(index-page*4,0);		
		// hy_delay_ms(10);
		// button = Button_Check();
		// if(button != 0){
		// 	time_cnt = 0;
		// 	if(button == 1){
		// 		lcd_cursor_close();
		// 		return (PAGE)(index+SettingPage11);
		// 	}
		// 	else if(button == 3){
		// 		index = index+1;
		// 		if(index == 4){
		// 			lcd_cursor_close();
		// 			return SettingMainPage2;//next page
		// 		}
		// 		else if(index == 7){
		// 			index = 6;
		// 		}
		// 	}
		// 	else if(button == 2){
		// 		if(index == 0){
		// 			index = 1;
		// 		}
		// 		index = index-1;
		// 		if(page == 1 && index == 3){
  // 				lcd_cursor_close();
		// 			return SettingMainPage1;//next page
		// 		}
		// 	}
		// 	else if(button == 4){
		// 		lcd_cursor_close();
		// 		return DisplayPage1;
		// 	}
		// 	lcd_cursor_goto(index-page*4,0);
		// }
	}
	lcd_cursor_close();
	return DisplayPage1;
}

/*充电步骤一：
* 
*/
PAGE settingpage11(PAGE father_page)
{
	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(yu);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(liu);//预充恒流

	lcd_goto_pos(1,0);
	lcd_display_chinese(gd);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);//给定电流
	lcd_display_colon();
	lcd_display_num4((config_data->chargecurrent_1*10),'A');

	lcd_goto_pos(2,0);
	lcd_display_chinese(xz);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);//限制电压
	lcd_display_colon();
	lcd_display_num4((config_data->limitvoltage_1*10),'V');

	lcd_goto_pos(3,0);
	lcd_display_chinese(tz);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);//跳转电压
	lcd_display_colon();
	lcd_display_num4((config_data->switchvoltage_1*10),'V');

	lcd_cursor_goto(index,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 1:
							dataname = chargecurrent_1;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage11 index chargecurrent_1");
						break;
						case 2:
							dataname = limitvoltage_1;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage11 index limitvoltage_1");
						break;
						case 3:
							dataname = switchvoltage_1;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage11 index switchvoltage_1");
						break;
						default:
							LOG_ERROR_TAG(HY_LOG_TAG,"gui settingpage11 index error");
							return DisplayPage1;
						break;
					}
					return datasettingpage(dataname,SettingPage11);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					index--;
					if (index <= 1){
						index = 1;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_down://0x03
					index++;
					if (index > 3){
						LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage11 change to next page SettingPage12");
						return SettingPage12;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}		

	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage12(PAGE father_page)
{
	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(yu);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(liu);//预充恒流

	lcd_goto_pos(1,0);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	lcd_display_chinese(sj);//充电时间
	lcd_display_colon();	
	lcd_display_time4((config_data->chargetimeout_1_min));


	lcd_cursor_goto(1,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_1_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage12 index chargetimeout_1_min");
					return datasettingpage(dataname,SettingPage12);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage11;
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}		
	}
	lcd_cursor_close();
	return DisplayPage1;

}


PAGE settingpage21(PAGE father_page)
{

	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(kuai);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(liu);//快充恒流

	lcd_goto_pos(1,0);
	lcd_display_chinese(gd);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);//给定电流
	lcd_display_colon();
	lcd_display_num4((config_data->chargecurrent_2*10),'A');

	lcd_goto_pos(2,0);
	lcd_display_chinese(xz);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);//限制电压
	lcd_display_colon();
	lcd_display_num4((config_data->limitvoltage_2*10),'V');

	lcd_goto_pos(3,0);
	lcd_display_chinese(tz);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);//跳转电压
	lcd_display_colon();
	lcd_display_num4((config_data->switchvoltage_2*10),'V');

	lcd_cursor_goto(index,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 1:
							dataname = chargecurrent_2;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage21 index chargecurrent_2");
						break;
						case 2:
							dataname = limitvoltage_2;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage21 index limitvoltage_2");
						break;
						case 3:
							dataname = switchvoltage_2;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage21 index switchvoltage_2");
						break;
						default:
							LOG_ERROR_TAG(HY_LOG_TAG,"gui settingpage21 index error");
							return DisplayPage1;
						break;
					}
					return datasettingpage(dataname,SettingPage21);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					index--;
					if (index <= 1){
						index = 1;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_down://0x03
					index++;
					if (index > 3){
						return SettingPage22;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}		

	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage22(PAGE father_page)
{
	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(kuai);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(liu);//快充恒流

	lcd_goto_pos(1,0);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	lcd_display_chinese(sj);//充电时间
	lcd_display_colon();	
	lcd_display_time4((config_data->chargetimeout_2_min));


	lcd_cursor_goto(1,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_2_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage22 index chargetimeout_2_min");
					return datasettingpage(dataname,SettingPage22);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage21;
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}	
	}	
	lcd_cursor_close();
	return DisplayPage1;

}

PAGE settingpage31(PAGE father_page)
{
	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(man);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(ya);//慢充恒压

	lcd_goto_pos(1,0);
	lcd_display_chinese(gd);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);//给定电流
	lcd_display_colon();
	lcd_display_num4((config_data->chargevoltage_3*10),'V');

	lcd_goto_pos(2,0);
	lcd_display_chinese(xz);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);//限制电压
	lcd_display_colon();
	lcd_display_num4((config_data->limitcurrent_3*10),'A');

	lcd_goto_pos(3,0);
	lcd_display_chinese(tz);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);//跳转电压
	lcd_display_colon();
	lcd_display_num4((config_data->switchcurrent_3*10),'A');

	lcd_cursor_goto(index,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 1:
							dataname = chargevoltage_3;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage31 index chargevoltage_3");
						break;
						case 2:
							dataname = limitcurrent_3;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage31index limitcurrent_3");
						break;
						case 3:
							dataname = switchcurrent_3;
							LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage31 index switchcurrent_3");
						break;
						default:
							LOG_ERROR_TAG(HY_LOG_TAG,"gui settingpage31 index error");
							return DisplayPage1;
						break;
					}
					return datasettingpage(dataname,SettingPage31);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					index--;
					if (index <= 1){
						index = 1;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_down://0x03
					index++;
					if (index > 3){
						return SettingPage32;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}		

	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage32(PAGE father_page)
{
	uint8_t index;
	uint32_t updatetime_ms;
	data_name dataname;

	updatetime_ms = hy_time_now_ms();
	index = 1;
	
	lcd_clear();

	lcd_goto_pos(0,0);
	lcd_display_space();
	lcd_display_chinese(man);
	lcd_display_chinese(chong);
	lcd_display_space();
	lcd_display_chinese(heng);
	lcd_display_chinese(ya);//慢充恒压

	lcd_goto_pos(1,0);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	lcd_display_chinese(sj);//充电时间
	lcd_display_colon();	
	lcd_display_time4((config_data->chargetimeout_3_min));


	lcd_cursor_goto(1,0);	

	while(systime_elapse_ms(updatetime_ms) <= 30000){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_3_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage32 index chargetimeout_3_min");
					return datasettingpage(dataname,SettingPage32);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage31;
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}
	}	
	lcd_cursor_close();
	return DisplayPage1;

}
//PAGE settingpage2(uint8_t page){//0 1
//	uint16_t time_cnt;
//	uint8_t button,index;
//	uint8_t pa;
//	time_cnt = 0;
//	index = 1;
//	
//	lcd_clear();
//	for(pa = 0 ; pa<4 ; pa++){
//		lcd_goto_pos(pa,0);
//		showsettingitem(1,pa+page);
//	}
//	
//	if(page == 0){
//		index = 1;
//		lcd_cursor_goto(1,0);
//	}
//	else{
//		index = 4;
//		lcd_cursor_goto(3,0);		
//	}
//	
//	while(time_cnt<1000){
//		time_cnt++;		
//		hy_delay_ms(10);
//		button = Button_Check();
//		switch(button){
//			case 1:
//				lcd_cursor_close();
//				return datasettingpage(index+3);
//				break;
//			case 2:
//				index = (index>1?index-1:1);
//				if(page == 1 && index == 1){
//					lcd_cursor_close();
//					return SettingPage21;//next page
//				}
//				break;
//			case 3:
//				index = (index<4?index+1:4);
//				if(page == 0 && index == 4){
//					lcd_cursor_close();
//					return SettingPage22;//next page
//				}
//				break;
//			case 4:
//				lcd_cursor_close();
//				return SettingMainPage1;
//				break;
//			default:
//				break;
//		}
//		if(button != 0){
//			time_cnt = 0;
//			lcd_cursor_goto(index-page,0);
//		}
//	}
//	lcd_cursor_close();
//	return DisplayPage1;
//}

//PAGE settingpage3(uint8_t page){//0 1
//	uint16_t time_cnt;
//	uint8_t button,index;
//	uint8_t pa;
//	time_cnt = 0;
//	index = 1;
//	
//	lcd_clear();
//	for(pa = 0 ; pa<4 ; pa++){
//		lcd_goto_pos(pa,0);
//		showsettingitem(2,pa+page);
//	}
//	
//	if(page == 0){
//		index = 1;
//		lcd_cursor_goto(1,0);
//	}
//	else{
//		index = 4;
//		lcd_cursor_goto(3,0);		
//	}
//	
//	while(time_cnt<1000){
//		time_cnt++;		
//		hy_delay_ms(10);
//		button = Button_Check();
//		switch(button){
//			case 1:
//				lcd_cursor_close();
//				return datasettingpage(index+7);
//				break;
//			case 2:
//				index = (index>1?index-1:1);
//				if(page == 1 && index == 1){
//					lcd_cursor_close();
//					return SettingPage31;//next page
//				}
//				break;
//			case 3:
//				index = (index<4?index+1:4);
//				if(page == 0 && index == 4){
//					lcd_cursor_close();
//					return SettingPage32;//next page
//				}
//				break;
//			case 4:
//				lcd_cursor_close();
//				return SettingMainPage1;
//				break;
//			default:
//				break;
//		}
//		if(button != 0){
//			time_cnt = 0;
//			lcd_cursor_goto(index-page,0);
//		}
//	}
//	lcd_cursor_close();
//	return DisplayPage1;
//}

PAGE settingpage4(PAGE father_page){

	int8_t index;
	uint32_t updatetime_ms;
	data_name dataname;
	
	updatetime_ms = hy_time_now_ms();
	lcd_clear();
	lcd_goto_pos(0,0);
	lcd_display_chinese(kz);
	lcd_display_chinese(fs);//控制方式
	lcd_display_colon();
	if(config_data->controlstyle == HY_CONTROLSTYLE_CAN){
		lcd_display_chinese(tx);
	}else{
		lcd_display_chinese(bd);
	}

	lcd_goto_pos(1,0);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_chinese(lc);//电压量程
	lcd_display_colon();		
	lcd_display_num4(config_data->voltagerange*10,'V');

	lcd_goto_pos(2,0);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);
	lcd_display_chinese(lc);//电流量程
	lcd_display_colon();		
	lcd_display_num4(config_data->currentrange*10,'A');

	lcd_goto_pos(3,0);
	lcd_display_chinese(tx);
	lcd_display_chinese(sl);//通信速率
	lcd_display_colon();
	lcd_display_botelv4(config_data->communicaterate/1000);

	index = 0;
	lcd_cursor_goto(0,0);
	while(systime_elapse_ms(updatetime_ms) <= 30000){				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 0:
							dataname = controlstyle;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set controlstyle");
						break;
						case 1:
							dataname = voltagerange;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set controlstyle");
						break;
						case 2:
							dataname = currentrange;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set controlstyle");
						break;
						case 3:
							dataname = communicaterate;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set communicationrate");
						break;
					}
					/*todo if need set into flash*/
					return datasettingpage(dataname,SettingPage4);
				break;
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					index--;
					if(index <= 0){
						index = 0;
					}
					lcd_cursor_goto(index,0);
				break;
				case button_down://0x03
					index++;
					if (index>=3){
						index = 3;
					}
					lcd_cursor_goto(index,0);
				break;
				case button_esc://0x04
					return father_page;
				break;
			}
		}					
	}

	lcd_cursor_close();
	return DisplayPage1;
}

//PAGE settingpage5(){
//	uint16_t time_cnt;
//	uint8_t button,index;
//	uint8_t passport[6];	
//	time_cnt = 0;
//	index = 0;
//	lcd_clear();
//	lcd_goto_pos(0,1);
//	lcd_display_chinese(hf);
//	lcd_display_chinese(cc);
//	lcd_display_chinese(cs);
//	lcd_goto_pos(1,2);
//	lcd_display_chinese(qsmm);
//	lcd_goto_pos(2,1);	
//	lcd_display_ascii(" ");
//	while(time_cnt<1000){
//		hy_delay_ms(10);
//		time_cnt++;
//		button = Button_Check();
//		if(button != 0){
//			time_cnt = 0;
//			passport[index] = button;
//			index++;
//			lcd_display_ascii("*");
//			lcd_display_ascii(" ");
//		}
//		if(index == 6){
//			if( pass[0]==passport[0] && pass[1]==passport[1] &&
//					pass[2]==passport[2] && pass[3]==passport[3] &&
//					pass[4]==passport[4] && pass[5]==passport[5]){
//						Config_EraseConfig();
//						hy_delay_ms(50);
//						Config_DataReading();
//			}
//			else{
//				lcd_clear();
//				lcd_display_chinese_at(2,1,mmcw);
//				hy_delay_ms(2000);
//			}
//			return SettingMainPage2;
//		}
//	}
//	return DisplayPage1;
//}

//PAGE settingpage6(){
//	uint16_t time_cnt;
//	uint8_t button,data;
//	time_cnt = 0;
//	lcd_clear();
//	lcd_goto_pos(0,0);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(liu);
//	lcd_display_chinese(ph);
//	lcd_goto_pos(1,0);
//	lcd_display_chinese(cs);
//	lcd_display_colon();
//	data = ConfigReadFromAddr(16);
//	lcd_display_num2_big(data,unit[16]);
//	lcd_goto_pos(2,0);
//	lcd_display_chinese(yx);
//	lcd_display_chinese(zt);
//	lcd_display_colon();
//	lcd_display_chinese(tj);
//	while(time_cnt<1000){
//		time_cnt++;		
//		hy_delay_ms(10);
//		button = Button_Check();
//		if(button != 0){
//			time_cnt = 0;
//		}
//		switch(button){
//			case 1:
//				return datasettingpage(16);
//				break;
//			case 4:
//				return SettingMainPage2;
//				break;
//		}
//	}
//	return DisplayPage1;
//}

//PAGE settingpage7(){
//	uint16_t time_cnt;
//	uint8_t button;
//	time_cnt = 0;
//	lcd_clear();
//	lcd_goto_pos(0,0);
//	lcd_display_chinese(cp);
//	lcd_display_chinese(xh);
//	lcd_display_colon();
//	lcd_display_ascii("BC2000");
//	lcd_goto_pos(1,0);
//	lcd_display_chinese(rj);
//	lcd_display_chinese(bb);
//	lcd_display_colon();
//	lcd_display_ascii("V1.0.2");
//	lcd_goto_pos(2,0);
//	lcd_display_chinese(cj);
//	lcd_display_colon();
//	lcd_display_chinese(tianj);
//	lcd_display_chinese(hy);
//	lcd_goto_pos(3,0);
//	lcd_display_chinese(dh);
//	lcd_display_colon();
//	lcd_display_ascii("15102299468");
//	while(time_cnt<1000){
//		time_cnt++;		
//		hy_delay_ms(10);
//		button = Button_Check();
//		if(button != 0){
//			time_cnt = 0;
//			if(button == 1  || button == 4){
//				return SettingMainPage2;
//			}
//		}
//	}
//	return DisplayPage1;
//}

PAGE datasettingpage(data_name name,PAGE father_page){//0~16

	uint32_t updatetime_ms ;
	uint32_t data;
	int8_t num[5];//对应4个位置，千 百 十 个
	int8_t num_index;
	int8_t index;

	updatetime_ms = hy_time_now_ms();
	lcd_clear();
	switch(name){
    	case voltagerange:
	    	lcd_goto_pos(0,0);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);
	    	lcd_display_chinese(lc);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->voltagerange*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->voltagerange = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set voltagerange  [%d]x0.1V ",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;

    	break;

		case currentrange:
	   		lcd_goto_pos(0,0);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);
	    	lcd_display_chinese(lc);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->currentrange*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->currentrange = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set currentrange  [%d]x0.1A ",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case balancecurrent:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(ph);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->balancecurrent*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->balancecurrent = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set balancecurrent  [%d]x0.1A ",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case controlstyle:
			lcd_goto_pos(0,0);
			lcd_display_chinese(kz);
			lcd_display_chinese(fs);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			lcd_display_index(1);
			lcd_display_chinese(bd);
			lcd_display_chinese(ms);

			lcd_goto_pos(2,0);
			lcd_display_index(2);
			lcd_display_chinese(tx);
			lcd_display_chinese(ms);

			index = 1;
			lcd_cursor_goto(index,0);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							if (index == 1){
								config_data->controlstyle = 1;
							}else if(index == 2){
								config_data->controlstyle = 0;
							}
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage controlstyle set [%d] 0 for can 1 for local",config_data->controlstyle);
							return father_page;
						break;
						case button_off://0x06
						break;
						case button_on://0x05
						break;						
						case button_up://0x02
							index--;
							if (index <= 1){
								index = 1;
							}
							lcd_cursor_goto(index,0);
						break;
						case button_down://0x03
							index++;
							if (index>=2){
								index = 2;
							}
							lcd_cursor_goto(index,0);
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case chargecurrent_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargecurrent_1*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargecurrent_1 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set chargecurrent_1 [%d]x0.1A",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;						
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case limitvoltage_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitvoltage_1*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitvoltage_1 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitvoltage_1 set [%d]x0.1V",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case chargetimeout_1_min:
			lcd_display_chinese_at(0,0,bz);
			lcd_display_chinese(yi);
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargetimeout_1_min;
			lcd_display_time4_forset(data);
			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[3] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_1_min = data;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_1_min set [%d] min",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 3)
								num_index = 3;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							lcd_goto_pos(1,num_index);
							num[num_index]++;
							if (num[num_index] >= 9){
								num[num_index] = 9;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_down://0x03
							lcd_goto_pos(1,num_index);
							num[num_index]--;
							if (num[num_index] <= 0){
								num[num_index] = 0;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	

		break;

		case switchvoltage_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->switchvoltage_1*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchvoltage_1 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchvoltage_1 set [%d]x0.1V",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	
		break;

		case chargecurrent_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargecurrent_2*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargecurrent_2 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargecurrent_2 set [%d]x0.1A",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case limitvoltage_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitvoltage_2*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitvoltage_2 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitvoltage_2 set [%d]x0.1V",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case chargetimeout_2_min:
			lcd_display_chinese_at(0,0,bz);
			lcd_display_chinese(er);
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargetimeout_2_min;
			lcd_display_time4_forset(data);
			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[3] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_2_min = data;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_2_min set [%d] min",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 3)
								num_index = 3;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							lcd_goto_pos(1,num_index);
							num[num_index]++;
							if (num[num_index] >= 9){
								num[num_index] = 9;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_down://0x03
							lcd_goto_pos(1,num_index);
							num[num_index]--;
							if (num[num_index] <= 0){
								num[num_index] = 0;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	
		break;

		case switchvoltage_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->switchvoltage_2*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchvoltage_2 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchvoltage_2 set [%d]x0.1V",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case chargevoltage_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargevoltage_3*10;
			lcd_display_num4_forset(data,'V');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargevoltage_3 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargevoltage_3 set [%d]x0.1V",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			return DisplayPage1;
		break;

		case limitcurrent_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitcurrent_3*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitcurrent_3 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitcurrent_3 set [%d]x0.1A",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case chargetimeout_3_min:
			lcd_display_chinese_at(0,0,bz);
			lcd_display_chinese(san);
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargetimeout_3_min;
			lcd_display_time4_forset(data);
			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[3] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_3_min = data;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_3_min set [%d] min",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 3)
								num_index = 3;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							lcd_goto_pos(1,num_index);
							num[num_index]++;
							if (num[num_index] >= 9){
								num[num_index] = 9;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_down://0x03
							lcd_goto_pos(1,num_index);
							num[num_index]--;
							if (num[num_index] <= 0){
								num[num_index] = 0;
							}
							write_data(num[num_index]+0x30);
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	
		break;

		case switchcurrent_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->switchcurrent_3*10;
			lcd_display_num4_forset(data,'A');

			if (data>=9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchcurrent_3 = data/10;
							/*todo if need set into flash*/
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchcurrent_3 set [%d]x0.1A",data);
							return father_page;
						break;
						case button_off://0x06
							num_index ++;
							if(num_index >= 4)
								num_index = 4;
							lcd_cursor_goto(1,num_index);
						break;
						case button_on://0x05
							num_index --;
							if(num_index <= 0)
								num_index = 0;
							lcd_cursor_goto(1,num_index);
						break;
						case button_up://0x02
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]++;
								if (num[num_index] >= 9){
									num[num_index] = 9;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_down://0x03
							if(num_index != 3){//小数点
								lcd_goto_pos(1,num_index);
								num[num_index]--;
								if (num[num_index] <= 0){
									num[num_index] = 0;
								}
								write_data(num[num_index]+0x30);
							}
						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

		case communicaterate:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tx);
	    	lcd_display_chinese(botelv);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			lcd_display_index(1);
			lcd_display_botelv4(125);

			lcd_goto_pos(2,0);
			lcd_display_index(2);
			lcd_display_botelv4(250);

			lcd_goto_pos(3,0);
			lcd_display_index(3);
			lcd_display_botelv4(500);

			index = 1;
			lcd_cursor_goto(1,0);

			while(systime_elapse_ms(updatetime_ms) <= 30000){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							switch(index){
								case 1:
								config_data->communicaterate = 125000;
								break;
								case 2:
								config_data->communicaterate = 250000;
								break;
								case 3:
								config_data->communicaterate = 500000;
								break;
								default:
								config_data->communicaterate = 250000;
								break;
							}
							/*todo if need set into flash*/
							return father_page;
						break;
						case button_off://0x06
						break;
						case button_on://0x05
						break;
						case button_up://0x02
							index--;
							if (index <= 1){
								index = 1;
							}
							lcd_cursor_goto(index,0);
						break;
						case button_down://0x03
							index++;
							if (index >= 3){
								index = 3;
							}
							lcd_cursor_goto(index,0);

						break;
						case button_esc://0x04
							return father_page;
						break;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		break;

	}
	lcd_cursor_close();
	return DisplayPage1;
	// uint16_t data,time_cnt;
	// uint8_t button,index,databit[3];
	// time_cnt = 0;
	// index = 0;
	// if(ID ==12){
	// 	// return modesettingpage();
	// }
	// else if(ID == 15){
	// 	// return communicationratesettingpage();
	// }
	// else if(ID ==16){
	// 	// return balancesettingpage();
	// }
	// else{
	// 	data = ConfigReadFromAddr(ID);
	// 	databit[0] = data/100;
	// 	databit[1] = data/10%10;
	// 	databit[2] = data%10;
		
	// 	lcd_clear();
	// 	lcd_goto_pos(1,3);
	// 	lcd_display_chinese(sz);
	// 	lcd_goto_pos(2,2);

	// 	lcd_display_num3_big(data,unit[ID]);
	// 	lcd_cursor_goto(2,2);
	// 	while(time_cnt<1000 && index<3){
	// 		hy_delay_ms(10);
	// 		time_cnt++;
	// 		button = 7;
	// 		if(button != 0){
	// 			time_cnt = 0;
	// 		}
	// 		switch(button){
	// 			case 1:
	// 				index++;
	// 				lcd_cursor_goto(2,2+index);			
	// 				break;
	// 			case 3:
	// 				lcd_cursor_goto(2,2);			
	// 				databit[index] = (databit[index]+10-1)%10;
	// 				data = databit[0]*100+databit[1]*10+databit[2];
	// 				lcd_display_num3_big(data,unit[ID]);
	// 				lcd_cursor_goto(2,2+index);			
	// 				break;
	// 			case 2:
	// 				lcd_cursor_goto(2,2);			
	// 				databit[index] = (databit[index]+1)%10;
	// 				data = databit[0]*100+databit[1]*10+databit[2];
	// 				lcd_display_num3_big(data,unit[ID]);
	// 				lcd_cursor_goto(2,2+index);			
	// 				break;
	// 			case 4:
	// 				lcd_cursor_close();
	// 				switch(ID){
	// 					case 0:
	// 					case 1:
	// 					case 2:
	// 						return SettingPage11;//7
	// 						break;
	// 					case 3:
	// 						return SettingPage12;//14
	// 						break;
	// 					case 4:
	// 					case 5:
	// 					case 6:
	// 						return SettingPage21;//8
	// 						break;
	// 					case 7:
	// 						return SettingPage22;//15
	// 						break;
	// 					case 8:
	// 					case 9:
	// 					case 10:
	// 						return SettingPage31;//9
	// 						break;
	// 					case 11:
	// 						return SettingPage32;//16
	// 						break;
	// 					case 12:
	// 					case 13:
	// 					case 14:
	// 					case 15:
	// 						return SettingPage4;//10
	// 						break;
	// 					case 16:
	// 						return SettingPage6;//12
	// 						break;
	// 					default:
	// 						return DisplayPage1;
	// 						break;
	// 				}
	// 		}
	// 	}	
	// 	lcd_cursor_close();
	// 	if(index == 3){
	// 		if(data > max[ID])
	// 			data = max[ID];
	// 		if(data < min[ID])
	// 			data = min[ID];
	// 		ConfigWriteToAddr(ID,data);
	// 		Config_DataWriting();
	// 		switch(ID){
	// 			case 0:
	// 			case 1:
	// 			case 2:
	// 				return SettingPage11;//7
	// 			case 3:
	// 				return SettingPage12;//14
	// 			case 4:
	// 			case 5:
	// 			case 6:
	// 				return SettingPage21;//8
	// 			case 7:
	// 				return SettingPage22;//15
	// 			case 8:
	// 			case 9:
	// 			case 10:
	// 				return SettingPage31;//9
	// 			case 11:
	// 				return SettingPage32;//16
	// 			case 12:
	// 			case 13:
	// 			case 14:
	// 			case 15:
	// 				return SettingPage4;//10
	// 				break;
	// 			case 16:
	// 				return SettingPage6;//12
	// 			default:
	// 				return DisplayPage1;
	// 		}
	// 	}
	// 	else{
	// 		return DisplayPage1; 
	// 	}
	// }
}


//PAGE modesettingpage(void){
//	uint16_t data,time_cnt;
//	uint8_t button,index;
//	time_cnt = 0;
//	data = ConfigReadFromAddr(12);	
//	index = data;
//	lcd_clear();
//	lcd_goto_pos(0,3);
//	lcd_display_chinese(sz);
//	lcd_goto_pos(2,2);
//	lcd_display_ascii("1 . ");
//	lcd_display_chinese(tx);
//	lcd_goto_pos(3,2);
//	lcd_display_ascii("2 . ");
//	lcd_display_chinese(bd);
//	if(data ==0)
//		lcd_cursor_goto(2,2);
//	else
//		lcd_cursor_goto(3,2);

//	while(time_cnt<1000){
//		hy_delay_ms(10);
//		time_cnt++;
//		button = Button_Check();
//		if(button != 0){
//			time_cnt = 0;
//		}
//		switch(button){
//			case 1:
//				ConfigWriteToAddr(12,index);
//				Config_DataWriting();
//				return SettingPage4;//12
//				break;
//			case 3:
//			case 2:
//				index = (index+1)%2;
//				lcd_cursor_goto(2+index,2);			
//				break;
//			case 4:
//				lcd_cursor_close();
//				return SettingPage4;//12
//		}
//	}
//	lcd_cursor_close();
//	return DisplayPage1; 
//}	


//PAGE communicationratesettingpage(void){
//	uint16_t data,time_cnt;
//	uint8_t button,index;
//	uint8_t no[1];
//	
//	time_cnt = 0;
//	data = ConfigReadFromAddr(15);	
//	lcd_clear();
//	lcd_goto_pos(0,3);
//	lcd_display_chinese(sz);
//	for(index = 0 ;index<5;index++){
//		lcd_goto_pos(index/2+1,4*(index%2));
//		no[0]=0x31+index;
//		lcd_display_ascii(no);
//		lcd_display_ascii(".");
//		lcd_display_num3(communicationrate[index],'K');
//	}
//	index = data;
//	lcd_cursor_goto(index/2+1,4*(index%2));

//	while(time_cnt<1000){
//		hy_delay_ms(10);
//		time_cnt++;
//		button = Button_Check();
//		if(button != 0){
//			time_cnt = 0;
//		}
//		switch(button){
//			case 1:
//				ConfigWriteToAddr(15,index);
//				Config_DataWriting();
//				return SettingPage4;//12

//			case 3:
//				index = (index+1)%5;
//				lcd_cursor_goto(index/2+1,4*(index%2));
//				break;
//			case 2:
//				index = (index-1)%5;
//				lcd_cursor_goto(index/2+1,4*(index%2));
//				break;
//			case 4:
//				lcd_cursor_close();
//				return SettingPage4;//12
//		}
//	}
//	lcd_cursor_close();
//	return DisplayPage1; 
//}	


//PAGE balancesettingpage(void){
//	uint16_t data,time_cnt;
//	uint8_t button;
//	time_cnt = 0;
//	data = ConfigReadFromAddr(16);	
//	lcd_clear();
//	lcd_goto_pos(1,3);
//	lcd_display_chinese(sz);
//	lcd_goto_pos(2,2);
//	lcd_display_num2_big(data,'%');

//	while(time_cnt<1000){
//		hy_delay_ms(10);
//		time_cnt++;
//		button = Button_Check();
//		switch(button){
//			case 1:
//				ConfigWriteToAddr(16,data);
//				Config_DataWriting();
//				return SettingPage6;//12
//				break;
//			case 3:
//				if(data>0)
//					data--;
//				break;
//			case 2:
//				if(data<60)
//					data++;
//				break;
//			case 4:
//				return SettingPage6;//12
//		}
//		if(button != 0){
//			time_cnt = 0;
//			lcd_goto_pos(2,2);
//			lcd_display_num2_big(data,'%');
//		}
//	}
//	lcd_cursor_close();
//	return DisplayPage1; 
//}	

//PAGE errorpage(uint8_t err){
//	
//// #define  ERR_TIMEOUT                        1
//// #define  ERR_LACKPHASE                      2
//// #define  ERR_OVERHEAT                       3

//	uint8_t button;
//	lcd_clear();
//	lcd_goto_pos(1,2);
//	if(err == 1){
//		lcd_display_chinese(tx);
//	}
//	else if(err == 2){
//		lcd_display_chinese(qx);
//	}
//	else if(err == 3){
//		lcd_display_chinese(gw);
//	}
//	lcd_display_chinese(gz);
//	while(1){
//		button = Button_Check();
//		hy_delay_ms(20);
//		if(button == 4){
//			return DisplayPage2; 
//		}
//	}
//}


//PAGE finishpage(uint32_t vol,uint32_t time){
//	uint8_t button;
//	lcd_clear();
//	lcd_goto_pos(1,2);
//	lcd_display_chinese(chong);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(js);
//	lcd_goto_pos(2,1);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(ya);
//	lcd_display_colon();
//	lcd_display_num3(vol/10,'V');
//	lcd_goto_pos(3,1);
//	lcd_display_chinese(sj);
//	lcd_display_colon();
//	lcd_display_num3(time,'M');	
//	while(1){
//		button = Button_Check();
//		hy_delay_ms(20);
//		if(button == 4){
//			return DisplayPage1; 
//		}
//	}
//}
//PAGE finishpage2(uint32_t vol,uint32_t time,uint8_t * flag){
//	uint16_t time_cnt;
//	uint8_t button;
//	time_cnt =0;
//	lcd_clear();
//	lcd_goto_pos(1,2);
//	lcd_display_chinese(chong);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(js);
//	lcd_goto_pos(2,1);
//	lcd_display_chinese(dian);
//	lcd_display_chinese(ya);
//	lcd_display_colon();
//	lcd_display_num3(vol/10,'V');
//	lcd_goto_pos(3,1);
//	lcd_display_chinese(sj);
//	lcd_display_colon();
//	lcd_display_num3(time,'M');	
//	chargehandler();
//	while(time_cnt<100){
//		time_cnt++;
//		button = Button_Check();
//		hy_delay_ms(10);
//		if(button == 6){
//			*flag = 0;
//			return DisplayPage1; 
//		}
//	}
//	return DisplayPage1;
//	
//}


PAGE errorpage1(void)
{
	uint32_t updatetime_ms;

	lcd_clear();

    lcd_display_chinese_at(0,0,wz);
    lcd_display_chinese(cw);

    lcd_display_chinese_at(0,1,qing);
    lcd_display_chinese(chongqi);

	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) <= 30000){
		if(s_gui->button_flag == BUTTON_MSG){
			s_gui->button_flag = NO_MSG;
			return DisplayPage1;
		}	
	}
	return DisplayPage1;
}


