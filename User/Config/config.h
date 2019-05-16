#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stdint.h"

void Config_Init(void);
void Config_EraseConfig(void);
uint8_t Config_DataReading(void);
void Config_DataWriting(void);


uint16_t Config_ReadCurrent_1(void);
uint16_t Config_ReadVolLimit_1(void);
uint16_t Config_ReadChargeTime_1(void);
uint16_t Config_ReadSwitchVol_1(void);

uint16_t Config_ReadCurrent_2(void);
uint16_t Config_ReadVolLimit_2(void);
uint16_t Config_ReadChargeTime_2(void);
uint16_t Config_ReadSwitchVol_2(void);

uint16_t Config_ReadVoltage_3(void);
uint16_t Config_ReadCurLimit_3(void);
uint16_t Config_ReadChargeTime_3(void);
uint16_t Config_ReadSwitchCur_3(void);

uint16_t Config_ReadCtrlStyle(void);
uint16_t Config_ReadVoltageRange(void);
uint16_t Config_ReadCurrentRange(void);
uint16_t Config_ReadCommunicationRate(void);
uint16_t Config_ReadCurrentBalane(void);

void Config_WrtieCurrent_1(uint16_t data);
void Config_WrtieVolLimit_1(uint16_t data);
void Config_WrtieChargeTime_1(uint16_t data);
void Config_WrtieSwitchVol_1(uint16_t data);

void Config_WrtieCurrent_2(uint16_t data);
void Config_WrtieVolLimit_2(uint16_t data);
void Config_WrtieChargeTime_2(uint16_t data);
void Config_WrtieSwitchVol_2(uint16_t data);

void Config_WrtieCurrent_3(uint16_t data);
void Config_WrtieVolLimit_3(uint16_t data);
void Config_WrtieChargeTime_3(uint16_t data);
void Config_WrtieSwitchVol_3(uint16_t data);

void Config_WriteCtrlStyle(uint16_t data);
void Config_WriteVoltageRange(uint16_t data);
void Config_WriteCurrentRange(uint16_t data);
void Config_WriteCommunicationRate(uint16_t data);
void Config_WriteCurrentBalane(uint16_t data);

uint16_t ConfigReadFromAddr(uint8_t addr);
void ConfigWriteToAddr(uint8_t addr, uint16_t data);
void eepromtest(void); 


#endif
