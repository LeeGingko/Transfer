/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc081_flash.c
 * 文件标识：
 * 内容摘要： Flash外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2019年3月5日
 *
 * 修改记录1：
 * 修改日期：2019年3月5日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/

#include "lks32mc08x_cmp.h"
#include "lks32mc08x.h"
#include "string.h"
/*******************************************************************************
 函数名称：    void Comparator_init(CMP_InitTypeDef* CMP_InitStruct)
 功能描述：    CMP初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void Comparator_init(CMP_InitTypeDef* CMP_InitStruct)
{
   SYS_AFE_REG3 |= (CMP_InitStruct->CMP0_SELN << 2) | (CMP_InitStruct->CMP1_SELN << 4) | (CMP_InitStruct->CMP0_SELP << 8) |
	                 (CMP_InitStruct->CMP1_SELP << 12) | (CMP_InitStruct->CMP_HYS << 7);
	
	 /* CMP_HYS 比较器滞回功能，默认20mV, 写0关闭 */
	 
	 SYS_AFE_REG1 |= CMP_InitStruct->CMP_FT; /* 快速比较，Enable后 30nS */
	
	 SYS_AFE_REG5 |= (CMP_InitStruct->CMP0_EN << 6) | (CMP_InitStruct->CMP1_EN << 7);
	
	
	
	 CMP_TCLK = (CMP_InitStruct->CMP_CLK_EN << 3) | (CMP_InitStruct->CMP_CNK_PRSC) | (CMP_InitStruct->CMP_FltCnt << 4);
	 CMP_IE = CMP_InitStruct->CMP0_IE | (CMP_InitStruct->CMP1_IE<<1);
	 CMP_CFG = CMP_InitStruct->CMP0_Polarity | (CMP_InitStruct->CMP0_InEnable << 1)| 
	           (CMP_InitStruct->CMP1_Polarity << 4) | (CMP_InitStruct->CMP1_InEnable << 5);
}

/*******************************************************************************
 函数名称：    void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
 功能描述：    CMP初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2018/11/26      V1.0           Howlet Li          创建
 *******************************************************************************/
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{
	 
	 memset(CMP_InitStruct,0,sizeof(CMP_InitTypeDef));
}

