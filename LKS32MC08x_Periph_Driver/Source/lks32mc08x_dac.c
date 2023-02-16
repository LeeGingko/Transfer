/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： LKS32MC081_dac.c
 * 文件标识：
 * 内容摘要： DAC外设驱动程序
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
#include "lks32mc08x_dac.h"
#include "string.h"

/*******************************************************************************
 函数名称：    void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
 功能描述：    DAC结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              创建
 *******************************************************************************/
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
    memset(DAC_InitStruct, 0, sizeof(DAC_InitTypeDef));
}
/*******************************************************************************
 函数名称：    void DAC_init(DAC_InitTypeDef* DAC_InitStruct)
 功能描述：    DAC初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18      V1.0          Zhu Jie           创建
 *******************************************************************************/
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct)
{
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_DAC, ENABLE);  /* DAC 时钟使能 */
	
	  SYS_WR_PROTECT = 0x7a83;  /* 解锁寄存器写保护 */
	
	  SYS_AFE_REG1 &= ~((u32)0x03 << 6);              /* DAC增益寄存器清零 */
    SYS_AFE_REG1 |= DAC_InitStruct->DAC_GAIN << 6;  /* 设置DAC满量程为1.2V */
	  
	  if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_1V2)
		{ /* 加载DAC 1.2V量程校正值 */
			SYS_AFE_DAC_DC = Read_Trim(0x00000344);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000334);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_3V0)
		{ /* 加载DAC 3.0V量程校正值 */
			SYS_AFE_DAC_DC = Read_Trim(0x00000340);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000330);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_4V85)
		{ /* 加载DAC 4.85V量程校正值 */
			SYS_AFE_DAC_DC = Read_Trim(0x00000348);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000338);
		}
		
	  SYS_AFE_REG3 |= DAC_InitStruct->DACOUT_EN << 11;/* DAC输出配置 */
	  
	  SYS_WR_PROTECT = 0xffff;  /* 锁定寄存器写保护 */
}

/*******************************************************************************
 函数名称：    void DAC_OutputValue(uint32_t DACValue)
 功能描述：    DAC输出数字量数值设置
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              创建
 *******************************************************************************/
void DAC_OutputValue(uint32_t DACValue)
{
    SYS_WR_PROTECT = 0x7a83;  /* 解锁寄存器写保护 */
	
	  if(DACValue >=4095)
		{
		    DACValue = 4095;
		} 
	  SYS_AFE_DAC = DACValue ;
	
	  SYS_WR_PROTECT = 0xffff;  /* 锁定寄存器写保护 */
}

/*******************************************************************************
 函数名称：    void DAC_OutputVoltage(uint32_t DACVoltage)
 功能描述：    DAC输出模拟量数值设置
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    输入变量DACVoltage为Q12格式，即输出1V对应DACVoltage = 1<<12 = 4096
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              创建
 *******************************************************************************/
void DAC_OutputVoltage(uint16_t DACVoltage)
{
	  u32 temp = 0;
	  u32 range = 0;
	
	  if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_1V2)
		{ /* 加载DAC 1.2V量程校正值 */
			range = (uint16_t)((1.0/1.2)*BIT12);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_3V0)
		{ /* 加载DAC 3.0V量程校正值 */
			range = (uint16_t)((1.0/3.0)*BIT12);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_4V85)
		{ /* 加载DAC 4.85V量程校正值 */
			range = (uint16_t)((1.0/4.85)*BIT12);
		} 
	
	  temp = (DACVoltage * range + BIT11) >> 12; 
	
		if(temp >=4095)
		{
		    temp = 4095;
		}
	  SYS_AFE_DAC = temp ;
	
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
