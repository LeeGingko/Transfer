/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_init.c
 * 文件标识：
 * 内容摘要： 硬件初始化代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet Li
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------- */
#include "SEGGER_RTT.h"
#include "hardware_config.h"
#include "hw_uart.h"
#include "hw_485.h"
#include "hw_can.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine --------------------------------------------------------------------- */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
// u32 time_tick;
u32 SystemCoreClock = 96000000LL;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
u32 HW_SysTick_Config(uint32_t ticks);

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名：  void HW_NVIC_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  外设NVIC初始化
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月21日->创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HW_NVIC_Init(void)
{
    // UART0_IF = UART_IF_RcvOver;
    UART_ClearIRQFlag(UART0, UART_IF_RcvOver);
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn, 14);
    // NVIC_SetPriority(TIMER0_IRQn, 12); /* TIMER0中断优先级配置*/
    // NVIC_EnableIRQ(TIMER0_IRQn);       /* 使能UTimer定时器中断*/
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名：  static void HW_SysLed_Init(void)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  系统指示LED
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月21日->创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static void HW_SysLed_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -----------系统LED IO设置----------- */
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = SYS_LED_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SYS_LED_PORT, &GPIO_InitStruct);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名：  int fputc(int ch, FILE *f)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  重定向printf
 * 输入参数：无
 * 输出参数：无
 * 备  注：  2023年2月14日->创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int fputc(int ch, FILE *f)
{
    SEGGER_RTT_PutChar(0, ch);

    return ch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    void Hardware_init(void)
 * 功能描述：    硬件部分初始化
 * 输入参数：    无
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Hardware_Init(void)
{
    __disable_irq();         /* 关闭中断 中断总开关 */
    SYS_WR_PROTECT = 0x7a83; /*使能系统寄存器写操作*/
    FLASH_CFG |= 0x00080000; /* enable prefetch ，FLASH预取加速使能*/

    HW_SysLed_Init();
    HW_UART_Init();
    HW_CAN_Init();
    HW_485_Init();
    SoftDelay(1000);
    HW_NVIC_Init();                /* NVIC设置 */
    NVIC_EnableIRQ(DMA_IRQn);      /* 使能DMA中断 */
    NVIC_SetPriority(DMA_IRQn, 1); /* 配置DMA中断优先级*/
    SYS_WR_PROTECT = 0x0;          /*关闭系统寄存器写操作*/
    __enable_irq();                /* 开启中断 */
    /* 系统嘀嗒定时器初始化 */
    if (HW_SysTick_Config(12000) == 0) {
        printf("SysTick Config Ok!\r\n");
    } else {
        printf("SysTick Config Err!\r\n");
    }
    SoftDelay(100);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    void Clock_Init(void)
 * 功能描述：    时钟配置
 * 输入参数：    无
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;     /* 解除系统寄存器写保护 */
    SYS_AFE_REG5 |= BIT15;       /* BIT15:PLLPDN*/
    SoftDelay(500);              /* 等待硬件初始化完毕*/
    SYS_CLK_CFG    = 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff对应96M时钟 ; 开启所有时钟 */
    SYS_WR_PROTECT = 0x0;        /*关闭系统寄存器写操作*/
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    void SystemInit(void)
 * 功能描述：    硬件系统初始化，调用时钟初始化函数
 * 输入参数：    无
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SystemInit(void)
{
    Clock_Init(); /* 时钟初始化 */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    void SoftDelay(void)
 * 功能描述：    软件延时函数
 * 输入参数：    无
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for (t_cnt = 0; t_cnt < cnt; t_cnt++) {
        __NOP();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
 * 功能描述：    GPIO电平反转函数
 * 输入参数：    无
 * 输出参数：    无
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/14     V1.0          HuangMG             创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
u8 Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    u16 reg;
    reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); // 读GPIO电平SYST_CSR
    if (reg) {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
        return 0;
    } else {
        GPIO_SetBits(GPIOx, GPIO_Pin);
        return 1;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 函数名称：    vu32 HW_SysTick_Config(uint32_t ticks)
 * 功能描述：    嘀嗒定时器初始化
 * 输入参数：    计数节拍
 * 输出参数：    1：失败，设置节拍大于最大节拍 0：初始化成功
 * 返 回 值：    无
 * 其它说明：
 * 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2023/02/12    V1.0          Frank Lee          创建
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
u32 HW_SysTick_Config(uint32_t ticks)
{
    if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) {
        return (1UL); /* Reload value impossible */
    }

    SysTick->LOAD = (uint32_t)(ticks - 1UL);                         /* set reload register */
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL = 0UL;                                              /* Load the SysTick Counter Value */
    SysTick->CTRL &= (~SysTick_CTRL_CLKSOURCE_Msk);                  /* MACLK / 8 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;                       /* Enable SysTick IRQ */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                        /* Enable SysTick IRQ and SysTick Timer */

    return (0UL); /* Function successful */
}

/* USER IMPLEMENTED FUNCTIONS END */
