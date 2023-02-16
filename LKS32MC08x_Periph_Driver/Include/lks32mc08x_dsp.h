/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_dsp.h
 * �ļ���ʶ��
 * ����ժҪ�� DSP����ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� 
 * ������ڣ� 
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#ifndef __LKS32MC08x_DSP_H
#define __LKS32MC08x_DSP_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
	  s32 Dividend;     /* ������ */
	  s32 Divisor;      /* ����   */
    s16 Quotient;     /* ��     */
    s16 Remainder;    /* ����   */
} stru_DiviComponents;/* ��������ṹ�� */

typedef struct
{
	  s16 Theta;          /* �Ƕ�-32768~32767��Ӧ-pi~pi    */
    s16 SinValue;       /* ����õ�sinֵ                 */
    s16 CosValue;       /* ����õ�cosֵ                 */
	  s16 ArctanX;        /* Arctan�����ӦXֵ arctan =Y/X */
	  s16 ArctanY;        /* Arctan�����ӦYֵ arctan =Y/X */
	  s16 ArctanValue;    /* ����õ�arctanֵ              */
} stru_CordicComponents;/* ���Ǻ�������ṹ�� */

typedef enum {ARCTAN = 0, SINCOS = 1} Arc_or_SinCFuction;/* 0����arctan;1����sin cos */

void DSP_Cmd(FuncState state);                          /* DSPʹ�ܺ͹ر� */
void DSP_CalcDivision(stru_DiviComponents *stru_Divi);  /* DSP�������� */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise);  /* DSP���Ǻ����������� */
uint32_t DSP_GetSqrt(u32 Data); /*DSPȡ�ÿ����� sprt(Data)*/
s16 DSP_CalcSprt(s16 X,s16 Y);  /*DSP���㿪���� sprt(X^2+Y^2)*/

#endif 



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
