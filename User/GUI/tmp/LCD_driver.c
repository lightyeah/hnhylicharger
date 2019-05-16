#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "debug_frmwrk.h"
#include "systen_delay.h"
#include "lpc17xx_gpio.h"


#define uchar unsigned char
#define uint  unsigned int


#define  MAX_X  128
#define  MAX_Y  64   

/*********************** Hardware specific configuration **********************/

/* 8bit to 16bit LCD Interface

   PINS:
   - RS      =  44  P1.28
   - EN      =  43  P1.27
   - PSB/CS1 =  40  P1.26
   - DB[0.7] =  94 95 92 93 90 91 88 89  
                P1. 1 0 8 4 10 9 15 14                                                     */

#define PIN_RS    (1 << 28)  
#define PIN_EN    (1 << 27)  
#define PIN_PSB   (1 << 26)  
#define PIN_BUS0  (1 << 1 )
#define PIN_BUS1  (1 << 0 )
#define PIN_BUS2  (1 << 8 )
#define PIN_BUS3  (1 << 4 )
#define PIN_BUS4  (1 << 10)
#define PIN_BUS5  (1 << 9 )
#define PIN_BUS6  (1 << 15)
#define PIN_BUS7  (1 << 14)
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
#define LCD_PSB(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_DIR) : (LPC_GPIO1->FIOCLR = PIN_DIR));

/* Pin LCD_BUS setting to 0 or 1                                                   */
#define LCD_BUS0(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS0) : (LPC_GPIO1->FIOCLR = PIN_BUS0));
#define LCD_BUS1(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS1) : (LPC_GPIO1->FIOCLR = PIN_BUS1));
#define LCD_BUS2(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS2) : (LPC_GPIO1->FIOCLR = PIN_BUS2));
#define LCD_BUS3(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS3) : (LPC_GPIO1->FIOCLR = PIN_BUS3));
#define LCD_BUS4(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS4) : (LPC_GPIO1->FIOCLR = PIN_BUS4));
#define LCD_BUS5(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS5) : (LPC_GPIO1->FIOCLR = PIN_BUS5));
#define LCD_BUS6(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS6) : (LPC_GPIO1->FIOCLR = PIN_BUS6));
#define LCD_BUS7(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_BUS7) : (LPC_GPIO1->FIOCLR = PIN_BUS7));


uchar code zk[]={
0x08,0x20,0x1c,0x10,0x1c,0x1c,0xff,0x9e,
0x7f,0x1e,0x1c,0x1f,0x3e,0x1f,0x3e,0x1f,
0x77,0x1f,0x41,0x3f,0x00,0x7e,0x00,0xfe,
0x83,0xfc,0x7f,0xf8,0x3f,0xf0,0x0f,0xc0,
};


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
    FIO_SetDir(1,(1<<0)|(1<<1)|(1<<8)|(1<<4)|(1<<9)|(1<<10)|(1<<14)|(1<<15),1);
    FIO_SetValue(1,(1<<0)|(1<<1)|(1<<8)|(1<<4)|(1<<9)|(1<<10)|(1<<14)|(1<<15));
}

void LCD_BUS(unsigned char cmd){
  LCD_BUS0( cmd ＆ (1<<0) );
  LCD_BUS1( cmd ＆ (1<<1) );
  LCD_BUS2( cmd ＆ (1<<2) );
  LCD_BUS3( cmd ＆ (1<<3) );
  LCD_BUS4( cmd ＆ (1<<4) );
  LCD_BUS5( cmd ＆ (1<<5) );
  LCD_BUS6( cmd ＆ (1<<6) );
  LCD_BUS7( cmd ＆ (1<<7) );
}

void write_com(unsigned char cmdcode){
	LCD_RS(0);
	LCD_EN(1);
  LCD_BUS(cmdcode);
	delay(1);
  LCD_EN(0);
	delay(1);
}

void write_data(unsigned char Dispdata){
	LCD_RS(1);
  LCD_EN(1);
	LCD_BUS(Dispdata);
	delay(1);
  LCD_EN(0);
  delay(1);
}

void lcdreset()
{  
   delay(2000);
   write_com(0x30);      
   delay(10);             
   write_com(0x30);       
   delay(5);
   write_com(0x0c);       
   delay(10);
   write_com(0x01);      
   delay(500);
   write_com(0x06);       
   delay(0);
}

void CGRAM()
{    
     int i;
     write_com(0x30);  
     write_com(0x40);
     for(i=0;i<16;i++)
     {
      write_data(zk[i*2]);
      write_data(zk[i*2+1]);
     }
}      

void hzkdis(uchar code *s)
{ 
   while(*s>0)    //*s>0
   {  
      write_data(*s);
      s++;
      delay(50);
   }
}


#endif
