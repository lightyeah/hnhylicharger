#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "lpc17xx_gpio.h"
#include "GUI_driver.h"

#include "lpc17xx_pinsel.h"

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

#ifndef DEBUG_PRINT
#define DEBUG_PRINT
#define DEBUG_PRINT(format, ...) _printf (format,##__VA_ARGS__)
#endif
void delay(uint t){
  while(t--);
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



void Button_Init(){
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
}

uint8_t Button_Check(){
	if(!(GPIO_ReadValue(0) & (1<<29))){
// 		DEBUG_PRINT("%d \r\n",4);
		delay_ms(300);
		return 0x04;
	}
	else if(!(GPIO_ReadValue(0) & (1<<30))){
// 		DEBUG_PRINT("%d \r\n",3);
		delay_ms(300);
		return 0x03;
	}
	else if(!(GPIO_ReadValue(3) & (1<<25))){
// 		DEBUG_PRINT("%d \r\n",5);
		delay_ms(300);
		return 0x05;
	}
	else if(!(GPIO_ReadValue(3) & (1<<26))){
// 		DEBUG_PRINT("%d \r\n",6);
		delay_ms(300);
		return 0x06;
	}
	else if(!(GPIO_ReadValue(1) & (1<<19))){
// 		DEBUG_PRINT("%d \r\n",1);
		delay_ms(300);
		return 0x01;
	}
	else if(!(GPIO_ReadValue(1) & (1<<18))){
// 		DEBUG_PRINT("%d \r\n",4);
		delay_ms(300);
		return 0x02;	
	}
	else 
		return 0;
}


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
	 delay_ms(100);
   write_com(0x30);      
   delay_ms(5);             
   write_com(0x30);       
   delay_ms(5);             
   write_com(0x0c);       
   delay_ms(5);             
   write_com(0x14);      
   delay_ms(5);             
   write_com(0x01);      
   delay_ms(50);             
   write_com(0x06);       
}

void lcd_clear()
{ 
  write_com(0x01);
  delay_ms(5);
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
	delay_ms(1); 
}

void lcd_display_ascii(uint8_t *s){
	while(*s>0){  
		write_data(*s);
    s++;
		delay_ms(1);
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
	delay_ms(1); 
  while(*s>0){  
		write_data(*s);
    s++;
		delay_ms(1);
  }
}

void lcd_display_chinese(uint8_t *s){ 
  while(*s>0){  
		write_data(*s);
    s++;
		delay_ms(1);
  }
}

void lcd_display_colon(void){
	write_data(0xa3);
	delay_ms(1);	
	write_data(0xba);
	delay_ms(1);	
}

void lcd_display_space(void){
	write_data(0x20);
	delay_ms(1);	
	write_data(0x20);
	delay_ms(1);	
}


void lcd_display_num4(uint16_t num,uint8_t unit){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	uint16_t numtmp;
	numtmp = num/10;
	if(num<1000 && num>=0){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		delay_ms(1);	
		write_data(bai+0x30);
		delay_ms(1);	
		write_data(shi+0x30);
		delay_ms(1);
		write_data(0x2e);
		delay_ms(1);		
		write_data(ge+0x30);
		delay_ms(1);	
	}
	else if(num<100000 && num>=1000){
		ge = numtmp%10;
		shi = numtmp/10%10;
		bai = numtmp/100%10;
		qian = numtmp/1000;
		write_data(qian+0x30);
		delay_ms(1);	
		write_data(bai+0x30);
		delay_ms(1);	
		write_data(shi+0x30);
		delay_ms(1);
		write_data(ge+0x30);
		delay_ms(1);	
	}
	else{
		ge = 0;
		shi =0;
		bai =0;
		qian = 0;
	}

	if(unit != ' ')
		write_data(unit);
	delay_ms(1);
}

void lcd_display_time4(uint16_t num){
	uint8_t ge;
	uint8_t shi;
	uint8_t bai;
	uint8_t qian;
	if(num<10000 && num>=0){
		ge = num%10;
		shi = num/10%10;
		bai = num/100%10;
		qian = num/1000;
		write_data(qian+0x30);
		delay_ms(1);	
		write_data(bai+0x30);
		delay_ms(1);	
		write_data(shi+0x30);
		delay_ms(1);
		write_data(ge+0x30);
		delay_ms(1);	
	}
	else{
		ge = 0;
		shi =0;
		bai =0;
		qian = 0;
	}
	write_data(' ');
	delay_ms(1);
	write_data('M');
	delay_ms(1);
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
	delay_ms(1);	
	write_data(shi+0x30);
	delay_ms(1);	
	write_data(ge+0x30);
	delay_ms(1);	
	write_data(unit);
	delay_ms(1);
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
	delay_ms(1);	
	write_data(bai+0xb0);
	delay_ms(1);	
	write_data(0xa3);
	delay_ms(1);	
	write_data(shi+0xb0);
	delay_ms(1);	
	write_data(0xa3);
	delay_ms(1);	
	write_data(ge+0xb0);
	delay_ms(1);	
	write_data(0x20);
	delay_ms(1);
	write_data(unit);
	delay_ms(1);
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
	if(num<61 && num>=0){
		ge = data%10;
		shi = data/10%10;
	}
	else{
		ge = 0;
		shi =0;
	}
	write_data(0xa3);
	delay_ms(1);	
	write_data(0xad-flag*2);//+ flag=1 - flag=0
	delay_ms(1);	
	write_data(0xa3);
	delay_ms(1);	
	write_data(shi+0xb0);
	delay_ms(1);	
	write_data(0xa3);
	delay_ms(1);	
	write_data(ge+0xb0);
	delay_ms(1);	
	write_data(0x20);
	delay_ms(1);
	write_data(unit);
	delay_ms(1);
}

void lcd_cursor_goto(uint8_t posy,uint8_t posx){
	write_com(0x0f);       
  delay_ms(5);             
	lcd_goto_pos(posy,posx);
	delay_ms(1);
}

void lcd_cursor_close(void){
	write_com(0x0C);
	delay_ms(5);             

}


// void lcd_highlight(uint8_t no){
// 	write_com(0x34);
// 	delay_ms(1);
// 	write_com(0x04+no);
// 	delay_ms(1);	
// 	write_com(0x30);
// 	delay_ms(1);	

// }





