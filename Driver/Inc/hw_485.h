#ifndef __HW_485__H
#define __HW_485__H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
#include "lks32mc08x_dma.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define RS485_EN_PORT GPIO1
#define RS485_RN_PIN  GPIO_Pin_3

#define RS485_RX_LEN  8

#define F485_HEAD     0xAA /* Э��֡ͷ */
#define F485_MAIN     0x12 /* MCU�ڵ� */
#define F485_TENS     0x08 /* �����ڵ� */
#define F485_CTRL     0x07 /* �������� */
#define F485_DATA     0x08 /* �������� */
#define F485_UPDA     0x10 /* �ϴ����� */

/* USER DEFINED MACROS END */
/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
typedef struct
{
    u8 f_head;    /* ֡��ʼ */
    u8 f_src;     /* Դ��ַ */
    u8 f_dst;     /* �޵�ַ */
    u8 f_type;    /* ֡���� */
    u8 f_len;     /* ���ݳ��� */
    u8 f_data[0]; /* ���� */
    u8 f_par;     /* ��У�� */
} HW_485_t;

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */

void HW_485_GPIO_Init(void);
void HW_485_Init(void);
void HW_485_DMA_Init(void);

extern u8 RS485_RxFlag;
extern u8 RS485_RX[RS485_RX_LEN];

extern void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx);

/* USER STATEMENTS END */

#endif
