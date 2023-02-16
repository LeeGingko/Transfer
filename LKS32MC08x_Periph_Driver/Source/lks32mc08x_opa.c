/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_opa.c
 * 文件标识：
 * 内容摘要： OPA运放驱动程序
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
#include "lks32mc08x_opa.h"

/*******************************************************************************
 函数名称：    void OPA_Init(OPA_InitTypeDef* OPA_InitStruct)
 功能描述：    OPA初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/02      V1.0           cfwu          创建
 *******************************************************************************/
void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct)
{
	uint32_t tmp1, tmp2;
	SYS_WR_PROTECT = 0x7a83;   /* 解除系统寄存器写保护 */
	tmp1 = SYS_AFE_REG0;
	tmp2 = SYS_AFE_REG5;

	if (opa == OPA0)
	{
		tmp1 &= ~(BIT0 | BIT1);
		tmp1 |= OPA_InitStruct->OPA_Gain;

		tmp2 &= ~BIT2;
		tmp2 |= OPA_InitStruct->OPA_CLEna << 2;
	}
	else if (opa == OPA1)
	{
		tmp1 &= ~(BIT2 | BIT3);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 2);

		tmp2 &= ~BIT3;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 3);
	}
	else if (opa == OPA2)
	{
		tmp1 &= ~(BIT4 | BIT5);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 4);

		tmp2 &= ~BIT4;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 4);
	}
	else if (opa == OPA3)
	{
		tmp1 &= ~(BIT6 | BIT7);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 6);

		tmp2 &= ~BIT5;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 5);
	}

	SYS_AFE_REG0 = tmp1;
	SYS_AFE_REG5 = tmp2;
	SYS_WR_PROTECT = 0x0;   /* 开启系统寄存器写保护 */
}

/*******************************************************************************
 函数名称：    void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct)
 功能描述：    OPA结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/06/02      V1.0           cfwu          创建
 *******************************************************************************/
void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct)
{
   OPA_InitStruct->OPA_Gain = PGA_GAIN_4P25;
   OPA_InitStruct->OPA_CLEna = DISABLE;
}

/*******************************************************************************
 函数名称：    void OPA_OUT(enumOPA opa, FuncState state)
 功能描述：    OPA外部输出，输出到外部P2.7引脚
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/20      V1.0          HuangMG          创建
 *******************************************************************************/
void OPA_OUT(enumOPA opa, FuncState state)
{
	SYS_WR_PROTECT = 0x7a83;   /* 解除系统寄存器写保护 */
  if(state != DISABLE)
	{
		if (opa == OPA0)
		{		
		  SYS_AFE_REG2 = 0x01;
		}
		else if (opa == OPA1)
		{
			SYS_AFE_REG2 = 0x02;
		}
		else if (opa == OPA2)
		{
			SYS_AFE_REG2 = 0x03;
		}
		else if (opa == OPA3)
		{
			SYS_AFE_REG2 = 0x04;
		}
  }else{
		  SYS_AFE_REG2 = 0x00;
	}
   SYS_WR_PROTECT = 0x0;   /* 开启系统寄存器写保护 */
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
