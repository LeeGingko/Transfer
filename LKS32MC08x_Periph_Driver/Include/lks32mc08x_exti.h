/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc05x_exti.h
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
 
#ifndef __LKS32MC08x_EXTI_H
#define __LKS32MC08x_EXTI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "lks32mc08x_gpio.h"
#include "basic.h"

typedef struct  
{
   __IO uint32_t CR0;
   __IO uint32_t CR1;
   __IO uint32_t IF;   
   __IO uint32_t POL;
   __IO uint32_t EN;
}EXTI_TypeDef;

#ifndef EXTI
#define EXTI             ((EXTI_TypeDef *) EXTI_BASE)
#endif
typedef enum
{
   EXTI_Trigger_None    = 0x00, // 不触发
   EXTI_Trigger_Negedge = 0x01, // 下降沿触发
   EXTI_Trigger_Posedge = 0x02, // 上升沿触发
   EXTI_Trigger_Edge    = 0x03  // 上升沿、下降沿均触发
} EXTIMode_TypeDef;

void EXTI_Trigger_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint16_t EXTI_Trigger);
void EXTI_ClearIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource);
u8 EXTI_GetIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource);

#endif /*__lks32mc05x_EXTI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
