/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_exti.h
 * 文件标识：
 * 内容摘要： EXTI外部中断驱动头文件
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
 
#ifndef __lks32mc08x_EXTI_H
#define __lks32mc08x_EXTI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct  
{
   __IO uint32_t CR0;
   __IO uint32_t CR1;
   __IO uint32_t IF;   
}EXTI_TypeDef;

typedef struct
{
   uint32_t Pad0Trig;   /*触发方式*/
   uint32_t Pad1Trig;
   uint32_t Pad2Trig;
   uint32_t Pad3Trig;
   uint32_t Pad4Trig;
   uint32_t Pad5Trig;
   uint32_t Pad6Trig;
   uint32_t Pad7Trig;	
   uint32_t Pad8Trig;	
   uint32_t Pad9Trig;	
   uint32_t Pad10Trig;	
   uint32_t Pad11Trig;	
   uint32_t Pad12Trig;	
   uint32_t Pad13Trig;	
   uint32_t Pad14Trig;	
   uint32_t Pad15Trig;	
}EXTI_InitTypeDef;


#define EXTI_NONE_TRG     ((uint32_t)0x00) /*不触发*/
#define EXTI_FALL_TRG     ((uint32_t)0x01) /*下降沿触发*/
#define EXTI_RISE_TRG     ((uint32_t)0x02) /*上升沿触发*/
#define EXTI_FALLRISE_TRG ((uint32_t)0x03) /*双沿触发*/


void EXTI_Init(EXTI_TypeDef* EXTIx, EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);

uint32_t EXTI_ReadIFFlag(EXTI_TypeDef* EXTIx);
void EXTI_ResetIF(EXTI_TypeDef* EXTIx, uint32_t temp);

#endif /*__lks32mc08x_EXTI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
