/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "tc_uart.h"
#include "tc_485.h"
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

TC_FsmStateNode_t tc_FsmNodeTable[10];  /* 状态机状态节点表 */
TC_485Manage_t tc_485_Manage;           /* 485接收管理函数 */
TC_485Transmit_t tc_485_Transmit = {0}; /* 485发送帧结构体 */
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */
static TC_485FsmState_t fsmActionIdle(TC_485FsmEvent_t *pEvent, u8 *pData);   /* Idle状态动作函数 */
static TC_485FsmState_t fsmActionHead(TC_485FsmEvent_t *pEvent, u8 *pData);   /* Head状态动作函数 */
static TC_485FsmState_t fsmActionSrc(TC_485FsmEvent_t *pEvent, u8 *pData);    /* Src状态动作函数 */
static TC_485FsmState_t fsmActionDst(TC_485FsmEvent_t *pEvent, u8 *pData);    /* Dst状态动作函数 */
static TC_485FsmState_t fsmActionType(TC_485FsmEvent_t *pEvent, u8 *pData);   /* Type状态动作函数 */
static TC_485FsmState_t fsmActionLen(TC_485FsmEvent_t *pEvent, u8 *pData);    /* Len状态动作函数 */
static TC_485FsmState_t fsmActionData1(TC_485FsmEvent_t *pEvent, u8 *pData);  /* Data1状态动作函数 */
static TC_485FsmState_t fsmActionData2(TC_485FsmEvent_t *pEvent, u8 *pData);  /* Data2状态动作函数 */
static TC_485FsmState_t fsmActionParity(TC_485FsmEvent_t *pEvent, u8 *pData); /* Pari状态动作函数 */
static TC_485FsmState_t fsmActionError(TC_485FsmEvent_t *pEvent, u8 *pData);  /* Err状态动作函数 */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_TimeoutTimer_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  接收超时定时器初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月12日->创建
 *--------------------------------------------------------------------------------------------*/
void TC_TimeoutTimer_Init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerStrutInit(&TIM_InitStruct);               /* Timer结构体初始化*/
    TIM_InitStruct.Timer_TH       = 24000;             /* 定时器重载计数值为2毫秒*/
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div8;      /* 设置Timer模块数据分频系数 */
    TIM_InitStruct.Timer_IRQEna   = Timer_IRQEna_Zero; /* 开启Timer模块比较中断*/
    TIM_TimerInit(TIMER0, &TIM_InitStruct);            /* 初始化结构体 */
    TIM_TimerCmd(TIMER0, ENABLE);                      /* Timer0 模块使能 */
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_485_GPIO_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  485GPIO硬件初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月12日->创建
 *--------------------------------------------------------------------------------------------*/
void TC_485_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -----------485串口0 IO设置----------- */
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体
    // UART0_RXD  P0.15
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    // UART0_TXD  P1.0
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO1, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);

    /* -----------485使能 IO设置----------- */
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = RS485_EN_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RS485_EN_PORT, &GPIO_InitStruct);
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_485_UART_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  485串口初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月12日->创建
 *--------------------------------------------------------------------------------------------*/
void TC_485_UART_Init(void)
{
    UART_InitTypeDef UART_InitStruct;

    /* -----------485串口0 结构体设置----------- */
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate   = 57600;                                                                                   /* 设置波特率115200 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;                                                                      /* 发送数据长度8位 */
    UART_InitStruct.StopBits   = UART_STOPBITS_1b;                                                                        /* 停止位1位 */
    UART_InitStruct.FirstSend  = UART_FIRSTSEND_LSB;                                                                      /* 先发送LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;                                                                          /* 无奇偶校验 */
    UART_InitStruct.IRQEna     = UART_IRQEna_SendOver | UART_IRQEna_RcvOver | UART_IF_CheckError | UART_IRQEna_StopError; /* 接收完成中断使能*/
    UART_Init(UART0, &UART_InitStruct);                                                                                   /* 初始化结构体 */
    UART0_IF = 0xFF;                                                                                                      /* 此处全部写1清零 */
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  TmOpState TC_485TransmitFrame(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  485发送数据帧
 * 输入参数：无
 * 输出参数：TmOpState 发送成功与否标志
 * 备  注：  2023年2月23日->创建
 *          空指针或零长度返回校验异常tmChErr，
 *          数据帧发送成功返回tmOk，发送失败返回tmErr。
 *--------------------------------------------------------------------------------------------*/
TmOpState TC_485TransmitFrame(void)
{
    volatile s16 chk = 0xFFFF;

    RS485_SWITCHTO(RS485_TX_EN);

    tc_485_Transmit.f_head = F485_HEAD;
    tc_485_Transmit.f_src  = F485_TENS;
    tc_485_Transmit.f_dst  = F485_MAIN;
    tc_485_Transmit.f_type = F485_DATA;
    tc_485_Transmit.f_len  = 0x01;
    tc_485_Transmit.f_data = 0x00;

    chk = CheckSum((u8 *)&tc_485_Transmit, 6);
    if (chk == -1) {
        RS485_SWITCHTO(RS485_RX_EN);
        return tmChErr;
    }
    tc_485_Transmit.f_pari = (chk & 0xFF);

    if (TC_UARTSendBytes(UART0, (u8 *)&tc_485_Transmit, 7) == tmErr) {
        RS485_SWITCHTO(RS485_RX_EN);
        return tmErr;
    }
    RS485_SWITCHTO(RS485_RX_EN);

    return tmOk;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_FsmStateTable_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  状态机状态表初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月22日->创建
 *          对10个状态节点进行初始化，每个状态机节点中，只初始化其动作函数指针以及状态校验值，下一个状态
 *          对于当前节点来说是不确定的，需在动作结束后指定下一个状态。
 *--------------------------------------------------------------------------------------------*/
void TC_FsmStateTable_Init(void)
{
    tc_FsmNodeTable[0].fpAction      = fsmActionIdle;
    tc_FsmNodeTable[0].fsmStateCheck = fsmStaIdle;
    tc_FsmNodeTable[1].fpAction      = fsmActionHead;
    tc_FsmNodeTable[1].fsmStateCheck = fsmStaHead;
    tc_FsmNodeTable[2].fpAction      = fsmActionSrc;
    tc_FsmNodeTable[2].fsmStateCheck = fsmStaSrc;
    tc_FsmNodeTable[3].fpAction      = fsmActionDst;
    tc_FsmNodeTable[3].fsmStateCheck = fsmStaDst;
    tc_FsmNodeTable[4].fpAction      = fsmActionType;
    tc_FsmNodeTable[4].fsmStateCheck = fsmStaType;
    tc_FsmNodeTable[5].fpAction      = fsmActionLen;
    tc_FsmNodeTable[5].fsmStateCheck = fsmStaLen;
    tc_FsmNodeTable[6].fpAction      = fsmActionData1;
    tc_FsmNodeTable[6].fsmStateCheck = fsmStaDat1;
    tc_FsmNodeTable[7].fpAction      = fsmActionData2;
    tc_FsmNodeTable[7].fsmStateCheck = fsmStaDat2;
    tc_FsmNodeTable[8].fpAction      = fsmActionParity;
    tc_FsmNodeTable[8].fsmStateCheck = fsmStaPari;
    tc_FsmNodeTable[9].fpAction      = fsmActionError;
    tc_FsmNodeTable[9].fsmStateCheck = fsmStaErr;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_485_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  485初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月17日->创建
 *--------------------------------------------------------------------------------------------*/
void TC_485_Init(void)
{
    RS485_SWITCHTO(RS485_TX_EN);
    TC_485_GPIO_Init();
    TC_485_UART_Init();
    TC_TimeoutTimer_Init();
    RS485_SWITCHTO(RS485_RX_EN);
    memset(&tc_485_Manage, 0, 10);
    TC_FsmStateTable_Init();
    tc_485_Manage.curState                 = fsmStaIdle;
    tc_485_Manage.eventType                = fsmEveIdle;
    tc_485_Manage.fsmCurNode.fpAction      = fsmActionIdle;
    tc_485_Manage.fsmCurNode.fsmNexState   = fsmStaHead;
    tc_485_Manage.fsmCurNode.fsmStateCheck = fsmStaIdle;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionIdle(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Idle状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionIdle(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            tc_485_Manage.f_head                 = F485_HEAD;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionHead(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Head状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionHead(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            tc_485_Manage.f_head                 = F485_HEAD;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveSrc:
            tc_485_Manage.f_src                  = F485_MAIN;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaSrc;
            break;
        case fsmEveErr:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionSrc(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Src状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionSrc(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDst:
            tc_485_Manage.f_dst                  = F485_TENS;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaDst;
            break;
        case fsmEveErr:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionDst(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Dst状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionDst(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveType:
            tc_485_Manage.f_type                 = F485_UPDA;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaType;
            break;
        case fsmEveErr:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionType(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Type状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionType(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveLen:
            tc_485_Manage.f_len                  = 0x02;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaLen;
            break;
        case fsmEveErr:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionLen(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Len状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionLen(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDat1:
            tc_485_Manage.f_data[0]              = *pData;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaDat1;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionData1(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Data1状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionData1(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveDat2:
            tc_485_Manage.f_data[1]              = *pData;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaDat2;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionData2(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Data2状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionData2(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEvePari:
            tc_485_Manage.f_pari                 = *pData;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaPari;
            printf("FSM Done!\r\n");
            break;
        case fsmEveErr:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaErr;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionParity(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Pari状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionParity(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            tc_485_Manage.f_head                 = F485_HEAD;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t fsmActionError(TC_485FsmEvent_t *pEvent, u8 *pData)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  Err状态动作函数
 * 输入参数：事件及串口中断接收数据
 * 输出参数：当前状态下，事件动作后的状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t fsmActionError(TC_485FsmEvent_t *pEvent, u8 *pData)
{
    switch (*pEvent) {
        case fsmEveHead:
            tc_485_Manage.f_head                 = F485_HEAD;
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaHead;
            break;
        case fsmEveIdle:
            tc_485_Manage.fsmCurNode.fsmNexState = fsmStaIdle;
            break;
        default:
            break;
    }

    return tc_485_Manage.fsmCurNode.fsmNexState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static void TC_SetCurState(TC_485FsmState_t sta)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  设置当前状态函数
 * 输入参数：要设置的状态
 * 输出参数：无
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static void TC_SetCurState(TC_485FsmState_t sta)
{
    tc_485_Manage.curState                 = sta;
    tc_485_Manage.fsmCurNode.fsmStateCheck = sta;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmState_t TC_GetCurState(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  获取当前状态函数
 * 输入参数：无
 * 输出参数：状态机当前状态
 * 备  注：  2023年2月22日->创建
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmState_t TC_GetCurState(void)
{
    return tc_485_Manage.curState;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  static TC_485FsmEvent_t TC_GetCurEvent(const u8 data)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  获取事件类型并返回
 * 输入参数：串口中断接收字节数据
 * 输出参数：事件类型
 * 备  注：  2023年2月22日->创建
 *          状态分类：Idle Head Src Dst Type Len Data1 Data2 Pari Err，与TC_485Manage_t中数据帧对应，
 *                   Err状态为接收错误状态；
 *          事件分类：Idle Head Src Dst Type Len Data1 Data2 Pari Err，与TC_485Manage_t中数据帧对应，
 *                   Err状态为接收错误事件；
 *          根据当前状态和接收到的字节判断事件类型，以下几点需注意：
 *          （1） Idle状态只能转移到Idle状态和Head状态，无法转移到Err状态和其他状态；
 *          （2） 除Idle状态外，剩余状态中，Len、Data1、Pari三个状态也无法转换到Err状态；
 *          （3） Idle、Pari、Err在Head事件发生后转移到Head状态，Head状态本身也可在Head事件发生
 *                后保持在自身Head状态，因此状态机具备数据帧连续.接收能力；
 *          （4） 校验比对在Pari事件发生时完成，若校验异常，状态机应置于Idle状态；若校验对比不通
 *                过，则状态机进入Err状态。
 *--------------------------------------------------------------------------------------------*/
static TC_485FsmEvent_t TC_GetCurEvent(const u8 data)
{
    TC_485FsmEvent_t eveTmp;
    s16 chkSum;

    switch (tc_485_Manage.curState) {
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
            chkSum = CheckSum((u8 *)&tc_485_Manage, 7);
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
 * 函数名：  static void TC_FsmCrash(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  状态机崩溃处理
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月23日->创建
 *--------------------------------------------------------------------------------------------*/
static void TC_FsmCrash(void)
{
    printf("TC_FsmCrash\r\n");
    memset(&tc_485_Manage, 0, 10);
    tc_485_Manage.curState                 = fsmStaIdle;
    tc_485_Manage.eventType                = fsmEveIdle;
    tc_485_Manage.fsmCurNode.fpAction      = fsmActionIdle;
    tc_485_Manage.fsmCurNode.fsmNexState   = fsmStaHead;
    tc_485_Manage.fsmCurNode.fsmStateCheck = fsmStaIdle;
}

/*--------------------------------------------------------------------------------------------*
 * 函数名：  void TC_FsmRunningFunc(u8 data)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  状态机运行操作函数
 * 输入参数：串口中断接收字节数据
 * 输出参数：无
 * 备  注：  2023年2月22日->创建
 *          获取当前状态及事件类型，根据类型确定状态在系节点表中位置，获取对应动作函数；
 *          当前状态校验通过后执行该事件动作函数，动作执行后更新当前状态，即更新前的下个状态。
 *--------------------------------------------------------------------------------------------*/
void TC_FsmRunningFunc(u8 data)
{
    static TC_485FsmState_t tmpState;
    static TC_485FsmEvent_t tmpEvent;

    tmpState                 = TC_GetCurState();
    tmpEvent                 = TC_GetCurEvent(data);
    tc_485_Manage.fsmCurNode = tc_FsmNodeTable[tmpState];
    if (tc_485_Manage.fsmCurNode.fsmStateCheck == tmpState) {
        tmpState = tc_485_Manage.fsmCurNode.fpAction(&tmpEvent, &data);
        TC_SetCurState(tmpState);
    } else {
        TC_FsmCrash();
    }
}

/* USER IMPLEMENTED FUNCTIONS END */
