#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "stdint.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "GUI.h"


void delay(uint16_t t);
void LCD_Configuration(void);
void LCD_BUS(uint8_t cmd);
void write_com(uint8_t cmdcode);
void write_data(uint8_t Dispdata);
void lcdreset(void);

void lcd_display_chinese_at(uint8_t posx,uint8_t posy,uint8_t *s);
void lcd_display_chinese(uint8_t *s);
void lcd_display_ascii(uint8_t *s);
void lcd_display_num4(uint16_t num,uint8_t unit);
void lcd_display_time4(uint16_t num);
void lcd_display_num3(uint16_t num,uint8_t unit);
void lcd_display_num3_big(uint16_t num,uint8_t unit);
void lcd_display_num2_big(uint16_t num,uint8_t unit);
void lcd_goto_pos(uint8_t posy,uint8_t posx);
void lcd_display_colon(void);
void lcd_display_space(void);
void lcd_highlight(uint8_t no);
void lcd_cursor_goto(uint8_t posy,uint8_t posx);
void lcd_cursor_close(void);
void lcd_clear(void);   


void hy_button_init(void* gui_handle);
int hy_button_check(hy_button_name name);

#endif
