/*******************************************************************************
 * 版权所有 (C)2018, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_adc.h
 * 文件标识：
 * 内容摘要： ADC外设驱动程序头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： William Zhang
 * 完成日期： 2018月25日
 *
 * 修改记录1：
 * 修改日期：2018年7月25日
 * 版 本 号：V 1.0
 * 修 改 人：William Zhang
 * 修改内容：创建
 *
 * 修改记录：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/

#ifndef _LKS32MC08x_ADC_H_
#define _LKS32MC08x_ADC_H_


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
    __IO uint32_t DAT0;   /* ADC数据寄存器0 */
    __IO uint32_t DAT1;   /* ADC数据寄存器1 */
    __IO uint32_t DAT2;   /* ADC数据寄存器2 */
    __IO uint32_t DAT3;   /* ADC数据寄存器3 */
    __IO uint32_t DAT4;   /* ADC数据寄存器4 */
    __IO uint32_t DAT5;   /* ADC数据寄存器5 */
    __IO uint32_t DAT6;   /* ADC数据寄存器6 */
    __IO uint32_t DAT7;   /* ADC数据寄存器7 */
    __IO uint32_t DAT8;   /* ADC数据寄存器8 */
    __IO uint32_t DAT9;   /* ADC数据寄存器9 */
    __IO uint32_t DAT10;  /* ADC数据寄存器10 */
    __IO uint32_t DAT11;  /* ADC数据寄存器11 */
    __IO uint32_t DAT12;  /* ADC数据寄存器12 */
    __IO uint32_t DAT13;  /* ADC数据寄存器13 */
    __IO uint32_t DAT14;  /* ADC数据寄存器14 */
    __IO uint32_t DAT15;  /* ADC数据寄存器15 */
    __IO uint32_t DAT16;  /* ADC数据寄存器16 */
    __IO uint32_t DAT17;  /* ADC数据寄存器17 */
    __IO uint32_t DAT18;  /* ADC数据寄存器18 */
    __IO uint32_t DAT19;  /* ADC数据寄存器19 */

    __IO uint32_t CHN0;   /* ADC采样信号来源寄存器0 */
    __IO uint32_t CHN1;   /* ADC采样信号来源寄存器1 */
    __IO uint32_t CHN2;   /* ADC采样信号来源寄存器2 */
    __IO uint32_t CHN3;   /* ADC采样信号来源寄存器3 */
    __IO uint32_t CHN4;   /* ADC采样信号来源寄存器4 */
    __IO uint32_t CHN5;   /* ADC采样信号来源寄存器5 */
    __IO uint32_t CHN6;   /* ADC采样信号来源寄存器6 */
    __IO uint32_t CHN7;   /* ADC采样信号来源寄存器7 */
    __IO uint32_t CHN8;   /* ADC采样信号来源寄存器8 */
    __IO uint32_t CHN9;   /* ADC采样信号来源寄存器9 */

    __IO uint32_t CHNT0;   /* ADC分段通道数寄存器0 设置每段采样数量 */
    __IO uint32_t CHNT1;   /* ADC分段通道数寄存器1 设置每段采样数量 */
    __IO uint32_t SD0;     /* 保留 */
    __IO uint32_t SD1;     /* 保留 */
    __IO uint32_t GAIN_0;  /* ADC增益选择寄存器0 */
    __IO uint32_t GAIN_1;  /* ADC增益选择寄存器1 */
		/* 采样保持电路A对应通道范围是第0、2、4等偶数序号通道 */
    __IO uint32_t DC_A0;   /* ADC直流偏置寄存器0 采样保持电路A对应2/3倍增益通道偏置 */
    __IO uint32_t DC_A1;   /* ADC直流偏置寄存器1 采样保持电路A对应 1 倍增益通道偏置 */
    __IO uint32_t AMC_A0;  /* ADC增益校正寄存器0 采样保持电路A对应2/3倍增益通道增益校正 */
    __IO uint32_t AMC_A1;  /* ADC增益校正寄存器0 采样保持电路A对应 1 倍增益通道增益校正 */
		/* 采样保持电路B对应通道范围是第1、3、5等奇数序号通道 */
    __IO uint32_t DC_B0;   /* ADC直流偏置寄存器0 采样保持电路B对应2/3倍增益通道偏置 */
    __IO uint32_t DC_B1;   /* ADC直流偏置寄存器1 采样保持电路B对应1倍增益通道偏置 */
    __IO uint32_t AMC_B0;  /* ADC增益校正寄存器0 采样保持电路B对应2/3倍增益通道增益校正 */
    __IO uint32_t AMC_B1;  /* ADC增益校正寄存器0 采样保持电路B对应 1 倍增益通道增益校正 */

    __IO uint32_t IE;        /* ADC中断寄存器 */  
    __IO uint32_t IF;        /* ADC中断标志位寄存器 */ 
    __IO uint32_t CFG;       /* ADC模式配置寄存器 */
    __IO uint32_t TRG;       /* ADC触发控制寄存器 */
    __IO uint32_t SWT;       /* ADC软件触发寄存器 */
    __IO uint32_t DAT0_TH;   /* ADC通道0阈值寄存器 */
} ADC_TypeDef;

#ifndef  ADC0
#define  ADC0     ((ADC_TypeDef *) ADC0_BASE)
#endif

typedef struct
{
    uint16_t IE;                           /* ADC中断使能 */
    uint16_t Align;                        /* 采样数据对齐方式 */
    uint16_t Con_Sample;                   /* 连续采样模式 */
    uint16_t Trigger_Cnt;                  /* 单段模式下触发下一次采样所需的触发事件数 */
    uint16_t FirSeg_Ch;                    /* 第一段采样的总通道数 */
    uint16_t SecSeg_Ch;                    /* 第二段采样的总通道数 */
    uint16_t ThrSeg_Ch;                    /* 第三段采样的总通道数 */
    uint16_t FouSeg_Ch;                    /* 第四段采样的总通道数 */
    uint16_t Trigger_Mode;                 /* ADC 触发模式 单段、双段或四段 */
    uint16_t MCPWM_Trigger_En;             /* MCPWM触发采样使能 */
    uint16_t UTIMER_Trigger_En;            /* UTIMER触发采样使能 */
	
    uint16_t DAT0_TH_Type;                 /* 设置DAT0_TH作为：1上阈值，0下阈值 */
	  uint16_t DAT0_TH;                      /* 通道0阈值设置 */
		
	  uint16_t Gain0;                        /* ADC_DAT0-ADC_DAT9采样保持电路增益 */
	  uint16_t Gain1;                        /* ADC_DAT10-ADC_DAT19采样保持电路增益 */
} ADC_InitTypeDef;


#define ADC_1_TIMES_SAMPLE                ((u8)1)  /* ADC采样通道数1次采样 */
#define ADC_2_TIMES_SAMPLE                ((u8)2)  /* ADC采样通道数2次采样 */
#define ADC_3_TIMES_SAMPLE                ((u8)3)  /* ADC采样通道数3次采样 */
#define ADC_4_TIMES_SAMPLE                ((u8)4)  /* ADC采样通道数4次采样 */
#define ADC_5_TIMES_SAMPLE                ((u8)5)  /* ADC采样通道数5次采样 */
#define ADC_6_TIMES_SAMPLE                ((u8)6)  /* ADC采样通道数6次采样 */
#define ADC_7_TIMES_SAMPLE                ((u8)7)  /* ADC采样通道数7次采样 */
#define ADC_8_TIMES_SAMPLE                ((u8)8)  /* ADC采样通道数8次采样 */
#define ADC_9_TIMES_SAMPLE                ((u8)9)  /* ADC采样通道数9次采样 */
#define ADC_10_TIMES_SAMPLE               ((u8)10) /* ADC采样通道数10次采样 */
#define ADC_11_TIMES_SAMPLE               ((u8)11) /* ADC采样通道数11次采样 */
#define ADC_12_TIMES_SAMPLE               ((u8)12) /* ADC采样通道数12次采样 */
#define ADC_13_TIMES_SAMPLE               ((u8)13) /* ADC采样通道数13次采样 */
#define ADC_14_TIMES_SAMPLE               ((u8)14) /* ADC采样通道数14次采样 */
#define ADC_15_TIMES_SAMPLE               ((u8)15) /* ADC采样通道数15次采样 */
#define ADC_16_TIMES_SAMPLE               ((u8)16) /* ADC采样通道数16次采样 */
#define ADC_17_TIMES_SAMPLE               ((u8)17) /* ADC采样通道数17次采样 */
#define ADC_18_TIMES_SAMPLE               ((u8)18) /* ADC采样通道数18次采样 */
#define ADC_19_TIMES_SAMPLE               ((u8)19) /* ADC采样通道数19次采样 */
#define ADC_20_TIMES_SAMPLE               ((u8)20) /* ADC采样通道数20次采样 */

#define ADC_DAT0_OV_IRQ_EN                ((uint16_t)0x40) /* ADC0_DAT0超阈值中断 */
#define ADC_H_CONFLICT_IRQ_EN             ((uint16_t)0x20) /* 硬件触发冲突 */
#define ADC_S_CONFLICT_IRQ_EN             ((uint16_t)0x10) /* 软件触发冲突 */
#define ADC_EOS3_IRQ_EN                   ((uint16_t)0x08) /* 第四段扫描结束中断 */
#define ADC_EOS2_IRQ_EN                   ((uint16_t)0x04) /* 第三段扫描结束中断 */
#define ADC_EOS1_IRQ_EN                   ((uint16_t)0x02) /* 第二段扫描结束中断 */
#define ADC_EOS0_IRQ_EN                   ((uint16_t)0x01) /* 第一段扫描结束中断 */

#define ADC_DAT0_OV_IRQ_IF                ((uint16_t)0x40) /* ADC_DAT0超阈值中断标志位 */
#define ADC_H_CONFLICT_IRQ_IF             ((uint16_t)0x20) /* 硬件触发冲突标志 */
#define ADC_S_CONFLICT_IRQ_IF             ((uint16_t)0x10) /* 软件触发冲突标志 */
#define ADC_EOS3_IRQ_IF                   ((uint16_t)0x08) /* 第四段扫描结束中断标志 */
#define ADC_EOS2_IRQ_IF                   ((uint16_t)0x04) /* 第三段扫描结束中断标志 */
#define ADC_EOS1_IRQ_IF                   ((uint16_t)0x02) /* 第二段扫描结束中断标志 */
#define ADC_EOS0_IRQ_IF                   ((uint16_t)0x01) /* 第一段扫描结束中断标志 */
#define ADC_ALL_IRQ_IF                    ((uint16_t)0x7f)   /* ADC全部中断标志位——可用于初始化 */

#define ADC_LEFT_ALIGN                    ((uint16_t)0x0000) /* ADC数据输出左对齐 */
#define ADC_RIGHT_ALIGN                   ((uint16_t)0x0001) /* ADC数据输出右对齐 */

#define ADC_DAT0_HTH                      ((uint16_t)0x0001) /* ADC_DAT0_TH作为上阈值 */
#define ADC_DAT0_LTH                      ((uint16_t)0x0000) /* ADC_DAT0_TH作为下阈值 */

#define ADC_MCPWM_T0_TRG                  ((uint16_t)0x01)   /* ADC采用MCPWM T0事件触发 */
#define ADC_MCPWM_T1_TRG                  ((uint16_t)0x02)   /* ADC采用MCPWM T1事件触发 */
#define ADC_MCPWM_T2_TRG                  ((uint16_t)0x04)   /* ADC采用MCPWM T2事件触发 */
#define ADC_MCPWM_T3_TRG                  ((uint16_t)0x08)   /* ADC采用MCPWM T3事件触发 */
#define ADC_UTIMER_T0_TRG                 ((uint16_t)0x01)   /* ADC采用UTIMER T0事件触发 */
#define ADC_UTIMER_T1_TRG                 ((uint16_t)0x02)   /* ADC采用UTIMER T1事件触发 */
#define ADC_UTIMER_T2_TRG                 ((uint16_t)0x04)   /* ADC采用UTIMER T2事件触发 */
#define ADC_UTIMER_T3_TRG                 ((uint16_t)0x08)   /* ADC采用UTIMER T3事件触发 */

#define ADC_1SEG_TRG                      ((uint16_t)0x00)   /* ADC采用单段模式 */
#define ADC_2SEG_TRG                      ((uint16_t)0x01)   /* ADC采用2段模式 */
#define ADC_4SEG_TRG                      ((uint16_t)0x03)   /* ADC采用4段模式 */

#define ADC_CHANNEL_0                     ((uint16_t)0x00)   /* ADC通道0-OPA0_OUT */
#define ADC_CHANNEL_1                     ((uint16_t)0x01)   /* ADC通道1-OPA1_OUT */
#define ADC_CHANNEL_2                     ((uint16_t)0x02)   /* ADC通道2-OPA2_OUT */
#define ADC_CHANNEL_3                     ((uint16_t)0x03)   /* ADC通道3-OPA3_OUT */
#define ADC_CHANNEL_4                     ((uint16_t)0x04)   /* ADC通道4 */
#define ADC_CHANNEL_5                     ((uint16_t)0x05)   /* ADC通道5 */
#define ADC_CHANNEL_6                     ((uint16_t)0x06)   /* ADC通道6 */
#define ADC_CHANNEL_7                     ((uint16_t)0x07)   /* ADC通道7 */
#define ADC_CHANNEL_8                     ((uint16_t)0x08)   /* ADC通道8 */
#define ADC_CHANNEL_9                     ((uint16_t)0x09)   /* ADC通道9 */
#define ADC_CHANNEL_10                    ((uint16_t)0x0a)   /* ADC通道10 */
#define ADC_CHANNEL_11                    ((uint16_t)0x0b)   /* ADC通道11 */
#define ADC_CHANNEL_12                    ((uint16_t)0x0c)   /* ADC通道12 */
#define ADC_CHANNEL_13                    ((uint16_t)0x0d)   /* ADC通道13 */
#define ADC_CHANNEL_14                    ((uint16_t)0x0e)   /* ADC通道14 */
#define ADC_CHANNEL_15                    ((uint16_t)0x0f)   /* ADC通道15 */
#define ADC_CHANNEL_16                    ((uint16_t)0x10)   /* ADC通道16 */
#define ADC_CHANNEL_17                    ((uint16_t)0x11)   /* ADC通道17 */
#define ADC_CHANNEL_18                    ((uint16_t)0x12)   /* ADC通道18-Temp */
#define ADC_CHANNEL_19                    ((uint16_t)0x13)   /* ADC通道19-VSS */


#define ADC_HARDWARE_T0_TRG               ((uint16_t)0x01)   /* ADC采用硬件T0事件触发 */
#define ADC_HARDWARE_T1_TRG               ((uint16_t)0x02)   /* ADC采用硬件T1事件触发 */
#define ADC_HARDWARE_T2_TRG               ((uint16_t)0x04)   /* ADC采用硬件T2事件触发 */
#define ADC_HARDWARE_T3_TRG               ((uint16_t)0x08)   /* ADC采用硬件T3事件触发 */

/* ADC采样信号来源寄存器序号 */
typedef enum {
	CHN0 = 0, CHN1, CHN2, CHN3, CHN4, CHN5, CHN6, CHN7, CHN8, CHN9,\
	CHN10, CHN11, CHN12, CHN13, CHN14, CHN15, CHN16, CHN17, CHN18, CHN19,
} CHNx;

/* ADC数据寄存器序号 */
typedef enum {DAT0 = 0, DAT1, DAT2, DAT3, DAT4, DAT5, DAT6, DAT7, DAT8, DAT9,\
DAT10, DAT11, DAT12, DAT13, DAT14, DAT15, DAT16, DAT17, DAT18, DAT19,
} DATx;

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag);
void ADC_ChannelConfig(ADC_TypeDef* ADCx,CHNx CHNum, uint16_t nChannel);
u16 ADC_GetIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag);
s16 ADC_GetConversionValue(DATx DATNum);/* 取对应序号ADC数据寄存器的值 */
void ADC_SoftTrgEN(ADC_TypeDef* ADCx, FuncState state);
#endif /*_CONNIE_ADC_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
