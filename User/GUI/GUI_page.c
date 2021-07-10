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

#define HY_SETTING_PAGE_TIMEOUT_MS    30000/**/

static hy_gui_t* s_gui = NULL;
static hy_config* config_data = NULL;

uint8_t hysy[9] = {0xbb,0xb6,0xd3,0xad,0xca,0xb9,0xd3,0xc3,0x00};//欢迎使用
uint8_t hypcdj[13] = {0xea,0xbb,0xd1,0xc7,0xc5,0xc6,0xb3,0xe4,0xb5,0xe7,0xbb,0xfa,0x00};//昊亚牌充电机
uint8_t anchixinnengyuan[11] = {0xb0,0xb2,0xb3,0xdb,0xd0,0xc2,0xc4,0xdc,0xd4,0xb4,0x00};
uint8_t dian[3] = {0xb5,0xe7,0x00};//电
uint8_t liu[3] ={0xc1,0xf7,0x00};//流
uint8_t ya[3] = {0xd1,0xb9,0x00};//压
uint8_t sj[5] = {0xca,0xb1,0xbc,0xe4,0x00};//时间
uint8_t kz[5] = {0xbf,0xd8,0xd6,0xc6,0x00};//控制
uint8_t fs[5] = {0xb7,0xbd,0xca,0xbd,0x00};//方式
uint8_t tongxin[5] = {0xcd,0xa8,0xd0,0xc5,0x00};//通信
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
uint8_t zhong[3] = {0xd6,0xd0,0x00};//中
uint8_t wancheng[5] = {0xcd,0xea,0xb3,0xc9,0x00};//完成
uint8_t kaishi[5] = {0xbf,0xaa,0xca,0xbc,0x00};//开始
uint8_t ru[3] = {0xc8,0xe7,0x00};//如
uint8_t guo[3] = {0xb9,0xfb,0x00};//果
uint8_t duan[3] = {0xb6,0xcf,0x00};//断
uint8_t douhao[3] = {0xa3,0xac,0x00};//逗号“,”
uint8_t juhao[3] = {0xa1,0xa3,0x00};//句号“。”
uint8_t fanhui[5] = {0xb7,0xb5,0xbb,0xd8,0x00};//返回
uint8_t jiemian[5] = {0xbd,0xe7,0xc3,0xe6,0x00};//界面
uint8_t an[3] = {0xb0,0xb4,0x00};//按
uint8_t duqu[5] = {0xb6,0xc1,0xc8,0xa1,0x00};//读取
uint8_t lianxi[5] = {0xc1,0xaa,0xcf,0xb5,0x00};//联系
uint8_t zai_2[3] = {0xd4,0xda,0x00};//在

uint8_t mozu[5] = {0xc4,0xa3,0xd7,0xe9,0x00};//模组

uint8_t jiaoliu[5] = {0xbd,0xbb,0xc1,0xf7,0x00};//交流
uint8_t guoya[5] = {0xb9,0xfd,0xd1,0xb9,0x00};//过压
uint8_t qianya[5] = {0xc7,0xb7,0xd1,0xb9,0x00};//欠压

uint8_t shuchu[5] = {0xca,0xe4,0xb3,0xf6,0x00};//输出

uint8_t guoliu[5] = {0xb9,0xfd,0xc1,0xf7,0x00};//过流

uint8_t duanlu[5] = {0xb6,0xcc,0xc2,0xb7,0x00};//短路

uint8_t guowen[5] = {0xb9,0xfd,0xce,0xc2,0x00};//过温

uint8_t guanji[5] = {0xb9,0xd8,0xbb,0xfa,0x00};//关机

uint8_t yingjian[5] = {0xd3,0xb3,0xbc,0xfe,0x00};//硬件

uint8_t guzhang[5] = {0xb9,0xca,0xd5,0xcf,0x00};//故障

uint8_t fanjie[5] = {0xb7,0xb4,0xbd,0xd3,0x00};//反接

uint8_t yuanbian[5] = {0xd4,0xad,0xb1,0xdf,0x00};//原边

uint8_t jiange[5] = {0xbd,0xb5,0xb6,0xee,0x00};//降额

uint8_t fengshan[5] = {0xb7,0xe7,0xc9,0xc8,0x00};//风扇

uint8_t badiao[5] = {0xb0,0xce,0xb5,0xf4,0x00};//拔掉

uint8_t jueyuan[5] = {0xbe,0xf8,0xd4,0xb5,0x00};//绝缘

uint8_t diwen[5] = {0xb5,0xcd,0xce,0xc2,0x00};//低温

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
			lcd_display_chinese(cc);
			lcd_display_chinese(cs);
			lcd_display_chinese(sz);		
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
    lcd_display_chinese_at(2,2,anchixinnengyuan);
	hy_gui_delay_ms(2200);
	return DisplayPage1;
}

uint8_t charger_statu1;
uint8_t charger_statu2;
void set_gwcharger_status(uint8_t statu1,uint8_t statu2){
	charger_statu1 = statu1;
	charger_statu2 = statu2;
}
/*main page
*  充电电压：xxxV
*  充电电流：xxxA
*  充电时间：xxxmin
*  状态： 电池错误/通讯错误/其他错误/
		本地充电中/通讯充电中
**/
PAGE displaypage1(chargetask_gui_msg* gui_msg,
	uint32_t vol,
	uint32_t cur,
	uint32_t time)
{
		lcd_clear_5s();

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


		//LOG_INFO_TAG(HY_LOG_TAG,"gui get charge_module_connected [%d]",gui_msg->charge_module_connected);
		if (gui_msg->charge_module_connected == HY_FALSE){//模组没有连接
			hy_led_control(led_err);
			lcd_display_chinese(mozu);
			lcd_display_chinese(wei);
			lcd_display_chinese(lianjie);
			goto display_button_check;
		}
		if(gui_msg->battery_connected == HY_FALSE){//电池没有连接
			hy_led_control(led_offall);
			lcd_display_chinese(dian);
			lcd_display_chinese(chi);
			lcd_display_chinese(wei);
			lcd_display_chinese(lianjie);
			goto display_button_check;

		}
		if(s_gui->controlstyle == HY_CONTROLSTYLE_CAN){
			if (gui_msg->bms_connected == HY_FALSE){
				hy_led_control(led_err);
				lcd_display_chinese(tongxin);
				lcd_display_chinese(cw);
				lcd_display_space();
				goto display_button_check;					
			}
		}

		if (gui_msg->errorstate == HY_GUI_ERR_MASK)
		{
			hy_led_control(led_err);
			LOG_DEBUG_TAG("gui -- ", "status1 [%d] 2 [%d] b [%d]", charger_statu1,charger_statu2,gui_msg->bms_status);
			if(charger_statu1&(1<<0))//交流欠压 电压故障1
			{
				lcd_display_chinese(jiaoliu);
				lcd_display_chinese(qianya);
				lcd_display_space();
			}else if(charger_statu1&(1<<1)){//交流过压 电压故障2
				lcd_display_chinese(jiaoliu);
				lcd_display_chinese(guoya);
				lcd_display_space();
			}else if(charger_statu1&(1<<2)){//输出欠压 电压故障3
				lcd_display_chinese(shuchu);
				lcd_display_chinese(qianya);
				lcd_display_space();
			}else if(charger_statu1&(1<<3)){//输出过压 电压故障4
				lcd_display_chinese(shuchu);
				lcd_display_chinese(guoya);
				lcd_display_space();
			}else if(charger_statu1&(1<<4)){//输出过流 高温状态4
				lcd_display_chinese(shuchu);
				lcd_display_chinese(guoliu);
				lcd_display_space();
			}else if(charger_statu1&(1<<5)){//硬件故障 本地错误
				lcd_display_chinese(shuchu);
				lcd_display_chinese(duanlu);
				lcd_display_space();
			}else if(charger_statu1&(1<<6)){//过温关机
				lcd_display_chinese(guowen);
				lcd_display_chinese(guanji);
				lcd_display_space();
			}else if(charger_statu1&(1<<7)){//硬件故障
				lcd_display_chinese(yingjian);
				lcd_display_chinese(guzhang);
				lcd_display_space();
			}else if(charger_statu2&(1<<1)){//输出反接
				lcd_display_chinese(shuchu);
				lcd_display_chinese(fanjie);
				lcd_display_space();
			}else if(charger_statu2&(1<<2)){//原边降额
				lcd_display_chinese(yuanbian);
				lcd_display_chinese(jiange);
				lcd_display_space();
			}else if(charger_statu2&(1<<3)){//风扇故障
				lcd_display_chinese(fengshan);
				lcd_display_chinese(guzhang);
				lcd_display_space();
			}else if(charger_statu2&(1<<4)){//输出反接
				lcd_display_chinese(guowen);
				lcd_display_chinese(jiange);
				lcd_display_space();
			}else if(gui_msg->bms_status&(1<<1)){//bms 故障 电池过温
				lcd_display_chinese(dian);
				lcd_display_chinese(chi);
				lcd_display_chinese(guowen);
				lcd_display_space();
			}else if(gui_msg->bms_status&(1<<2)){//bms 故障 电池低温
				lcd_display_chinese(dian);
				lcd_display_chinese(chi);
				lcd_display_chinese(diwen);
				lcd_display_space();
			}else if(gui_msg->bms_status&(1<<3)){//bms 故障 充电过流
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
				lcd_display_chinese(guoliu);
				lcd_display_space();
			}else if(gui_msg->bms_status&(1<<4)){//bms 故障 绝缘故障
				lcd_display_chinese(jueyuan);
				lcd_display_chinese(guzhang);
				lcd_display_space();
			}else if(gui_msg->bms_status&(1<<6)){//bms 电池故障
				lcd_display_chinese(dian);
				lcd_display_chinese(chi);
				lcd_display_chinese(guzhang);
				lcd_display_space();
			}
			goto display_button_check;
			
		}
		
		switch(gui_msg->workstate)
		{
			case HY_GUI_CAN_ON_MASK: //通讯充电中
				hy_led_control(led_running);
				lcd_display_chinese(tongxin);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
				lcd_display_space();
			break;
			case HY_GUI_CHARGETASK_ON_MASK:       //本地充电
				hy_led_control(led_running);
				lcd_display_chinese(bd);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
				lcd_display_space();
			break;
			case HY_GUI_CHARGETASK_END_MASK:      //充电完成
				hy_led_control(led_fullcharge);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
				lcd_display_chinese(wancheng);
				lcd_display_space();
			break;
			case HY_GUI_CHARGETASK_STOP_MASK:     //充电停止
				hy_led_control(led_running);
				lcd_display_chinese(chong);
				lcd_display_chinese(dian);
				lcd_display_chinese(tingzhi);
				lcd_display_space();
			break;
			case HY_GUI_CHARGETASK_IDLE:          //空闲状态
			break;

		}

		/*todo errmsg display!!!*/

display_button_check:		
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					if (gui_msg->battery_connected == HY_TRUE){//电池拔掉才允许设置
						return PassportPage1;
					}else{
						return PassportPage;
					}
					break;
				case button_off:/*stop charge*///0x06
					LOG_INFO_TAG(HY_LOG_TAG,"machine stopped by button");
					hy_chargetask_pause(CHARGETASK_BUTTON_STOP_CODE,NULL);
					return DisplayPage1_1;

				case button_on://0x05

				break;
				case button_up://0x02
				/*todo record of charge*/
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
				break;
			}
		}	


		return DisplayPage1;
}



PAGE displaypage1_1(chargetask_gui_msg* gui_msg,
	uint32_t vol,
	uint32_t cur,
	uint32_t time)
{
	uint32_t flag = 0;
	lcd_clear_5s();
	

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


	lcd_display_chinese_at(0,3,zt);
	lcd_display_colon();
	lcd_display_chinese(tj);
	lcd_display_chinese(zhong);

	hy_led_control(led_offall);	

		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					if (gui_msg->battery_connected == HY_TRUE){
						return PassportPage1;
					}else{
						return PassportPage;
					}

				case button_off:/*stop charge*///0x06
				break;
				case button_on://0x05
					LOG_INFO_TAG(HY_LOG_TAG,"machine startted by button");
					hy_chargetask_continue(HY_CONTROLSTYLE_CAN,NULL);
					return DisplayPage1_2;

				case button_up://0x02
				/*todo record of charge*/
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
				break;
			}
		}			
		return DisplayPage1_1;
	


}

PAGE displaypage1_2(PAGE father_page)
{
	uint32_t updatetime_ms = 0;
	lcd_clear();

	lcd_display_chinese_at(0,0,kaishi);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	write_data(0x2e);
	hy_delay_ms(1);
	write_data(0x2e);
	hy_delay_ms(1);
	write_data(0x2e);
	hy_delay_ms(1);

	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) <= 1500);
	
	return DisplayPage1;

}

PAGE displaypage1_3(PAGE father_page)
{
	
	lcd_clear();

	lcd_display_chinese_at(0,0,ru);
	lcd_display_chinese(guo);
	lcd_display_chinese(sz);
	lcd_display_chinese(wancheng);
	lcd_display_chinese(douhao);

	lcd_goto_pos(1,0);
	lcd_display_chinese(qing);
	lcd_display_chinese(duan);
	lcd_display_chinese(dian);
	lcd_display_chinese(chongqi);
	lcd_display_chinese(juhao);

	lcd_goto_pos(2,0);
	lcd_display_chinese(fanhui);
	lcd_display_chinese(sz);
	lcd_display_chinese(jiemian);
	lcd_display_chinese(juhao);

	lcd_goto_pos(3,0);
	lcd_display_chinese(qing);
	lcd_display_chinese(an);
	lcd_display_ascii("E");
	lcd_display_ascii("S");
	lcd_display_ascii("C");


	while(1){
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
				break;
				case button_off:/*stop charge*///0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
				/*todo record of charge*/
				break;
				case button_down://0x03
				break;
				case button_esc://0x04
					return SettingMainPage1;

			}
		}		
	}

}

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
			updatetime_ms = hy_time_now_ms();
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
					hy_delay_ms(1500);
					return DisplayPage1;
			
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
				hy_delay_ms(1500);
				return DisplayPage1;					
			}
		}else if (index > 6){
				lcd_clear();
				lcd_display_chinese_at(2,2,mmcw);
				hy_delay_ms(1500);
				return DisplayPage1;	
		}	

	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE passportpage1(void)
{
	uint32_t updatetime_ms;

	lcd_clear();

    lcd_display_chinese_at(0,0,qing);
    lcd_display_chinese(xian);
    lcd_display_chinese(badiao);
    lcd_display_chinese(dian);
    lcd_display_chinese(chi);

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
	
	uint8_t index;
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
	while(systime_elapse_ms(updatetime_ms) < HY_SETTING_PAGE_TIMEOUT_MS){

		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					if(index > 7){
						break;
					}
					lcd_cursor_close();
					return (PAGE)(index+SettingPage11);					
				
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
					if (index >= 6){
						index = 6;
					}
					if(index == 4){
						lcd_cursor_close();
						return SettingMainPage2;//next page
					}
				break;
				case button_esc://0x04
					lcd_cursor_close();
					return DisplayPage1;	
			
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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
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
						
					}
					return datasettingpage(dataname,SettingPage11);
		
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
	index = index;
	
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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_1_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage12 index chargetimeout_1_min");
					return datasettingpage(dataname,SettingPage12);
			
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage11;
			
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;
			
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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
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
					
					}
					return datasettingpage(dataname,SettingPage21);
		
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
	index = index;
	
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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_2_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage22 index chargetimeout_2_min");
					return datasettingpage(dataname,SettingPage22);
		
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage21;
			
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;

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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
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
					
					}
					return datasettingpage(dataname,SettingPage31);
		
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
	index = index;
	
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

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					dataname = chargetimeout_3_min;
					LOG_INFO_TAG(HY_LOG_TAG,"gui settingpage32 index chargetimeout_3_min");
					return datasettingpage(dataname,SettingPage32);
		
				case button_off://0x06
				break;
				case button_on://0x05
				break;
				case button_up://0x02
					return SettingPage31;
		
				case button_down://0x03
				break;
				case button_esc://0x04
					return father_page;
		
			}
		}
	}	
	lcd_cursor_close();
	return DisplayPage1;

}

PAGE settingpage4(PAGE father_page){

	int8_t index;
	int8_t indexmax;
	uint32_t updatetime_ms;
	data_name dataname;
	
	updatetime_ms = hy_time_now_ms();
	lcd_clear();
	lcd_goto_pos(0,0);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_chinese(lc);//电压量程
	lcd_display_colon();		
	lcd_display_num4(config_data->voltagerange*10,'V');

	lcd_goto_pos(1,0);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);
	lcd_display_chinese(lc);//电流量程
	lcd_display_colon();		
	lcd_display_num4(config_data->currentrange*10,'A');

	lcd_goto_pos(2,0);
	lcd_display_chinese(kz);
	lcd_display_chinese(fs);//控制方式
	lcd_display_colon();
	if(config_data->controlstyle == HY_CONTROLSTYLE_CAN){
		lcd_display_chinese(tongxin);
	}else{
		lcd_display_chinese(bd);
	}

	lcd_goto_pos(3,0);
	lcd_display_chinese(tongxin);
	lcd_display_chinese(sl);//通信速率
	lcd_display_colon();
	lcd_display_botelv4(config_data->communicaterate/1000);

	indexmax = 3;



	index = 0;
	lcd_cursor_goto(index,0);
	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 2:
							if (HY_TRUE){
								dataname = controlstyle;
								LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set controlstyle");
								return datasettingpage(dataname,SettingPage4);
							}else{
								return errorpage3(SettingPage4);
							}

						case 0:
							dataname = voltagerange;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set voltagerange");
							return datasettingpage(dataname,SettingPage4);
						case 1:
							dataname = currentrange;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set currentrange");
							return datasettingpage(dataname,SettingPage4);
						case 3:
							if (HY_TRUE){
								dataname = communicaterate;
								LOG_INFO_TAG(HY_LOG_TAG,"settingpage4 set communicationrate");
								return datasettingpage(dataname,SettingPage4);
							}else{
								return errorpage3(SettingPage4);
							}
					}
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
					if (index >= indexmax){
						index = indexmax;
					}
					lcd_cursor_goto(index,0);
				break;
				case button_esc://0x04
					return father_page;
			
			}
		}					
	}

	lcd_cursor_close();
	return DisplayPage1;
}

/*
*出厂参数
*/
PAGE settingpage5(PAGE father_page){
	int8_t index;
	uint32_t updatetime_ms;

	index = 0;
	updatetime_ms = hy_time_now_ms();
	lcd_clear();
	lcd_goto_pos(0,1);
	lcd_display_chinese(cc);
	lcd_display_chinese(cs);
	lcd_display_chinese(sz);

	lcd_goto_pos(1,0);
	lcd_display_index(1);
	lcd_display_chinese(hf);
	lcd_display_chinese(cc);
	lcd_display_chinese(cs);

	lcd_goto_pos(2,0);
	lcd_display_index(2);
	lcd_display_chinese(sz);
	lcd_display_chinese(cc);
	lcd_display_chinese(cs);

	index = 1;
	lcd_cursor_goto(index,0);

	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){	
				
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			updatetime_ms = hy_time_now_ms();
			switch (s_gui->button_msg_queue[0].button_name){
				case button_set://0x01
					switch(index){
						case 1:
							hy_config_reset();
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage5 index setting original parameters");
							return DisplayPage1;
				
						case 2:
							s_gui->set_in_flash = true;
							LOG_INFO_TAG(HY_LOG_TAG,"settingpage5 index reset parameters");
							return SettingMainPage1;
					
						default:
							LOG_ERROR_TAG(HY_LOG_TAG,"settingpage5 error index = [%d]!!",index);
							return DisplayPage1;
					
					}
			
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
					if (index >= 2){
						index = 2;
					}
					lcd_cursor_goto(index,0);	
				break;
				case button_esc://0x04
					return father_page;
			
			}
		}		

	}

	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage6(PAGE father_page){
	data_name dataname = balancecurrent;
	return datasettingpage(dataname,father_page);
}

PAGE settingpage7(PAGE father_page){
	uint32_t updatetime_ms;

	updatetime_ms = hy_time_now_ms();
	lcd_clear();
	lcd_goto_pos(0,0);
	lcd_display_chinese(cp);
	lcd_display_chinese(xh);
	lcd_display_colon();
	lcd_display_ascii("BC2000");
	lcd_goto_pos(1,0);
	lcd_display_chinese(rj);
	lcd_display_chinese(bb);
	lcd_display_colon();
	lcd_display_ascii("V2.0.1");
	lcd_goto_pos(2,0);
	lcd_display_chinese(cj);
	lcd_display_colon();
	lcd_display_chinese(tianj);
	lcd_display_chinese(hy);
	lcd_goto_pos(3,0);
	lcd_display_chinese(dh);
	lcd_display_colon();
	lcd_display_ascii("15102299468");

	while(systime_elapse_ms(updatetime_ms) <= 10000){
		if(s_gui->button_flag == BUTTON_MSG)
		{
			s_gui->button_flag = NO_MSG;
			return father_page;
		}	
	}
	return father_page;
}

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
			if (data >= 9999){
				data = 9999;
			}else if (data <= 0){
				data = 0;
			}
			num[0] = data/1000;
			num[1] = (data%1000)/100;
			num[2] = (data%100)/10;
			num[4] = data%10;
			num_index = 0;
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->voltagerange = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set voltagerange  [%d]x0.1V ",data);
							if (HY_TRUE){
								hy_config_writevoltagerange(config_data->voltagerange);
								Config_DataWriting();
							}
							return father_page;
						
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
						
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;

    

		case currentrange:
	   		lcd_goto_pos(0,0);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);
	    	lcd_display_chinese(lc);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->currentrange*10;
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
			lcd_display_num4_forset(data,'A');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->currentrange = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set currentrange  [%d]x0.1A ",data);
							if (HY_TRUE){
								hy_config_writecurrentrange(config_data->currentrange);
								hy_config_datawriting();
							}
							return father_page;
						
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
					
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
	

		case balancecurrent:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(ph);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->balancecurrent*10;
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
			lcd_display_num4_forset(data,'A');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->balancecurrent = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set balancecurrent  [%d]x0.1A ",data);
							if (HY_TRUE){
								hy_config_writebalancecurrent(config_data->balancecurrent);
								hy_config_datawriting();
							}
							return father_page;
				
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
				
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		

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
			lcd_display_chinese(tongxin);
			lcd_display_chinese(ms);

			if(HY_TRUE){
				lcd_goto_pos(3,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			index = 1;
			if (config_data->controlstyle == HY_CONTROLSTYLE_LOCAL){
				index = 1;
			}else if(config_data->controlstyle == HY_CONTROLSTYLE_CAN){
				index = 2;
			}		
			lcd_cursor_goto(index,0);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
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
							if (HY_TRUE){
								hy_config_writectrlstyle(config_data->controlstyle);
								hy_config_datawriting();
							}
							return father_page;
					
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
					
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
		

		case chargecurrent_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargecurrent_1*10;
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
			lcd_display_num4_forset(data,'A');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargecurrent_1 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage set chargecurrent_1 [%d]x0.1A",data);
							if (HY_TRUE){
								hy_config_writechargecurrent_1(config_data->chargecurrent_1);
								hy_config_datawriting();
							}
							return father_page;
					
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

					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
	

		case limitvoltage_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitvoltage_1*10;
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
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitvoltage_1 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitvoltage_1 set [%d]x0.1V",data);
							if (HY_TRUE){
								hy_config_writelimitvoltage_1(config_data->limitvoltage_1);
								hy_config_datawriting();
							}
							return father_page;
				
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
				
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


		case chargetimeout_1_min:
			lcd_display_chinese_at(0,0,bz);
			lcd_display_chinese(yi);
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargetimeout_1_min;
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
			lcd_display_time4_forset(data);

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_1_min = data;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_1_min set [%d] min",data);
							if (HY_TRUE){
								hy_config_writechargetimeout_1(config_data->chargetimeout_1_min);
								hy_config_datawriting();
							}
							return father_page;
		
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
			
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	

	

		case switchvoltage_1:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();


			lcd_goto_pos(1,0);
			data = config_data->switchvoltage_1*10;
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
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchvoltage_1 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchvoltage_1 set [%d]x0.1V",data);
							if (HY_TRUE){
								hy_config_writeswitchvoltage_1(config_data->switchvoltage_1);
								hy_config_datawriting();
							}
							return father_page;
					
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
		
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	
		

		case chargecurrent_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargecurrent_2*10;
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
			lcd_display_num4_forset(data,'A');


			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}		

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargecurrent_2 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargecurrent_2 set [%d]x0.1A",data);
							if (HY_TRUE){
								hy_config_writechargecurrent_2(config_data->chargecurrent_2);
								hy_config_datawriting();
							}
							return father_page;
				
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
	
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


		case limitvoltage_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitvoltage_2*10;
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
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitvoltage_2 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitvoltage_2 set [%d]x0.1V",data);
							if (HY_TRUE){
								hy_config_writelimitvoltage_2(config_data->limitvoltage_2);
								hy_config_datawriting();
							}
							return father_page;
		
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
	
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


		case chargetimeout_2_min:
			lcd_display_chinese_at(0,0,bz);
			lcd_display_chinese(er);
			lcd_display_chinese(chong);
			lcd_display_chinese(dian);
			lcd_display_chinese(sj);
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargetimeout_2_min;
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
			lcd_display_time4_forset(data);

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_2_min = data;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_2_min set [%d] min",data);
							if (HY_TRUE){
								hy_config_writechargetimeout_2(config_data->chargetimeout_2_min);
								hy_config_datawriting();
							}
							return father_page;
			
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
		
					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	
		

		case switchvoltage_2:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->switchvoltage_2*10;
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
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchvoltage_2 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchvoltage_2 set [%d]x0.1V",data);
							if (HY_TRUE){
								hy_config_writeswitchvoltage_2(config_data->switchvoltage_2);
								hy_config_datawriting();
							}
							return father_page;
			
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
				
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


		case chargevoltage_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(gd);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(ya);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->chargevoltage_3*10;
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
			lcd_display_num4_forset(data,'V');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->chargevoltage_3 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargevoltage_3 set [%d]x0.1V",data);
							if (HY_TRUE){
								hy_config_writechargevoltage_3(config_data->chargevoltage_3);
								hy_config_datawriting();
							}
							return father_page;
			
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
				
					}
				}					
			}
			return DisplayPage1;
		

		case limitcurrent_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(xz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->limitcurrent_3*10;
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
			lcd_display_num4_forset(data,'A');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->limitcurrent_3 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage limitcurrent_3 set [%d]x0.1A",data);
							if (HY_TRUE){
								hy_config_writelimitcurrent_3(config_data->limitcurrent_3);
								hy_config_datawriting();
							}
							return father_page;
						
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
					
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


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

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[3];
							config_data->chargetimeout_3_min = data;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage chargetimeout_3_min set [%d] min",data);
							if (HY_TRUE){
								hy_config_writechargetimeout_3(config_data->chargetimeout_3_min);
								hy_config_datawriting();
							}
							return father_page;

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

					}
				}					
			}	
			lcd_cursor_close();
			return DisplayPage1;	


		case switchcurrent_3:
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tz);
	    	lcd_display_chinese(dian);
	    	lcd_display_chinese(liu);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			data = config_data->switchcurrent_3*10;
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
			lcd_display_num4_forset(data,'A');

			if(HY_TRUE){
				lcd_goto_pos(2,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			lcd_cursor_goto(1,num_index);
			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							data = num[0]*1000+num[1]*100+num[2]*10+num[4];
							config_data->switchcurrent_3 = data/10;
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage switchcurrent_3 set [%d]x0.1A",data);
							if (HY_TRUE){
								hy_config_writeswitchcurrent_3(config_data->switchcurrent_3);
								hy_config_datawriting();
							}
							return father_page;

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

					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;


		case communicaterate:
communicaterate_page1:	
			lcd_clear();	
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tongxin);
	    	lcd_display_chinese(botelv);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			lcd_display_index(1);
			lcd_display_botelv4(50);

			lcd_goto_pos(2,0);
			lcd_display_index(2);
			lcd_display_botelv4(125);

			lcd_goto_pos(3,0);
			lcd_display_index(3);
			lcd_display_botelv4(250);

			index = 1;
			switch(config_data->communicaterate){
				case 50000:
				index = 1;
				break;
				case 125000:
				index = 2;
				break;
				case 250000:
				index = 3;
				break;
				default:
				break;
			}			
			lcd_cursor_goto(index,0);

			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							switch(index){
								case 1:
								config_data->communicaterate = 50000;
								break;
								case 2:
								config_data->communicaterate = 125000;
								break;
								case 3:
								config_data->communicaterate = 250000;
								break;
								default:
								config_data->communicaterate = 250000;
								break;
							}
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage communicaterate [%d]",config_data->communicaterate);
							if (HY_TRUE){
								hy_config_writecommunicaterate(config_data->communicaterate);
								hy_config_datawriting();
							}
							return father_page;

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
								goto communicaterate_page2;
							}
							lcd_cursor_goto(index,0);

						break;
						case button_esc://0x04
							return father_page;

					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;
communicaterate_page2:
			lcd_clear();
	   		lcd_goto_pos(0,0);
	   		lcd_display_chinese(tongxin);
	    	lcd_display_chinese(botelv);	    	
			lcd_display_colon();

			lcd_goto_pos(1,0);
			lcd_display_index(4);
			lcd_display_botelv4(500);

			lcd_goto_pos(2,0);
			lcd_display_index(5);
			lcd_display_botelv4(800);

			if(HY_TRUE){
				lcd_goto_pos(3,0);
				lcd_display_chinese(sz);
				lcd_display_chinese(cc);
				lcd_display_chinese(cs);
				lcd_display_chinese(zhong);				
			}	

			index = 4;
			lcd_cursor_goto(1,0);

			while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){				
				if(s_gui->button_flag == BUTTON_MSG)
				{
					s_gui->button_flag = NO_MSG;
					updatetime_ms = hy_time_now_ms();
					switch (s_gui->button_msg_queue[0].button_name){
						case button_set://0x01
							switch(index){
								case 1:
								config_data->communicaterate = 500000;
								break;
								case 2:
								config_data->communicaterate = 800000;
								break;
								default:
								config_data->communicaterate = 250000;
								break;
							}
							LOG_INFO_TAG(HY_LOG_TAG,"datasettingpage communicaterate [%d]",config_data->communicaterate);
							if (HY_TRUE){
								hy_config_writecommunicaterate(config_data->communicaterate);
								hy_config_datawriting();
							}
							return father_page;

						case button_off://0x06
						break;
						case button_on://0x05
						break;
						case button_up://0x02
							index--;
							if (index < 4){
								goto communicaterate_page1;
							}
							lcd_cursor_goto(index,0);
						break;
						case button_down://0x03
							index++;
							if (index >= 5){
								index = 5;
							}
							lcd_cursor_goto((index-3),0);
						break;
						case button_esc://0x04
							return father_page;
					}
				}					
			}
			lcd_cursor_close();
			return DisplayPage1;

	}
	lcd_cursor_close();
	return DisplayPage1;
}


PAGE errorpage1(void)
{
	uint32_t updatetime_ms;

	lcd_clear();

    lcd_display_chinese_at(0,0,wz);
    lcd_display_chinese(cw);

    lcd_display_chinese_at(0,1,qing);
    lcd_display_chinese(chongqi);

	updatetime_ms = hy_time_now_ms();
	while(systime_elapse_ms(updatetime_ms) <= HY_SETTING_PAGE_TIMEOUT_MS){
		if(s_gui->button_flag == BUTTON_MSG){
			s_gui->button_flag = NO_MSG;
			return DisplayPage1;
		}	
	}
	return DisplayPage1;
}

/**/
PAGE errorpage2(void)
{

	lcd_clear();

    lcd_display_chinese_at(0,0,duqu);
    lcd_display_chinese(sz);
    lcd_display_chinese(cw);
    lcd_display_chinese(douhao);

    lcd_display_chinese_at(0,1,qing);
    lcd_display_chinese(lianxi);
    lcd_display_chinese(cj);
    lcd_display_chinese(juhao);

	while(1);
}

/*请在出厂模式下设置*/
PAGE errorpage3(PAGE father_page)
{
	lcd_clear();

    lcd_display_chinese_at(0,0,qing);
    lcd_display_chinese(zai_2);
    lcd_display_chinese(cc);
    lcd_display_chinese(ms);
    lcd_display_chinese(sz);

	hy_gui_delay_ms(1500);
	return father_page;
}

int page4updatetime_ms=0;
PAGE errorpage4(PAGE father_page)//高温警告
{
	if(systime_elapse_ms(page4updatetime_ms)<10000){
		return father_page;
	}
	page4updatetime_ms = hy_time_now_ms();
	lcd_clear();

    lcd_display_chinese_at(0,0,gw);
    lcd_display_chinese(zt);

	hy_gui_delay_ms(3000);
	return father_page;
}

int page5updatetime_ms=0;
PAGE errorpage5(PAGE father_page)//电流错误
{

	if(systime_elapse_ms(page5updatetime_ms)<10000){
		return father_page;
	}
	page5updatetime_ms = hy_time_now_ms();
	lcd_clear();

    
    lcd_display_chinese_at(0,0,dian);
	lcd_display_chinese(liu);//电流
    lcd_display_chinese(gz);

	hy_gui_delay_ms(3000);
	return father_page;
}


int page6updatetime_ms=0;
PAGE errorpage6(PAGE father_page)//电压错误
{

	if(systime_elapse_ms(page6updatetime_ms)<10000){
		return father_page;
	}
	page6updatetime_ms = hy_time_now_ms();
	lcd_clear();

    
    lcd_display_chinese_at(0,0,dian);
		lcd_display_chinese(ya);//电流
    lcd_display_chinese(gz);

	hy_gui_delay_ms(3000);
	return father_page;
}
