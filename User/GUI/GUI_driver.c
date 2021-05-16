
#include "GUI_driver.h"
#include "hy_dbg.h"
#include "hy_systime.h"

#include "string.h"
#define HY_LOG_TAG  "gui_driver"

static hy_gui_t* s_gui = NULL;

#define uchar uint8_t
#define uint  uint16_t


#define  MAX_X  128
#define  MAX_Y  64   

/*********************** Hardware specific configuration **********************/

/* 8bit to 16bit LCD Interface

   PINS:
   - RS      =  44  P1.27
   - EN      =  43  P1.28
   - PSB/CS1 =  40  P1.25
   - DB[0.7] =  95 94 93 92 91 90 89 88  
                P1. 0 1 4 8 9 10 14 15                                                     */

#define PIN_RS    (1 << 27)  
#define PIN_EN    (1 << 28)  
#define PIN_PSB   (1 << 25)  
#define PIN_BUS0  (1 << 0 )
#define PIN_BUS1  (1 << 1 )
#define PIN_BUS2  (1 << 4 )
#define PIN_BUS3  (1 << 8 )
#define PIN_BUS4  (1 << 9 )
#define PIN_BUS5  (1 << 10)
#define PIN_BUS6  (1 << 14)
#define PIN_BUS7  (1 << 15)
/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    8

/*--------------- Graphic LCD interface hardware definitions -----------------*/
/* Pin RS setting to 0 or 1                                                   */
#define LCD_RS(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_RS)  : (LPC_GPIO1->FIOCLR = PIN_RS));
/* Pin EN setting to 0 or 1                                                   */
#define LCD_EN(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_EN)  : (LPC_GPIO1->FIOCLR = PIN_EN));
/* Pin DIR setting to 0 or 1                                                   */
#define LCD_PSB(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_PSB) : (LPC_GPIO1->FIOCLR = PIN_PSB));

/* Pin LCD_BUS setting to 0 or 1                                                   */
#define LCD_BUS0(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS0) : (LPC_GPIO1->FIOCLR = PIN_BUS0));
#define LCD_BUS1(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS1) : (LPC_GPIO1->FIOCLR = PIN_BUS1));
#define LCD_BUS2(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS2) : (LPC_GPIO1->FIOCLR = PIN_BUS2));
#define LCD_BUS3(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS3) : (LPC_GPIO1->FIOCLR = PIN_BUS3));
#define LCD_BUS4(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS4) : (LPC_GPIO1->FIOCLR = PIN_BUS4));
#define LCD_BUS5(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS5) : (LPC_GPIO1->FIOCLR = PIN_BUS5));
#define LCD_BUS6(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS6) : (LPC_GPIO1->FIOCLR = PIN_BUS6));
#define LCD_BUS7(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS7) : (LPC_GPIO1->FIOCLR = PIN_BUS7));


void delay(uint t){
  while(t--);
}

void hy_gui_delay_ms(uint32_t ms)
{
	static uint32_t systime = 0;
	systime = hy_time_now_ms();
	while(systime_elapse_ms(systime)<=ms);
}
void LCD_Configuration(void)
{
    /* Configure the LCD Control pins
        set 
        P1[0 1 4 8 9 10 14 15]
        as output.
    */
	FIO_SetDir(1,(1<<25)|(1<<27)|(1<<28),1);
  FIO_SetDir(1,(1<<0)|(1<<1)|(1<<8)|(1<<4)|(1<<9)|(1<<10)|(1<<14)|(1<<15),1);
  FIO_SetValue(1,(1<<25)|(1<<27)|(1<<28));
	FIO_SetValue(1,(1<<0)|(1<<1)|(1<<8)|(1<<4)|(1<<9)|(1<<10)|(1<<14)|(1<<15));
	LCD_PSB(1);
   
}

/*
LED 对应引脚
35 P1.21 LED2
36 P1.22 LED3
37 P1.23

参考文档 Open1768-Schematic 引脚对应图

COM  
RUN
FULL
ERR
*/
void hy_led_init(void* gui_handle){
	PINSEL_CFG_Type PinCfg;
		//led
    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  	PinCfg.Pinmode = PINSEL_PINMODE_PULLDOWN;

    PinCfg.Pinnum = 21;
    PinCfg.Portnum = 1;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 22;
    PinCfg.Portnum = 1;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 23;
    PinCfg.Portnum = 1;
    PINSEL_ConfigPin(&PinCfg);

    // PinCfg.Pinnum = 25;
    // PinCfg.Portnum = 1;
    // PINSEL_ConfigPin(&PinCfg);


    GPIO_SetDir(1, (1<<21)|(1<<22)|(1<<23),1);	

    GPIO_ClearValue(1,(1<<21)|(1<<22)|(1<<23));
}
void hy_button_init(void* gui_handle){
	
	PINSEL_CFG_Type PinCfg;
		//26 off
    PinCfg.Funcnum = 0;
    PinCfg.Pinnum = 26;
    PinCfg.Portnum = 3;
    PINSEL_ConfigPin(&PinCfg);
		//27 on
    PinCfg.Pinnum = 25;
    PINSEL_ConfigPin(&PinCfg);
		//29 esc
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 29;
    PINSEL_ConfigPin(&PinCfg);
		//30 down
    PinCfg.Pinnum = 30;
    PINSEL_ConfigPin(&PinCfg);
		//32 up
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 18;
    PINSEL_ConfigPin(&PinCfg);
		//33 set
    PinCfg.Pinnum = 19;
    PINSEL_ConfigPin(&PinCfg);
		
		GPIO_SetDir(3, (1<<26)|(1<<25),0);
		GPIO_SetDir(0, (1<<29)|(1<<30),0);
    GPIO_SetDir(1, (1<<18)|(1<<19),0);
		
		s_gui = (hy_gui_t*)gui_handle;
		s_gui->button.name = button_default;
		s_gui->button.state = pressed_none;
		s_gui->button.pressed_time_ms = 0;
		s_gui->button.work_state = HY_BUTTON_WORKED;
		LOG_INFO_TAG(HY_LOG_TAG,"hy gui button init done!!");
}

int hy_led_control(hy_led_name led){
	switch(led){
		case led_running:
			GPIO_ClearValue(1,(1<<22)|(1<<23));
			GPIO_SetValue(1,(1<<21));
		break;
		case led_fullcharge:
			GPIO_ClearValue(1,(1<<21)|(1<<23));
			GPIO_SetValue(1,(1<<22));
		break;
		case led_err:
			GPIO_ClearValue(1,(1<<21)|(1<<22));
			GPIO_SetValue(1,(1<<23));
		break;
		case led_offall:
			GPIO_ClearValue(1,(1<<21)|(1<<22)|(1<<23));
		break;
		default:
		break;
	}

}

int hy_button_check(hy_button_name name){
	switch (name){
		case button_default:
			return 0;


		case button_off:
			if(!(GPIO_ReadValue(3) & (1<<26))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}


		case button_on:
			if(!(GPIO_ReadValue(3) & (1<<25))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}


		case button_esc:
			if(!(GPIO_ReadValue(0) & (1<<29))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}


		case button_down:
			if(!(GPIO_ReadValue(0) & (1<<30))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}


		case button_up:
			if(!(GPIO_ReadValue(1) & (1<<18))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}


		case button_set:
			if(!(GPIO_ReadValue(1) & (1<<19))){
				return HY_BUTTON_PRESSED;
			}else{
				return HY_BUTTON_LOOSEN;
			}

	}
	
	return HY_BUTTON_LOOSEN;
}

/*uint8_t Button_Check(){
	if(!(GPIO_ReadValue(0) & (1<<29))){
		hy_delay_ms(300);
		return 0x04;
	}
	else if(!(GPIO_ReadValue(0) & (1<<30))){
		hy_delay_ms(300);
		return 0x03;
	}
	else if(!(GPIO_ReadValue(3) & (1<<25))){
		hy_delay_ms(300);
		return 0x05;
	}
	else if(!(GPIO_ReadValue(3) & (1<<26))){
		hy_delay_ms(300);
		return 0x06;
	}
	else if(!(GPIO_ReadValue(1) & (1<<19))){
		hy_delay_ms(300);
		return 0x01;
	}
	else if(!(GPIO_ReadValue(1) & (1<<18))){
		hy_delay_ms(300);
		return 0x02;	
	}
	else 
		return 0;
}*/


void LCD_BUS(uint8_t cmd){
  LCD_BUS0( cmd & (1<<0) );
  LCD_BUS1( cmd & (1<<1) );
  LCD_BUS2( cmd & (1<<2) );
  LCD_BUS3( cmd & (1<<3) );
  LCD_BUS4( cmd & (1<<4) );
  LCD_BUS5( cmd & (1<<5) );
  LCD_BUS6( cmd & (1<<6) );
  LCD_BUS7( cmd & (1<<7) );
}

void write_com(uint8_t cmdcode){
	LCD_RS(0);
	LCD_EN(1);
  LCD_BUS(cmdcode);
	delay(2);
  LCD_EN(0);
	delay(10);
}

void write_data(uint8_t Dispdata){
	LCD_RS(1);
  LCD_EN(1);
	LCD_BUS(Dispdata);
	delay(2);
  LCD_EN(0);
  delay(10);
}

void lcdreset()
{  
   hy_delay_ms(100);
   write_com(0x30);      
   hy_delay_ms(5);             
   write_com(0x30);       
   hy_delay_ms(5);             
   write_com(0x0c);       
   hy_delay_ms(5);             
   write_com(0x14);      
   hy_delay_ms(5);             
   write_com(0x01);      
   hy_delay_ms(50);             
   write_com(0x06);       
}

void lcd_clear()
{ 
  write_com(0x01);
  hy_delay_ms(5);
  lcd_cursor_close();
}


void lcd_clear_5s(){
	static lastcleartime = 0;
	if(systime_elapse_ms(lastcleartime)>=5000){
		lastcleartime=hy_time_now_ms();
		lcd_clear();
	}
}
 
void lcd_goto_pos(uint8_t posy,uint8_t posx){
	uint8_t addr,x,y;
	x = posx%8;
	y = posy%4;
	switch(y){
		case 0:
			addr = 0x80+x;
			break;
		case 1:
			addr = 0x90+x;
			break;
		case 2:
			addr = 0x88+x;
			break;
		case 3:
			addr = 0x98+x;
			break;
	}
	write_com(addr);
	hy_delay_ms(1); 
}

void lcd_display_ascii(uint8_t *s){
	while(*s>0){  
		write_data(*s);
    s++;
		hy_delay_ms(1);
  }
}

void lcd_display_chinese_at(uint8_t posx,uint8_t posy,uint8_t *s)
{ 
	uint8_t addr,x,y;
	x = posx%8;
	y = posy%4;
	switch(y){
		case 0:
			addr = 0x80+x;
			break;
		case 1:
			addr = 0x90+x;
			break;
		case 2:
			addr = 0x88+x;
			break;
		case 3:
			addr = 0x98+x;
			break;
	}
	write_com(addr);
	hy_delay_ms(1); 
  while(*s>0){  
		write_data(*s);
    s++;
		hy_delay_ms(1);
  }
}

void lcd_display_chinese(uint8_t *s){ 
  while(*s>0){  
		write_data(*s);
    s++;
		hy_delay_ms(1);
  }
}

void lcd_display_colon(void){
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(0xba);
	hy_delay_ms(1);	
}

void lcd_display_space(void){
	write_data(0x20);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);	
}

void lcd_display_index(int index)
{
	write_data(index+0x30);
	hy_delay_ms(1);
	write_data(0x2e);
	hy_delay_ms(1);
}

void lcd_display_num4(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	uint16_t numtmp;
	numtmp = num/10;
	if(num<=9999){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		hy_delay_ms(1);	
		write_data(bai+0x30);
		hy_delay_ms(1);	
		write_data(shi+0x30);
		hy_delay_ms(1);
		write_data(0x2e);
		hy_delay_ms(1);		
		write_data(ge+0x30);
		hy_delay_ms(1);	
	}
	else if(num >= 10000){
		ge = 9;
		shi = 9;
		bai = 9;
		qian = 9;
		write_data(qian+0x30);
		hy_delay_ms(1);	
		write_data(bai+0x30);
		hy_delay_ms(1);	
		write_data(shi+0x30);
		hy_delay_ms(1);
		write_data(0x2e);
		hy_delay_ms(1);	
		write_data(ge+0x30);
		hy_delay_ms(1);	
	}

	if(unit != ' ')
		write_data(unit);
	hy_delay_ms(1);
}

void lcd_display_num4_forset(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	uint16_t numtmp;
	numtmp = num/10;
	if(num<=9999){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
	}
	else if(num >= 10000){
		ge = 9;
		shi = 9;
		bai = 9;
		qian = 9;
	}
	write_data(qian+0x30);
	hy_delay_ms(1);
	write_data(0x20);
	hy_delay_ms(1);	
	write_data(bai+0x30);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);	
	write_data(shi+0x30);
	hy_delay_ms(1);
	write_data(0x20);
	hy_delay_ms(1);
	write_data(0x2e);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);	
	write_data(ge+0x30);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);
	if(unit != ' ')
		write_data(unit);
	hy_delay_ms(1);
}


void lcd_display_botelv4(uint16_t num)
{
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	if(num<10000){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		hy_delay_ms(1);	
		write_data(bai+0x30);
		hy_delay_ms(1);	
		write_data(shi+0x30);
		hy_delay_ms(1);
		write_data(ge+0x30);
		hy_delay_ms(1);	
	}
	else{
		ge = 9;
		shi = 9;
		bai = 9;
		qian = 9;
	}
	write_data(' ');
	hy_delay_ms(1);
	write_data('K');
	hy_delay_ms(1);
}

void lcd_display_time4(uint16_t num)
{
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	if(num<10000){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		hy_delay_ms(1);	
		write_data(bai+0x30);
		hy_delay_ms(1);	
		write_data(shi+0x30);
		hy_delay_ms(1);
		write_data(ge+0x30);
		hy_delay_ms(1);	
	}
	else{
		ge = 9;
		shi = 9;
		bai = 9;
		qian = 9;
	}
	write_data(' ');
	hy_delay_ms(1);
	write_data('M');
	hy_delay_ms(1);
}

void lcd_display_time4_forset(uint16_t num)
{
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	if(num<10000){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		hy_delay_ms(1);	
		write_data(0x20);
		hy_delay_ms(1);
		write_data(bai+0x30);
		hy_delay_ms(1);	
		write_data(0x20);
		hy_delay_ms(1);
		write_data(shi+0x30);
		hy_delay_ms(1);
		write_data(0x20);
		hy_delay_ms(1);
		write_data(ge+0x30);
		hy_delay_ms(1);	
		write_data(0x20);
		hy_delay_ms(1);
	}
	else{
		ge = 9;
		shi = 9;
		bai = 9;
		qian = 9;
	}
	write_data(' ');
	hy_delay_ms(1);
	write_data('M');
	hy_delay_ms(1);
}

void lcd_display_num3(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	if(num<1000 && num>0){
		ge = num%10;
		shi = num/10%10;
		bai = num/100;
	}
	else{
		ge = 0;
		shi =0;
		bai =0;
	}
	write_data(bai+0x30);
	hy_delay_ms(1);	
	write_data(shi+0x30);
	hy_delay_ms(1);	
	write_data(ge+0x30);
	hy_delay_ms(1);	
	write_data(unit);
	hy_delay_ms(1);
}


void lcd_display_num3_big(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	if(num<1000 && num>0){
		ge = num%10;
		shi = num/10%10;
		bai = num/100;
	}
	else{
		ge = 0;
		shi =0;
		bai =0;
	}
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(bai+0xb0);
	hy_delay_ms(1);	
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(shi+0xb0);
	hy_delay_ms(1);	
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(ge+0xb0);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);
	write_data(unit);
	hy_delay_ms(1);
}


void lcd_display_num2_big(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t data,flag;
	if(num>=30){
		data = num-30;
		flag = 1;
	}
	else{
		data = 30 - num;
		flag = 0;
	}
	if(num<61){
		ge = data%10;
		shi = data/10%10;
	}
	else{
		ge = 0;
		shi =0;
	}
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(0xad-flag*2);//+ flag=1 - flag=0
	hy_delay_ms(1);	
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(shi+0xb0);
	hy_delay_ms(1);	
	write_data(0xa3);
	hy_delay_ms(1);	
	write_data(ge+0xb0);
	hy_delay_ms(1);	
	write_data(0x20);
	hy_delay_ms(1);
	write_data(unit);
	hy_delay_ms(1);
}

void lcd_cursor_goto(uint8_t posy,uint8_t posx){
	write_com(0x0f);       
    hy_delay_ms(5);             
	lcd_goto_pos(posy,posx);
	hy_delay_ms(1);
}

void lcd_cursor_close(void){
	write_com(0x0C);
	hy_delay_ms(5);             

}


// void lcd_highlight(uint8_t no){
// 	write_com(0x34);
// 	hy_delay_ms(1);
// 	write_com(0x04+no);
// 	hy_delay_ms(1);	
// 	write_com(0x30);
// 	hy_delay_ms(1);	

// }





