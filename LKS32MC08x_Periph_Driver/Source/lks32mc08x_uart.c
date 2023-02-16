/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_uart.c
 * 文件标识：
 * 内容摘要： UART外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者：
 * 完成日期：
 *
 * 修改记录1：
 * 修改日期：
 * 版 本 号：V 1.0
 * 修 改 人：
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/
#include "lks32mc08x_uart.h"


/*******************************************************************************
 函数名称：    void UART_Init(UART_TypeDef* UARTx, EUART_InitTypeDef* UART_InitStruct)
 功能描述：    UART初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    uint32_t clkDivUART;
    uint32_t divCoefficient;
    uint32_t parity;
    uint32_t parityEna;

    if(UARTx == UART0)
    {
        SYS_ModuleClockCmd(SYS_Module_UART0, ENABLE);
    }
    else if(UARTx == UART1)
    {
        SYS_ModuleClockCmd(SYS_Module_UART1, ENABLE);
    }
    
    clkDivUART = 0;
    divCoefficient = UART_MCU_MCLK / UART_InitStruct->BaudRate / (1 + clkDivUART);
    while (divCoefficient > 0xFFFF)
    {
        clkDivUART++;
        divCoefficient = UART_MCU_MCLK / UART_InitStruct->BaudRate / (1 + clkDivUART);
    }

    SYS_CLK_DIV2 = clkDivUART;
    divCoefficient = divCoefficient - 1;
    UARTx->DIVL = divCoefficient & 0xFF;
    UARTx->DIVH = (divCoefficient & 0xFF00) >> 8;

    if (UART_InitStruct->ParityMode == UART_Parity_EVEN)
    {
        parityEna = ENABLE;
        parity = 0;
    }
    else if (UART_InitStruct->ParityMode == UART_Parity_ODD)
    {
        parityEna = ENABLE;
        parity = 1;
    }
    else
    {
        parityEna = DISABLE;
        parity = 0;
    }
    UARTx->CTRL = UART_InitStruct->WordLength | (UART_InitStruct->StopBits << 1)
                  | (UART_InitStruct->FirstSend << 2) | (parity << 3)
                  | (parityEna << 4) | (UART_InitStruct->MultiDropEna << 5)
                  | (UART_InitStruct->Bit9Value << 6) ;

    UARTx->INV = (UART_InitStruct->RXD_INV << 1) | UART_InitStruct->TXD_INV;

    UARTx->ADR = UART_InitStruct->Match485Addr;
    UARTx->IE = UART_InitStruct->IRQEna;
}

/*******************************************************************************
 函数名称：    void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
 功能描述：    UART结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
    UART_InitStruct->BaudRate = 9600;
    UART_InitStruct->WordLength = UART_WORDLENGTH_8b;
    UART_InitStruct->StopBits = UART_STOPBITS_1b;
    UART_InitStruct->FirstSend = UART_FIRSTSEND_LSB;
    UART_InitStruct->ParityMode = UART_Parity_NO;

    UART_InitStruct->MultiDropEna = DISABLE;
    UART_InitStruct->Bit9Value = 0;

    UART_InitStruct->Match485Addr = 0;
    UART_InitStruct->IRQEna = 0;
    UART_InitStruct->RXD_INV = DISABLE;
    UART_InitStruct->TXD_INV = DISABLE;
}

/*******************************************************************************
 函数名称：    void UART_SENDDATA(UART_TypeDef *UARTx, uint32_t n)
 功能描述：    UART发送数据
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void UART_SendData(UART_TypeDef *UARTx, uint32_t n)
{
    UARTx->BUFF = n;
}

/*******************************************************************************
 函数名称：    uint32_t UART_ReadData(UART_TypeDef *UARTx)
 功能描述：    UART读缓冲区数据
 操作的表：    无
 输入参数：    UART_TypeDef *UARTx
 输出参数：    无
 返 回 值：    缓冲区数据
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
uint32_t UART_ReadData(UART_TypeDef *UARTx)
{
    return UARTx->BUFF;
}

/*******************************************************************************
 函数名称：    uint8_t UART_SendAddr_485(UART_TypeDef *UARTx, uint8_t n)
 功能描述：    485发送地址
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/21     V1.0           HuangMG           创建
 *******************************************************************************/
uint8_t UART_SendAddr_485(UART_TypeDef *UARTx, uint8_t n)
{
    u32 cnt = 0xffffff;
    UARTx->CTRL |= BIT6;
    UARTx->BUFF = n;
    while (!(UARTx->STT & BIT0)) /*发送缓冲区非空*/
    {
        if (cnt > 0)
        {
            cnt--;
            __NOP();
        }
        else
        {
            return 1; /*发送失败*/
        }
    }
    return 0; /*发送成功*/
}

/*******************************************************************************
 函数名称：    uint8_t UART_SendData_485(UART_TypeDef *UARTx, uint8_t n)
 功能描述：    485发送数据
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/21     V1.0           HuangMG           创建
 *******************************************************************************/
uint8_t UART_SendData_485(UART_TypeDef *UARTx, uint8_t n)
{
    u32 cnt = 0xffffff;
    UARTx->CTRL &= ~BIT6;
    UARTx->BUFF = n;
    while (!(UARTx->STT & BIT0)) /*发送缓冲区非空*/
    {
        if (cnt > 0)
        {
            cnt--;
            __NOP();
        }
        else
        {
            return 1; /*发送失败*/
        }
    }
    return 0; /*发送成功*/
}

/*******************************************************************************
 函数名称：    uint32_t UART_GetIRQFlag(UART_TypeDef *UARTx , uint32_t tempFlag)
 功能描述：    取得UART中断标志
 操作的表：    无
 输入参数：    UART_TypeDef *UARTx:要操作的UART对象
 输出参数：    无
 返 回 值：    UART中断标志
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/10/14      V1.0         HuangMG             创建
 *******************************************************************************/
uint32_t UART_GetIRQFlag(UART_TypeDef *UARTx , uint32_t tempFlag)
{
  if((UARTx->IF) & tempFlag & (UARTx->IE))
	{
	  return 1;
	}
	  return 0;
}

/*******************************************************************************
 函数名称：    void UART_ClearIRQFlag(UART_TypeDef *UARTx, uint32_t nFlag)
 功能描述：    清除UART中断标志位
 操作的表：    无
 输入参数：    UART_TypeDef *UARTx:要操作的UART对象, uint32_t nFlag当前中断标志
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/25     V1.0           cfwu              创建
 *******************************************************************************/
void UART_ClearIRQFlag(UART_TypeDef *UARTx, uint32_t tempFlag)
{
    UARTx->IF = tempFlag;
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
