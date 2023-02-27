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

#define F485_HEAD         0xAA /* Э��֡ͷ */
#define F485_MAIN         0x12 /* MCU�ڵ� */
#define F485_TENS         0x08 /* �����ڵ� */
#define F485_CTRL         0x07 /* �������� */
#define F485_DATA         0x08 /* �������� */
#define F485_UPDA         0x10 /* �ϴ����� */

#define RS485_RX_EN       0x55 /* 485����ʹ�� */
#define RS485_TX_EN       0xAA /* 485����ʹ�� */
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
    TC_485FsmState_t (*fpAction)(TC_485FsmEvent_t *pEvent, u8 *pData); /* �ڵ㶯������ָ�� */
    TC_485FsmState_t fsmNexState;                                      /* �ڵ��¸�״̬ */
    TC_485FsmState_t fsmStateCheck;                                    /* �ڵ�״̬У��ֵ */
} TC_FsmStateNode_t;

typedef struct __TC_485Manage_t {
    u8 f_head;                    /* ֡��ʼ */
    u8 f_src;                     /* Դ��ַ */
    u8 f_dst;                     /* �޵�ַ */
    u8 f_type;                    /* ֡���� */
    u8 f_len;                     /* ���ݳ��� */
    u8 f_data[2];                 /* ���� */
    u8 f_pari;                    /* ��У�� */
    TC_485FsmState_t curState;    /* ��ǰ״̬ */
    TC_485FsmEvent_t eventType;   /* �¼����� */
    TC_FsmStateNode_t fsmCurNode; /* ��ǰ�ڵ� */
} TC_485Manage_t;

typedef struct __TC_485Transmit_t {
    u8 f_head; /* ֡��ʼ */
    u8 f_src;  /* Դ��ַ */
    u8 f_dst;  /* �޵�ַ */
    u8 f_type; /* ֡���� */
    u8 f_len;  /* ���ݳ��� */
    u8 f_data; /* ���� */
    u8 f_pari; /* ��У�� */
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