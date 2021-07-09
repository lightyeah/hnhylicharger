#ifndef __GW_MODULE_H__
#define __GW_MODULE_H__

/*GW 模块协议*/

#define GW_MOUDLE_NUM                     1

#define GW_TIME_OUT                       5000//ms

#define GW_CONTROL_FRAME_ID               0x200

#define GW_MODULE_1_MSG_100MS_MSG_FRAME_ID  0x201
#define GW_MODULE_1_MSG_500MS_MSG_FRAME_ID  0x301

#if (GW_MOUDLE_NUM >= 2)
#define GW_MODULE_2_MSG_100MS_MSG_FRAME_ID  0x202
#define GW_MODULE_2_MSG_500MS_MSG_FRAME_ID  0x302
#endif

#if (GW_MOUDLE_NUM >= 3)
#define GW_MODULE_3_MSG_100MS_MSG_FRAME_ID  0x203
#define GW_MODULE_3_MSG_500MS_MSG_FRAME_ID  0x303
#endif



#endif /*__GW_MODULE_H__*/