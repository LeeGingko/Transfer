/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_sys.c
 * 文件标识：
 * 内容摘要： SYS外设驱动程序
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
#include "lks32mc08x_sys.h"

WAKE_InitTypeDef WAKE_InitStruct;

/*******************************************************************************
 函数名称：    void SYS_Init(SYS_InitTypeDef* SYS_InitStruct)
 功能描述：    SYS模块初始化函数
 操作的表：    无
 输入参数：    SYS_InitTypeDef* SYS_InitStruct
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/07/02      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_Init(SYS_InitTypeDef *SYS_InitStruct)
{
    uint32_t RCHPd, XTALPdn;
    uint32_t tmp1, tmp2;
    SYS_WR_PROTECT = 0x7a83; /*开启寄存器写使能*/
    tmp1 = SYS_AFE_REG6;
    tmp1 &= 0x7FFF;
    tmp1 |= (SYS_InitStruct->PLL_SrcSel << 15);
    SYS_AFE_REG6 = tmp1;

    if (SYS_InitStruct->PLL_SrcSel == SYS_PLLSRSEL_CRYSTAL)
    {
        RCHPd = ENABLE;   /*RCH(4MHz)时钟关闭*/
        XTALPdn = ENABLE; /*晶体起振电路开启*/
    }
    else
    {
        RCHPd = DISABLE;   /*RCH(4MHz)时钟不关闭*/
        XTALPdn = DISABLE; /*晶体起振电路不开启*/
    }

    tmp2 = SYS_AFE_REG5;
    tmp2 &= 0xBBFF;
    tmp2 |= (RCHPd << 10);
    tmp2 |= (XTALPdn << 14);
    SYS_AFE_REG5 = tmp2;

    SYS_CLK_CFG = SYS_InitStruct->PLL_DivSel + (SYS_InitStruct->PLL_ReDiv << 8);
    SYS_RST_CFG = SYS_InitStruct->WDT_Ena | (SYS_InitStruct->PORFilter_Ena << 1);

    SYS_CLK_DIV0 = SYS_InitStruct->Clk_DivSPI;
    SYS_CLK_DIV2 = SYS_InitStruct->Clk_DivUART;
    SYS_CLK_FEN = SYS_InitStruct->Clk_FEN;
		SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
 函数名称：    void SYS_StructInit(SYS_InitTypeDef* SYS_InitStruct)
 功能描述：    SYS结构体初始化
 操作的表：    无
 输入参数：    SYS_InitTypeDef* SYS_InitStruct
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/07/02      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_StructInit(SYS_InitTypeDef *SYS_InitStruct)
{
    SYS_InitStruct->PLL_SrcSel = SYS_PLLSRSEL_RCH;
    SYS_InitStruct->PLL_DivSel = 0xFF;
    SYS_InitStruct->PLL_ReDiv = SYS_PLLREDIV_1;

    SYS_InitStruct->Clk_DivSPI = SYS_Clk_SPIDiv1;
    SYS_InitStruct->Clk_DivUART = SYS_Clk_UARTDiv1;
    SYS_InitStruct->Clk_FEN = 0;

    SYS_InitStruct->WDT_Ena = DISABLE;
    SYS_InitStruct->PORFilter_Ena = DISABLE;
}

/*******************************************************************************
 函数名称：    void SYS_ClearRst(void)
 功能描述：    SYS清除复位标志
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    由于复位记录工作于低速时钟域，清除执行完成需要一定时间，不应清除后立即读记录状态
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/24      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_ClearRst(void)
{
	  SYS_WR_PROTECT = 0x7a83; /*开启寄存器写使能*/
    SYS_CLR_RST = 0xDEAD;
	  SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
 函数名称：    uint32_t SYS_GetRstSource(void)
 功能描述：    获得SYS复位源信号
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/24      V1.0           cfwu          创建
 *******************************************************************************/
uint32_t SYS_GetRstSource(void)
{
    return SYS_RST_SRC;
}

/*******************************************************************************
 函数名称：    void SYS_FallSleep(void)
 功能描述：    触发系统进入休眠状态
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    无
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/24      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_FallSleep(void)
{
    SYS_WR_PROTECT = 0x7a83; /*开启寄存器写使能*/
    SYS_CLK_SLP = 0xDEAD;
    SYS_WR_PROTECT = 0x0;
    __WFI(); /*PC指针停止运行，等待中断*/
}

/*******************************************************************************
 函数名称：    void SYS_SetWakeInterval(uint32_t nInterval)
 功能描述：    设置休眠唤醒周期
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    无
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/24      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_SetWakeInterval(uint32_t nInterval)
{
    uint32_t tempReg = SYS_RST_CFG;
	  SYS_WR_PROTECT = 0x7a83;        /*开启寄存器写使能*/
    tempReg |= (nInterval << 2);
    SYS_RST_CFG = tempReg;
	  SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
 函数名称：    void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, bool ena)
 功能描述：    设置休眠唤醒IO
 操作的表：    无
 输入参数：    1、wakeIO = BIT3: GPIO 1[1] / P1[1] 外部唤醒使能。1：使能；0：禁用。
                 wakeIO = BIT2: GPIO 1[0] / P1[0] 外部唤醒使能。1：使能；0：禁用。
                 wakeIO = BIT1: GPIO 0[1] / P0[1] 外部唤醒使能。1：使能；0：禁用。
                 wakeIO = BIT0: GPIO 0[0] / P0[0] 外部唤醒使能。1：使能；0：禁用。
              2、唤醒电平选择：0、低电平唤醒，1:高电平唤醒
              3、ENABLE： 开启IO唤醒
                 DISABLE：关闭IO唤醒
 输出参数：    无
 返 回 值：    无
 其它说明：    无
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            创建
 *******************************************************************************/
void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, bool ena)
{
    if (ena == ENABLE)
    {
        GPIO_WAKE_EN |= wakeIO;
    }
    else
    {
        GPIO_WAKE_EN &= ~wakeIO;
    }

    if (trigLevel == WAKETRIG_HIGH)
    {
        GPIO_WAKE_POL |= wakeIO;
    }
    else
    {
        GPIO_WAKE_POL &= ~wakeIO;
    }
}
/*******************************************************************************
 函数名称：    void SoftDelay(void)
 功能描述：    延时函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            创建
 *******************************************************************************/
static void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;
    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}
/*******************************************************************************
 函数名称：    void Switch2PLL(void)
 功能描述：    休眠唤醒后，系统时钟切换到PLL时钟函数
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    无
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/25      V1.0          HuangMG            创建
 *******************************************************************************/
void Switch2PLL(void)
{
    SYS_WR_PROTECT = 0x7a83;        /*开启寄存器写使能*/
    SYS_AFE_REG5 = WAKE_InitStruct.AFE_REG5_RECORD; /*开启 PLL,HRC,BGP*/
    SoftDelay(200);
    SYS_AFE_REG6 = WAKE_InitStruct.AFE_REG6_RECORD; /*开启掉电检测电路*/
    SYS_CLK_CFG = WAKE_InitStruct.CLK_CFG_RECORD;   /*将主时钟切换到PLL时钟*/
    SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
 函数名称：    void Switch2HRC(void)
 功能描述：    休眠唤醒前一时刻，关闭高速时钟与BGP和掉电检测电路函数
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    无
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/25      V1.0          HuangMG            创建
 *******************************************************************************/
void Switch2HRC(void)
{
    SYS_WR_PROTECT = 0x7a83; /*开启寄存器写使能*/
    WAKE_InitStruct.AFE_REG5_RECORD = SYS_AFE_REG5;
    WAKE_InitStruct.AFE_REG6_RECORD = SYS_AFE_REG6;
    WAKE_InitStruct.CLK_CFG_RECORD = SYS_CLK_CFG;
    SYS_CLK_CFG = 0;       /* 切换到HRC时钟*/
    SYS_AFE_REG5 = 0x0500; /* 准备关闭 PLL,HRC,BGP*/
    SYS_AFE_REG6 |= 1;     /* 关闭掉电检测电路*/
    SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
函数名称：     void SYS_WatchDogCmd(FuncState state);
功能描述：    看门狗控制函数，ENABLE/DISABLE
操作的表：    无
输入参数：    无
输出参数：    无
返 回 值：    无
其它说明：
修改日期      版本号          修改人            修改内容
-----------------------------------------------------------------------------
2016/6/28      V1.0           cfwu          创建
*******************************************************************************/
void SYS_WatchDogCmd(FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //解除写保护

    if (state == ENABLE)
    {
        SYS_RST_CFG |= BIT0; //使能看门狗
    }
    else
    {
        SYS_RST_CFG &= (~BIT0); //关闭看门狗
    }
		SYS_WR_PROTECT = 0x0;
}

/*******************************************************************************
 函数名称：    void SYS_EnablePORFilter(void)
 功能描述：    允许对POR进行滤波处理
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_EnablePORFilter(void)
{  
    uint32_t tempReg = SYS_RST_CFG;
	  SYS_WR_PROTECT = 0x7A83; //解除系统寄存器写保护
    tempReg |= BIT1;
    SYS_RST_CFG = tempReg;
	  SYS_WR_PROTECT = 0x0;      /*关闭系统寄存器写操作*/
}

/*******************************************************************************
 函数名称：    void SYS_FeedDog(uint32_t timeOut)
 功能描述：    喂狗
 操作的表：    无
 输入参数：    看门狗超时时间
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_FeedDog(uint32_t timeOut)
{
    SYS_WR_PROTECT = 0xCAFE; // 0xCAFE;

    if (timeOut == (uint32_t)SYS_WD_TimeOut64s)
    {
        SYS_WDT_CLR = 0x7989; // 1 00 1
    }
    else if (timeOut == (uint32_t)SYS_WD_TimeOut8s)
    {
        SYS_WDT_CLR = 0x798B; // 1 01 1
    }
    else if (timeOut == (uint32_t)SYS_WD_TimeOut4s)
    {
        SYS_WDT_CLR = 0x798D; // 1 10 1
    }
    else // SYS_WD_TimeOut2s
    {
        SYS_WDT_CLR = 0x798F; // 1 11 1
    }

    SYS_WR_PROTECT = 0;
}

/*******************************************************************************
 函数名称：    void SYS_ModuleClockCmd(uint32_t nModule, FuncState state)
 功能描述：    模块时钟使能和停止
 操作的表：    无
 输入参数：    uint32_t nModule：对应的模块
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_ModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //解除系统寄存器写保护

    if (state != DISABLE)
    {
        SYS_CLK_FEN |= nModule;
    }
    else
    {
        SYS_CLK_FEN &= ~nModule;
    }
		
	  SYS_WR_PROTECT = 0x0;      /*关闭系统寄存器写操作*/
}

/*******************************************************************************
 函数名称：    void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state)
 功能描述：    模拟模使能和停止
 操作的表：    无
 输入参数：    uint32_t nModule：对应的模块
 输出参数：    无
 返 回 值：    无
 其它说明：    模拟部分时钟是使能：PGA、ADC、DAC
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //解除系统寄存器写保护

    if (state != DISABLE)
    {
        SYS_AFE_REG5 |= nModule;
    }
    else
    {
        SYS_AFE_REG5 &= ~nModule;
    }
		SYS_WR_PROTECT = 0x0;      /*关闭系统寄存器写操作*/
}

/*******************************************************************************
 函数名称：    void SYS_SoftResetModule(uint32_t nModule)
 功能描述：    模块软复位
 操作的表：    无
 输入参数：    uint32_t nModule：对应的模块
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void SYS_SoftResetModule(uint32_t nModule)
{
    SYS_WR_PROTECT = 0x7A83;
    SYS_SFT_RST = nModule;
    SYS_WR_PROTECT = 1;
	  SYS_WR_PROTECT = 0x0;      /*关闭系统寄存器写操作*/
}

/*******************************************************************************
 函数名称：    void DSP_Init(void)
 功能描述：    DSP时钟使能
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          创建
 *******************************************************************************/
void DSP_Init(void)
{
    SYS_ModuleClockCmd(SYS_Module_DSP, ENABLE); /* DSP时钟使能*/
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
