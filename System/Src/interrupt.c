/*-------------------------------------------------------------------------------------------*
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： interrupt.c
 * 文件标识：
 * 内容摘要： 中断服务程序文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：2015年11月5日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *-------------------------------------------------------------------------------------------*/
/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "hardware_config.h"
#include "FreeRTOS.h" //FreeRTOS使用
#include "task.h"
#include "hw_uart.h"
#include "hw_485.h"
#include "hw_can.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
u16 timetik = 0;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
extern void xPortSysTickHandler(void);
/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        SoftDelay(6400000);
        Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        SoftDelay(3200000);
        Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        SoftDelay(1600000);
        Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void ADC0_IRQHandler(void)
 功能描述：    ADC0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void ADC0_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void MCPWM_IRQHandler(void)
 功能描述：    MCPWM中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void MCPWM_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void HALL_IRQHandler(void)
 功能描述：    HALL中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void HALL_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TIMER0_IRQHandler(void)
 功能描述：    TIMER0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UTIMER0_IRQHandler(void) // 定时器0捕获电压
{
    if (UTIMER_IF & Timer_IRQEna_Zero) // 判断UTimer0的CH0是否发生捕下降沿中断TIM0_CH0: P0.15
    {
        UTIMER_IF = Timer_IRQEna_Zero; // 清除UTimer中断标志位
        timetik++;
        if (timetik >= 500) {
            timetik = 0;
            // Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
        }
    }
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TIMER1_IRQHandler(void)
 功能描述：    TIMER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UTIMER1_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TIMER2_IRQHandler(void)
 功能描述：    TIMER2中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UTIMER2_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void TIMER3_IRQHandler(void)
 功能描述：    TIMER3中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UTIMER3_IRQHandler(void)
{
}
/*-------------------------------------------------------------------------------------------*
 函数名称：    void ENCODER0_IRQHandler(void)
 功能描述：    ENCODER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void ENCODER0_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void ENCODER0_IRQHandler(void)
 功能描述：    ENCODER1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void ENCODER1_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void CMP_IRQHandler(void)
 功能描述：    比较器中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void CMP_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void UART1_IRQHandler(void)
 功能描述：    UART1中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void SysTick_Handler(void)
 功能描述：    系统滴答定时中断
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) // 系统已经运行
    {
        xPortSysTickHandler();
    }
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void SleepTimer_IRQHandler(void)
 功能描述：    休眠唤醒中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void SleepTimer_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void GPIO_IRQHandler(void)
 功能描述：    GPIO中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void GPIO_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void I2C0_IRQHandler(void)
 功能描述：    I2C0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void I2C0_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void SPI0_IRQHandler(void)
 功能描述：    SPI0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void SPI0_IRQHandler(void)
{
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void UART0_IRQHandler(void)
 功能描述：    UART0中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    // volatile register u8 val;
    volatile u8 val;
    // printf("UART0_IRQHandler\r\n ");

    if (UART0_IF & UART_IF_StopError) {
        UART0_IF = UART_IF_StopError;
        printf("UART_IF_StopError\r\n ");
    }
    if (UART0_IF & UART_IRQEna_CheckError) {
        UART0_IF = UART_IRQEna_CheckError;
        printf("UART_IRQEna_CheckError\r\n ");
    }
    if (UART0_IF & UART_IF_RcvOver) {

        UART0_IF = UART_IF_RcvOver;
        val      = UART0_BUFF & 0xFF;
        // HW_485_SMTransition((u8)val);
        // HW_485_SMUpdateState();
        HW_FsmRunningFunc(val);
        printf("%02X ", val);
    }
    if (UART0_IF & UART_IF_SendOver) {

        UART0_IF = UART_IF_SendOver;
        rs485_RxFlag = 1;
    }
}

/*-------------------------------------------------------------------------------------------*
 函数名称：    void DMA_IRQHandler(void)
 功能描述：    DMA中断处理函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *-------------------------------------------------------------------------------------------*/
void DMA_IRQHandler(void)
{
    // if (DMA_IF & DMA_CH2_FIF) // DMA通道2完成中断标志
    // {
    //     DMA_ClearIRQFlag(DMA_CH2, DMA_CH2_FIF);
    //     DMA_CHx_EN(DMA_CH2, ENABLE); /*使能DMA_CH2通道*/
    // }
}

/* USER IMPLEMENTED FUNCTIONS END */
