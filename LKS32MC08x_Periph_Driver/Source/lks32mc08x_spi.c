/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_spi.c
 * 文件标识：
 * 内容摘要： SPI外设驱动程序
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
#include "lks32mc08x_spi.h"

/*******************************************************************************
 函数名称：    void SPI_Init(SPI_TypeDef* SPIx, ESPI_InitTypeDef* SPI_InitStruct)
 功能描述：    SPI初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
	  SYS_WR_PROTECT = 0x7a83;/* 使能系统寄存器写操作*/
    SYS_SFT_RST |= BIT8;  //复位SPI
    SYS_SFT_RST &= ~BIT8; //释放SPI
	  SYS_WR_PROTECT = 0x0; /* 关闭系统寄存器写操作*/
    SYS_ModuleClockCmd(SYS_Module_SPI, ENABLE);

    SPIx->IE   =   SPI_InitStruct->Trig   << 3  | (SPI_InitStruct->IRQEna        );

    SPIx->DIV  =   SPI_InitStruct->BaudRate     | (SPI_InitStruct->DMA       << 7);

    SPIx->SIZE =   SPI_InitStruct->ByteLength;

    SPIx->CFG  =   SPI_InitStruct->EN           | (SPI_InitStruct->DataOrder << 1)
                   | (SPI_InitStruct->CPOL   << 2) | (SPI_InitStruct->CPHA      << 3)
                   | (SPI_InitStruct->Mode   << 4) | (SPI_InitStruct->SS        << 5)
                   | (SPI_InitStruct->Duplex << 6);

}


/*******************************************************************************
 函数名称：    void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
 功能描述：    SPI结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{

    SPI_InitStruct->Duplex = SPI_Full;
    SPI_InitStruct->SS     = 0;
    SPI_InitStruct->Mode   = SPI_Slave;
    SPI_InitStruct->CPOL   = 0;
    SPI_InitStruct->CPHA   = 0;
    SPI_InitStruct->DataOrder = DISABLE;
    SPI_InitStruct->EN        = 0;

    SPI_InitStruct->IRQEna    = DISABLE;
    SPI_InitStruct->Trig      = 0;

    SPI_InitStruct->DMA       = 0;
    SPI_InitStruct->BaudRate  = 3;

    SPI_InitStruct->ByteLength = 0;
}

/*******************************************************************************
 函数名称：    void SPI_SENDDATA(SPI_TypeDef *SPIx, uint32_t n)
 功能描述：    SPI发送数据
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void SPI_SendData(SPI_TypeDef *SPIx, uint8_t n)
{
    SPIx->TX_DATA = n;
}

/*******************************************************************************
 函数名称：    uint32_t SPI_ReadData(SPI_TypeDef *SPIx)
 功能描述：    SPI读缓冲区数据
 操作的表：    无
 输入参数：    SPI_TypeDef *SPIx
 输出参数：    无
 返 回 值：    缓冲区数据
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
uint8_t SPI_ReadData(SPI_TypeDef *SPIx)
{
    return SPIx->RX_DATA;
}

/*******************************************************************************
 函数名称：    uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
 功能描述：    取得SPI中断标志
 操作的表：    无
 输入参数：    SPI_InitTypeDef *SPIx:要操作的SPI对象
 输出参数：    无
 返 回 值：    SPI中断标志
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie          创建
 *******************************************************************************/
uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
{
    u8 SPI_IRQFlag = 0;
	  if(SPIx->IE & IRQ_Flag)
		{
		    SPI_IRQFlag = 1;
		}
		else
		{
		    SPI_IRQFlag = 0;
		}
    return SPI_IRQFlag;
}

/*******************************************************************************
 函数名称：    void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
 功能描述：    SPI中断标志位清零
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：    写1清零
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/04/20    V1.0            Zhu Jie          创建
 *******************************************************************************/
void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
{
    SPIx->IE |= IRQ_Flag;
}
