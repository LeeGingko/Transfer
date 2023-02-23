/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
// #include <assert.h>
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_uart.h"
#include "common.h"
#include "hw_485.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
u8 rs485_Rx[RS485_RX_LEN];

u8 rs485_RxFlag = 0;

HW_FsmStateNode_t hw_FsmNodeTable[10]; /* ״̬�ڵ�� */

HW_485Manage_t hw_485_Manage;

// static DMA_InitTypeDef DMA_InitStruct;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */
static HW_485FsmState_t fsmActionIdle(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionHead(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionSrc(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionDst(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionType(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionLen(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionData1(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionData2(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionParity(HW_485FsmEvent_t *pEvent, u8 *pData);
static HW_485FsmState_t fsmActionError(HW_485FsmEvent_t *pEvent, u8 *pData);

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*******************************************************************************
 �������ƣ�    void HW_TimeoutTimer_Init(void)
 ����������    UTimerӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_TimeoutTimer_Init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerStrutInit(&TIM_InitStruct);               /* Timer�ṹ���ʼ��*/
    TIM_InitStruct.Timer_TH       = 24000;             /* ��ʱ�����ؼ���ֵΪ2����*/
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div8;      /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna   = Timer_IRQEna_Zero; /* ����Timerģ��Ƚ��ж�*/
    TIM_TimerInit(TIMER0, &TIM_InitStruct);            /* ��ʼ���ṹ�� */
    TIM_TimerCmd(TIMER0, ENABLE);                      /* Timer0 ģ��ʹ�� */
}

/*******************************************************************************
 �������ƣ�    void HW_485_GPIO_Init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -----------485����0 IO����----------- */
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    // UART0_RXD  P0.15
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    // UART0_TXD  P1.0
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO1, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);

    /* -----------485ʹ�� IO����----------- */
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = RS485_EN_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RS485_EN_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void HW_485_Init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_UART_Init(void)
{
    UART_InitTypeDef UART_InitStruct;

    /* -----------485����0 �ṹ������----------- */
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate   = 57600;                                                                                   /* ���ò�����115200 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;                                                                      /* �������ݳ���8λ */
    UART_InitStruct.StopBits   = UART_STOPBITS_1b;                                                                        /* ֹͣλ1λ */
    UART_InitStruct.FirstSend  = UART_FIRSTSEND_LSB;                                                                      /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;                                                                          /* ����żУ�� */
    UART_InitStruct.IRQEna     = UART_IRQEna_SendOver | UART_IRQEna_RcvOver | UART_IF_CheckError | UART_IRQEna_StopError; /* ��������ж�ʹ��*/
    UART_Init(UART0, &UART_InitStruct);
    UART0_IF = 0x00;
}

#if (0)
/*******************************************************************************
 �������ƣ�    void HW_485_DMA_Init(void)
 ����������    DMA��ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ʣ�����ж�DMA_UART�Ĵ��䷽��
               ��DMA_DIR����DMA_UART�Ĵ��䷽��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_DMA_Init(void)
{
    DMA_StructInit(&DMA_InitStruct);
    /*UART0 DMA����ʹ�ܣ����յ�8�����ݺ󣬲���DMA�ж�*/
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;          /* �������ڴ� */
    DMA_InitStruct.DMA_CIRC   = DISABLE;              /* DMA����ģʽ��ѭ��ģʽ������Ч */
    DMA_InitStruct.DMA_PINC   = DISABLE;              /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC   = ENABLE;               /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ���,����Ч */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;       /* �������λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;       /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH2_UART0_REQ_EN; /* ͨ�� DMA_UART0_REQ_EN DMA ����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES  = 1;                    /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND  = 8;                    /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&UART0_BUFF;     /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR   = (u32)rs485_Rx;        /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH2, &DMA_InitStruct);               /* ��ʼ��DMAͨ�� */
    DMA_CHx_EN(DMA_CH2, ENABLE);                      /* ʹ��DMAͨ�� */
}
#endif

void HW_SM_StateTable_Init(void)
{
    hw_FsmNodeTable[0].fpAction      = fsmActionIdle;
    hw_FsmNodeTable[0].fsmStateCheck = fsmStaIdle;
    hw_FsmNodeTable[1].fpAction      = fsmActionHead;
    hw_FsmNodeTable[1].fsmStateCheck = fsmStaHead;
    hw_FsmNodeTable[2].fpAction      = fsmActionSrc;
    hw_FsmNodeTable[2].fsmStateCheck = fsmStaSrc;
    hw_FsmNodeTable[3].fpAction      = fsmActionDst;
    hw_FsmNodeTable[3].fsmStateCheck = fsmStaDst;
    hw_FsmNodeTable[4].fpAction      = fsmActionType;
    hw_FsmNodeTable[4].fsmStateCheck = fsmStaType;
    hw_FsmNodeTable[5].fpAction      = fsmActionLen;
    hw_FsmNodeTable[5].fsmStateCheck = fsmStaLen;
    hw_FsmNodeTable[6].fpAction      = fsmActionData1;
    hw_FsmNodeTable[6].fsmStateCheck = fsmStaDat1;
    hw_FsmNodeTable[7].fpAction      = fsmActionData2;
    hw_FsmNodeTable[7].fsmStateCheck = fsmStaDat2;
    hw_FsmNodeTable[8].fpAction      = fsmActionParity;
    hw_FsmNodeTable[8].fsmStateCheck = fsmStaPari;
    hw_FsmNodeTable[9].fpAction      = fsmActionError;
    hw_FsmNodeTable[9].fsmStateCheck = fsmStaErr;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void HW_485_Init(void)
// ��д�ߣ�  F.L
// �ο����ϣ���
// ��  �ܣ�  485��ʼ��
// �����������
// �����������
// ��  ע��  2023��2��17��->����
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_Init(void)
{
    HW_485_GPIO_Init();
    HW_485_UART_Init();
    HW_TimeoutTimer_Init();
    GPIO_ResetBits(RS485_EN_PORT, RS485_EN_PIN);
    memset(&hw_485_Manage, 0, 10);
    HW_SM_StateTable_Init();
    hw_485_Manage.curState                 = fsmStaIdle;
    hw_485_Manage.eventType                = fsmEveIdle;
    hw_485_Manage.fsmCurNode.fpAction      = fsmActionIdle;
    hw_485_Manage.fsmCurNode.fsmNexState   = fsmStaHead;
    hw_485_Manage.fsmCurNode.fsmStateCheck = fsmStaIdle;
}

#if (0)
////////////////////////////////////////////////////////////////////////////////////////////
// ��������  TmOpState HW_485_DMA_RxCompleteCallback()
// ��д�ߣ�  F.L
// �ο����ϣ���
// ��  �ܣ�  485����DMA�����жϻص�
// ���������
// ���������
// ��  ע��  2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_DMA_RxCompleteCallback(UART_TypeDef *UARTx)
{
    return tmOk;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  �����ֽڲ�����ָ�����Ⱥ�������մ��ڵ�ָ�����Ȼ�������ֽڲ���֡ͷ������������ڴ�ʱ
//           ���DMA�Ĵ���[8:0]������д DMA_CTMS������DMA�ڲ����ִμ���
// ��������� DMA_RegTypeDef *DMAx DMA���ͼĴ���
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
{
    DMA_CHx_EN(DMAx, DISABLE);
    DMAx->DMA_CTMS &= 0xFFFFFE00;
    DMA_Init(DMAx, &DMA_InitStruct); /* ��ʼ��DMAͨ�� */
    DMA_ClearIRQFlag(DMA_CH2, DMA_CH2_FIF);
    DMA_CHx_EN(DMAx, ENABLE); /* ʹ��DMAͨ�� */
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  DMA_CPAR [16:0] DMA_CMAR [12:0]
// ��������� DMA_RegTypeDef *DMAx DMA���ͼĴ���
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx)
{
    DMA_CHx_EN(DMAx, DISABLE);
    DMAx->DMA_CPAR &= (u32)&UART0_BUFF;
    DMAx->DMA_CMAR &= (u32)rs485_Rx;
    DMA_InitStruct.DMA_CMAR = (u32)rs485_Rx;
    DMA_Init(DMAx, &DMA_InitStruct); /* ��ʼ��DMAͨ�� */
    DMA_ClearIRQFlag(DMA_CH2, DMA_CH2_FIF);
    DMA_CHx_EN(DMAx, ENABLE); /* ʹ��DMAͨ�� */
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  TmOpState HW_485_TransmitFrameCallback(HW_485_t *hw485)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  485֡���ͻص�����
// ��������� ��
// ��  ע��   2023��2��17��->����
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_TransmitFrameCallback(HW_485_t *hw485)
{
    // if (NULL == hw485)
    // {
    //     tmErr;
    // }
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  TmOpState HW_485_DeocdeFrameCallback(HW_485_t *hw485)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  485֡�����ص�����
// ���������
// ��������� ����״̬
// ��  ע��   2023��2��17��->����
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_DeocdeFrameCallback(HW_485_t *hw485)
{
    // if (NULL == hw485)
    // {
    //     return tmErr;
    // }
}

void HW_485_SMUpdateState(void)
{
    hw_485_Manage.curState = hw_485_Manage.nexState;
}

void HW_485_SMTransition(const u8 byte)
{
    switch (hw_485_Manage.curState) {
        case fsmStaIdle:
            if (byte == F485_HEAD) {
                hw_485_Manage.f_head   = F485_HEAD;
                hw_485_Manage.nexState = fsmStaHead;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaHead:
            if (byte == F485_MAIN) {
                hw_485_Manage.f_src    = F485_MAIN;
                hw_485_Manage.nexState = fsmStaSrc;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaSrc:
            if (byte == F485_TENS) {
                hw_485_Manage.f_dst    = F485_TENS;
                hw_485_Manage.nexState = fsmStaDst;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaDst:
            if (byte == F485_UPDA) {
                hw_485_Manage.f_type   = F485_UPDA;
                hw_485_Manage.nexState = fsmStaType;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaType:
            if (byte == 0x02) {
                hw_485_Manage.f_len    = byte;
                hw_485_Manage.nexState = fsmStaType;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaDat1:
            hw_485_Manage.f_data[0] = byte;
            hw_485_Manage.nexState  = fsmStaDat2;
            break;
        case fsmStaDat2:
            hw_485_Manage.f_data[1] = byte;
            hw_485_Manage.nexState  = fsmStaPari;
            break;
        case fsmStaPari:
            if (byte == CheckSum((u8 *)&hw_485_Manage, 7)) {
                hw_485_Manage.f_pari   = byte;
                hw_485_Manage.nexState = fsmStaIdle;
            }
            break;
        case fsmStaDone:
            if (byte == F485_HEAD) {
                hw_485_Manage.f_head   = F485_HEAD;
                hw_485_Manage.nexState = fsmStaHead;
            } else {
                hw_485_Manage.nexState = fsmStaErr;
            }
            break;
        case fsmStaErr:

            hw_485_Manage.nexState = fsmStaIdle;
            break;
        default:
            hw_485_Manage.nexState = fsmStaIdle;
            break;
    }
}
#endif

static HW_485FsmState_t fsmActionIdle(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            hw_485_Manage.f_head                 = F485_HEAD;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionHead(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveSrc:
            hw_485_Manage.f_src                  = F485_MAIN;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaSrc;
            break;
        case fsmEveErr:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionSrc(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDst:
            hw_485_Manage.f_dst                  = F485_TENS;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaDst;
            break;
        case fsmEveErr:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionDst(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveType:
            hw_485_Manage.f_type                 = F485_UPDA;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaType;
            break;
        case fsmEveErr:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionType(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveLen:
            hw_485_Manage.f_len                  = 0x02;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaLen;
            break;
        case fsmEveErr:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionLen(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDat1:
            hw_485_Manage.f_data[0]              = *pData;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaDat1;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionData1(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDat2:
            hw_485_Manage.f_data[1]              = *pData;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaDat2;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionData2(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEvePari:
            hw_485_Manage.f_pari                 = *pData;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaPari;
            printf("FSM Done!\r\n");
            break;
        case fsmEveErr:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionParity(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            hw_485_Manage.f_head                 = F485_HEAD;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

static HW_485FsmState_t fsmActionError(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    // switch ( hw_485_Manage.curState)
    // {
    // case /* constant-expression */:
    //     /* code */
    //     break;

    // default:
    //     break;
    // }
    switch (*pEvent) {
        case fsmEveHead:
            hw_485_Manage.f_head                 = F485_HEAD;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return hw_485_Manage.fsmCurNode.fsmNexState;
}

void HW_SetCurState(HW_485FsmState_t sta)
{
    hw_485_Manage.curState                 = sta;
    hw_485_Manage.fsmCurNode.fsmStateCheck = sta;
}

HW_485FsmState_t HW_GetCurState(void)
{
    return hw_485_Manage.curState;
}

HW_485FsmEvent_t HW_GetCurEvent(const u8 data)
{
    HW_485FsmEvent_t eveTmp;
    s16 chkSum;

    switch (hw_485_Manage.curState) {
        case fsmStaIdle:
            if (data == F485_HEAD) {
                eveTmp = fsmEveHead;
            } else {
                eveTmp = fsmEveIdle;
            }
            break;
        case fsmStaHead:
            if (data == F485_MAIN) {
                eveTmp = fsmEveSrc;
            } else {
                eveTmp = fsmEveErr;
            }
            break;
        case fsmStaSrc:
            if (data == F485_TENS) {
                eveTmp = fsmEveDst;
            } else {
                eveTmp = fsmEveErr;
            }
            break;
        case fsmStaDst:
            if (data == F485_UPDA) {
                eveTmp = fsmEveType;
            } else {
                eveTmp = fsmEveErr;
            }
            break;
        case fsmStaType:
            if (data == 0x02) {
                eveTmp = fsmEveLen;
            } else {
                eveTmp = fsmEveErr;
            }
            break;
        case fsmStaLen:
            eveTmp = fsmEveDat1;
            break;
        case fsmStaDat1:
            eveTmp = fsmEveDat2;
            break;
        case fsmStaDat2:
            chkSum = CheckSum((u8 *)&hw_485_Manage, 7);
            if (chkSum == -1) {
                return fsmEveIdle;
            }
            if (data == chkSum) {
                eveTmp = fsmEvePari;
            } else {
                eveTmp = fsmEveErr;
            }
            break;
        case fsmStaPari:
            if (data == F485_HEAD) {
                eveTmp = fsmEveHead;
            } else {
                eveTmp = fsmEveIdle;
            }
            break;
        case fsmStaErr:
            if (data == F485_HEAD) {
                eveTmp = fsmEveHead;
            } else {
                eveTmp = fsmEveIdle;
            }
            break;
        default:
            eveTmp = fsmEveIdle;
            break;
    }

    return eveTmp;
}

void HW_FsmRunningFunc(u8 data)
{
    static HW_485FsmState_t tmpState;
    static HW_485FsmEvent_t tmpEvent;

    tmpState = HW_GetCurState();

    tmpEvent = HW_GetCurEvent(data);
    if (tmpEvent == 0xFF) {
        HW_SetCurState(tmpState);
        return;
    }
    hw_485_Manage.fsmCurNode = hw_FsmNodeTable[tmpState];
    if (hw_485_Manage.fsmCurNode.fsmStateCheck == tmpState) {
        tmpState = hw_485_Manage.fsmCurNode.fpAction(&tmpEvent, &data);
        HW_SetCurState(tmpState);
    }
}

/* USER IMPLEMENTED FUNCTIONS END */
