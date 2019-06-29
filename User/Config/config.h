#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stdint.h"
#include "hy_dbg.h"

#define HY_MAX_VOLTAGE                  7500/*unit 0.1 V*/
#define HY_MAX_CURRENT                  4500/*unit 0.1 A*/

#define hy_config_init                  Config_Init
#define hy_config_eraseconfig           Config_EraseConfig
#define hy_config_datareading           Config_DataReading
#define hy_config_datawriting           Config_DataWriting

#define hy_config_readchargecurrent_1     Config_ReadCurrent_1
#define hy_config_readlimitvoltage_1      Config_ReadVolLimit_1
#define hy_config_readchargetimeout_1     Config_ReadChargeTime_1
#define hy_config_readswitchvoltage_1     Config_ReadSwitchVol_1

#define hy_config_readchargecurrent_2     Config_ReadCurrent_2
#define hy_config_readlimitvoltage_2      Config_ReadVolLimit_2
#define hy_config_readchargetimeout_2     Config_ReadChargeTime_2
#define hy_config_readswitchvoltage_2     Config_ReadSwitchVol_2

#define hy_config_readchargevoltage_3     Config_ReadVoltage_3
#define hy_config_readlimitcurrent_3      Config_ReadCurLimit_3
#define hy_config_readchargetimeout_3     Config_ReadChargeTime_3
#define hy_config_readswitchcurrent_3     Config_ReadSwitchCur_3

#define hy_config_readctrlstyle           Config_ReadCtrlStyle
#define hy_config_readvoltagerange        Config_ReadVoltageRange
#define hy_config_readcurrentrange        Config_ReadCurrentRange
#define hy_config_readcommunicaterate     Config_ReadCommunicationRate
#define hy_config_readbalancecurrent      Config_ReadCurrentBalane

#define hy_config_writechargecurrent_1    Config_WrtieCurrent_1
#define hy_config_writelimitvoltage_1     Config_WrtieVolLimit_1
#define hy_config_writechargetimeout_1    Config_WrtieChargeTime_1
#define hy_config_writeswitchvoltage_1    Config_WrtieSwitchVol_1

#define hy_config_writechargecurrent_2    Config_WrtieCurrent_2
#define hy_config_writelimitvoltage_2     Config_WrtieVolLimit_2
#define hy_config_writechargetimeout_2    Config_WrtieChargeTime_2
#define hy_config_writeswitchvoltage_2    Config_WrtieSwitchVol_2

#define hy_config_writechargecurrent_3    Config_WrtieCurrent_3
#define hy_config_writelimitvoltage_3     Config_WrtieVolLimit_3
#define hy_config_writechargetimeout_3    Config_WrtieChargeTime_3
#define hy_config_writeswitchvoltage_3    Config_WrtieSwitchVol_3

#define hy_config_writectrlstyle          Config_WriteCtrlStyle
#define hy_config_writevoltagerange       Config_WriteVoltageRange
#define hy_config_writecurrentrange       Config_WriteCurrentRange
#define hy_config_writecommunicaterate    Config_WriteCommunicationRate
#define hy_config_writebalancecurrent     Config_WriteCurrentBalane

#define hy_config_readfromaddr            ConfigReadFromAddr
#define hy_config_writetoaddr             ConfigWriteToAddr
#define hy_config_eepromtest              eepromtest

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
uint32_t Config_ReadCommunicationRate(void);
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
void Config_WriteCommunicationRate(uint32_t data);
void Config_WriteCurrentBalane(uint16_t data);

uint16_t ConfigReadFromAddr(uint8_t addr);
void ConfigWriteToAddr(uint8_t addr, uint16_t data);
void eepromtest(void); 


#endif
