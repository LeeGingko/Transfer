#ifndef __HW_485__H
#define __HW_485__H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
#include "lks32mc08x_dma.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define RS485_EN_PORT  GPIO0
#define RS485_EN_PIN   GPIO_Pin_11

#define RS485_RX_LEN   8
#define RS485_RX_RETRY 3

#define F485_HEAD      0xAA /* Э��֡ͷ */
#define F485_MAIN      0x12 /* MCU�ڵ� */
#define F485_TENS      0x08 /* �����ڵ� */
#define F485_CTRL      0x07 /* �������� */
#define F485_DATA      0x08 /* �������� */
#define F485_UPDA      0x10 /* �ϴ����� */

/* USER DEFINED MACROS END */
/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
typedef enum {
    frameRecErr = 0,
    frameRecOk,
    frameRecHead,
    frameRecSrc,
    frameRecDst,
    frameRecType,
    frameRecLen,
    frameRecDat1,
    frameRecDat2,
    frameRecPari,
} HW_FrameEnum_t;

typedef struct __HW_485Manege_t {
    u8 f_head;                                             /* ֡��ʼ */
    u8 f_src;                                              /* Դ��ַ */
    u8 f_dst;                                              /* �޵�ַ */
    u8 f_type;                                             /* ֡���� */
    u8 f_len;                                              /* ���ݳ��� */
    u8 f_data[2];                                          /* ���� */
    u8 f_pari;                                             /* ��У�� */
    void (*TransmitFrame)(struct __HW_485Manege_t *hw485); /* ֡���ͺ���ָ�� */
    void (*DecodeFrame)(struct __HW_485Manege_t *hw485);   /* ֡��������ָ�� */
} HW_485Manege_t;

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */
extern u8 rs485_RxFlag;

extern u8 rs485_Rx[RS485_RX_LEN];

extern void HW_485_Init(void);

#if (0)
extern void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx);

extern void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx);

extern void HW_485_SMUpdateState(void);

extern void HW_485_SMTransition(const u8 *pByte);
#endif

/* USER STATEMENTS END */

#endif
