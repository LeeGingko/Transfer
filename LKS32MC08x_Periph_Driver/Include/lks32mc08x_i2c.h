/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_spi.h
 * 文件标识：
 * 内容摘要： I2C驱动头文件
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
 
#ifndef __LKS32MC08x_I2C_H
#define __LKS32MC08x_I2C_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

typedef struct
{
   __IO uint32_t ADDR;
   __IO uint32_t CFG;
   __IO uint32_t SCR;
   __IO uint32_t DATA;
   __IO uint32_t MSCR;
   __IO uint32_t BCR;
}I2C_TypeDef;

#ifndef I2C
#define I2C         ((I2C_TypeDef *) I2C0_BASE)
#endif

typedef struct
{
    u8  ADRCMP          ;   //  I2C 硬件地址比较使能开关，只有在 DMA 模式下开启才有效。
    u8  MST_MODE        ;   //  I2C 主模式使能
    u8  SLV_MODE        ;   //  I2C 从模式使能
    u8  DMA             ;   //  I2C DMA传输使能
    u32 BaudRate        ;   //  I2C 波特率
    u8  IE              ;   //  I2C 中断使能
    u8  TC_IE           ;   //  I2C 数据传输完成中断使能
    u8  BUS_ERR_IE      ;   //  I2C 总线错误事件中断使能
    u8  STOP_IE         ;   //  I2C STOP 事件中断使能
    u8  BURST_NACK      ;   //  I2C 传输，NACK 事件中断使能
    u8  BURST_ADDR_CMP  ;   //  I2C 传输，硬件地址匹配中断使能
}I2C_InitTypeDef;

void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);

void    I2C_SendData(I2C_TypeDef *I2Cx, uint8_t n);
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx);

// i2c&dma发送&接收
void i2c_dma_rx(u8 addr, u8 *data, u8 len);
void i2c_dma_tx(u8 addr, u8 *data, u8 len);
void i2c_dma_init(void);
void i2c_dma_state_over(void);

#endif /*__lks32mc08x_I2C_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
