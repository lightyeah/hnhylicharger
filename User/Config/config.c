/*
----------------------------------------------------------------------
File        : config.c
Purpose     : Write config data to EEPROM
              Read config data from EEPROM   
----------------------------------------------------------------------
*/

#include "config.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"
#include "systen_delay.h"
#include "hy_dbg.h"

#define I2CDEV LPC_I2C2
#define Debug

#ifdef Open_I2C0
    #include "uart.h"
#endif
#define PCA8581_SLVADDR	(0xA6>>1)


#define HY_LOG_TAG    "HY_config"


/************************** PRIVATE DATA *************************/
uint8_t config_wrdat[] = {
  0x00,  //Page1 Memory Address
	0x00,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x00,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x00,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x08,  	//Page2 Memory Address
	0x00,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x00,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x00,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high 
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x10,  	//Page3 Memory Address
	0x00,   // 16 bits Voltage low
	0x00,   // 16 bits Voltage high
	0x00,   // 16 bits CurLimit low
	0x00,   // 16 bits CurLimit high
	0x00,   // 16 bits ChargeTime low 
	0x00,   // 16 bits ChargeTime high
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x18,  	//Page4 Memory Address
	0x00,   // 16 bits CtrlStyle low
	0x00,   // 16 bits CtrlStyle high
	0x00,   // 16 bits VoltageRange low
	0x00,   // 16 bits VoltageRange high
	0x00,   // 16 bits CurrentRange low
	0x00,   // 16 bits CurrentRange high
	0x00,   // 16 bits CommunicationRate low
	0x00,   // 16 bits CommunicationRate high

	0x20,  	//Page4 Memory Address
	0x00,   // 16 bits CurrentBalane low
	0x00,   // 16 bits CurrentBalane high
	0x00,   // flash flag
	0x00,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag
};

uint8_t config_rddat[] = {
	0x00,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x00,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x00,   // 16 bits ChargeTime low 
	0x00,   // 16 bits ChargeTime high 
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x00,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x00,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x00,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x00,   // 16 bits Voltage low
	0x00,   // 16 bits Voltage high
	0x00,   // 16 bits CurLimit low
	0x00,   // 16 bits CurLimit high
	0x00,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high
	0x00,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x00,   // 16 bits CtrlStyle low
	0x00,   // 16 bits CtrlStyle high
	0x00,   // 16 bits VoltageRange low
	0x00,   // 16 bits VoltageRange high
	0x00,   // 16 bits CurrentRange low
	0x00,   // 16 bits CurrentRange high
	0x00,   // 16 bits CommunicationRate low
	0x00,   // 16 bits CommunicationRate high

	0x00,   // 16 bits CurrentBalane low
	0x00,   // 16 bits CurrentBalane high
	0x00,   // flash flag
	0x00,   // flash flag
	0x00,   // flash flag
	0x00,   // flash flag
	0x00,   // flash flag
	0x00,   // flash flag


};

uint8_t config_defdat[] = {
	0x00,  // Memory Address, data will be written at this address	
	0x1e,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x28,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x0f,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high
	0x28,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x08,  // Memory Address, data will be written at this address
	0x96,   // 16 bits Current low
	0x00,   // 16 bits Current high
	0x38,   // 16 bits VolLimit low
	0x00,   // 16 bits VolLimit high
	0x78,   // 16 bits ChargeTime low
	0x00,   // 16 bits ChargeTime high 
	0x38,   // 16 bits SwitchVol low
	0x00,   // 16 bits SwitchVol high

	0x10,  // Memory Address, data will be written at this address
	0x3b,   // 16 bits Voltage low
	0x00,   // 16 bits Voltage high
	0x50,   // 16 bits CurLimit low
	0x00,   // 16 bits CurLimit high
	0x0f,   // 16 bits ChargeTime low 
	0x00,   // 16 bits ChargeTime high
	0x1e,   // 16 bits Switchcur low
	0x00,   // 16 bits Switchcur high

  0x18,  // Memory Address, data will be written at this address
	0x01,   // 16 bits CtrlStyle low
	0x00,   // 16 bits CtrlStyle high
	0x3f,   // 16 bits VoltageRange low
	0x00,   // 16 bits VoltageRange high
	0x96,   // 16 bits CurrentRange low
	0x00,   // 16 bits CurrentRange high
	0x01,   // 16 bits CommunicationRate low
	0x00,   // 16 bits CommunicationRate high

	0x20,  // Memory Address, data will be written at this address
	0x00,   // 16 bits CurrentBalane low
	0x00,   // 16 bits CurrentBalane high
	0x00,   // flash flag
	0x00,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag
	0x01,   // flash flag

};

/* Transmit setup */
I2C_M_SETUP_Type txsetup;
/* Receive setup */
I2C_M_SETUP_Type rxsetup;

/************************** PRIVATE FUNCTIONS *************************/
int32_t ATMLH436_Write(void);
int32_t ATMLH436_Read(void);

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief       Write data to EEPROM
 * @param[in]   None
 * @return      0: if success, otherwise (-1) returned.
 **********************************************************************/
int32_t ATML_Write(void){
	uint8_t index;
	for(index = 0;index<5;index++){
		txsetup.sl_addr7bit = PCA8581_SLVADDR;
		txsetup.tx_data = config_wrdat+index*9;
	//    txsetup.tx_length = sizeof(config_wrdat);
		txsetup.tx_length = 9;
		txsetup.rx_data = NULL;
		txsetup.rx_length = 0;
		txsetup.retransmissions_max = 1;

		if (I2C_MasterTransferData(I2CDEV, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
		{
				LOG_INFO_TAG(HY_LOG_TAG,"ATML_Write page%d success\r\n",index);
		} else {
				LOG_INFO_TAG(HY_LOG_TAG,"ATML_Write page%d error\r\n",index);
				return (-1);
		}
	}
	return (0);
}

/*********************************************************************//**
 * @brief       Read a number of data byte from EEPROM 
 * @param[in]   None
 * @return      0: if success, otherwise (-1) returned.
 **********************************************************************/
int32_t ATML_Read(void)
{

    rxsetup.sl_addr7bit = PCA8581_SLVADDR;
    rxsetup.tx_data = config_wrdat;    // Get address to read at writing address
    rxsetup.tx_length = 1;
    rxsetup.rx_data = config_rddat;
    rxsetup.rx_length = sizeof(config_rddat);
    rxsetup.retransmissions_max = 1;

    if (I2C_MasterTransferData(I2CDEV, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS){
        LOG_INFO_TAG(HY_LOG_TAG,"ATML_Read success");
        return (0);
    } else {        
        LOG_ERROR_TAG(HY_LOG_TAG,"ATML_Read error");
        return (-1);
    }
}

/*-------------------------PUBLIC FUNCTIONS------------------------------*/
void Config_Init(void){
    PINSEL_CFG_Type PinCfg;

    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PinCfg.Pinmode = 0;
    PinCfg.Funcnum = 2;
		PinCfg.Pinnum = 10;
		PinCfg.Portnum = 0;	//P0.10 SDA2
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 11;	//P0.11 SCL2
		PINSEL_ConfigPin(&PinCfg);

    I2C_Init(I2CDEV, 100000);
    I2C_Cmd(I2CDEV, ENABLE);
}

void Config_EraseConfig(void){
	uint8_t index;
	for(index = 0;index<5;index++){
		txsetup.sl_addr7bit = PCA8581_SLVADDR;
		txsetup.tx_data = config_defdat+index*9;
	//    txsetup.tx_length = sizeof(config_wrdat);
		txsetup.tx_length = 9;
		txsetup.rx_data = NULL;
		txsetup.rx_length = 0;
		txsetup.retransmissions_max = 1;

		if (I2C_MasterTransferData(I2CDEV, &txsetup, I2C_TRANSFER_POLLING) == SUCCESS)
		{
				LOG_INFO_TAG(HY_LOG_TAG,"ATML_Write page%d success\r\n",index);
		} else {
				LOG_INFO_TAG(HY_LOG_TAG,"ATML_Write page%d error\r\n",index);
		}
	}
}


uint8_t Config_DataReading(void){
	uint8_t i;
	if(ATML_Read() != 0){
			for (i = 0 ; i < sizeof(config_wrdat) ; i++){
				 config_wrdat[i] = config_defdat[i];
			}
	}
	else{		
			for (i = 0; i < 8; i++){
					config_wrdat[i+1]  = config_rddat[i] ;
					config_wrdat[i+10] = config_rddat[i+8] ;
					config_wrdat[i+19] = config_rddat[i+16] ;
					config_wrdat[i+28] = config_rddat[i+24] ;
					config_wrdat[i+37] = config_rddat[i+32] ;
			}
	}
	if((config_rddat[34]==0)&&(config_rddat[35]==0)&&(config_rddat[36]==1)&&
		(config_rddat[37]==1)&&(config_rddat[38]==1)&&(config_rddat[39]==1)){
			LOG_INFO_TAG(HY_LOG_TAG, "The eeprom has been configured!");	
			return 0;
		}
	else{
			LOG_WARN_TAG(HY_LOG_TAG,"ATML_Read success,but did not init");		
			LOG_WARN_TAG(HY_LOG_TAG,"I will Init it soon!");	
			Config_EraseConfig();
			return 1;
	}
}

void Config_DataWriting(void){
	while(ATML_Write() != 0){
		LOG_INFO_TAG(HY_LOG_TAG,"ATML_Read error,retrying\r\n");
		delay_ms(100);
	}
}

uint16_t Config_ReadCurrent_1(void){
    return (uint16_t)(config_wrdat[2]*256+config_wrdat[1]);
}
uint16_t Config_ReadVolLimit_1(void){
    return (uint16_t)(config_wrdat[4]*256+config_wrdat[3]);
}
uint16_t Config_ReadChargeTime_1(void){
    return (uint16_t)(config_wrdat[6]*256+config_wrdat[5]);
}
uint16_t Config_ReadSwitchVol_1(void){
    return (uint16_t)(config_wrdat[8]*256+config_wrdat[7]);
}
uint16_t Config_ReadCurrent_2(void){
    return (uint16_t)(config_wrdat[11]*256+config_wrdat[10]);
}
uint16_t Config_ReadVolLimit_2(void){
    return (uint16_t)(config_wrdat[13]*256+config_wrdat[12]);
}
uint16_t Config_ReadChargeTime_2(void){
    return (uint16_t)(config_wrdat[15]*256+config_wrdat[14]);
}
uint16_t Config_ReadSwitchVol_2(void){
    return (uint16_t)(config_wrdat[17]*256+config_wrdat[16]);
}
uint16_t Config_ReadVoltage_3(void){
    return (uint16_t)(config_wrdat[20]*256+config_wrdat[19]);
}
uint16_t Config_ReadCurLimit_3(void){
    return (uint16_t)(config_wrdat[22]*256+config_wrdat[21]);
}
uint16_t Config_ReadChargeTime_3(void){
    return (uint16_t)(config_wrdat[24]*256+config_wrdat[23]);
}
uint16_t Config_ReadSwitchCur_3(void){
    return (uint16_t)(config_wrdat[26]*256+config_wrdat[25]);
}
uint16_t Config_ReadCtrlStyle(void){
    return (uint16_t)(config_wrdat[29]*256+config_wrdat[28]);
}
uint16_t Config_ReadVoltageRange(void){
    return (uint16_t)(config_wrdat[31]*256+config_wrdat[30]);
}
uint16_t Config_ReadCurrentRange(void){
    return (uint16_t)(config_wrdat[33]*256+config_wrdat[32]);
}

/*for can baudrate*/
const uint32_t baudrate[5] = {50000,125000,250000,500000,800000};
void Config_WriteCommunicationRate_n(uint16_t data){
    config_wrdat[35] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[34] =(uint8_t)( data & 0x00ff);
}
uint16_t Config_ReadCommunicationRate_n(void){
    return (uint16_t)(config_wrdat[35]*256+config_wrdat[34]);
}
void Config_WriteCommunicationRate(uint32_t data){
		switch(data){
			case 50000:
				Config_WriteCommunicationRate_n(0);
				break;
			case 125000:
				Config_WriteCommunicationRate_n(1);
				break;
			case 250000:
				Config_WriteCommunicationRate_n(2);
				break;
			case 500000:
				Config_WriteCommunicationRate_n(3);
				break;
			case 800000:
				Config_WriteCommunicationRate_n(4);
				break;
			default:
				Config_WriteCommunicationRate_n(2);
				break;
		}
}
uint32_t Config_ReadCommunicationRate(void){
		uint16_t rate_n = 0;
		rate_n = Config_ReadCommunicationRate_n();
		if(rate_n >= 0 && rate_n < 5){
			return baudrate[rate_n];
		}else{/*error for default baudrete 250k*/
			return baudrate[2];
		}
}
uint16_t Config_ReadCurrentBalane(void){
    return (uint16_t)(config_wrdat[38]*256+config_wrdat[37]);
}

void Config_WriteCurrent_1(uint16_t data){
    config_wrdat[2] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[1] = (uint8_t)(data & 0x00ff);
}
void Config_WriteVolLimit_1(uint16_t data){
    config_wrdat[4] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[3] = (uint8_t)(data & 0x00ff);
}
void Config_WriteChargeTime_1(uint16_t data){
    config_wrdat[6] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[5] = (uint8_t)(data & 0x00ff);
}
void Config_WriteSwitchVol_1(uint16_t data){
    config_wrdat[8] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[7] = (uint8_t)(data & 0x00ff);
}
void Config_WriteCurrent_2(uint16_t data){
    config_wrdat[11] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[10] = (uint8_t)(data & 0x00ff);
}
void Config_WriteVolLimit_2(uint16_t data){
    config_wrdat[13] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[12] = (uint8_t)(data & 0x00ff);
}
void Config_WriteChargeTime_2(uint16_t data){
    config_wrdat[15] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[14] = (uint8_t)(data & 0x00ff);
}
void Config_WriteSwitchVol_2(uint16_t data){
    config_wrdat[17] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[16] =(uint8_t)( data & 0x00ff);
}

void Config_WriteCurrent_3(uint16_t data){
    config_wrdat[20] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[19] =(uint8_t)( data & 0x00ff);
}
void Config_WriteVolLimit_3(uint16_t data){
    config_wrdat[22] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[21] =(uint8_t)( data & 0x00ff);
}
void Config_WriteChargeTime_3(uint16_t data){
    config_wrdat[24] = (uint8_t)((data & 0xff00)>>8);
    config_wrdat[23] = (uint8_t)(data & 0x00ff);
}
void Config_WriteSwitchVol_3(uint16_t data){
    config_wrdat[26] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[25] =(uint8_t)( data & 0x00ff);
}
void Config_WriteCtrlStyle(uint16_t data){
    config_wrdat[29] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[28] =(uint8_t)( data & 0x00ff);
}
void Config_WriteVoltageRange(uint16_t data){
    config_wrdat[31] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[30] =(uint8_t)( data & 0x00ff);
}
void Config_WriteCurrentRange(uint16_t data){
    config_wrdat[33] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[32] =(uint8_t)( data & 0x00ff);
}

void Config_WriteCurrentBalane(uint16_t data){
    config_wrdat[38] =(uint8_t)( (data & 0xff00)>>8);
    config_wrdat[37] =(uint8_t)( data & 0x00ff);
}

uint16_t ConfigReadFromAddr(uint8_t addr){
	uint8_t index;
	index = 0;
	if (addr<17 && addr>=0){
		index = addr / 4;
		return config_wrdat[addr*2+2+index]*256+config_wrdat[addr*2+1+index];
	}
	else
		return 0;
}

void ConfigWriteToAddr(uint8_t addr, uint16_t data){//0-16
    uint8_t index;
    index = addr / 4;
    config_wrdat[addr*2+2+index] =(uint8_t)((data & 0xff00)>>8);
    config_wrdat[addr*2+1+index] =(uint8_t)(data & 0x00ff);
}

void eepromtest(){
	unsigned int index;
	for(index = 0;index<sizeof((config_rddat));index++){
		LOG_INFO_TAG(HY_LOG_TAG,"%d: %d\r\n",index,config_rddat[index]);
		delay_ms(200);
	}
}
