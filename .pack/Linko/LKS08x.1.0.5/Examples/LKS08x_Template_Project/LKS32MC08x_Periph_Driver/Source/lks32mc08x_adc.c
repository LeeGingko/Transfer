/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc081_adc.c
 * 文件标识：
 * 内容摘要： ADC外设驱动程序
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
 *******************************************************************************/
#include "lks32mc08x_adc.h"
#include "lks32mc08x.h"

/*******************************************************************************
 函数名称：    void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
 功能描述：    ADC初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{

    uint16_t t_reg;

    ADCx->IE = ADC_InitStruct->IE;

    t_reg = (ADC_InitStruct->Align) | (ADC_InitStruct->DAT0_TH_Type << 1);
    ADCx->CFG = t_reg;

    t_reg = (ADC_InitStruct->MCPWM_Trigger_En) | (ADC_InitStruct->UTIMER_Trigger_En << 4) |
            (ADC_InitStruct->Trigger_Cnt << 8) | (ADC_InitStruct->Trigger_Mode << 12) |
            (ADC_InitStruct->Con_Sample << 14);

    ADCx->TRG = t_reg;//TRIG

    ADCx->CHNT0 = ADC_InitStruct->FirSeg_Ch | (ADC_InitStruct->SecSeg_Ch << 8);
    ADCx->CHNT1 = ADC_InitStruct->ThrSeg_Ch | (ADC_InitStruct->FouSeg_Ch << 8);


    if(ADC_InitStruct->Align == 0)
    {
        FLASH_CFG |= 0x00000800;
        FLASH_ADDR     = 0x59000000;
        FLASH_ADDR     = 0x00000310;
        ADC0_DC_A0 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000314;
        ADC0_DC_A1 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000320;
        ADC0_DC_B0 = FLASH_RDATA<<4;
			  FLASH_ADDR     = 0x00000324;
        ADC0_DC_B1 = FLASH_RDATA<<4;

        FLASH_ADDR     = 0x95000000;

        FLASH_CFG &=~0x00000800;
    }
		else
		{
			  FLASH_CFG |= 0x00000800;
        FLASH_ADDR     = 0x59000000;
        FLASH_ADDR     = 0x00000310;
        ADC0_DC_A0 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000314;
        ADC0_DC_A1 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000320;
        ADC0_DC_B0 = FLASH_RDATA;
			  FLASH_ADDR     = 0x00000324;
        ADC0_DC_B1 = FLASH_RDATA;

        FLASH_ADDR     = 0x95000000;

        FLASH_CFG &=~0x00000800;
		}
		




}

/*******************************************************************************
 函数名称：    void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
 功能描述：    ADC结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
    ADC_InitStruct->IE = 0;
    ADC_InitStruct->Align = 0;
    ADC_InitStruct->Con_Sample = 0;
    ADC_InitStruct->Trigger_Cnt = 0;
    ADC_InitStruct->FirSeg_Ch = 0;
    ADC_InitStruct->SecSeg_Ch = 0;
    ADC_InitStruct->ThrSeg_Ch = 0;
    ADC_InitStruct->FouSeg_Ch = 0;
    ADC_InitStruct->Trigger_Mode = 0;
    ADC_InitStruct->MCPWM_Trigger_En = 0;
    ADC_InitStruct->UTIMER_Trigger_En = 0;
    ADC_InitStruct->DAT0_TH_Type = 0;

}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
