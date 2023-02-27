/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_DMA.h"
#include "tc_uart.h"

/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
struct __FILE {
    int handle;
};

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
FILE __stdout;

u8 DUART_DMA_RX[DUART_RX_LEN];

u8 rs485_TxFlag = 0;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名：  void _sys_exit(int x)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  标准库支持函数
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月24日->创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void _sys_exit(int x)
{
    x = x;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名：  int fputc(int ch, FILE *f)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  重定向printf
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月24日->创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int fputc(int ch, FILE *f)
{
    // SEGGER_RTT_PutChar(0, ch);
    UART_SendData(UART1, ch);
    while ((UART1->STT & BIT0) == FALSE)
        ;
    return ch;
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void GPIO_init(void)
 功能描述：    GPIO硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void TC_UART_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* -----------调试串口1 IO设置----------- */

    // UART1_RXD  P0.7
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    // UART1_TXD  P0.6
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_7, AF4_UART);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_6, AF4_UART);
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TC_UART_Init(void)
 功能描述：    UART0寄存器配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void TC_UART_Init(void)
{
    UART_InitTypeDef UART_InitStruct;

    /* -----------调试串口1 IO设置----------- */
    TC_UART_GPIO_Init();
    /* -----------调试串口1 结构体设置----------- */
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate   = 57600;                                      /* 设置波特率9600 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;                         /* 发送数据长度8位 */
    UART_InitStruct.StopBits   = UART_STOPBITS_1b;                           /* 停止位1位 */
    UART_InitStruct.FirstSend  = UART_FIRSTSEND_LSB;                         /* 先发送LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;                             /* 无奇偶校验 */
    UART_InitStruct.IRQEna     = UART_IRQEna_SendOver | UART_IRQEna_RcvOver; /* 串口中断使能 */
    UART_Init(UART1, &UART_InitStruct);
    UART1_IF = 0xFF;
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TC_UART_DMA_Init(void)
 功能描述：    DMA初始化配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    问：如何判断DMA_UART的传输方向
               答：DMA_DIR配置DMA_UART的传输方向
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void TC_UART_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStruct;

    /*UART1 DMA接收使能，接收到10个数据后，产生DMA中断*/
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA 传输完成中断使能 */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;          /* 外设至内存 */
    DMA_InitStruct.DMA_CIRC   = DISABLE;              /* DMA传输模式：循环模式，高有效 */
    DMA_InitStruct.DMA_PINC   = DISABLE;              /* 外设地址每轮内是否递增,高有效 */
    DMA_InitStruct.DMA_MINC   = ENABLE;               /* 内存地址第二轮是否在第一轮地址的基础上递增,高有效 */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;       /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;       /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_UART1_REQ_EN; /* 通道 DMA_UART0_REQ_EN DMA 请求使能，高有效 */
    DMA_InitStruct.DMA_TIMES  = 1;                    /* DMA 通道 x 每轮数据搬运次数 1~511 */
    DMA_InitStruct.DMA_ROUND  = 10;                   /* DMA 通道 x 采样轮数 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&UART1_BUFF;     /* DMA 通道 x 外设地址 */
    DMA_InitStruct.DMA_CMAR   = (u32)DUART_DMA_RX;    /* DMA 通道 x 内存地址 */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    DMA_CHx_EN(DMA_CH3, ENABLE); /*使能DMA_CH2通道*/
}

/*-------------------------------------------------------------------------------------------*
 * 函数名称：    TmOpState TC_UARTSendBytes(UART_TypeDef *UARTx, const u8 *pData, u16 uLen)
 * 功能描述：    串口多字节发送函数
 * 输入参数：    UART_TypeDef *UARTx    串口类型
 *              const u8 *pData        数据指针
 *              u16 uLen               发送长度
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
TmOpState TC_UARTSendBytes(UART_TypeDef *UARTx, const u8 *pData, u16 uLen)
{
    if (NULL == pData) {
        return tmErr;
    }

    while (UARTx->IF & UART_IF_SendOver)
        ;
    for (int i = 0; i < uLen; i++) { /* C99 */
        UART_SendData(UARTx, pData[i]);
        while ((UARTx->STT & BIT0) == FALSE)
            ;
        printf("%02X ", pData[i]);
    }

    return tmOk;
}

/* USER IMPLEMENTED FUNCTIONS END */
