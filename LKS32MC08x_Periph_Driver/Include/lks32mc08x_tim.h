/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_tim.h
 * 文件标识：
 * 内容摘要： TIM驱动头文件
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

#ifndef __LKS32MC08x_TIM_H
#define __LKS32MC08x_TIM_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"


typedef struct
{
    __IO uint32_t CFG;
    __IO uint32_t TH;
    __IO uint32_t CNT;
    __IO uint32_t CMPT0;
    __IO uint32_t CMPT1;
    __IO uint32_t EVT;
} TIM_TimerTypeDef;

typedef struct
{
    __IO uint32_t CFG;
    __IO uint32_t TH;
    __IO uint32_t CNT;
} TIM_ECDTypeDef;

#ifndef TIMER0
#define TIMER0              ((TIM_TimerTypeDef *)  UTIMER_BASE)
#endif
#ifndef TIMER1
#define TIMER1              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x20))
#endif
#ifndef TIMER2
#define TIMER2              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x40))
#endif
#ifndef TIMER3
#define TIMER3              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x60))
#endif
#ifndef ECD0
#define ECD0                ((TIM_ECDTypeDef *)   (UTIMER_BASE+0x80))
#endif
#ifndef ECD1
#define ECD1                ((TIM_ECDTypeDef *)   (UTIMER_BASE+0x90))
#endif

#define TIM_Clk_Div1          0x00         /*Timer工作频率96M*/
#define TIM_Clk_Div2          0x01         /*Timer工作频率48M*/
#define TIM_Clk_Div4          0x02         /*Timer工作频率24M*/
#define TIM_Clk_Div8          0x03         /*Timer工作频率12M*/

#define ECD_Clk_Div1          0x00         /*ECD工作频率96M*/
#define ECD_Clk_Div2          0x01         /*ECD工作频率48M*/
#define ECD_Clk_Div4          0x02         /*ECD工作频率24M*/
#define ECD_Clk_Div8          0x03         /*ECD工作频率12M*/

/*中断使能配置定义*/
typedef enum
{
    Timer_IRQEna_None = 0,
    Timer_IRQEna_Zero = BIT0,     /*使能过零中断*/
    Timer_IRQEna_CH0 = BIT1,      /*使能CH0中断，含比较、捕获中断*/
    Timer_IRQEna_CH1 = BIT2,      /*使能CH1中断，含比较、捕获中断*/
    Timer_IRQEna_All = 0x07       /*使能Timer全部中断*/
} Timer_IRQEnaDef;

/*中断标志定义*/																 											 
#define TIMER_IF_CH1            BIT2     /*Timer CH1中断，比较或捕获中断*/
#define TIMER_IF_CH0            BIT1     /*Timer CH0中断，比较或捕获中断*/
#define TIMER_IF_ZERO           BIT0     /*Timer 过零中断*/
																 												 
/*编码器中断使能配置定义*/         
#define ECD_IRQEna_None          0     /*无使能*/
#define ECD_IRQEna_UnderFlow     BIT0  /*使能下溢中断*/
#define ECD_IRQEna_OverFlow      BIT1  /*使能上溢中断*/
																 
/*编码器中断标志定义*/            
#define ECD_IF_UNDERFLOW        BIT0 /*Encoder下溢中断*/
#define ECD_IF_OVERFLOW         BIT1 /*Encoder上溢中断*/
																 
/*捕获模式定义*/
#define TIMER_CapMode_None       ((uint32_t)0x00000000)   /*无捕获*/
#define TIMER_CapMode_Rise       ((uint32_t)0x00000001)   /*上升沿捕获*/
#define TIMER_CapMode_Fall       ((uint32_t)0x00000002)   /*下降沿捕获*/
#define TIMER_CapMode_RiseFall   ((uint32_t)0x00000003)   /*双沿捕获*/
															   
#define TIMER_OPMode_CAP         ((uint32_t)0x0000001)    /*捕获模式*/
#define TIMER_OPMode_CMP         ((uint32_t)0x0000000)    /*比较模式*/


#define ECD_Mode_T1              ((uint32_t)0x00) /*counting  on T1*/
#define ECD_Mode_T1_T2           ((uint32_t)0x01) /*counting  on T1&T2*/
#define ECD_Mode_CCWSIGN         ((uint32_t)0x02) /*CCW+SIGN, 仅计数信号上升沿*/
#define ECD_Mode_CCWCW           ((uint32_t)0x03) /*CCW+CW,   仅计数信号上升沿*/
#define ECD_Mode_CCWSIGN_UpDown  ((uint32_t)0x06) /*符号加脉冲信号计数模式, 信号的上下沿均计数*/
#define ECD_Mode_CCWCW_UpDown    ((uint32_t)0x07) /*CCW+CW 双脉冲信号计数模式，信号的上下沿均计数*/


typedef struct
{
    uint32_t Timer_CH0_WorkMode;  /*channel0工作模式:捕获模式，比较模式*/
    uint32_t Timer_CH0_CapMode;   /*channel0捕获模式:上升沿、下降沿、双沿*/
    uint32_t Timer_CH0Output;     /*计数器回零时，比较模式IO输出*/

    uint32_t Timer_CH1_WorkMode;  /*channel1工作模式:捕获模式，比较模式*/
    uint32_t Timer_CH1_CapMode;   /*channel1捕获模式:上升沿、下降沿、双沿*/
    uint32_t Timer_CH1Output;     /*计数器回零时，比较模式IO输出*/

    uint32_t Timer_TH;            /*定时器计数门限*/
    uint32_t Timer_CMP0;          /*CH0比较计数器值*/
    uint32_t Timer_CMP1;          /*CH1比较计数器值*/

    uint32_t Timer_Filter0;       /*CH0滤波分频, 0:不滤波 N:滤波采用的时钟是系统时钟N分频*/
    uint32_t Timer_Filter1;       /*CH1滤波分频*/

    uint32_t Timer_ClockDiv;      /*定时器分频*/
    uint32_t Timer_IRQEna;        /*定时器中断使能*/
} TIM_TimerInitTypeDef;

typedef struct
{
    uint32_t ECD_Mode;     /*编码器模式选择*/
    uint32_t ECD_TH;       /*编码器计数门限值*/
    uint32_t ECD_ClockDiv; /*编码器对应UTIMER时钟分频系数*/
    uint32_t ECD_Filter0;  /*编码器0滤波分频*/
    uint32_t ECD_Filter1;  /*编码器1滤波分频*/
    uint32_t ECD_IRQEna;   /*编码器中断使能*/
} TIM_ECDInitTypeDef;


/*Timer初始化*/
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct);
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct);
// void TIM_StartTimer(TIM_TimerTypeDef *TIMERx);
// void TIM_StopTimer(TIM_TimerTypeDef *TIMERx);
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state);

/*TIM获取中断标志*/
uint32_t TIM_GetIRQFlag(TIM_TimerTypeDef *TIMERx , u32 timer_if);
/*TIM中断清除*/
void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag);

uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIM_TIMERx);
uint32_t TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIM_TIMERx);
uint32_t TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIM_TIMERx);

/*ECD初始化*/
void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct);
void TIM_ECDStructInit(TIM_ECDInitTypeDef *TIM_ECDInitStruct);
void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state);

uint32_t TIM_ECD_GetCount(TIM_ECDTypeDef *TIM_ECDx);
/*ECD获取中断标志*/
uint32_t ECD_GetIRQFlag(TIM_ECDTypeDef *ECDx , u32 timer_if);
/*ECD中断清除*/
void ECD_ClearIRQFlag(TIM_ECDTypeDef *ECDx , uint32_t tempFlag);
#endif /*__lks32mc08x_TIM_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
