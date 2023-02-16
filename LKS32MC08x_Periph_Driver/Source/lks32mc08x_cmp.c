/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc081_flash.c
 * 文件标识：
 * 内容摘要： Flash外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2019年3月5日
 *
 * 修改记录1：
 * 修改日期：2019年3月5日
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
 *******************************************************************************/

#include "lks32mc08x_cmp.h"
/*******************************************************************************
 函数名称：    void Comparator_init(CMP_InitTypeDef* CMP_InitStruct)
 功能描述：    CMP初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void CMP_Init(CMP_InitTypeDef* this)
{
    // Comparator初始化
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP1,this->CMP1_EN);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP0,this->CMP0_EN);
    SYS_WR_PROTECT = 0x7A83;
    
    SYS_AFE_REG3 |= (this->CMP0_SELN << 2) | (this->CMP1_SELN << 4) | (this->CMP0_SELP << 8) |
                    (this->CMP1_SELP << 12) | (this->CMP_HYS << 7);

    /* CMP_HYS 比较器滞回功能，默认20mV, 写0关闭 */

    SYS_AFE_REG1 |= this->CMP_FT; /* 快速比较，Enable后 30nS */

    SYS_AFE_REG5 |= (this->CMP0_EN << 6) | (this->CMP1_EN << 7);
    
    SYS_WR_PROTECT = 0;
    
    CMP -> IE      = (this -> CMP1_IE           <<  1) | (this -> CMP0_IE);
    if(this->CLK10_EN == DISABLE)
    {
        if(this->CMP1_IN_EN || this->CMP0_IN_EN)
        {
            CMP->TCLK = BIT3;
        }
        else
        {
            CMP->TCLK = 0;
        }
    }
    else
    {
        CMP->TCLK  = (this -> FIL_CLK10_DIV16   <<  4) | (this -> CLK10_EN          <<  3) |
                     (this -> FIL_CLK10_DIV2);
    }
    CMP -> CFG     = (this -> CMP1_W_PWM_POL    <<  7) | (this -> CMP1_IRQ_TRIG     <<  6) |
                     (this -> CMP1_IN_EN        <<  5) | (this -> CMP1_POL          <<  4) |
                     (this -> CMP0_W_PWM_POL    <<  3) | (this -> CMP0_IRQ_TRIG     <<  2) |
                     (this -> CMP0_IN_EN        <<  1) | (this -> CMP0_POL);           
    CMP -> BLCWIN  = (this -> CMP1_CHN3P_WIN_EN <<  7) | (this -> CMP1_CHN2P_WIN_EN <<  6) |
                     (this -> CMP1_CHN1P_WIN_EN <<  5) | (this -> CMP1_CHN0P_WIN_EN <<  4) |
                     (this -> CMP0_CHN3P_WIN_EN <<  3) | (this -> CMP0_CHN2P_WIN_EN <<  2) |
                     (this -> CMP0_CHN1P_WIN_EN <<  1) | (this -> CMP0_CHN0P_WIN_EN);
    CMP -> IF = 3;
}

/*******************************************************************************
 函数名称：    void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
 功能描述：    CMP初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{

    memset(CMP_InitStruct, 0, sizeof(CMP_InitTypeDef));
}

/*******************************************************************************
 函数名称：    u8 CMP_GetIRQFlag(u8 CMPx)
 功能描述：    读取比较器中断
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    TIM中断标志
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/21    V1.0           Yangzj            创建
 *******************************************************************************/
u8 CMP_GetIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        if(CMP_IF&BIT0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if(CMPx == CMP1)
    {
        if(CMP_IF&BIT1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

/*******************************************************************************
 函数名称：    void CMP_ClearIRQFlag(u8 CMPx)
 功能描述：    清除TIM中断标志
 操作的表：    无
 输入参数：    TIM中断标志
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/11      V1.0         HuangMG             创建
 *******************************************************************************/
void CMP_ClearIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        CMP_IF=BIT0;
    }
    else if(CMPx == CMP1)
    {
        CMP_IF=BIT1;
    }
}
