#include "UART.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

uint32_t tmp;
uint8_t buf[10]; 
uint8_t idx;


void UART0_Init(void){
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	// Pin configuration for UART0
	PINSEL_CFG_Type PinCfg;
	
	PinCfg.Funcnum = 1; //as UART function
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;          //P0.2 = TXD0
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;          //P0.3 = RXD0
	PINSEL_ConfigPin(&PinCfg);

	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 115200;
	UART_Init(LPC_UART0, &UARTConfigStruct);
	
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);
	UART_TxCmd(LPC_UART0, ENABLE);
}

void UART0_Send_Mes(uint8_t *txbuf, uint32_t buflen){
	uint32_t idx;
	idx = 0;
	while (idx<buflen){
		UART_Send(LPC_UART0, &txbuf[idx], 1, BLOCKING);
		idx++;
	}
}

void UART0_Send_Str(uint8_t *str){
	while(*str){
		UART_Send(LPC_UART0, str, 1, BLOCKING); 
		*str++;
	}
}

void UART0_Send_ln(void){
	uint8_t str;
	str = '\n';
	UART_Send(LPC_UART0, &str, 1, BLOCKING); 
}

void UART0_Send_Uint32(uint32_t data){
// 	uint32_t tmp;
// 	uint8_t buf[10]; 
// 	uint8_t idx;
	tmp = data;
	idx = 0;
	while(tmp>0){
		buf[9-idx] = tmp % 10 + '0';
		tmp = tmp / 10;
		idx++;
	}
	if(data == 0){
		buf[9] = '0';
		idx=1;
	}
	UART_Send(LPC_UART0, buf+10-idx, idx, BLOCKING); 
}

void UART0_Send_Uint16(uint16_t data){
//	uint32_t tmp;
//	uint8_t buf[10]; 
//	uint8_t idx;
	tmp = data;
	idx = 0;
	while(tmp>0){
		buf[9-idx] = tmp % 10 + '0';
		tmp = tmp / 10;
		idx++;
	}
	if(data == 0){
		buf[9] = '0';
		idx=1;
	}
	UART_Send(LPC_UART0, buf+10-idx, idx, BLOCKING); 
}

void UART0_DeInit(void){
	UART_DeInit(LPC_UART0);
}
