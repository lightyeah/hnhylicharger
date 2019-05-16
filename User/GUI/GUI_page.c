#include "GUI_page.h"
#include "GUI_driver.h"
#include "config.h"
#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "dataprocess.h"
#define FIRST_ADDR 0  

#define key_time 700
#ifndef DEBUG_PRINT
#define DEBUG_PRINT
#define DEBUG_PRINT(format, ...) _printf (format,##__VA_ARGS__)
#endif


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
uint8_t wz[5] = {0xce,0xb4,0xd6,0xaa};//未知
uint8_t js[5] = {0xbd,0xe1,0xca,0xf8};//结束
uint8_t mmcw[9] = {0xc3,0xdc,0xc2,0xeb,0xb4,0xed,0xce,0xf3,0x00};//密码错误
uint8_t qsmm[9] ={0xc7,0xeb,0xca,0xe4,0xc3,0xdc,0xc2,0xeb,0x00}; 

uint8_t pass[6] ={2,2,2,3,3,3};
uint8_t unit[17] = {'A','V','M','V','A','V','M','V','V','A','M','A',' ','V','A','K','%'};
uint16_t max[17] = {300,300,999,300,300,300,999,300,300,300,999,300,1,300,300,4,60};
uint16_t min[17] = {  1,  1,  1,	1,	1,	1,	1,	1,	1,	1,	1,	1,0,	1,	1,0, 0};
uint16_t communicationrate[5] = {50,125,250,500,800};


void show(){
	datasettingpage(3);
}
void showmainsettingitem(uint8_t no){
	write_data(no+0x31);
	delay_ms(1);
	write_data(0x2e);
	delay_ms(1);
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
		delay_ms(1);
		write_data(0x2e);
		delay_ms(1);
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
		delay_ms(5000);
		return DisplayPage1;
}

PAGE displaypage1(uint8_t status,uint16_t vol,uint16_t cur,uint16_t time,uint8_t * flag){
		uint8_t button;
		uint16_t time_cnt;
		uint16_t state;
		time_cnt = 0;
		state = Config_ReadCtrlStyle();
		lcd_clear();
	
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

		lcd_display_chinese_at(0,3,kz);
		lcd_display_colon();

		if(*flag == 0 ){//
			if(state == 0){
					lcd_display_chinese(tx);
			}
			else{
					lcd_display_chinese(bd);
			}
			lcd_display_chinese(tj);//
		}//
		else{
			switch(status){
				case 0:
					if(state == 0){
							lcd_display_chinese(tx);
							lcd_display_chinese(zc);			
					}
					else{
							lcd_display_chinese(bd);
							lcd_display_chinese(zc);			
					}
					break;
				case 1:
					lcd_display_chinese(tx);//通讯错误
					lcd_display_chinese(cw);			
					break;
				case 2:
					lcd_display_chinese(qx);//缺相保护
					lcd_display_chinese(bh);			
					break;
				case 3:
					lcd_display_chinese(gw);//高温保护
					lcd_display_chinese(bh);
					break;
				case 4:
					lcd_display_chinese(dian);//电池保护
					lcd_display_chinese(chi);			
					lcd_display_chinese(bh);			
					break;
				case 5:
					lcd_display_chinese(wz);//未知错误
					lcd_display_chinese(cw);		
					break;

				default:
					break;
			}
		}		
		while(time_cnt<100){
			time_cnt++;
			chargehandler();
			
			button = Button_Check();	
			switch(button){
				case 1:
					if(*flag == 0){
						return PassportPage;
					}
					break;
				case 4:
					//查询
					return DisplayPage2;
				case 5:
					//开始充电/
					*flag = 1;
					return DisplayPage1;
				case 6:
					//停止充电
					*flag = 0;
					return DisplayPage1;
				default:
					delay_ms(10);
					break;
			}
		}
		return DisplayPage1;
}

PAGE displaypage2(uint8_t mode,uint16_t no,uint16_t vol,uint8_t status,uint8_t * flag){
	uint8_t button;
	uint16_t time_cnt;
	time_cnt = 0;
	lcd_clear();
	lcd_display_chinese_at(0,0,ms);
	lcd_display_colon();
	switch(mode){
// #define STATE_IDEL 	                   			00
// #define CAN_STATE                            10
// #define CAN_STOP 		                        11
// #define LOCAL_STATE_CHECK                    20
// #define LOCAL_STATE_ONE                      21
// #define LOCAL_STATE_TWO                      22
// #define LOCAL_STATE_THREE                    23
// #define LOCAL_STATE_END                      24
		case 0:
			lcd_display_chinese(tj);
			break;
		case 24:
			lcd_display_chinese(tj);
			break;
		case 10:
			lcd_display_ascii("CAN ");
			lcd_display_chinese(chong);			
			lcd_display_chinese(dian);			
			break;
		case 11:
		case 12:
			lcd_display_ascii("CAN ");
			lcd_display_chinese(tj);
			break;
		case 20:
			lcd_display_chinese(jc);		
			break;
		case 21:
			lcd_display_chinese(yu);
			lcd_display_chinese(chong);			
			lcd_display_chinese(dian);			
			break;
		case 22:
			lcd_display_chinese(heng);
			lcd_display_chinese(liu);
			lcd_display_chinese(chong);			
			lcd_display_chinese(dian);			
			break;
		case 23:
			lcd_display_chinese(heng);
			lcd_display_chinese(ya);
			lcd_display_chinese(chong);			
			lcd_display_chinese(dian);			
			break;
		default:
			break;
	}
	lcd_display_chinese_at(0,1,dian);
	lcd_display_chinese(chi);
	lcd_display_chinese(bianh);
	lcd_display_colon();
	lcd_display_num4(no,' ');

	lcd_display_chinese_at(0,2,dian);
	lcd_display_chinese(chi);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_colon();
	lcd_display_num4(vol,'V');

// #define  ERR_OK                             0
// #define  ERR_TIMEOUT                        1
// #define  ERR_LACKPHASE                      2
// #define  ERR_OVERHEAT                       3
// #define  ERR_BATTERYPROTECT								 4
// #define  ERR_UNKNOW                         5
	lcd_display_chinese_at(0,3,zt);
	lcd_display_colon();
	switch(status){
		case 0:
			lcd_display_chinese(zc);
			break;
		case 1:
			lcd_display_chinese(tx);
			lcd_display_chinese(cw);			
			break;
		case 2:
			lcd_display_chinese(qx);
			lcd_display_chinese(bh);			
			break;
		case 3:
			lcd_display_chinese(gw);
			lcd_display_chinese(bh);
			break;
		case 4:
			lcd_display_chinese(dian);			
			lcd_display_chinese(chi);			
			lcd_display_chinese(bh);			
			break;
		case 5:
			lcd_display_chinese(wz);			
			lcd_display_chinese(cw);		
			break;

		default:
			break;
	}
	
	while(time_cnt<100){
		time_cnt++;		
		chargehandler();
		button = Button_Check();
		switch(button){
			case 4:
				return DisplayPage1;
				break;
			case 6:
				*flag = 0;
				return DisplayPage1;
				break;
			default:
				delay_ms(10);
				break;
		}
	}
	return DisplayPage2;
}



PAGE passportpage(void){
	uint8_t button,index,passport[6];
	uint16_t time_cnt;
	time_cnt = 0;
	index = 0;
	
	lcd_clear();
	lcd_goto_pos(1,2);
	lcd_display_chinese(qsmm);
	lcd_goto_pos(2,1);	
	lcd_display_ascii(" ");
	while(time_cnt<1000){
		delay_ms(10);
		time_cnt++;
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
			passport[index] = button;
			index++;
			lcd_display_ascii("*");
			lcd_display_ascii(" ");
		}
		if(index == 6){
			if( pass[0]==passport[0] && pass[1]==passport[1] &&
					pass[2]==passport[2] && pass[3]==passport[3] &&
					pass[4]==passport[4] && pass[5]==passport[5]){
						DEBUG_PRINT("right\r\n");
						return SettingMainPage1;
			}
			else{
				lcd_clear();
				lcd_display_chinese_at(2,2,mmcw);
				delay_ms(2000);
				return DisplayPage1;
			}
		}
	}
	return DisplayPage1;
}

PAGE settingmainpage(uint8_t page,uint8_t cursor){//0,1;0,1,2,3
	uint16_t time_cnt;
	uint8_t button,index;
	uint8_t pa;
	time_cnt = 0;
	index = 1;
	
	lcd_clear();
	for(pa = 0 ; pa < 4-page; pa++){
		lcd_goto_pos(pa,0);
		showmainsettingitem(pa+page*4);
	}
	
	lcd_cursor_goto(cursor,0);
	index = cursor+page*4;
	//to do
	
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
			if(button == 1){
				lcd_cursor_close();
				return (PAGE)(index+SettingPage11);
			}
			else if(button == 3){
				index = index+1;
				if(index == 4){
					lcd_cursor_close();
					return SettingMainPage2;//next page
				}
				else if(index == 7){
					index = 6;
				}
			}
			else if(button == 2){
				if(index == 0){
					index = 1;
				}
				index = index-1;
				if(page == 1 && index == 3){
   				lcd_cursor_close();
					return SettingMainPage1;//next page
				}
			}
			else if(button == 4){
				lcd_cursor_close();
				return DisplayPage1;
			}
			lcd_cursor_goto(index-page*4,0);
		}
	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage1(uint8_t page){//0 1
	uint16_t time_cnt;
	uint8_t button,index;
	uint8_t pa;
	time_cnt = 0;
	index = 1;
	
	lcd_clear();
	for(pa = 0 ; pa<4 ; pa++){
		lcd_goto_pos(pa,0);
		showsettingitem(0,pa+page);
	}
	
	if(page == 0){
		index = 1;
		lcd_cursor_goto(1,0);
	}
	else{
		index = 4;
		lcd_cursor_goto(3,0);		
	}
	
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		switch(button){
			case 1:
				lcd_cursor_close();
				return datasettingpage(index-1);
				break;
			case 2:
				index = (index>1?index-1:1);
				if(page == 1 && index == 1){
					lcd_cursor_close();
					return SettingPage11;//next page
				}
				break;
			case 3:
				index = (index<4?index+1:4);
				if(page == 0 && index == 4){
					lcd_cursor_close();
					return SettingPage12;//next page
				}
				break;
			case 4:
				lcd_cursor_close();
				return SettingMainPage1;
				break;
			default:
				break;
		}
		if(button != 0){
			time_cnt = 0;
			lcd_cursor_goto(index-page,0);
		}
	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage2(uint8_t page){//0 1
	uint16_t time_cnt;
	uint8_t button,index;
	uint8_t pa;
	time_cnt = 0;
	index = 1;
	
	lcd_clear();
	for(pa = 0 ; pa<4 ; pa++){
		lcd_goto_pos(pa,0);
		showsettingitem(1,pa+page);
	}
	
	if(page == 0){
		index = 1;
		lcd_cursor_goto(1,0);
	}
	else{
		index = 4;
		lcd_cursor_goto(3,0);		
	}
	
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		switch(button){
			case 1:
				lcd_cursor_close();
				return datasettingpage(index+3);
				break;
			case 2:
				index = (index>1?index-1:1);
				if(page == 1 && index == 1){
					lcd_cursor_close();
					return SettingPage21;//next page
				}
				break;
			case 3:
				index = (index<4?index+1:4);
				if(page == 0 && index == 4){
					lcd_cursor_close();
					return SettingPage22;//next page
				}
				break;
			case 4:
				lcd_cursor_close();
				return SettingMainPage1;
				break;
			default:
				break;
		}
		if(button != 0){
			time_cnt = 0;
			lcd_cursor_goto(index-page,0);
		}
	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage3(uint8_t page){//0 1
	uint16_t time_cnt;
	uint8_t button,index;
	uint8_t pa;
	time_cnt = 0;
	index = 1;
	
	lcd_clear();
	for(pa = 0 ; pa<4 ; pa++){
		lcd_goto_pos(pa,0);
		showsettingitem(2,pa+page);
	}
	
	if(page == 0){
		index = 1;
		lcd_cursor_goto(1,0);
	}
	else{
		index = 4;
		lcd_cursor_goto(3,0);		
	}
	
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		switch(button){
			case 1:
				lcd_cursor_close();
				return datasettingpage(index+7);
				break;
			case 2:
				index = (index>1?index-1:1);
				if(page == 1 && index == 1){
					lcd_cursor_close();
					return SettingPage31;//next page
				}
				break;
			case 3:
				index = (index<4?index+1:4);
				if(page == 0 && index == 4){
					lcd_cursor_close();
					return SettingPage32;//next page
				}
				break;
			case 4:
				lcd_cursor_close();
				return SettingMainPage1;
				break;
			default:
				break;
		}
		if(button != 0){
			time_cnt = 0;
			lcd_cursor_goto(index-page,0);
		}
	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage4(){
	uint16_t time_cnt;
	uint8_t button,index;
	uint16_t data1,data2,data3,data4;
	time_cnt = 0;
	index = 0;
	data1 = ConfigReadFromAddr(12);
	data2 = ConfigReadFromAddr(13);
	data3 = ConfigReadFromAddr(14);
	data4 = ConfigReadFromAddr(15);
	
	lcd_clear();
	lcd_goto_pos(0,0);
	write_data(0x31);
	delay_ms(1);
	write_data(0x2e);
	delay_ms(1);
	lcd_display_chinese(kz);
	lcd_display_chinese(fs);
	lcd_display_colon();
	if(data1 == 0){
		lcd_display_chinese(tx);
	}
	else{
		lcd_display_chinese(bd);
	}
	lcd_goto_pos(1,0);
	write_data(0x32);
	delay_ms(1);
	write_data(0x2e);
	delay_ms(1);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_chinese(lc);
	lcd_display_colon();		
	lcd_display_num3(data2,unit[13]);
	lcd_goto_pos(2,0);
	write_data(0x33);
	delay_ms(1);
	write_data(0x2e);
	delay_ms(1);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);
	lcd_display_chinese(lc);
	lcd_display_colon();		
	lcd_display_num3(data3,unit[14]);
	lcd_goto_pos(3,0);
	write_data(0x34);
	delay_ms(1);
	write_data(0x2e);
	delay_ms(1);
	lcd_display_chinese(tx);
	lcd_display_chinese(sl);
	lcd_display_colon();
	lcd_display_num3(communicationrate[data4],unit[15]);
	lcd_cursor_goto(0,0);
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		switch(button){
			case 1:
				lcd_cursor_close();
				return datasettingpage(index+12);
				break;
			case 2:
				index = (index+4-1)%4;
				break;
			case 3:
				index = (index+1)%4;
				break;
			case 4:
				lcd_cursor_close();
				return SettingMainPage1;
				break;
			default:
				break;
		}
		if(button != 0){
			time_cnt = 0;				
			lcd_cursor_goto(index,0);
		}
	}
	lcd_cursor_close();
	return DisplayPage1;
}

PAGE settingpage5(){
	uint16_t time_cnt;
	uint8_t button,index;
	uint8_t passport[6];	
	time_cnt = 0;
	index = 0;
	lcd_clear();
	lcd_goto_pos(0,1);
	lcd_display_chinese(hf);
	lcd_display_chinese(cc);
	lcd_display_chinese(cs);
	lcd_goto_pos(1,2);
	lcd_display_chinese(qsmm);
	lcd_goto_pos(2,1);	
	lcd_display_ascii(" ");
	while(time_cnt<1000){
		delay_ms(10);
		time_cnt++;
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
			passport[index] = button;
			index++;
			lcd_display_ascii("*");
			lcd_display_ascii(" ");
		}
		if(index == 6){
			if( pass[0]==passport[0] && pass[1]==passport[1] &&
					pass[2]==passport[2] && pass[3]==passport[3] &&
					pass[4]==passport[4] && pass[5]==passport[5]){
						Config_EraseConfig();
						delay_ms(50);
						Config_DataReading();
			}
			else{
				lcd_clear();
				lcd_display_chinese_at(2,1,mmcw);
				delay_ms(2000);
			}
			return SettingMainPage2;
		}
	}
	return DisplayPage1;
}

PAGE settingpage6(){
	uint16_t time_cnt;
	uint8_t button,data;
	time_cnt = 0;
	lcd_clear();
	lcd_goto_pos(0,0);
	lcd_display_chinese(dian);
	lcd_display_chinese(liu);
	lcd_display_chinese(ph);
	lcd_goto_pos(1,0);
	lcd_display_chinese(cs);
	lcd_display_colon();
	data = ConfigReadFromAddr(16);
	lcd_display_num2_big(data,unit[16]);
	lcd_goto_pos(2,0);
	lcd_display_chinese(yx);
	lcd_display_chinese(zt);
	lcd_display_colon();
	lcd_display_chinese(tj);
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
		}
		switch(button){
			case 1:
				return datasettingpage(16);
				break;
			case 4:
				return SettingMainPage2;
				break;
		}
	}
	return DisplayPage1;
}

PAGE settingpage7(){
	uint16_t time_cnt;
	uint8_t button;
	time_cnt = 0;
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
	lcd_display_ascii("V1.0.2");
	lcd_goto_pos(2,0);
	lcd_display_chinese(cj);
	lcd_display_colon();
	lcd_display_chinese(tianj);
	lcd_display_chinese(hy);
	lcd_goto_pos(3,0);
	lcd_display_chinese(dh);
	lcd_display_colon();
	lcd_display_ascii("15102299468");
	while(time_cnt<1000){
		time_cnt++;		
		delay_ms(10);
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
			if(button == 1  || button == 4){
				return SettingMainPage2;
			}
		}
	}
	return DisplayPage1;
}

PAGE datasettingpage(uint8_t ID){//0~16
	uint16_t data,time_cnt;
	uint8_t button,index,databit[3];
	time_cnt = 0;
	index = 0;
	if(ID ==12){
		return modesettingpage();
	}
	else if(ID == 15){
		return communicationratesettingpage();
	}
	else if(ID ==16){
		return balancesettingpage();
	}
	else{
		data = ConfigReadFromAddr(ID);
		databit[0] = data/100;
		databit[1] = data/10%10;
		databit[2] = data%10;
		
		lcd_clear();
		lcd_goto_pos(1,3);
		lcd_display_chinese(sz);
		lcd_goto_pos(2,2);

		lcd_display_num3_big(data,unit[ID]);
		lcd_cursor_goto(2,2);
		while(time_cnt<1000 && index<3){
			delay_ms(10);
			time_cnt++;
			button = Button_Check();
			if(button != 0){
				time_cnt = 0;
			}
			switch(button){
				case 1:
					index++;
					lcd_cursor_goto(2,2+index);			
					break;
				case 3:
					lcd_cursor_goto(2,2);			
					databit[index] = (databit[index]+10-1)%10;
					data = databit[0]*100+databit[1]*10+databit[2];
					lcd_display_num3_big(data,unit[ID]);
					lcd_cursor_goto(2,2+index);			
					break;
				case 2:
					lcd_cursor_goto(2,2);			
					databit[index] = (databit[index]+1)%10;
					data = databit[0]*100+databit[1]*10+databit[2];
					lcd_display_num3_big(data,unit[ID]);
					lcd_cursor_goto(2,2+index);			
					break;
				case 4:
					lcd_cursor_close();
					switch(ID){
						case 0:
						case 1:
						case 2:
							return SettingPage11;//7
							break;
						case 3:
							return SettingPage12;//14
							break;
						case 4:
						case 5:
						case 6:
							return SettingPage21;//8
							break;
						case 7:
							return SettingPage22;//15
							break;
						case 8:
						case 9:
						case 10:
							return SettingPage31;//9
							break;
						case 11:
							return SettingPage32;//16
							break;
						case 12:
						case 13:
						case 14:
						case 15:
							return SettingPage4;//10
							break;
						case 16:
							return SettingPage6;//12
							break;
						default:
							return DisplayPage1;
							break;
					}
			}
		}	
		lcd_cursor_close();
		if(index == 3){
			if(data > max[ID])
				data = max[ID];
			if(data < min[ID])
				data = min[ID];
			ConfigWriteToAddr(ID,data);
			Config_DataWriting();
			switch(ID){
				case 0:
				case 1:
				case 2:
					return SettingPage11;//7
				case 3:
					return SettingPage12;//14
				case 4:
				case 5:
				case 6:
					return SettingPage21;//8
				case 7:
					return SettingPage22;//15
				case 8:
				case 9:
				case 10:
					return SettingPage31;//9
				case 11:
					return SettingPage32;//16
				case 12:
				case 13:
				case 14:
				case 15:
					return SettingPage4;//10
					break;
				case 16:
					return SettingPage6;//12
				default:
					return DisplayPage1;
			}
		}
		else{
			return DisplayPage1; 
		}
	}
}


PAGE modesettingpage(void){
	uint16_t data,time_cnt;
	uint8_t button,index;
	time_cnt = 0;
	data = ConfigReadFromAddr(12);	
	index = data;
	lcd_clear();
	lcd_goto_pos(0,3);
	lcd_display_chinese(sz);
	lcd_goto_pos(2,2);
	lcd_display_ascii("1 . ");
	lcd_display_chinese(tx);
	lcd_goto_pos(3,2);
	lcd_display_ascii("2 . ");
	lcd_display_chinese(bd);
	if(data ==0)
		lcd_cursor_goto(2,2);
	else
		lcd_cursor_goto(3,2);

	while(time_cnt<1000){
		delay_ms(10);
		time_cnt++;
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
		}
		switch(button){
			case 1:
				ConfigWriteToAddr(12,index);
				Config_DataWriting();
				return SettingPage4;//12
				break;
			case 3:
			case 2:
				index = (index+1)%2;
				lcd_cursor_goto(2+index,2);			
				break;
			case 4:
				lcd_cursor_close();
				return SettingPage4;//12
		}
	}
	lcd_cursor_close();
	return DisplayPage1; 
}	


PAGE communicationratesettingpage(void){
	uint16_t data,time_cnt;
	uint8_t button,index;
	uint8_t no[1];
	
	time_cnt = 0;
	data = ConfigReadFromAddr(15);	
	lcd_clear();
	lcd_goto_pos(0,3);
	lcd_display_chinese(sz);
	for(index = 0 ;index<5;index++){
		lcd_goto_pos(index/2+1,4*(index%2));
		no[0]=0x31+index;
		lcd_display_ascii(no);
		lcd_display_ascii(".");
		lcd_display_num3(communicationrate[index],'K');
	}
	index = data;
	lcd_cursor_goto(index/2+1,4*(index%2));

	while(time_cnt<1000){
		delay_ms(10);
		time_cnt++;
		button = Button_Check();
		if(button != 0){
			time_cnt = 0;
		}
		switch(button){
			case 1:
				ConfigWriteToAddr(15,index);
				Config_DataWriting();
				return SettingPage4;//12

			case 3:
				index = (index+1)%5;
				lcd_cursor_goto(index/2+1,4*(index%2));
				break;
			case 2:
				index = (index-1)%5;
				lcd_cursor_goto(index/2+1,4*(index%2));
				break;
			case 4:
				lcd_cursor_close();
				return SettingPage4;//12
		}
	}
	lcd_cursor_close();
	return DisplayPage1; 
}	


PAGE balancesettingpage(void){
	uint16_t data,time_cnt;
	uint8_t button;
	time_cnt = 0;
	data = ConfigReadFromAddr(16);	
	lcd_clear();
	lcd_goto_pos(1,3);
	lcd_display_chinese(sz);
	lcd_goto_pos(2,2);
	lcd_display_num2_big(data,'%');

	while(time_cnt<1000){
		delay_ms(10);
		time_cnt++;
		button = Button_Check();
		switch(button){
			case 1:
				ConfigWriteToAddr(16,data);
				Config_DataWriting();
				return SettingPage6;//12
				break;
			case 3:
				if(data>0)
					data--;
				break;
			case 2:
				if(data<60)
					data++;
				break;
			case 4:
				return SettingPage6;//12
		}
		if(button != 0){
			time_cnt = 0;
			lcd_goto_pos(2,2);
			lcd_display_num2_big(data,'%');
		}
	}
	lcd_cursor_close();
	return DisplayPage1; 
}	

PAGE errorpage(uint8_t err){
	
// #define  ERR_TIMEOUT                        1
// #define  ERR_LACKPHASE                      2
// #define  ERR_OVERHEAT                       3

	uint8_t button;
	lcd_clear();
	lcd_goto_pos(1,2);
	if(err == 1){
		lcd_display_chinese(tx);
	}
	else if(err == 2){
		lcd_display_chinese(qx);
	}
	else if(err == 3){
		lcd_display_chinese(gw);
	}
	lcd_display_chinese(gz);
	while(1){
		button = Button_Check();
		delay_ms(20);
		if(button == 4){
			return DisplayPage2; 
		}
	}
}


PAGE finishpage(uint32_t vol,uint32_t time){
	uint8_t button;
	lcd_clear();
	lcd_goto_pos(1,2);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	lcd_display_chinese(js);
	lcd_goto_pos(2,1);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_colon();
	lcd_display_num3(vol/10,'V');
	lcd_goto_pos(3,1);
	lcd_display_chinese(sj);
	lcd_display_colon();
	lcd_display_num3(time,'M');	
	while(1){
		button = Button_Check();
		delay_ms(20);
		if(button == 4){
			return DisplayPage1; 
		}
	}
}
PAGE finishpage2(uint32_t vol,uint32_t time,uint8_t * flag){
	uint16_t time_cnt;
	uint8_t button;
	time_cnt =0;
	lcd_clear();
	lcd_goto_pos(1,2);
	lcd_display_chinese(chong);
	lcd_display_chinese(dian);
	lcd_display_chinese(js);
	lcd_goto_pos(2,1);
	lcd_display_chinese(dian);
	lcd_display_chinese(ya);
	lcd_display_colon();
	lcd_display_num3(vol/10,'V');
	lcd_goto_pos(3,1);
	lcd_display_chinese(sj);
	lcd_display_colon();
	lcd_display_num3(time,'M');	
	chargehandler();
	while(time_cnt<100){
		time_cnt++;
		button = Button_Check();
		delay_ms(10);
		if(button == 6){
			*flag = 0;
			return DisplayPage1; 
		}
	}
	return DisplayPage1;
	
}

