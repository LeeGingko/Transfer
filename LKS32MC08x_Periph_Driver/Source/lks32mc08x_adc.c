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
 * 修改日期：2020.12.15
 * 版 本 号：V1.1
 * 修 改 人：Howlet
 * 修改内容：调用NVR_Trim访问ADC加载值
 *
 *******************************************************************************/
#include "lks32mc08x_adc.h"

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

    SYS_AnalogModuleClockCmd(SYS_AnalogModule_ADC,ENABLE);// ADC模块使能
    
    ADCx->IE = ADC_InitStruct->IE;

    t_reg = (ADC_InitStruct->Align) | (ADC_InitStruct->DAT0_TH_Type << 1);
    ADCx->CFG = t_reg;

    t_reg = (ADC_InitStruct->MCPWM_Trigger_En) | (ADC_InitStruct->UTIMER_Trigger_En << 4) |
            (ADC_InitStruct->Trigger_Cnt << 8) | (ADC_InitStruct->Trigger_Mode << 12) |
            (ADC_InitStruct->Con_Sample << 14);

    ADCx->TRG = t_reg;//TRIG

    ADCx->CHNT0 = ADC_InitStruct->FirSeg_Ch | (ADC_InitStruct->SecSeg_Ch << 8);
    ADCx->CHNT1 = ADC_InitStruct->ThrSeg_Ch | (ADC_InitStruct->FouSeg_Ch << 8);

	  ADCx->DAT0_TH = ADC_InitStruct->DAT0_TH;

    ADCx->CHN0 = ADC_InitStruct->Gain0;
		
		ADCx->CHN1 = ADC_InitStruct->Gain1;

    if(ADC_InitStruct->Align == 0)
    {
			  ADCx->DC_A0 = Read_Trim(0x00000310) << 4 ; 
        ADCx->DC_A1 = Read_Trim(0x00000314) << 4 ;	
        ADCx->AMC_A0 = Read_Trim(0x00000318) ;		
        ADCx->AMC_A1 = Read_Trim(0x0000031C) ;	

        ADCx->DC_B0 = Read_Trim(0x00000320) << 4 ; 
        ADCx->DC_B1 = Read_Trim(0x00000324) << 4 ;	
        ADCx->AMC_B0 = Read_Trim(0x00000328) ;		
        ADCx->AMC_B1 = Read_Trim(0x0000032C) ;				
    }
    else
    {	
			  ADCx->DC_A0 = Read_Trim(0x00000310) ; 
        ADCx->DC_A1 = Read_Trim(0x00000314) ;	
        ADCx->AMC_A0 = Read_Trim(0x00000320) ;		
        ADCx->AMC_A1 = Read_Trim(0x00000324) ;	

        ADCx->DC_B0 = Read_Trim(0x00000320) ; 
        ADCx->DC_B1 = Read_Trim(0x00000324) ;	
        ADCx->AMC_B0 = Read_Trim(0x00000328) ;		
        ADCx->AMC_B1 = Read_Trim(0x0000032C) ;	
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
    ADC_InitStruct->Gain0 = 0;
    ADC_InitStruct->Gain1 = 0;
}
/*******************************************************************************
 函数名称：    void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
 功能描述：    ADC中断标志位清零
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    写1清零
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie          创建
 *******************************************************************************/
void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
{
    ADCx->IF = INT_flag;
}
/*******************************************************************************
 函数名称：    uint16_t ADC_GetIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
 功能描述：    读ADC中断标志位
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    读标志位是否为1
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie           创建
 *******************************************************************************/
u16 ADC_GetIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
{
	  u8 ADCIRQFlag = 0;
	  if(ADCx->IF & INT_flag & ADCx->IE)
		{
		    ADCIRQFlag = 1;
		}
		else
		{
		    ADCIRQFlag = 0;
		}
    return ADCIRQFlag;
}
/*******************************************************************************
 函数名称：    void ADC_ChannelConfig(ADC_TypeDef* ADCx,CHNx CHNum, uint16_t Channel_0 ,uint16_t Channel_1)
 功能描述：    ADC采样通道设置
 操作的表：    无
 输入参数：    ADCx，08系列固定选择ADC0；CHNum为枚举类型，对应0-19；Channel_0为采样通道序号对应0-19
 输出参数：    无
 返 回 值：    无
 其它说明：    
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie          创建
 *******************************************************************************/
void ADC_ChannelConfig(ADC_TypeDef* ADCx,CHNx CHNum, uint16_t nChannel)
{
  u32 *p;
	u32 reg;
	p = (u32*)&ADCx->CHN0;
	reg = p[CHNum >> 1];
	reg &= ~(0xff << (8*(CHNum&1)));
	reg |= nChannel << (8*(CHNum&1));
	p[CHNum >> 1] = reg;
}
/*******************************************************************************
 函数名称：    s16 ADC_GetConversionValue(DATNx DATNum)
 功能描述：    读ADC采样值
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie          创建
 *******************************************************************************/
s16 ADC_GetConversionValue(DATx DATNum)
{
    if(DATNum == DAT0)
		{
		  return ADC0_DAT0;
		}
		else if(DATNum == DAT1)
		{
		  return ADC0_DAT1;
		}
		else if(DATNum == DAT2)
		{
		  return ADC0_DAT2;
		}
		else if(DATNum == DAT3)
		{
		  return ADC0_DAT3;
		}
		else if(DATNum == DAT4)
		{
		  return ADC0_DAT4;
		}
		else if(DATNum == DAT5)
		{
		  return ADC0_DAT5;
		}
		else if(DATNum == DAT6)
		{
		  return ADC0_DAT6;
		}
		else if(DATNum == DAT7)
		{
		  return ADC0_DAT7;
		}
		else if(DATNum == DAT8)
		{
		  return ADC0_DAT8;
		}
		else if(DATNum == DAT9)
		{
		  return ADC0_DAT9;
		}
		else if(DATNum == DAT10)
		{
		  return ADC0_DAT10;
		}
		else if(DATNum == DAT11)
		{
		  return ADC0_DAT11;
		}
		else if(DATNum == DAT12)
		{
		  return ADC0_DAT12;
		}
		else if(DATNum == DAT13)
		{
		  return ADC0_DAT13;
		}
		else if(DATNum == DAT14)
		{
		  return ADC0_DAT14;
		}
		else if(DATNum == DAT15)
		{
		  return ADC0_DAT15;
		}
		else if(DATNum == DAT16)
		{
		  return ADC0_DAT16;
		}
		else if(DATNum == DAT17)
		{
		  return ADC0_DAT17;
		}
		else if(DATNum == DAT18)
		{
		  return ADC0_DAT18;
		}
		else//(DATNum == DAT19)
		{
		  return ADC0_DAT19;
		}
}

/*******************************************************************************
 函数名称：    void ADC_SoftTrgEN(ADC_TypeDef* ADCx, FuncState state)
 功能描述：    ADC采样软件触发使能
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    ADC0_SWT写0x5AA5仅触发一次，完成后自动清零，等待下次软件触发
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/24    V1.0            Zhu Jie          创建
 *******************************************************************************/
void ADC_SoftTrgEN(ADC_TypeDef* ADCx, FuncState state)
{
    if( state == ENABLE)
		{
			  ADC0_SWT = 0x5AA5;
		}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
