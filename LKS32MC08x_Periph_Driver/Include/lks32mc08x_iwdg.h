/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc03x_IWDG.h
 * 文件标识：
 * 内容摘要： 看门狗驱动头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： YangZJ
 * 完成日期： 2021年11月09日
 *
 *******************************************************************************/
#ifndef __LKS32MC08x_IWDG_H
#define __LKS32MC08x_IWDG_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

typedef struct
{
    u32 WDG_EN;             // 独立看门狗使能
    u32 RTH;                // 看门狗超时复位时间
}IWDG_InitTypeDef;

#define WDT_RTH_2S   3
#define WDT_RTH_4S   2
#define WDT_RTH_8S   1
#define WDT_RTH_64S  0

void IWDG_Init(IWDG_InitTypeDef *);                 // 看门狗初始化
void IWDG_StrutInit(IWDG_InitTypeDef *);            // 看门狗配置结构体初始化
void IWDG_DISABLE(void);                            // 关闭看门狗
void IWDG_ENABLE(void);                             // 使能看门狗
void IWDG_Feed(void);                               // 看门狗喂狗
#endif /*__lks32mc03x_IWDG_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
