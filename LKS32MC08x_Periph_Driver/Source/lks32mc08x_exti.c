/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc081_exti.c
 * 文件标识：
 * 内容摘要： EXTI外设驱动程序
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
 * 修改日期： 2016年6月19日
 * 版 本 号： v1.1
 * 修 改 人： cfwu
 * 修改内容： 按照ChanHomB定义修改
 *
 *******************************************************************************/
#include "lks32mc08x_exti.h"


/*******************************************************************************
 函数名称：    void EXTI_Trigger_Config(GPIO_TypeDef* GPIOx, uint32_t GPIO_PadSource, uint16_t EXTI_Trigger)
 功能描述：    GPIO外部触发极性选择
 操作的表：    无
 输入参数：    GPIOx:GPIO对象, GPIO_PadSource:指定的PadSource, EXTI_Trigger:指定的触发极性
 输出参数：    无
 返 回 值：    无
 其它说明：    
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/11/10    V1.0           YangZJ            创建
 *******************************************************************************/
void EXTI_Trigger_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint16_t EXTI_Trigger)
{
    if(GPIOx == GPIO0)
    {
        if(GPIO_PinSource>16)
        {
            return;
        }
        if(GPIO_PinSource<8)
        {
            EXTI->CR0 = (EXTI->CR0 & 0xfffc) | (EXTI_Trigger << (2 * GPIO_PinSource));
        }
        else
        {
            EXTI->CR1 = (EXTI->CR0 & 0xfffc) | (EXTI_Trigger << (2 * (GPIO_PinSource - 8)));
        }
    }
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
u8 EXTI_GetIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
{
    if(GPIO_PinSource>=16)
    {
        return 0;
    }
    if(GPIOx == GPIO0)
    {
        if(EXTI_IF & (1<<GPIO_PinSource))
        {
            return 1;
        }
        else
        {
            return 0;
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
void EXTI_ClearIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
{
    if(GPIOx == GPIO0)
    {
        EXTI_IF = (1<<GPIO_PinSource);
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
