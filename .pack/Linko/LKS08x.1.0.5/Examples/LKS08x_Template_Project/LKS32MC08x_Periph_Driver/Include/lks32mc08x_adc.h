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
 
#ifndef _CONNIE_ADC_H_
#define _CONNIE_ADC_H_


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"
    
typedef struct
{
     __IO uint32_t DAT0;
     __IO uint32_t DAT1;
     __IO uint32_t DAT2;
     __IO uint32_t DAT3;
     __IO uint32_t DAT4;
     __IO uint32_t DAT5;
     __IO uint32_t DAT6;
     __IO uint32_t DAT7;
     __IO uint32_t DAT8;
     __IO uint32_t DAT9;
     __IO uint32_t DAT10;
     __IO uint32_t DAT11;
     __IO uint32_t DAT12;
     __IO uint32_t DAT13;
     __IO uint32_t DAT14;
     __IO uint32_t DAT15;
     __IO uint32_t DAT16;
     __IO uint32_t DAT17;
     __IO uint32_t DAT18;
     __IO uint32_t DAT19;    

     __IO uint32_t CHN0;
     __IO uint32_t CHN1;
     __IO uint32_t CHN2;
     __IO uint32_t CHN3;
     __IO uint32_t CHN4;
     __IO uint32_t CHN5;
     __IO uint32_t CHN6;
     __IO uint32_t CHN7;
     __IO uint32_t CHN8;
     __IO uint32_t CHN9;
     
     __IO uint32_t CHNT0;
     __IO uint32_t CHNT1;
     __IO uint32_t SD0;
     __IO uint32_t SD1;
		 __IO uint32_t GAIN_0;
		 __IO uint32_t GAIN_1;
		 __IO uint32_t DC_A0;
     __IO uint32_t DC_A1;
		 __IO uint32_t AMC_A0;
		 __IO uint32_t AMC_A1;
		 __IO uint32_t DC_B0;
     __IO uint32_t DC_B1;
		 __IO uint32_t AMC_B0;
		 __IO uint32_t AMC_B1;
		 
     __IO uint32_t IE;
		 __IO uint32_t IF;
     __IO uint32_t CFG;
     __IO uint32_t TRG;     
     __IO uint32_t SWT;
     __IO uint32_t DAT0_TH;
} ADC_TypeDef;

typedef struct
{
   uint16_t IE;                           /* Configures ADC_IRQ */
   uint16_t Align;                        /* Configures ADC Result Align */
   uint16_t Con_Sample;                   /* 连续采样模式 */
   uint16_t Trigger_Cnt;                  /* 单段模式下触发下一次采样所需的触发事件数 */
   uint16_t FirSeg_Ch;                    /* 第一段采样的总通道数 */
   uint16_t SecSeg_Ch;                    /* 第二段采样的总通道数 */
   uint16_t ThrSeg_Ch;                    /* 第三段采样的总通道数 */
   uint16_t FouSeg_Ch;                    /* 第四段采样的总通道数 */
   uint16_t Trigger_Mode;                 /* ADC trigger mode */
   uint16_t MCPWM_Trigger_En;             /* mcpwm trigger enable */
   uint16_t UTIMER_Trigger_En;            /* utimer trigger enable */
   uint16_t DAT0_TH_Type;               
   uint16_t AMC0;
   uint16_t AMC1;
   uint16_t SD0;
   uint16_t SD1;
}ADC_InitTypeDef;


#define ADC_1_TIMES_SAMPLE                ((u8)1) /* ADC采样通道数1次采样 */
#define ADC_2_TIMES_SAMPLE                ((u8)2) /* ADC采样通道数2次采样 */
#define ADC_3_TIMES_SAMPLE                ((u8)3) /* ADC采样通道数3次采样 */
#define ADC_4_TIMES_SAMPLE                ((u8)4)
#define ADC_5_TIMES_SAMPLE                ((u8)5)
#define ADC_6_TIMES_SAMPLE                ((u8)6)
#define ADC_7_TIMES_SAMPLE                ((u8)7)
#define ADC_8_TIMES_SAMPLE                ((u8)8)
#define ADC_9_TIMES_SAMPLE                ((u8)9)
#define ADC_10_TIMES_SAMPLE               ((u8)10) 
#define ADC_11_TIMES_SAMPLE               ((u8)11)
#define ADC_12_TIMES_SAMPLE               ((u8)12)
#define ADC_13_TIMES_SAMPLE               ((u8)13)
#define ADC_14_TIMES_SAMPLE               ((u8)14)
#define ADC_15_TIMES_SAMPLE               ((u8)15)
#define ADC_16_TIMES_SAMPLE               ((u8)16)
#define ADC_17_TIMES_SAMPLE               ((u8)17)
#define ADC_18_TIMES_SAMPLE               ((u8)18)
#define ADC_19_TIMES_SAMPLE               ((u8)19)
#define ADC_20_TIMES_SAMPLE               ((u8)20) /* ADC采样通道数20次采样 */

#define ADC_EOS0_IRQ_EN                   ((uint16_t)0x01) /* 第一段扫描结束中断 */
#define ADC_EOS1_IRQ_EN                   ((uint16_t)0x02) /* 第二段扫描结束中断 */
#define ADC_EOS2_IRQ_EN                   ((uint16_t)0x04) /* 第三段扫描结束中断 */
#define ADC_EOS3_IRQ_EN                   ((uint16_t)0x08) /* 第四段扫描结束中断 */
#define ADC_CONFLICT_IRQ_EN               ((uint16_t)0x10) /* 软件触发冲突 */
#define ADC_DAT0_OV_IRQ_EN                ((uint16_t)0x40) /* ADC_DAT0超阈值中断 */

#define ADC_CONFLICT_IRQ_IF               ((uint16_t)0x10) /* 软件触发冲突标志 */
#define ADC_EOS3_IRQ_IF                   ((uint16_t)0x08) /* 通道全部扫描结束标志 */
#define ADC_EOS2_IRQ_IF                   ((uint16_t)0x04) /* 通道全部扫描结束标志 */
#define ADC_EOS1_IRQ_IF                   ((uint16_t)0x02) /* 通道全部扫描结束标志 */
#define ADC_EOS0_IRQ_IF                   ((uint16_t)0x01) /* 第一段扫描结束中断标志 */
#define ADC_DAT0_OV_IRQ_IF                ((uint16_t)0x40) /* ADC_DAT0超阈值中断 */

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

#define ADC_CHANNEL_0                     ((uint16_t)0x00)   /* ADC通道0 */
#define ADC_CHANNEL_1                     ((uint16_t)0x01)   /* ADC通道1 */
#define ADC_CHANNEL_2                     ((uint16_t)0x02)   /* ADC通道2 */
#define ADC_CHANNEL_3                     ((uint16_t)0x03)   /* ADC通道3 */
#define ADC_CHANNEL_4                     ((uint16_t)0x04)   /* ADC通道4 */
#define ADC_CHANNEL_5                     ((uint16_t)0x05)   /* ADC通道5 */
#define ADC_CHANNEL_6                     ((uint16_t)0x06)   /* ADC通道6 */
#define ADC_CHANNEL_7                     ((uint16_t)0x07)   /* ADC通道7 */
#define ADC_CHANNEL_8                     ((uint16_t)0x08)   /* ADC通道8 */
#define ADC_CHANNEL_9                     ((uint16_t)0x09)   /* ADC通道9 */
#define ADC_CHANNEL_10                    ((uint16_t)0x0a)   /* ADC通道10 */
#define ADC_CHANNEL_11                    ((uint16_t)0x0b)   /* ADC通道11 */
#define ADC_CHANNEL_12                    ((uint16_t)0x0c)   /* ADC通道11 */
#define ADC_CHANNEL_13                    ((uint16_t)0x0d)   /* ADC通道11 */
#define ADC_CHANNEL_14                    ((uint16_t)0x0e)   /* ADC通道11 */
#define ADC_CHANNEL_15                    ((uint16_t)0x0f)   /* ADC通道11 */
#define ADC_CHANNEL_16                    ((uint16_t)0x10)   /* ADC通道11 */
#define ADC_CHANNEL_17                    ((uint16_t)0x11)   /* ADC通道11 */
#define ADC_CHANNEL_18                    ((uint16_t)0x12)   /* ADC通道11 */
#define ADC_CHANNEL_19                    ((uint16_t)0x13)   /* ADC通道11 */


#define ADC_HARDWARE_T0_TRG               ((uint16_t)0x01)   /* ADC采用硬件T0事件触发 */
#define ADC_HARDWARE_T1_TRG               ((uint16_t)0x02)   /* ADC采用硬件T1事件触发 */
#define ADC_HARDWARE_T2_TRG               ((uint16_t)0x04)   /* ADC采用硬件T2事件触发 */
#define ADC_HARDWARE_T3_TRG               ((uint16_t)0x08)   /* ADC采用硬件T3事件触发 */

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);

#endif /*_CONNIE_ADC_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
