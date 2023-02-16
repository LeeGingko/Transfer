/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_opa.h
 * 文件标识：
 * 内容摘要： OPA驱动头文件
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
 
#ifndef __LKS32MC08x_OPA_H
#define __LKS32MC08x_OPA_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

typedef enum
{
   OPA0 = 0,
   OPA1 = 1,
   OPA2 = 2,
   OPA3 = 3
}enumOPA;

typedef struct
{
   uint32_t OPA_Gain;       /*运放闭环增益*/
   uint32_t OPA_CLEna;      /*运放闭环使能:Enable,Disable*/
}OPA_InitTypeDef;

/* ------------------------------PGA操作相关定义 ------------------------------ */
#define PGA_GAIN_20                    0x00                   /* 反馈电阻200:10.2 */
#define PGA_GAIN_9P5                   0x01                   /* 反馈电阻190:20.2 */
#define PGA_GAIN_6                     0x02                   /* 反馈电阻180:30.4 */
#define PGA_GAIN_4P25                  0x03                   /* 反馈电阻170:40.4 */                                                                                 

void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct);
void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct);
void OPA_OUT(enumOPA opa, FuncState state);

#endif /*__lks32mc08x_OPA_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
