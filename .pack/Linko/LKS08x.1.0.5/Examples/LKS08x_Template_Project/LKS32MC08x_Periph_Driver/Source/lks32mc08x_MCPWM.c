/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： LKS32MC081_adc.c
 * 文件标识：
 * 内容摘要： MCPWM外设驱动程序
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
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x.h"
#include "string.h"

void PWMOutputs(FuncState t_state)
{
   MCPWM_PRT       = 0x0000DEAD;
  
   if(t_state == ENABLE)
   {
      MCPWM_FAIL |= MCPWM_MOE_ENABLE_MASK;
   }
   else
   {
      MCPWM_FAIL &= MCPWM_MOE_DISABLE_MASK;
   }
   MCPWM_PRT       = 0x0000CAFE;
}

/*******************************************************************************
 函数名称：    void MCPWM_StructInit(UART_InitTypeDef* UART_InitStruct)
 功能描述：    MCPWM结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2019/05/21     V1.0           howlet              创建
 *******************************************************************************/
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct)
{
	 
	 memset(MCPWM_InitStruct,0,sizeof(MCPWM_InitTypeDef));
}


/*******************************************************************************
 函数名称：    void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct)
 功能描述：    MCPWM初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct)
{
	 MCPWM_PRT = 0x0000DEAD; /*enter password to unlock write protection */
	 MCPWM_TCLK = MCPWM_InitStruct->CLK_DIV | (MCPWM_InitStruct->MCLK_EN<<2) | (MCPWM_InitStruct->MCPWM_Cnt_EN<<3)
	              | (MCPWM_InitStruct->GPIO_BKIN_Filter << 8) | (MCPWM_InitStruct->CMP_BKIN_Filter << 12);
	 MCPWM_TH = MCPWM_InitStruct->MCPWM_PERIOD;
	
	 MCPWM_TMR0 = MCPWM_InitStruct->TriggerPoint0;
	 MCPWM_TMR1 = MCPWM_InitStruct->TriggerPoint1;
	 MCPWM_TMR2 = MCPWM_InitStruct->TriggerPoint2;
	 MCPWM_TMR3 = MCPWM_InitStruct->TriggerPoint3;
	
	 MCPWM_IO01 = MCPWM_InitStruct->CH0N_Polarity_INV | (MCPWM_InitStruct->CH0P_Polarity_INV<<1) \
	             | (MCPWM_InitStruct->Switch_CH0N_CH0P<<6) | (MCPWM_InitStruct->MCPWM_WorkModeCH0<<7) \
	             | (MCPWM_InitStruct->CH1N_Polarity_INV<<8) | (MCPWM_InitStruct->CH1P_Polarity_INV<<9) \
	             | (MCPWM_InitStruct->Switch_CH1N_CH1P<<14) | (MCPWM_InitStruct->MCPWM_WorkModeCH1<<15);
	
	 MCPWM_IO23 = MCPWM_InitStruct->CH2N_Polarity_INV | (MCPWM_InitStruct->CH2P_Polarity_INV<<1) \
	             | (MCPWM_InitStruct->Switch_CH2N_CH2P<<6) | (MCPWM_InitStruct->MCPWM_WorkModeCH2<<7) \
	             | (MCPWM_InitStruct->CH3N_Polarity_INV<<8) | (MCPWM_InitStruct->CH3P_Polarity_INV<<9) \
	             | (MCPWM_InitStruct->Switch_CH3N_CH3P<<14) | (MCPWM_InitStruct->MCPWM_WorkModeCH3<<15);
	
	 MCPWM_FAIL =  MCPWM_InitStruct->FAIL0_Signal_Sel | (MCPWM_InitStruct->FAIL0_Polarity<<2)
	             | (MCPWM_InitStruct->FAIL0_INPUT_EN<<4) | (MCPWM_InitStruct->FAIL1_INPUT_EN<<5) 
							 | (MCPWM_InitStruct->FAIL1_Signal_Sel<<1) | (MCPWM_InitStruct->FAIL1_Polarity<<3)
							 | (MCPWM_InitStruct->DebugMode_PWM_out<<7)
               | (MCPWM_InitStruct->CH0P_default_output<<8) | (MCPWM_InitStruct->CH0N_default_output<<9) 
               | (MCPWM_InitStruct->CH1P_default_output<<10) | (MCPWM_InitStruct->CH1N_default_output<<11) 
               | (MCPWM_InitStruct->CH2P_default_output<<12) | (MCPWM_InitStruct->CH2N_default_output<<13) 
               | (MCPWM_InitStruct->CH3P_default_output<<14) | (MCPWM_InitStruct->CH3N_default_output<<15); 							 
   
   MCPWM_SDCFG = MCPWM_InitStruct->MCPWM_UpdateInterval | (MCPWM_InitStruct->MCPWM_T0_UpdateEN<<4)
	             | (MCPWM_InitStruct->MCPWM_T1_UpdateEN<<5) | (MCPWM_InitStruct->MCPWM_Auto_ERR_EN<<6);
							 
   MCPWM_DTH00 = MCPWM_InitStruct->DeadTimeCH0N;
	 MCPWM_DTH01 = MCPWM_InitStruct->DeadTimeCH0P;
	 MCPWM_DTH10 = MCPWM_InitStruct->DeadTimeCH1N;
	 MCPWM_DTH11 = MCPWM_InitStruct->DeadTimeCH1P;
	 MCPWM_DTH20 = MCPWM_InitStruct->DeadTimeCH2N;
	 MCPWM_DTH21 = MCPWM_InitStruct->DeadTimeCH2P;
	 MCPWM_DTH30 = MCPWM_InitStruct->DeadTimeCH3N;
	 MCPWM_DTH31 = MCPWM_InitStruct->DeadTimeCH3P;
	 
	 MCPWM_EIF = 0xffff;
	 MCPWM_IE = MCPWM_InitStruct->T0_Update_INT_EN | (MCPWM_InitStruct->T1_Update_INT_EN << 1)
	         | (MCPWM_InitStruct->TMR0_Match_INT_EN << 10) | (MCPWM_InitStruct->TMR1_Match_INT_EN << 11);
					 
	 MCPWM_EIE = (MCPWM_InitStruct->FAIL0_INT_EN<<4) | (MCPWM_InitStruct->FAIL1_INT_EN<<5);
	 
	 MCPWM_UPDATE = 0xffff;    /* write whatever value to trigger update */

   MCPWM_UPDATE = 0x00;

   MCPWM_PRT = 0x0000CAFE;  /* write any value other than 0xDEAD to enable write protection */
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
