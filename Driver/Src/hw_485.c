/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "hw_uart.h"
#include "hw_485.h"
/* USER INCLUDE FILES END */
/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define RS485_EN_PORT GPIO0
#define RS485_EN_PIN  GPIO_Pin_11

/* USER DEFINED MACROS END */
/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
u8 rs485_Rx[RS485_RX_LEN];

HW_FsmStateNode_t hw_FsmNodeTable[10];  /* ״̬��״̬�ڵ�� */
HW_485Manage_t hw_485_Manage;           /* 485���չ����� */
HW_485Transmit_t hw_485_Transmit = {0}; /* 485����֡�ṹ�� */
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */
static HW_485FsmState_t fsmActionIdle(HW_485FsmEvent_t *pEvent, u8 *pData);   /* Idle״̬�������� */
static HW_485FsmState_t fsmActionHead(HW_485FsmEvent_t *pEvent, u8 *pData);   /* Head״̬�������� */
static HW_485FsmState_t fsmActionSrc(HW_485FsmEvent_t *pEvent, u8 *pData);    /* Src״̬�������� */
static HW_485FsmState_t fsmActionDst(HW_485FsmEvent_t *pEvent, u8 *pData);    /* Dst״̬�������� */
static HW_485FsmState_t fsmActionType(HW_485FsmEvent_t *pEvent, u8 *pData);   /* Type״̬�������� */
static HW_485FsmState_t fsmActionLen(HW_485FsmEvent_t *pEvent, u8 *pData);    /* Len״̬�������� */
static HW_485FsmState_t fsmActionData1(HW_485FsmEvent_t *pEvent, u8 *pData);  /* Data1״̬�������� */
static HW_485FsmState_t fsmActionData2(HW_485FsmEvent_t *pEvent, u8 *pData);  /* Data2״̬�������� */
static HW_485FsmState_t fsmActionParity(HW_485FsmEvent_t *pEvent, u8 *pData); /* Pari״̬�������� */
static HW_485FsmState_t fsmActionError(HW_485FsmEvent_t *pEvent, u8 *pData);  /* Err״̬�������� */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_TimeoutTimer_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ���ճ�ʱ��ʱ����ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��12��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_485_GPIO_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  485GPIOӲ����ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��12��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_485_UART_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  485���ڳ�ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��12��->����
 *--------------------------------------------------------------------------------------------*/
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
    UART_Init(UART0, &UART_InitStruct);                                                                                   /* ��ʼ���ṹ�� */
    UART0_IF = 0xFF;                                                                                                      /* �˴�ȫ��д1���� */
}

/*--------------------------------------------------------------------------------------------*
 * ��������  TmOpState HW_485TransmitFrame(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  485��������֡
 * �����������
 * ���������TmOpState ���ͳɹ�����־
 * ��  ע��  2023��2��23��->����
 *          ��ָ����㳤�ȷ���У���쳣tmChErr��
 *          ����֡���ͳɹ�����tmOk������ʧ�ܷ���tmErr��
 *--------------------------------------------------------------------------------------------*/
TmOpState HW_485TransmitFrame(void)
{
    volatile s16 chk = 0xFFFF;

    RS485_SWITCHTO(RS485_TX_EN);

    hw_485_Transmit.f_head = F485_HEAD;
    hw_485_Transmit.f_src  = F485_TENS;
    hw_485_Transmit.f_dst  = F485_MAIN;
    hw_485_Transmit.f_type = F485_DATA;
    hw_485_Transmit.f_len  = 0x01;
    hw_485_Transmit.f_data = 0x00;

    chk = CheckSum((u8 *)&hw_485_Transmit, 6);
    if (chk == -1) {
        RS485_SWITCHTO(RS485_RX_EN);
        return tmChErr;
    }
    hw_485_Transmit.f_pari = (chk & 0xFF);

    if (HW_UARTSendBytes(UART0, (u8 *)&hw_485_Transmit, 7) == tmErr) {
        RS485_SWITCHTO(RS485_RX_EN);
        return tmErr;
    }
    RS485_SWITCHTO(RS485_RX_EN);

    return tmOk;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_FsmStateTable_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ״̬��״̬���ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��22��->����
 *          ��10��״̬�ڵ���г�ʼ����ÿ��״̬���ڵ��У�ֻ��ʼ���䶯������ָ���Լ�״̬У��ֵ����һ��״̬
 *          ���ڵ�ǰ�ڵ���˵�ǲ�ȷ���ģ����ڶ���������ָ����һ��״̬��
 *--------------------------------------------------------------------------------------------*/
void HW_FsmStateTable_Init(void)
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

/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_485_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  485��ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��17��->����
 *--------------------------------------------------------------------------------------------*/
void HW_485_Init(void)
{
    RS485_SWITCHTO(RS485_TX_EN);
    HW_485_GPIO_Init();
    HW_485_UART_Init();
    HW_TimeoutTimer_Init();
    RS485_SWITCHTO(RS485_RX_EN);
    memset(&hw_485_Manage, 0, 10);
    HW_FsmStateTable_Init();
    hw_485_Manage.curState                 = fsmStaIdle;
    hw_485_Manage.eventType                = fsmEveIdle;
    hw_485_Manage.fsmCurNode.fpAction      = fsmActionIdle;
    hw_485_Manage.fsmCurNode.fsmNexState   = fsmStaHead;
    hw_485_Manage.fsmCurNode.fsmStateCheck = fsmStaIdle;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionIdle(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Idle״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionHead(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Head״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
static HW_485FsmState_t fsmActionHead(HW_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            hw_485_Manage.f_head                 = F485_HEAD;
            hw_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionSrc(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Src״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionDst(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Dst״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionType(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Type״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionLen(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Len״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionData1(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Data1״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionData2(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Data2״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionParity(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Pari״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t fsmActionError(HW_485FsmEvent_t *pEvent, u8 *pData)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  Err״̬��������
 * ����������¼��������жϽ�������
 * �����������ǰ״̬�£��¼��������״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
static HW_485FsmState_t fsmActionError(HW_485FsmEvent_t *pEvent, u8 *pData)
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

/*--------------------------------------------------------------------------------------------*
 * ��������  static void HW_SetCurState(HW_485FsmState_t sta)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ���õ�ǰ״̬����
 * ���������Ҫ���õ�״̬
 * �����������
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
static void HW_SetCurState(HW_485FsmState_t sta)
{
    hw_485_Manage.curState                 = sta;
    hw_485_Manage.fsmCurNode.fsmStateCheck = sta;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmState_t HW_GetCurState(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ��ȡ��ǰ״̬����
 * �����������
 * ���������״̬����ǰ״̬
 * ��  ע��  2023��2��22��->����
 *--------------------------------------------------------------------------------------------*/
static HW_485FsmState_t HW_GetCurState(void)
{
    return hw_485_Manage.curState;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  static HW_485FsmEvent_t HW_GetCurEvent(const u8 data)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ��ȡ�¼����Ͳ�����
 * ��������������жϽ����ֽ�����
 * ����������¼�����
 * ��  ע��  2023��2��22��->����
 *          ״̬���ࣺIdle Head Src Dst Type Len Data1 Data2 Pari Err����HW_485Manage_t������֡��Ӧ��
 *                   Err״̬Ϊ���մ���״̬��
 *          �¼����ࣺIdle Head Src Dst Type Len Data1 Data2 Pari Err����HW_485Manage_t������֡��Ӧ��
 *                   Err״̬Ϊ���մ����¼���
 *          ���ݵ�ǰ״̬�ͽ��յ����ֽ��ж��¼����ͣ����¼�����ע�⣺
 *          ��1�� Idle״ֻ̬��ת�Ƶ�Idle״̬��Head״̬���޷�ת�Ƶ�Err״̬������״̬��
 *          ��2�� ��Idle״̬�⣬ʣ��״̬�У�Len��Data1��Pari����״̬Ҳ�޷�ת����Err״̬��
 *          ��3�� Idle��Pari��Err��Head�¼�������ת�Ƶ�Head״̬��Head״̬����Ҳ����Head�¼�����
 *                �󱣳�������Head״̬�����״̬���߱�����֡����.����������
 *          ��4�� У��ȶ���Pari�¼�����ʱ��ɣ���У���쳣��״̬��Ӧ����Idle״̬����У��ԱȲ�ͨ
 *                ������״̬������Err״̬��
 *--------------------------------------------------------------------------------------------*/
static HW_485FsmEvent_t HW_GetCurEvent(const u8 data)
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
            if (data == F485_HEAD) {
                eveTmp = fsmEveHead;
            } else if (data == F485_MAIN) {
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
            if (data == (chkSum & 0xFF)) {
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
            if (data == F485_HEAD) {
                eveTmp = fsmEveHead;
            } else {
                eveTmp = fsmEveIdle;
            }
            break;
    }

    return eveTmp;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  static void HW_FsmCrash(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ״̬����������
 * �����������
 * �����������
 * ��  ע��  2023��2��23��->����
 *--------------------------------------------------------------------------------------------*/
static void HW_FsmCrash(void)
{
    printf("HW_FsmCrash\r\n");
    memset(&hw_485_Manage, 0, 10);
    hw_485_Manage.curState                 = fsmStaIdle;
    hw_485_Manage.eventType                = fsmEveIdle;
    hw_485_Manage.fsmCurNode.fpAction      = fsmActionIdle;
    hw_485_Manage.fsmCurNode.fsmNexState   = fsmStaHead;
    hw_485_Manage.fsmCurNode.fsmStateCheck = fsmStaIdle;
}

/*--------------------------------------------------------------------------------------------*
 * ��������  void HW_FsmRunningFunc(u8 data)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ״̬�����в�������
 * ��������������жϽ����ֽ�����
 * �����������
 * ��  ע��  2023��2��22��->����
 *          ��ȡ��ǰ״̬���¼����ͣ���������ȷ��״̬��ϵ�ڵ����λ�ã���ȡ��Ӧ����������
 *          ��ǰ״̬У��ͨ����ִ�и��¼���������������ִ�к���µ�ǰ״̬��������ǰ���¸�״̬��
 *--------------------------------------------------------------------------------------------*/
void HW_FsmRunningFunc(u8 data)
{
    static HW_485FsmState_t tmpState;
    static HW_485FsmEvent_t tmpEvent;

    tmpState                 = HW_GetCurState();
    tmpEvent                 = HW_GetCurEvent(data);
    hw_485_Manage.fsmCurNode = hw_FsmNodeTable[tmpState];
    if (hw_485_Manage.fsmCurNode.fsmStateCheck == tmpState) {
        tmpState = hw_485_Manage.fsmCurNode.fpAction(&tmpEvent, &data);
        HW_SetCurState(tmpState);
    } else {
        HW_FsmCrash();
    }
}

/* USER IMPLEMENTED FUNCTIONS END */
