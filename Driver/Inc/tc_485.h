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
#define RS485_EN_PORT     GPIO0
#define RS485_EN_PIN      GPIO_Pin_11

#define RS485_RX_LEN      8
#define RS485_RX_RETRY    3

#define F485_HEAD         0xAA /* 协议帧头 */
#define F485_MAIN         0x12 /* MCU节点 */
#define F485_TENS         0x08 /* 张力节点 */
#define F485_CTRL         0x07 /* 控制命令 */
#define F485_DATA         0x08 /* 数据命令 */
#define F485_UPDA         0x10 /* 上传数据 */

#define RS485_RX_EN       0x55 /* 485接收使能 */
#define RS485_TX_EN       0xAA /* 485发送使能 */
#define RS485_SWITCHTO(x) ((x == RS485_RX_EN) ? (RS485_EN_PORT->PDO &= ~RS485_EN_PIN) : (RS485_EN_PORT->PDO |= RS485_EN_PIN))
/* USER DEFINED MACROS END */
/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
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
} TC_485FsmState_t;

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
} TC_485FsmEvent_t;

typedef struct __TC_FsmStateNode_t {
    TC_485FsmState_t (*fpAction)(TC_485FsmEvent_t *pEvent, u8 *pData); /* 节点动作函数指针 */
    TC_485FsmState_t fsmNexState;                                      /* 节点下个状态 */
    TC_485FsmState_t fsmStateCheck;                                    /* 节点状态校验值 */
} TC_FsmStateNode_t;

typedef struct __TC_485Manage_t {
    u8 f_head;                    /* 帧起始 */
    u8 f_src;                     /* 源地址 */
    u8 f_dst;                     /* 宿地址 */
    u8 f_type;                    /* 帧功能 */
    u8 f_len;                     /* 数据长度 */
    u8 f_data[2];                 /* 数据 */
    u8 f_pari;                    /* 和校验 */
    TC_485FsmState_t curState;    /* 当前状态 */
    TC_485FsmEvent_t eventType;   /* 事件类型 */
    TC_FsmStateNode_t fsmCurNode; /* 当前节点 */
} TC_485Manage_t;

typedef struct __TC_485Transmit_t {
    u8 f_head; /* 帧起始 */
    u8 f_src;  /* 源地址 */
    u8 f_dst;  /* 宿地址 */
    u8 f_type; /* 帧功能 */
    u8 f_len;  /* 数据长度 */
    u8 f_data; /* 数据 */
    u8 f_pari; /* 和校验 */
} TC_485Transmit_t;
/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */

extern u8 rs485_Rx[RS485_RX_LEN];

extern void TC_485_Init(void);

extern TmOpState TC_485TransmitFrame(void);

extern void TC_FsmRunningFunc(u8 data);

/* USER STATEMENTS END */

#endif
