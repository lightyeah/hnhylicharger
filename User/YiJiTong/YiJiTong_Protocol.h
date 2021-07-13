#ifdef __YIJIATONG_PROTOCOL_H__
#define __YIJIATONG_PROTOCOL_H__


#define YJT_BMS_FRAME_ID     0x272

#define YJT_CHARGER_FRAME_ID 0x227

#define YJT_FRAME_INTERVEL   1000//ms

#define YJT_CONNECT_TIMEOUT 5000//ms

typedef struct{
	uint32_t max_voltage_10V;//byte 0 高字节 1 低字节
	uint32_t max_current_10A;//byte 2 高 3 低

	uint8_t control;//0 开始 1 电池保护 关闭输出 2 开始加热

	uint8_t soc_full_flag;//是否满电 0 未满 1 满电100%

	uint8_t  soc_percent;//充电百分比
	

}


#endif /*__YIJIATONG_PROTOCOL_H__*/
