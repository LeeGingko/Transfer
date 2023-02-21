/*******************************************************************************
 * 版权所有 (C)2018, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_cmp.h
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
#include "lks32mc08x.h"
#include "basic.h"
 
 typedef struct
{

   uint8_t CMP0_SELN;                    /* 比较器0，负端信号选择 */
   uint8_t CMP1_SELN;                    /* 比较器1，负端信号选择 */
   uint8_t CMP_HYS;                      /* 比较器回头选择 */
   uint8_t CMP0_SELP;                    /* 比较器0，正端信号选择  */
   uint8_t CMP1_SELP;                    /* 比较器1，正端信号选择 */
   uint8_t CMP_FT;                       /* 比较器快速比较使能 */
	 uint8_t CMP0_EN;                      /* 比较器0使能 */
	 uint8_t CMP1_EN;                      /* 比较器1使能 */
	
	 uint8_t CMP_FltCnt;                   /* 比较器滤波选择 */
	 uint8_t CMP_CLK_EN;                   /* 比较器中断配置时钟打开 */
	 uint8_t CMP_CNK_PRSC;                 /* 比较器滤波时钟分频 */
	 
	 uint8_t CMP0_Polarity;                /* 比较器0极性选择 */
	 uint8_t CMP1_Polarity;                /* 比较器1极性选择 */
	 uint8_t CMP0_IE;                      /* 比较器0中断使能 */
	 uint8_t CMP1_IE;                      /* 比较器1中断使能 */	
	 uint8_t CMP0_InEnable;                /* 比较器0输入选择 */
	 uint8_t CMP1_InEnable;                /* 比较器1输入选择 */		

}CMP_InitTypeDef;

#define SELN_CMP_IN             0        /* N端输入 */
#define SELN_REF                1        /* N端内部参考REF输入 */
#define SELN_DAC                2        /* N端内部DAC输入 */
#define SELN_HALL_MID           3        /* N端反电势过零中点输入 */

#define SELP_CMP_IP0            0        /* P端IP0输入 */
#define SELP_CMP_IP1            4        /* P端IP1输入 */
#define SELP_CMP_IP2            5        /* P端IP2输入 */
#define SELP_CMP_IP3            6        /* P端IP3输入 */
#define SELP_CMP_IP4            7        /* P端IP4输入 */

void Comparator_init(CMP_InitTypeDef* CMP_InitStruct);
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct);


