/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_i2c.c
 * 文件标识：
 * 内容摘要： I2C外设驱动程序
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
#include "lks32mc08x_i2c.h"
#include "lks32mc08x_sys.h"
#include "hardware_config.h"
/*******************************************************************************
 函数名称：    void I2C_Init(I2C_TypeDef* I2Cx, EI2C_InitTypeDef* I2C_InitStruct)
 功能描述：    I2C初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{

   SYS_ModuleClockCmd(SYS_Module_I2C, ENABLE);		

   I2Cx->ADDR =   I2C_InitStruct->ADRCMP << 7 | (I2C_InitStruct->ADDR        );

   I2Cx->CFG  =   I2C_InitStruct->INTEN     << 7  | (I2C_InitStruct->DONEINT    << 6)
               | (I2C_InitStruct->BUSERRINT << 5) | (I2C_InitStruct->STOPINT    << 4)
               | (I2C_InitStruct->MASTER    << 1) | (I2C_InitStruct->SLAVE      << 0);

   I2Cx->SCR  =   I2C_InitStruct->BUSERRFLAG << 7  | (I2C_InitStruct->LOSTARBFLAG  << 6)
               | (I2C_InitStruct->STOPFLAG   << 5) | (I2C_InitStruct->ACKCTRL      << 4)
               | (I2C_InitStruct->ADDRFLAG   << 3) | (I2C_InitStruct->DIR          << 2)
               | (I2C_InitStruct->RESPFLAG   << 1) | (I2C_InitStruct->TRANSDONEFLAG<< 0);

   I2Cx->MSCR =   I2C_InitStruct->START            | (I2C_InitStruct->RESTART << 1)
               | (I2C_InitStruct->ARBSTATUS  << 2) | (I2C_InitStruct->BUSY    << 3);

   I2Cx->BCR  =   I2C_InitStruct->NACKINT << 7  | (I2C_InitStruct->ADRCMPINT  << 6)
               | (I2C_InitStruct->DMA     << 5) | (I2C_InitStruct->PREFETCH   << 4)
               | (I2C_InitStruct->ByteLength );

}


/*******************************************************************************
 函数名称：    void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
 功能描述：    I2C结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{

   I2C_InitStruct->ADRCMP = 0;
   I2C_InitStruct->ADDR   = 0;

   I2C_InitStruct->INTEN     = 0;
   I2C_InitStruct->DONEINT   = 0;
   I2C_InitStruct->BUSERRINT = 0;
   I2C_InitStruct->STOPINT   = 0;
   I2C_InitStruct->MASTER    = 0;
   I2C_InitStruct->SLAVE     = 0;

   I2C_InitStruct->BUSERRFLAG    = 0;
   I2C_InitStruct->LOSTARBFLAG   = 0;
   I2C_InitStruct->STOPFLAG      = 0;
   I2C_InitStruct->ACKCTRL       = 0;
   I2C_InitStruct->ADDRFLAG      = 0;
   I2C_InitStruct->DIR           = 0;
   I2C_InitStruct->RESPFLAG      = 0;
   I2C_InitStruct->TRANSDONEFLAG = 0;

   I2C_InitStruct->BUSY      = 0;
   I2C_InitStruct->ARBSTATUS = 0;
   I2C_InitStruct->RESTART   = 0;
   I2C_InitStruct->START     = 0;

   I2C_InitStruct->NACKINT    = 0;
   I2C_InitStruct->ADRCMPINT  = 0;
   I2C_InitStruct->DMA        = 0;
   I2C_InitStruct->PREFETCH   = 0;
   I2C_InitStruct->ByteLength = 0;
}

/*******************************************************************************
 函数名称：    void I2C_SENDDATA(I2C_TypeDef *I2Cx, uint32_t n)
 功能描述：    I2C发送数据
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t n)
{
	I2Cx->DATA = n;
}

/*******************************************************************************
 函数名称：    uint32_t I2C_ReadData(I2C_TypeDef *I2Cx)
 功能描述：    I2C读缓冲区数据
 操作的表：    无
 输入参数：    I2C_TypeDef *I2Cx
 输出参数：    无
 返 回 值：    缓冲区数据
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx)
{
   return I2Cx->DATA;
}


