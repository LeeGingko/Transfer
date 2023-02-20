#ifndef __HW_485__H
#define __HW_485__H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
#include "lks32mc08x_dma.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define RS485_EN_PORT  GPIO1
#define RS485_RN_PIN   GPIO_Pin_3

#define RS485_RX_LEN   8
#define RS485_RX_RETRY 3

#define F485_HEAD      0xAA /* 协议帧头 */
#define F485_MAIN      0x12 /* MCU节点 */
#define F485_TENS      0x08 /* 张力节点 */
#define F485_CTRL      0x07 /* 控制命令 */
#define F485_DATA      0x08 /* 数据命令 */
#define F485_UPDA      0x10 /* 上传数据 */

/* USER DEFINED MACROS END */
/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
typedef enum {
    flagErr = 0,
    flagOk,
    fsmRecErr,
    fsmRecIdle,
    fsmRecHead,
    fsmRecSrc,
    fsmRecDst,
    fsmRecType,
    fsmRecLen,
    fsmRecDat1,
    fsmRecDat2,
    fsmRecPari,
    fsmRecDone,
} HW_Rec_t;

typedef struct __HW_485_t {
    u8 f_head;                                       /* 帧起始 */
    u8 f_src;                                        /* 源地址 */
    u8 f_dst;                                        /* 宿地址 */
    u8 f_type;                                       /* 帧功能 */
    u8 f_len;                                        /* 数据长度 */
    u8 f_data[2];                                    /* 数据 */
    u8 f_pari;                                       /* 和校验 */
    u8 curState;                                     /* 当前状态 */
    u8 nexState;                                     /* 下个状态 */
    void (*TransmitFrame)(struct __HW_485_t *hw485); /* 帧发送函数指针 */
    void (*DecodeFrame)(struct __HW_485_t *hw485);   /* 帧解析函数指针 */
} HW_485_t;

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */
extern u8 rs485_RxFlag;

extern u8 rs485_Rx[RS485_RX_LEN];

extern void HW_485_Init(void);

extern void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx);

extern void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx);

extern void HW_485_SMUpdateState(void);

extern void HW_485_SMTransition(const u8 *pByte);
/* USER STATEMENTS END */

#endif
