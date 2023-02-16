/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_dsp.c
 * 文件标识：
 * 内容摘要： DSP驱动程序
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
#include "lks32mc08x_dsp.h"
/*******************************************************************************
 函数名称：    void DSP_Cmd(FuncState state)
 功能描述：    DSP使能和关闭
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/25      V1.0           cfwu          创建
  修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18      V2.0           Zhu Jie          时钟使能
 *******************************************************************************/
void DSP_Cmd(FuncState state)
{
   SYS_ModuleClockCmd(SYS_Module_DSP, state); /* DSP时钟使能 */
}

/*******************************************************************************
 函数名称：    void DSP_CalcDivision(stru_DiviComponents *stru_Divi)
 功能描述：    DSP除法运算
 操作的表：    无
 输入参数：     .nDID,被除数   .nDIS除数
 输出参数：    无
 返 回 值：    无
 其它说明：    除法10个总线周期完成
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/25      V1.0           cfwu              创建

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/18      V2.0          Zhu Jie           使用结构体变量
 *******************************************************************************/
static volatile u8 dsp_flg; // 标志位，用于判断程序是否被打断
void DSP_CalcDivision(stru_DiviComponents *stru_Divi)
{
	  u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_DID = stru_Divi->Dividend; /* 被除数寄存器赋值 */
        DSP_DIS = stru_Divi->Divisor;  /* 除数寄存器赋值   */
	
	      stru_Divi->Quotient = DSP_QUO; /* 取出商寄存器值 */
	      stru_Divi->Remainder = DSP_REM;/* 取出余数寄存器值 */
    } while (dsp_flg_old != dsp_flg);  /* 防止DSP计算被打断 */
}

/*******************************************************************************
 函数名称：    void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise)
 功能描述：    DSP三角运算
 操作的表：    无
 输入参数：    角度或坐标X和Y，计算模式选择 0 Arctan;1 sin/cos
 输出参数：    无
 返 回 值：    无
 其它说明：    开方8个总线周期完成
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/19      V1.0          Zhu Jie           使用结构体变量
 *******************************************************************************/
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise)
{
	  u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        if(choise)/* Sin Cos 值计算*/
				{
				 DSP_SC |= BIT2;/* 计算模式选择 Sin和Cos */
				 DSP_CORDIC_THETA = Cordic->Theta;   /* 角度寄存器赋值 */
				 Cordic->SinValue = DSP_CORDIC_SIN;  /* 取出sin寄存器值 */
				 Cordic->CosValue = DSP_CORDIC_COS;  /* 取出cos寄存器值 */
				}
				else /* Arctan 值计算*/
				{
				 DSP_SC &= (~BIT2);/* 计算模式选择 Arctan */
				 DSP_CORDIC_X = Cordic->ArctanX; /* 坐标X寄存器赋值 */
				 DSP_CORDIC_Y = Cordic->ArctanY; /* 坐标Y寄存器赋值 */
				 Cordic->ArctanValue = DSP_CORDIC_ARCTAN;/* 取出arctan寄存器值 */
				}
    } while (dsp_flg_old != dsp_flg);  /* 防止DSP计算被打断 */
	
}

/*******************************************************************************
 函数名称：    s16 DSP_CalcSprt(s16 X,s16 Y)
 功能描述：    DSP两个数平方后开方运算
 操作的表：    无
 输入参数：    X和Y
 输出参数：    无
 返 回 值：    sprt(X^2+Y^2)
 其它说明：    开方8个总线周期完成
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/19      V1.0          Zhu Jie           使用结构体变量
 *******************************************************************************/
s16 DSP_CalcSprt(s16 X,s16 Y)
{
	 u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        DSP_CORDIC_X = X;
		    DSP_CORDIC_Y = Y;
    } while (dsp_flg_old != dsp_flg);  /* 防止DSP计算被打断 */
		return DSP_CORDIC_MOD;
}

/*******************************************************************************
 函数名称：    uint32_t DSP_GetSqrt(void)
 功能描述：    DSP取得开方根
 操作的表：    无
 输入参数：    被开方数
 输出参数：    无
 返 回 值：    开方根
 其它说明：    
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/5/25      V1.0           cfwu          创建

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/19      V2.0          Zhu Jie           增加函数变量
 *******************************************************************************/
uint32_t DSP_GetSqrt(u32 Data)
{
	  u8 dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        DSP_RAD = Data;  /*被开方寄存器赋值*/
    } while (dsp_flg_old != dsp_flg);  /* 防止DSP计算被打断 */
	 
   return DSP_SQRT;
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
