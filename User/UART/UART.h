#ifndef __UART_H
#define __UART_H

#include "stdint.h"

void UART0_Init(void);
void UART0_Send_Str(uint8_t *str);
void UART0_Send_Uint32(uint32_t data);
void UART0_Send_Uint16(uint16_t data);
void UART0_Send_Mes(uint8_t *txbuf, uint32_t buflen);

void UART0_Send_ln(void);
void UART0_DeInit(void);


#endif
