/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_config.h
 * 文件标识：
 * 内容摘要： 硬件相关文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2018年1月20日
 *
 * 修改记录1：
 * 修改日期：2018年1月20日
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
 
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_
#define MCU_MCLK	((u32)96000000uL) /* MCU运行主频 */
/* ==============================PWM 频率及死区定义=========================== */
#define PWM_MCLK	((u32)96000000uL) /* PWM模块运行主频 */
#define PWM_PRSC ((u8)0) /* PWM模块运行预分频器 */
#define PWM_FREQ ((u16)15000) /* PWM斩波频率 */
/* PWM 周期计数器值 */
#define PWM_PERIOD         ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1))))
#define PWM_TIME_2S        (u16)(2*PWM_FREQ)
#define PWM_TIME_500MS     (u16)(500*PWM_FREQ/1000)
#define PWM_TIME_500US     (u16)(PWM_FREQ/500)
#define PWM_TIME_500uS     (u16)(PWM_FREQ/1000/2)
#define PWM_TIME_1MS       (u16)(PWM_FREQ/1000)
#define PWM_TIME_2MS       (u16)(2*PWM_FREQ/1000)
#define PWM_TIME_4MS       (u16)(4*PWM_FREQ/1000)
#define PWM_TIME_5MS       (u16)(5*PWM_FREQ/1000)
#define PWM_TIME_10MS      (u16)(20*PWM_FREQ/1000)
#define PWM_TIME_20MS      (u16)(40*PWM_FREQ/1000)

#define TIME_2S_BASE_5MS   ((u16)400)
#define TIME_1S6_BASE_5MS  ((u16)130)
#define TIME_0S5_BASE_5MS  ((u16)20)

#define HALL_BLOCK_TIME_S  (u8)(2)
#define HALL_TIMER_TH      (u32)(PWM_MCLK * HALL_BLOCK_TIME_S)
#define SAMPLING_FREQ      ((u16)PWM_FREQ)   // Resolution: 1Hz

#define MAX_MODULE_VALUE   19600 /* 最大调制比 */

#define DEADTIME_NS        ((u16)1000)/* 死区时间 */
#define DEADTIME           (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)

#define DEADTIMECOMPVOLTAGE (u16)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)	//385(1us/((1/15k)*1000000)us)*18907

 

/* ******************************ADC通道号定义****************************************** */

#define ADC0_CHANNEL_OPA0            ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1            ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2            ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3            ADC_CHANNEL_3

#define ADC1_CHANNEL_OPA0            ADC_CHANNEL_0
#define ADC1_CHANNEL_OPA1            ADC_CHANNEL_1
#define ADC1_CHANNEL_OPA2            ADC_CHANNEL_2
#define ADC1_CHANNEL_OPA3            ADC_CHANNEL_3


#endif
 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
