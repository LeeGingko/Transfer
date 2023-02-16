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
 
#ifndef __LKS32MC08x_MCPWM_H
#define __LKS32MC08x_MCPWM_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"



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
   
    uint8_t CH0P_SCTRLP;             /* 当CH0_PS = 1时，输出到CH0_P通道的值*/
    uint8_t CH0N_SCTRLN;             /* 当CH0_NS = 1时，输出到CH0_N通道的值*/
    uint8_t CH1P_SCTRLP;             /* 当CH1_PS = 1时，输出到CH1_P通道的值*/
    uint8_t CH1N_SCTRLN;             /* 当CH1_NS = 1时，输出到CH1_N通道的值*/
    uint8_t CH2P_SCTRLP;             /* 当CH2_PS = 1时，输出到CH2_P通道的值*/
    uint8_t CH2N_SCTRLN;             /* 当CH2_NS = 1时，输出到CH2_N通道的值*/
    uint8_t CH3P_SCTRLP;             /* 当CH3_PS = 1时，输出到CH3_P通道的值*/
    uint8_t CH3N_SCTRLN;             /* 当CH3_NS = 1时，输出到CH3_PN通道的值*/
   
	  uint8_t CH0_PS;                  /* CH0_P信号来源，1：来自CH0_SCTRLP；0：来自MCPWM实际运行计数器*/
		uint8_t CH0_NS;                  /* CH0_N信号来源，1：来自CH0_SCTRLN；0：来自MCPWM实际运行计数器*/
		uint8_t CH1_PS;                  /* CH1_P信号来源，1：来自CH1_SCTRLP；0：来自MCPWM实际运行计数器*/
		uint8_t CH1_NS;                  /* CH1_N信号来源，1：来自CH1_SCTRLN；0：来自MCPWM实际运行计数器*/
		uint8_t CH2_PS;                  /* CH2_P信号来源，1：来自CH2_SCTRLP；0：来自MCPWM实际运行计数器*/
		uint8_t CH2_NS;                  /* CH2_N信号来源，1：来自CH2_SCTRLN；0：来自MCPWM实际运行计数器*/
		uint8_t CH3_PS;                  /* CH3_P信号来源，1：来自CH3_SCTRLP；0：来自MCPWM实际运行计数器*/
		uint8_t CH3_NS;                  /* CH3_N信号来源，1：来自CH3_SCTRLN；0：来自MCPWM实际运行计数器*/
		
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
    uint8_t TMR2_Match_INT_EN;       /* TMR2计数事件匹配事件中断使能 */    
    uint8_t TMR3_Match_INT_EN;       /* TMR3计数事件匹配事件中断使能 */    

    uint8_t FAIL0_INT_EN;            /* FAIL0事件中断使能 */
    uint8_t FAIL1_INT_EN;            /* FAIL1事件中断使能 */	

    uint8_t IO0_PPE;                 /* IO0推挽模式使能：1使能，0关闭 */
		uint8_t IO1_PPE;                 /* IO1推挽模式使能：1使能，0关闭 */
		uint8_t IO2_PPE;                 /* IO2推挽模式使能：1使能，0关闭 */
		uint8_t IO3_PPE;                 /* IO3推挽模式使能：1使能，0关闭 */

}MCPWM_InitTypeDef;


/* ----------------------PWM 频率及死区定义----------------------------------- */
#define MCPWM_MOE_ENABLE_MASK                     ((uint16_t)0x0040)   /* 打开MOE位 */
#define MCPWM_MOE_DISABLE_MASK                    ((uint16_t)~0x0040)   /* 关MOE位 */


#define CENTRAL_PWM_MODE     0  /* 中心对齐PWM模式 */
#define EDGE_PWM_MODE        1  /* 边沿对齐PWM模式 */

#define HIGH_LEVEL           1  /* 高电平 */
#define LOW_LEVEL            0  /* 高电平 */

#define HIGH_LEVEL_ACTIVE    0  /* 高电平有效 */
#define LOW_LEVEL_ACTIVE     1  /* 低电平有效 */

#define FAIL_SEL_CMP         1  /* Fail事件来源比较器 */
#define FAIL_SEL_IO          0  /* Fail事件来源比较器 */

#define MCPWM_UPDATE_REG()     {MCPWM_PRT = 0x0000DEAD; MCPWM_UPDATE = 0x00ff; MCPWM_PRT = 0x0000CAFE;}

#define MCPWM_T0_IRQ_EN        BIT0  /* 计数器计数值回到MCPWM_TH中断使能 */
#define MCPWM_T1_IRQ_EN        BIT1  /* 计数器计数值回到0中断使能 */
#define MCPWM_TH00_IRQ_EN      BIT2  /* 计数器计数值回到MCPWM_TH00中断使能 */
#define MCPWM_TH01_IRQ_EN      BIT3  /* 计数器计数值回到MCPWM_TH01中断使能 */
#define MCPWM_TH10_IRQ_EN      BIT4  /* 计数器计数值回到MCPWM_TH10中断使能 */
#define MCPWM_TH11_IRQ_EN      BIT5  /* 计数器计数值回到MCPWM_TH11中断使能 */
#define MCPWM_TH20_IRQ_EN      BIT6  /* 计数器计数值回到MCPWM_TH20中断使能 */
#define MCPWM_TH21_IRQ_EN      BIT7  /* 计数器计数值回到MCPWM_TH21中断使能 */
#define MCPWM_TH30_IRQ_EN      BIT8  /* 计数器计数值回到MCPWM_TH30中断使能 */
#define MCPWM_TH31_IRQ_EN      BIT9  /* 计数器计数值回到MCPWM_TH31中断使能 */
#define MCPWM_TMR0_IRQ_EN      BIT10 /* 计数器计数值回到MCPWM_TMR0中断使能 */
#define MCPWM_TMR1_IRQ_EN      BIT11 /* 计数器计数值回到MCPWM_TMR1中断使能 */
#define MCPWM_TMR2_IRQ_EN      BIT12 /* 计数器计数值回到MCPWM_TMR0中断使能 */
#define MCPWM_TMR3_IRQ_EN      BIT13 /* 计数器计数值回到MCPWM_TMR1中断使能 */
#define MCPWM_SHADE_IRQ_EN     BIT14 /* MCPWM_TH/MCPWM_TH00-MCPWM_TH31/
MCPWM_TR0-MCPWM_TR3等寄存器更新到MCPWM实际运行系统中断使能 */

#define MCPWM_CH0_SHORT_IRQ_EN      BIT0     /* MCPWMCH0_P和CH0N同时有效中断使能 */
#define MCPWM_CH1_SHORT_IRQ_EN      BIT1     /* MCPWMCH1_P和CH1N同时有效中断使能 */
#define MCPWM_CH2_SHORT_IRQ_EN      BIT2     /* MCPWMCH2_P和CH2N同时有效中断使能 */
#define MCPWM_CH3_SHORT_IRQ_EN      BIT3     /* MCPWMCH3_P和CH3N同时有效中断使能 */
#define MCPWM_FAIL0_IRQ_EN          BIT4     /* MCPWM_FAIL0中断使能 */
#define MCPWM_FAIL1_IRQ_EN          BIT5     /* MCPWM_FAIL1中断使能 */

#define MCPWM_T0_IRQ_IF        BIT0  /* 计数器计数值回到MCPWM_TH中断标志位 */
#define MCPWM_T1_IRQ_IF        BIT1  /* 计数器计数值回到0中断标志位 */
#define MCPWM_TH00_IRQ_IF      BIT2  /* 计数器计数值回到MCPWM_TH00中断标志位 */
#define MCPWM_TH01_IRQ_IF      BIT3  /* 计数器计数值回到MCPWM_TH01中断标志位 */
#define MCPWM_TH10_IRQ_IF      BIT4  /* 计数器计数值回到MCPWM_TH10中断标志位 */
#define MCPWM_TH11_IRQ_IF      BIT5  /* 计数器计数值回到MCPWM_TH11中断标志位 */
#define MCPWM_TH20_IRQ_IF      BIT6  /* 计数器计数值回到MCPWM_TH20中断标志位 */
#define MCPWM_TH21_IRQ_IF      BIT7  /* 计数器计数值回到MCPWM_TH21中断标志位 */
#define MCPWM_TH30_IRQ_IF      BIT8  /* 计数器计数值回到MCPWM_TH30中断标志位 */
#define MCPWM_TH31_IRQ_IF      BIT9  /* 计数器计数值回到MCPWM_TH31中断标志位 */
#define MCPWM_TMR0_IRQ_IF      BIT10 /* 计数器计数值回到MCPWM_TMR0中断标志位 */
#define MCPWM_TMR1_IRQ_IF      BIT11 /* 计数器计数值回到MCPWM_TMR1中断标志位 */
#define MCPWM_TMR2_IRQ_IF      BIT12 /* 计数器计数值回到MCPWM_TMR0中断标志位 */
#define MCPWM_TMR3_IRQ_IF      BIT13 /* 计数器计数值回到MCPWM_TMR1中断标志位 */
#define MCPWM_SHADE_IRQ_IF     BIT14 /* MCPWM_TH/MCPWM_TH00-MCPWM_TH31/
MCPWM_TR0-MCPWM_TR3等寄存器更新到MCPWM实际运行系统中断标志位 */

#define MCPWM_CH0_SHORT_IRQ_IF      BIT15     /* MCPWMCH0_P和CH0N同时有效中断标志位 */
#define MCPWM_CH1_SHORT_IRQ_IF      BIT16     /* MCPWMCH1_P和CH1N同时有效中断标志位 */
#define MCPWM_CH2_SHORT_IRQ_IF      BIT17     /* MCPWMCH2_P和CH2N同时有效中断标志位 */
#define MCPWM_CH3_SHORT_IRQ_IF      BIT18     /* MCPWMCH3_P和CH3N同时有效中断标志位 */
#define MCPWM_FAIL0_IRQ_IF          BIT19     /* MCPWM_FAIL0中断标志位 */
#define MCPWM_FAIL1_IRQ_IF          BIT20     /* MCPWM_FAIL1中断标志位 */

void PWMOutputs(FuncState t_state);
void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct);
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct);
u16 MCPWM_GetIRQFlag( uint32_t INT_flag);
void MCPWM_ClearIRQFlag( uint32_t INT_flag);
#endif /*__CHANHOM_PWM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
