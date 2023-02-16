/*******************************************************************************
 * ��Ȩ���� (C)2018, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_tmp.h
 * �ļ���ʶ��
 * ����ժҪ�� ADC������������ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� William Zhang
 * ������ڣ� 2018��25��
 *
 *
 * �޸ļ�¼��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/

#ifndef _LKS32MC08x_TMP_H_
#define _LKS32MC08x_TMP_H_


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
typedef struct
{
   u16 nCofA;      /* �¶�ϵ��A */
   u16 nOffsetB;   /* �¶�ϵ��ƫ�� */
} Stru_TempertureCof_Def;


void TempSensor_Init(void);
s16 GetCurrentTemperature(s16 ADC_value);

#endif /*_CONNIE_TMP_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
