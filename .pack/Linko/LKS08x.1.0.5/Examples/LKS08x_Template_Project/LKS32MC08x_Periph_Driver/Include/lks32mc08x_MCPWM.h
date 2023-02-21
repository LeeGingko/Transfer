/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Chanhom_adc.h
 * 文件标识：
 * 内容摘要： ADC外设驱动程序头文件
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
 
#ifndef __LKS08X_PWM_H
#define __LKS08X_PWM_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"



typedef struct
{
   uint16_t MCPWM_PERIOD;           /* MCPWM输出周期*/
	 uint8_t CLK_DIV;                 /* MCPWM 分频系数 */
	 uint8_t MCLK_EN;                 /* MCPWM 时钟使能开关 */
	 uint8_t MCPWM_Cnt_EN;            /* MCPWM 主计数器使能开关 */
	 uint8_t GPIO_BKIN_Filter;        /* GPIO输入滤波时钟设置1-16 */
	 uint8_t CMP_BKIN_Filter;         /* 比较器CMP输入滤波时钟设置1-16 */	
	 uint16_t MCPWM_WorkModeCH0;      /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
	 uint16_t MCPWM_WorkModeCH1;      /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
	 uint16_t MCPWM_WorkModeCH2;      /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
	 uint16_t MCPWM_WorkModeCH3;      /* MCPWM CH0工作模式：边沿对齐/中心对齐 */	
	
	 uint16_t TriggerPoint0;          /* PWM触发ADC事件0，时间点设置 */
	 uint16_t TriggerPoint1;          /* PWM触发ADC事件1，时间点设置 */
	 uint16_t TriggerPoint2;          /* PWM触发ADC事件2，时间点设置 */
	 uint16_t TriggerPoint3;          /* PWM触发ADC事件3，时间点设置 */
	
   uint16_t DeadTimeCH0N;           /* CH0N死区时间设置　*/
   uint16_t DeadTimeCH0P;           /* CH0P死区时间设置　*/
   uint16_t DeadTimeCH1N;           /* CH1N死区时间设置　*/
   uint16_t DeadTimeCH1P;           /* CH1P死区时间设置　*/
   uint16_t DeadTimeCH2N;           /* CH2N死区时间设置　*/
   uint16_t DeadTimeCH2P;           /* CH2P死区时间设置　*/
   uint16_t DeadTimeCH3N;           /* CH3N死区时间设置　*/
   uint16_t DeadTimeCH3P;           /* CH3P死区时间设置　*/	
	 
	 uint8_t CH0N_Polarity_INV;       /* CH0N输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH0P_Polarity_INV;       /* CH0P输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH1N_Polarity_INV;       /* CH1N输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH1P_Polarity_INV;       /* CH1P输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH2N_Polarity_INV;       /* CH2N输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH2P_Polarity_INV;       /* CH2P输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH3N_Polarity_INV;       /* CH3N输出极性取反，0:正常输出；1:取反输出 */
	 uint8_t CH3P_Polarity_INV;       /* CH3P输出极性取反，0:正常输出；1:取反输出 */	

   uint8_t Switch_CH0N_CH0P;        /* 交换CH0N, CH0P信号输出使能开关 */
   uint8_t Switch_CH1N_CH1P;        /* 交换CH1N, CH1P信号输出使能开关 */
   uint8_t Switch_CH2N_CH2P;        /* 交换CH2N, CH2P信号输出使能开关 */
   uint8_t Switch_CH3N_CH3P;        /* 交换CH3N, CH3P信号输出使能开关 */	

   uint8_t MCPWM_UpdateInterval;    /* MCPWM T0/T1事件更新间隔 */
   uint8_t MCPWM_T0_UpdateEN;       /* MCPWM T0事件更新使能 */
	 uint8_t MCPWM_T1_UpdateEN;       /* MCPWM T1事件更新使能 */
	 uint8_t MCPWM_Auto_ERR_EN;       /* MCPWM 更新事件是否自动打开MOE, 使能开关 */
	 
	 uint8_t FAIL0_INPUT_EN;          /* FAIL0 输入功能使能 */
	 uint8_t FAIL1_INPUT_EN;          /* FAIL1 输入功能使能 */	 
	 uint8_t FAIL0_Signal_Sel;        /* FAIL0 信号选择，比较器0或GPIO */
	 uint8_t FAIL1_Signal_Sel;        /* FAIL1 信号选择，比较器0或GPIO */	 
	 uint8_t FAIL0_Polarity;          /* FAIL0 信号极性设置，高有效或低有效 */	 
	 uint8_t FAIL1_Polarity;          /* FAIL1 信号极性设置，高有效或低有效 */
   uint8_t DebugMode_PWM_out;       /* Debug时，MCU进入Halt, MCPWM信号是否正常输出 */
	 
	 uint8_t CH0P_default_output;     /* CH0P MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH0N_default_output;     /* CH0N MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH1P_default_output;     /* CH1P MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH1N_default_output;     /* CH1N MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH2P_default_output;     /* CH2P MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH2N_default_output;     /* CH2N MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH3P_default_output;     /* CH3P MOE为0时或发生FAIL事件时，默认电平输出 */
	 uint8_t CH3N_default_output;     /* CH3N MOE为0时或发生FAIL事件时，默认电平输出 */	

   uint8_t T0_Update_INT_EN;        /* T0更新事件中斷使能 */
   uint8_t T1_Update_INT_EN;        /* T1更新事件中斷使能 */  
   uint8_t TMR0_Match_INT_EN;       /* TMR0计数事件匹配事件中断使能 */    
   uint8_t TMR1_Match_INT_EN;       /* TMR1计数事件匹配事件中断使能 */    

   uint8_t FAIL0_INT_EN;            /* FAIL0事件中断使能 */
   uint8_t FAIL1_INT_EN;            /* FAIL1事件中断使能 */	 

}MCPWM_InitTypeDef;

#define MCPWM_MOE_ENABLE_MASK                     ((uint16_t)0x0040)   /* 打开MOE位MASK位 */
#define MCPWM_MOE_DISABLE_MASK                    ((uint16_t)~0x0040)   /* 关MOE位MASK位 */

#define PWM_F_SELECT_CMP0    BIT0
#define PWM_F_SELECT_CMP1    BIT1
#define PWM_F_LFAIL_CH0      BIT2
#define PWM_F_LFAIL_CH1      BIT3
#define PWM_F_ENABLE_CH0     BIT4
#define PWM_F_ENABLE_CH1     BIT5

#define PWM_ENABLE_BK0IF     BIT4
#define PWM_ENABLE_BK1IF     BIT5
#define PWM_ENABLE_CMP0IF    BIT6
#define PWM_ENABLE_CMP1IF    BIT7

#define CENTRAL_PWM_MODE     0  /* 中心对齐PWM模式 */
#define EDGE_PWM_MODE        1  /* 边沿对齐PWM模式 */

#define HIGH_LEVEL           1  /* 高电平 */
#define LOW_LEVEL            0  /* 高电平 */

#define MCPWM_UPDATE_REG()                        {MCPWM_PRT = 0x0000DEAD; MCPWM_UPDATE = 0x00ff; MCPWM_PRT = 0x0000CAFE;}

void PWMOutputs(FuncState t_state);
void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct);
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct);

#endif /*__CHANHOM_PWM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
