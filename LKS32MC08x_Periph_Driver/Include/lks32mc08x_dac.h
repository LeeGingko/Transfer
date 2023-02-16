/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Chanhom_dac.h
 * 文件标识：
 * 内容摘要： DAC外设驱动程序头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Zhu Jie
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/
#ifndef __LKS32MC08x_DAC_H
#define __LKS32MC08x_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
    uint8_t DAC_GAIN;                 /* DAC输出档位：0，3V；1，1.2V；2，4.85V。  */	
	  uint8_t DACOUT_EN;                /* DAC输出使能：0，不使能；1，使能。 */
}DAC_InitTypeDef;

/* ------------------------------DAC操作相关定义 ------------------------------ */
#define DAC_RANGE_3V0                  0                   /* DAC 3.0V量程 */
#define DAC_RANGE_1V2                  1                   /* DAC 1.2V量程 */
#define DAC_RANGE_4V85                 2                   /* DAC 4.85V量程 */

void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);/* DAC结构体初始化 */
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);      /* DAC初始化 */
void DAC_OutputValue(uint32_t DACValue);             /* DAC输出数值设置--数字量 */
void DAC_OutputVoltage(uint16_t DACVoltage);         /* DAC输出模拟量数值设置--模拟量 */

#endif

