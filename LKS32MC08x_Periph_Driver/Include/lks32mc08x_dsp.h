/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_dsp.h
 * 文件标识：
 * 内容摘要： DSP驱动头文件
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
 
#ifndef __LKS32MC08x_DSP_H
#define __LKS32MC08x_DSP_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
	  s32 Dividend;     /* 被除数 */
	  s32 Divisor;      /* 除数   */
    s16 Quotient;     /* 商     */
    s16 Remainder;    /* 余数   */
} stru_DiviComponents;/* 除法运算结构体 */

typedef struct
{
	  s16 Theta;          /* 角度-32768~32767对应-pi~pi    */
    s16 SinValue;       /* 计算得到sin值                 */
    s16 CosValue;       /* 计算得到cos值                 */
	  s16 ArctanX;        /* Arctan计算对应X值 arctan =Y/X */
	  s16 ArctanY;        /* Arctan计算对应Y值 arctan =Y/X */
	  s16 ArctanValue;    /* 计算得到arctan值              */
} stru_CordicComponents;/* 三角函数运算结构体 */

typedef enum {ARCTAN = 0, SINCOS = 1} Arc_or_SinCFuction;/* 0计算arctan;1计算sin cos */

void DSP_Cmd(FuncState state);                          /* DSP使能和关闭 */
void DSP_CalcDivision(stru_DiviComponents *stru_Divi);  /* DSP除法运算 */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise);  /* DSP三角函数运算运算 */
uint32_t DSP_GetSqrt(u32 Data); /*DSP取得开方根 sprt(Data)*/
s16 DSP_CalcSprt(s16 X,s16 Y);  /*DSP计算开方根 sprt(X^2+Y^2)*/

#endif 



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
