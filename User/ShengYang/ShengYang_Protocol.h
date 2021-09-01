/*
* 圣阳充电器协议
* LSB+MSB
****/

#ifndef __SHENGYANG_PROTOCOL_H
#define __SHENGYANG_PROTOCOL_H


/**address**/
#define SHENGYANG_TIMEOUT_TIME 5000//



#define TP_BAM_CONTROLER_ID 0x1CECFF28
#define TP_DP_CONTROLER_ID  0x1CEBFF28

#define TP_BAM_CHARGER_MASK   0x1CECFFFF
#define TP_DP_CHARGER_MASK    0x1CEBFFFF

#define DEFAULT_CHARGER_ADDR  0xFFFFFF3C

#define TP_BAM_CHARGER_DEFAULT_ID (TP_BAM_CHARGER_MASK&DEFAULT_CHARGER_ADDR)
#define TP_DP_CHARGER_DEFAULT_ID (TP_DP_CHARGER_MASK&DEFAULT_CHARGER_ADDR)

#define ADDRESS_CLAIM_MASK    0x0CEEFFFF
#define ADDRESS_CLAIM_DEFAULT_ID (ADDRESS_CLAIM_MASK&DEFAULT_CHARGER_ADDR)

/****  PGN 指令清单 *****/

#define CONTROL_REQUEST_PGN      0x01f100 //61697 [0x00f101] 段序调换
#define CONTROL_RESPONSE_PGN     0x00f101 //127232 [0x01f100]

#define CONTROL_REQUEST_2_BYTE       0x01f1 //约定字
#define CONTROL_RESPONSE_2_BYTE      0Xf101


#define CONFIG_REQUEST_PGN      0x02f100 //61698 [0x00f102]
#define CONFIG_RESPONSE_PGN     0x00f10x //192768 [0x02f100]

#define EXIT_REQUEST_PGN        0x03f100 //61699 [0x00f103]
#define EXIT_RESPONSE_PGN       0x00f103 //258304 [0x03f100]

#define EXIT_REQUEST_2_BYTE     0x03f1
#define EXIT_RESPONSE_2_BYTE    0xf103

#define BROADCAST_REQUEST_PGN   0x04f100 //61700 [0x00f104]
#define BROADCAST_RESPONSE_PGN  0x00f104 //323840 [0x04f100]

#define BROADCAST_REQUEST_2_BYTE 0x04f1 

#define ADDRESS_RESET_REQUEST_PGN 0x05f100 //61701 [0x00f105]

#define CONTROL_BROADCAST_PGN   0x06f100 //61702 [0x00f106]

#define ADDRESS_QUERY_FRAME_ID 0x18EAFF28

typedef struct{
	uint32_t aim_voltage_x100V;//低字节在前，高字节在后
	uint32_t aim_current_x100A;
	uint8_t charger_percent;// 2-->50% 3-->75% 4-->100%
	uint8_t battery_temperature;//需要-10
	uint8_t soc;
	uint8_t soh;
	uint8_t battery_status;//00空闲 01 充电 02 放电 03 故障
	uint8_t battery_warnning;//00 正常
}shengyang_msg;


typedef struct{
	uint32_t output_voltage_x100V;//低字节在前，高字节在后
	uint32_t output_current_x100A;
	uint32_t output_energy_x10Ah;

	uint8_t machine_status;//00 正常 bit 0 硬件状态 1 温度状态 2 输入电压状态 3 工作状态
	
	uint8_t error_code;//00正常 01 没有检测到电池 02 市电异常 03 机器高温保护 04 电池高温保护 05 机器内部故障 06 输出过压 07 地址声明错误

	uint8_t charge_stage;// 02 bulk stage 03 

	uint8_t battery_temperature;

	uint8_t charger_temperature;

	uint32_t input_220V_voltage_x100V;
	
	uint32_t machine_type;//100A24V ==> 10024

	uint32_t control_status;// 00 自主控制 01 bms控制 02执行配置参数 03 锁定配置参数 04 无法启动 220V输入故障 05 充电器故障中断

}charge_to_shengyang_msg;

#endif


