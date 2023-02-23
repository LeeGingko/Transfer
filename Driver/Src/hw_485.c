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

HW_FsmStateNode_t hw_FsmNodeTable[10]; /* 状态节点表 */

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
 函数名称：    void HW_TimeoutTimer_Init(void)
 功能描述：    UTimer硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void HW_TimeoutTimer_Init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerStrutInit(&TIM_InitStruct);               /* Timer结构体初始化*/
    TIM_InitStruct.Timer_TH       = 24000;             /* 定时器重载计数值为2毫秒*/
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div8;      /* 设置Timer模块数据分频系数 */
    TIM_InitStruct.Timer_IRQEna   = Timer_IRQEna_Zero; /* 开启Timer模块比较中断*/
    TIM_TimerInit(TIMER0, &TIM_InitStruct);            /* 初始化结构体 */
    TIM_TimerCmd(TIMER0, ENABLE);                      /* Timer0 模块使能 */
}

/*******************************************************************************
 函数名称：    void HW_485_GPIO_Init(void)
 功能描述：    GPIO硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void HW_485_GPIO_Init(void)
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

/*******************************************************************************
 函数名称：    void HW_485_Init(void)
 功能描述：    UART0寄存器配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void HW_485_UART_Init(void)
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
    UART_Init(UART0, &UART_InitStruct);
    UART0_IF = 0x00;
}

#if (0)
/*******************************************************************************
 函数名称：    void HW_485_DMA_Init(void)
 功能描述：    DMA初始化配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    问：如何判断DMA_UART的传输方向
               答：DMA_DIR配置DMA_UART的传输方向
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void HW_485_DMA_Init(void)
{
    DMA_StructInit(&DMA_InitStruct);
    /*UART0 DMA接收使能，接收到8个数据后，产生DMA中断*/
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA 传输完成中断使能 */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;          /* 外设至内存 */
    DMA_InitStruct.DMA_CIRC   = DISABLE;              /* DMA传输模式：循环模式，高有效 */
    DMA_InitStruct.DMA_PINC   = DISABLE;              /* 外设地址每轮内是否递增,高有效 */
    DMA_InitStruct.DMA_MINC   = ENABLE;               /* 内存地址第二轮是否在第一轮地址的基础上递增,高有效 */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;       /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;       /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH2_UART0_REQ_EN; /* 通道 DMA_UART0_REQ_EN DMA 请求使能，高有效 */
    DMA_InitStruct.DMA_TIMES  = 1;                    /* DMA 通道 x 每轮数据搬运次数 1~511 */
    DMA_InitStruct.DMA_ROUND  = 8;                    /* DMA 通道 x 采样轮数 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&UART0_BUFF;     /* DMA 通道 x 外设地址 */
    DMA_InitStruct.DMA_CMAR   = (u32)rs485_Rx;        /* DMA 通道 x 内存地址 */
    DMA_Init(DMA_CH2, &DMA_InitStruct);               /* 初始化DMA通道 */
    DMA_CHx_EN(DMA_CH2, ENABLE);                      /* 使能DMA通道 */
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
// 函数名：  void HW_485_Init(void)
// 编写者：  F.L
// 参考资料：无
// 功  能：  485初始化
// 输入参数：无
// 输出参数：无
// 备  注：  2023年2月17日->创建
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
// 函数名：  TmOpState HW_485_DMA_RxCompleteCallback()
// 编写者：  F.L
// 参考资料：无
// 功  能：  485串口DMA接收中断回调
// 输入参数：
// 输出参数：
// 备  注：  2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_DMA_RxCompleteCallback(UART_TypeDef *UARTx)
{
    return tmOk;
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// 编写者：  F.L
// 参考资料：
// 功  能：  接收字节不满足指定长度后继续接收大于等指定长度会出现首字节不是帧头的现象，因此需在此时
//           清除DMA寄存器[8:0]，即重写 DMA_CTMS以重置DMA内部的轮次计数
// 输入参数： DMA_RegTypeDef *DMAx DMA类型寄存器
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
{
    DMA_CHx_EN(DMAx, DISABLE);
    DMAx->DMA_CTMS &= 0xFFFFFE00;
    DMA_Init(DMAx, &DMA_InitStruct); /* 初始化DMA通道 */
    DMA_ClearIRQFlag(DMA_CH2, DMA_CH2_FIF);
    DMA_CHx_EN(DMAx, ENABLE); /* 使能DMA通道 */
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx)
// 编写者：  F.L
// 参考资料：
// 功  能：  DMA_CPAR [16:0] DMA_CMAR [12:0]
// 输入参数： DMA_RegTypeDef *DMAx DMA类型寄存器
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_RxDMAClearCPAR_CMAR(DMA_RegTypeDef *DMAx)
{
    DMA_CHx_EN(DMAx, DISABLE);
    DMAx->DMA_CPAR &= (u32)&UART0_BUFF;
    DMAx->DMA_CMAR &= (u32)rs485_Rx;
    DMA_InitStruct.DMA_CMAR = (u32)rs485_Rx;
    DMA_Init(DMAx, &DMA_InitStruct); /* 初始化DMA通道 */
    DMA_ClearIRQFlag(DMA_CH2, DMA_CH2_FIF);
    DMA_CHx_EN(DMAx, ENABLE); /* 使能DMA通道 */
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  TmOpState HW_485_TransmitFrameCallback(HW_485_t *hw485)
// 编写者：  F.L
// 参考资料：
// 功  能：  485帧发送回调函数
// 输出参数： 无
// 备  注：   2023年2月17日->创建
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_TransmitFrameCallback(HW_485_t *hw485)
{
    // if (NULL == hw485)
    // {
    //     tmErr;
    // }
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  TmOpState HW_485_DeocdeFrameCallback(HW_485_t *hw485)
// 编写者：  F.L
// 参考资料：
// 功  能：  485帧解析回调函数
// 输入参数：
// 输出参数： 操作状态
// 备  注：   2023年2月17日->创建
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
