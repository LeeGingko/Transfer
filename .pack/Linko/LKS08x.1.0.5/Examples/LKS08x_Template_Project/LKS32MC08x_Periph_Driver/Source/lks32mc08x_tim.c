/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_tim.c
 * 文件标识：
 * 内容摘要： TIM驱动程序
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
#include "lks32mc08x_tim.h"
#include "lks32mc08x.h"



/*******************************************************************************
 函数名称：    void TIM_TimerInit(TIM_Timer_TypeDef *TIM_TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct)
 功能描述：    Timer初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct)
{
   uint32_t tempCfg;
   uint32_t tempIE;
   uint32_t tempVal;
   uint32_t tempFilter;
   uint32_t filter0;
   uint32_t filter1;
   TIMERx->CFG = TIM_TimerInitStruct->Timer_CH0_CapMode | (TIM_TimerInitStruct->Timer_CH0_WorkMode << 2) 
	          | (TIM_TimerInitStruct->Timer_CH0Output << 3) | (TIM_TimerInitStruct->Timer_CH1_CapMode << 4) 
			      | (TIM_TimerInitStruct->Timer_CH1_WorkMode << 6) | (TIM_TimerInitStruct->Timer_CH1Output << 7)
			      | (TIM_TimerInitStruct->Timer_ClockDiv << 8);
   TIMERx->TH = TIM_TimerInitStruct->Timer_TH;
   TIMERx->CMPT0 = TIM_TimerInitStruct->Timer_CMP0;
   TIMERx->CMPT1 = TIM_TimerInitStruct->Timer_CMP1;
		
   /*使能定时器*/
   /*使能定时器中断*/
   /*滤波参数*/
   tempCfg = UTIMER_CFG;
   tempIE = UTIMER_IE;
   filter0 = TIM_TimerInitStruct->Timer_Filter0 & 0x000F;	/*滤波参数只使用低四位*/
   filter1 = TIM_TimerInitStruct->Timer_Filter1 & 0x000F;
   if (TIMERx == TIMER0)
   {
      tempCfg |= BIT4;
      tempVal = 0x0007;
      tempIE &= ~tempVal;
      tempIE |= (TIM_TimerInitStruct->Timer_IRQEna);

      tempFilter = UTIMER_FLT_TH01;
      tempFilter &= 0xFF00;
      tempFilter |= filter0;
      tempFilter |= (filter1 << 4);
      UTIMER_FLT_TH01 = tempFilter;
   }
   else if (TIMERx == TIMER1)
   {
      tempCfg |= BIT5;
      tempVal = 0x0038;
      tempIE &= ~tempVal;
      tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 3);

      tempFilter = UTIMER_FLT_TH01;
      tempFilter &= 0x00FF;
      tempFilter |= (filter0 << 8);
      tempFilter |= (filter1 << 12);
      UTIMER_FLT_TH01 = tempFilter;
   }
   else if (TIMERx == TIMER2)
   {
      tempCfg |= BIT6;
      tempVal = 0x01C0;
      tempIE &= ~tempVal;
      tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 6);

      tempFilter = UTIMER_FLT_TH23;
      tempFilter &= 0xFF00;
      tempFilter |= filter0;
      tempFilter |= (filter1 << 4);
      UTIMER_FLT_TH23 = tempFilter;
   }
   else if (TIMERx == TIMER3)
   {
      tempCfg |= BIT7;
      tempVal = 0x0E00;
      tempIE &= ~tempVal;
      tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 9);

      tempFilter = UTIMER_FLT_TH23;
      tempFilter &= 0x00FF;
      tempFilter |= (filter0 << 8);
      tempFilter |= (filter1 << 12);
      UTIMER_FLT_TH23 = tempFilter;
   }
   UTIMER_CFG = tempCfg;
   UTIMER_IE = tempIE;
}

/*******************************************************************************
 函数名称：    void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct)
 功能描述：    Timer结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct)
{
   TIM_TimerInitStruct->Timer_CH0_CapMode = TIMER_CapMode_None;
   TIM_TimerInitStruct->Timer_CH0_WorkMode = TIMER_OPMode_CMP;
   TIM_TimerInitStruct->Timer_CH0Output = 0;

   TIM_TimerInitStruct->Timer_CH1_CapMode = TIMER_CapMode_None;
   TIM_TimerInitStruct->Timer_CH1_WorkMode = TIMER_OPMode_CMP;
   TIM_TimerInitStruct->Timer_CH1Output = 0;

   TIM_TimerInitStruct->Timer_TH = 1000;
   TIM_TimerInitStruct->Timer_CMP0 = 500;
   TIM_TimerInitStruct->Timer_CMP1 = 500;
   TIM_TimerInitStruct->Timer_Filter0 = 0;
   TIM_TimerInitStruct->Timer_Filter1 = 0;
   TIM_TimerInitStruct->Timer_ClockDiv = TIM_Clk_Div1;
   TIM_TimerInitStruct->Timer_IRQEna = Timer_IRQEna_None;
}

/*******************************************************************************
 函数名称：    void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
 功能描述：    定时器TIMERx使能和停止
 操作的表：    无
 输入参数：    TIMERx
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/07/04      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
{
	if (state != DISABLE)
	{
		 if (TIMERx == TIMER0)
		 {
			  UTIMER_CFG |= BIT4;
		 }
		 else if (TIMERx == TIMER1)
		 {
			  UTIMER_CFG |= BIT5;
		 }
		 else if (TIMERx == TIMER2)
		 {
			  UTIMER_CFG |= BIT6;
		 }
		 else if (TIMERx == TIMER3)
		 {
			  UTIMER_CFG |= BIT7;
		 }
		
	}
	else
	{
		 if (TIMERx == TIMER0)
		 {
			  UTIMER_CFG &= ~BIT4;
		 }
		 else if (TIMERx == TIMER1)
		 {
			  UTIMER_CFG &= ~BIT5;
		 }
		 else if (TIMERx == TIMER2)
		 {
			  UTIMER_CFG &= ~BIT6;
		 }
		 else if (TIMERx == TIMER3)
		 {
			  UTIMER_CFG &= ~BIT7;
		 }
	}
}

/*******************************************************************************
 函数名称：    uint32_t TIM_GetIRQFlag(void)
 功能描述：    TIM中断标志
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    TIM中断标志
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
uint32_t TIM_GetIRQFlag(void)
{
   return UTIMER_IF;
}

/*******************************************************************************
 函数名称：    void TIM_ClearIRQFlag(uint32_t iFlag)
 功能描述：    清除TIM中断标志
 操作的表：    无
 输入参数：    TIM中断标志
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_ClearIRQFlag(uint32_t tempFlag)
{
   UTIMER_IF = tempFlag;
}

/*******************************************************************************
 函数名称：    uint32_t TIM_Timer_GetCount(TIM_Timer_TypeDef *TIM_TIMERx)
 功能描述：    Timer计数值
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
  2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx)
{
   return TIMERx->CNT;
}

/*******************************************************************************
 函数名称：    uint32_t TIM_Timer_GetCapture0(TIM_Timer_TypeDef *TIM_TIMERx)
 功能描述：    获取捕获值0
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
uint32_t TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIM_TIMERx)
{
   return TIM_TIMERx->CMPT0;
}

/*******************************************************************************
 函数名称：    uint32_t TIM_Timer_GetCapture1(TIM_Timer_TypeDef *TIM_TIMERx)
 功能描述：    获取捕获值1
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
uint32_t TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIM_TIMERx)
{
   return TIM_TIMERx->CMPT1;  
}

/*******************************************************************************
 函数名称：    void TIM_ECDInit(TIM_ECD_TypeDef *TIM_ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct)
 功能描述：    ECD初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct)
{
   uint32_t tempIE;
   uint32_t tempFilter;
   uint32_t filter0;
   uint32_t filter1;
   uint32_t tempTimerCfg;

   ECDx->CFG = TIM_ECDInitStruct->ECD_Mode << 8;
   ECDx->TH = TIM_ECDInitStruct->ECD_TH;

   filter0 = TIM_ECDInitStruct->ECD_Filter0 & 0x000F;	/*滤波参数只使用低四位*/
   filter1 = TIM_ECDInitStruct->ECD_Filter1 & 0x000F;
   tempIE = UTIMER_IE;
   if (ECDx == ECD0)
   {
      tempFilter = UTIMER_FLT_TH23;
      tempFilter &= 0xFF00;
      tempFilter |= filter0;
      tempFilter |= (filter1 << 4);
      UTIMER_FLT_TH23 = tempFilter;

      tempIE |= (TIM_ECDInitStruct->ECD_IRQEna << 12);

	  tempTimerCfg = TIMER2->CFG;
      tempTimerCfg &= ~(0x0300);
	  tempTimerCfg |= (TIM_ECDInitStruct->ECD_ClockDiv << 8);
	  TIMER2->CFG = tempTimerCfg;
		  UTIMER_CFG |= BIT8;
   }
   else if (ECDx == ECD1)
   {
      tempFilter = UTIMER_FLT_TH23;
      tempFilter &= 0x00FF;
      tempFilter |= (filter0 << 8);
      tempFilter |= (filter1 << 12);
	  UTIMER_FLT_TH23 = tempFilter;

      tempIE |= (TIM_ECDInitStruct->ECD_IRQEna << 14);

	  tempTimerCfg = TIMER3->CFG;
      tempTimerCfg &= ~(0x0300);
	  tempTimerCfg |= (TIM_ECDInitStruct->ECD_ClockDiv << 8);
	  TIMER3->CFG = tempTimerCfg;
		  UTIMER_CFG |= BIT9;
   }
   UTIMER_IE = tempIE;
}

/*******************************************************************************
 函数名称：    void TIM_ECDStructInit(TIM_ECDInitTypeDef *TIM_ECDInitStruct)
 功能描述：    ECD结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_ECDStructInit(TIM_ECDInitTypeDef *TIM_ECDInitStruct)
{
   TIM_ECDInitStruct->ECD_Mode = ECD_Mode_T1;
   TIM_ECDInitStruct->ECD_TH = 1000;
   TIM_ECDInitStruct->ECD_ClockDiv = TIM_Clk_Div1;
   TIM_ECDInitStruct->ECD_Filter0 = 0;
   TIM_ECDInitStruct->ECD_Filter1 = 0;
   TIM_ECDInitStruct->ECD_IRQEna = ECD_IRQEna_UnderFlow | ECD_IRQEna_OverFlow;
}

/*******************************************************************************
 函数名称：    void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state)
 功能描述：    编码器ECDx使能和停止
 操作的表：    无
 输入参数：    ECDx
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/07/21      V1.0           cfwu              创建
 *******************************************************************************/
void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state)
{
	uint32_t tempEna;
    tempEna = UTIMER_CFG;

	if (state != DISABLE)
	{
		if (ECDx == ECD0)
		{
			tempEna |= BIT8;
		}
		else if (ECDx == ECD1)
		{
			tempEna |= BIT9;
		}		
	}
	else
	{
		if (ECDx == ECD0)
		{
			tempEna &= ~BIT8;
		}
		else if (ECDx == ECD1)
		{
			tempEna &= ~BIT9;
		}
	}
	UTIMER_CFG = tempEna;
}

/*******************************************************************************
 函数名称：    uint32_t TIM_ECD_GetCount(TIM_ECD_TypeDef *TIM_ECDx)
 功能描述：    ECD计数值
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              创建
 *******************************************************************************/
uint32_t TIM_ECD_GetCount(TIM_ECDTypeDef *TIM_ECDx)
{
   return TIM_ECDx->CNT;	
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
