#ifndef __TC_485__H
#define __TC_485__H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
#include "lks32mc08x_dma.h"
#include "common.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define RS485_EN_PORT     GPIO2
#define RS485_EN_PIN      GPIO_Pin_12

#define RS485_RX_LEN      8
#define RS485_RX_RETRY    3

#define F485_CMD_HEAD     0xAA /* 协议帧头 */
#define F485_DAT_HEAD     0x55 /* 协议帧头 */
#define F485_MAIN         0x12 /* MCU节点 */
#define F485_TENS         0x08 /* 张力节点 */
#define F485_CTRL         0x07 /* 控制命令 */
#define F485_DATA         0x08 /* 数据命令 */
#define F485_UPDA         0x10 /* 上传数据 */

#define RS485_RX_EN       0x5A /* 485接收使能 */
#define RS485_TX_EN       0xA5 /* 485发送使能 */
#define RS485_SWITCHTO(x) ((x == RS485_RX_EN) ? (GPIO_ResetBits(RS485_EN_PORT, RS485_EN_PIN)) : (GPIO_SetBits(RS485_EN_PORT, RS485_EN_PIN)))
/* USER DEFINED MACROS END */
/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
/*
 #--------------------------------------状态机状态说明-------------------------------------—#
    @Idle [空闲]
    @Head [帧头]
    @Src  [源地址]
    @Dst  [宿地址]
    @Type [功能]
    @Len  [数据长度]
    @Dat1 [高字节]
    @Dat2 [低字节]
    @Pari [校验]
    @Err  [错误]
 #--------------------------------------状态机状态说明-------------------------------------—#
 */
typedef enum {
    fsmStaIdle = 0,
    fsmStaHead,
    fsmStaSrc,
    fsmStaDst,
    fsmStaType,
    fsmStaLen,
    fsmStaDat1,
    fsmStaDat2,
    fsmStaPari,
    fsmStaErr,
} TC_485FsmState_t; /* 状态机状态 */

/*
 #--------------------------------------状态机事件说明-------------------------------------—#
    @Idle [空闲事件]
    @Head [收到帧头]
    @Src  [收到源地址]
    @Dst  [收到宿地址]
    @Type [收到功能]
    @Len  [收到数据长度]
    @Dat1 [收到高字节]
    @Dat2 [收到低字节]
    @Pari [收到校验]
    @Err  [接收错误]
 #--------------------------------------状态机事件说明-------------------------------------—#
 */
typedef enum {
    fsmEveIdle = 0,
    fsmEveHead,
    fsmEveSrc,
    fsmEveDst,
    fsmEveType,
    fsmEveLen,
    fsmEveDat1,
    fsmEveDat2,
    fsmEvePari,
    fsmEveErr,
} TC_485FsmEvent_t; /* 状态机事件 */

typedef struct __TC_FsmStateNode_t {
    TC_485FsmState_t (*fpAction)(TC_485FsmEvent_t *pEvent, u8 *pData); /* 节点动作函数指针 */
    TC_485FsmState_t fsmNexState;                                      /* 节点下个状态 */
    TC_485FsmState_t fsmStateCheck;                                    /* 节点状态校验值 */
} TC_FsmStateNode_t;                                                   /* 状态节点 */

typedef struct __TC_485Manage_t {
    u8                f_head;     /* 帧起始 */
    u8                f_src;      /* 源地址 */
    u8                f_dst;      /* 宿地址 */
    u8                f_type;     /* 帧功能 */
    u8                f_len;      /* 数据长度 */
    u8                f_data[2];  /* 数据 */
    u8                f_pari;     /* 和校验 */
    TC_485FsmState_t  curState;   /* 当前状态 */
    TC_485FsmEvent_t  eventType;  /* 事件类型 */
    TC_FsmStateNode_t fsmCurNode; /* 当前节点 */
} TC_485Manage_t;                 /* 接收管理结构体 */

typedef struct __TC_485Transmit_t {
    u8 f_head;      /* 帧起始 */
    u8 f_src;       /* 源地址 */
    u8 f_dst;       /* 宿地址 */
    u8 f_type;      /* 帧功能 */
    u8 f_len;       /* 数据长度 */
    u8 f_data;      /* 数据 */
    u8 f_pari;      /* 和校验 */
} TC_485Transmit_t; /* 发送结构体 */
/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */

extern u8 rs485_Rx[RS485_RX_LEN];

extern void TC_485_Init(void);

extern TmOpState TC_485TransmitFrame(void);

extern void TC_FsmRunningFunc(u8 data);

/* USER STATEMENTS END */

#endif
