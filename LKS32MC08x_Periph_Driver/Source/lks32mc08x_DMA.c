/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： LKS32MC081_DMA.c
 * 文件标识：
 * 内容摘要： DMA外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2020年08月1日
 *
 * 修改记录1：
 * 修改日期：2020年08月1日
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
#include "lks32mc08x_DMA.h"
#include "string.h"

/*******************************************************************************
 函数名称：    void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
 功能描述：    DMA结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020.08.1      V1.0           howlet              创建
 *******************************************************************************/
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
{
    memset(DMAInitStruct, 0, sizeof(DMA_InitTypeDef));
}

/*******************************************************************************
 函数名称：    void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
 功能描述：    DMA初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020.08.1      V1.0           howlet              创建
 *******************************************************************************/
void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
{
    /* 通道配置寄存器 斌值 */
    DMAx->DMA_CCR = 0;
    DMAx->DMA_CTMS = DMAInitStruct->DMA_TIMES | (DMAInitStruct->DMA_ROUND<<16); /* 传输次数寄存器 */
    DMAx->DMA_CPAR = DMAInitStruct->DMA_CPAR;   /* 外设地址寄存器 */
    DMAx->DMA_CMAR = DMAInitStruct->DMA_CMAR;   /* 内存地址寄存器 */
    DMAx->DMA_CCR  =  (DMAInitStruct->DMA_IRQ_EN<<1) | (DMAInitStruct->DMA_DIR<<4) |\
                     (DMAInitStruct->DMA_CIRC<<5) | (DMAInitStruct->DMA_PINC<<6) | (DMAInitStruct->DMA_MINC<<7) |\
                     (DMAInitStruct->DMA_PBTW<<8) | (DMAInitStruct->DMA_MBTW<<10) | (DMAInitStruct->DMA_REQ_EN<<12);

    DMA_CTRL  = 0x0001;  /*enable dma, mcu has higher priorit */
}  

/*******************************************************************************
 函数名称：    void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
 功能描述：    使能DMA通道
 操作的表：    无
 输入参数：    DMAx：DMA通道选择
              Channel_EN：ENABLE：使能DMA通道，DISABLE：关闭DMA通道使能
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            创建
 *******************************************************************************/
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
{
   if(Channel_EN)
	 {
	   DMAx->DMA_CCR  |= BIT0;/*使能DMA搬运*/
	 }else{
	   DMAx->DMA_CCR  &= ~BIT0; /*关闭通道使能*/
	 }
}

/*******************************************************************************
 函数名称：    uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
 功能描述：    获取DMA中断标志
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    DMA中断标志
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            创建
 *******************************************************************************/
uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
{
	 u8 reg = 0;	 
   if((DMAx->DMA_CCR) & BIT1)/*传输完成中断使能*/
	 {
		  reg = timer_if & 0x0f;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
	 }
   if((DMAx->DMA_CCR) & BIT2)/*错误中断使能*/
	 {
		  reg = timer_if & 0xf0;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
	 }
	 return 0;
}
/*******************************************************************************
 函数名称：    void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
 功能描述：    清除DMA中断标志
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            创建
 *******************************************************************************/
void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
{
    DMA_IF = tempFlag;
}
