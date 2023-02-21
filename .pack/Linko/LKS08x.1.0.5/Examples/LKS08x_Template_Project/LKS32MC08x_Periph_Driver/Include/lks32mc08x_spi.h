/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_spi.h
 * 文件标识：
 * 内容摘要： SPI驱动头文件
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
 
#ifndef __lks32mc08x_SPI_H
#define __lks32mc08x_SPI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef enum
{
   SPI_Master = 0x01,
   SPI_Slave  = 0x00
}SPI_Mode;

typedef enum
{
   SPI_Full    = 0x0,
   SPI_Half_Tx = 0x2,
   SPI_Half_Rx = 0x3
}SPI_Duplex;

typedef struct
{
   __IO uint32_t CFG;
   __IO uint32_t IE;
   __IO uint32_t DIV;
   __IO uint32_t TX_DATA;
   __IO uint32_t RX_DATA;
   __IO uint32_t SIZE;
}SPI_TypeDef;

typedef struct
{
   SPI_Duplex Duplex;    //Full or Half Duplex Work Mode
   uint8_t    SS;        //Chip Select Signal, 0: Tied to 0;1: from master device. Low is active
   SPI_Mode   Mode;      //Work Mose, 0: Slave Mode;1: Master Mode
   uint8_t    CPHA;      //Clock Phase
   uint8_t    CPOL;      //Clock Polarity
   uint8_t    DataOrder; //LSB or MSB, 1: LSB; 0: MSB
   uint8_t    EN;        //SPI Enable Signal, 0: disable;1: enable

   uint8_t    IRQEna;
   uint8_t    Trig;      //Transfer Start,1: External Start; 0: Auto Start by SPI module

   uint8_t    DMA;       //DMA or MCU, 1: MCU; 0: DMA
   uint8_t    BaudRate;  //BaudRate, Based on 96MHz

   uint8_t    ByteLength;//Transfer Data Length
}SPI_InitTypeDef;

#define SPI_FIRSTSEND_LSB 1
#define SPI_FIRSTSEND_MSB 0

#define SPI_DMA_ENABLE  0
#define SPI_DMA_DISABLE 1

/*中断使能定义*/
#define SPI_IRQEna_Enable              BIT7            //SPI Interrupt Enable
#define SPI_IRQEna_TranDone            BIT6            //Transmit Done IRQ
#define SPI_IRQEna_SSErr               BIT5            //SS Signal Error IRQ
#define SPI_IRQEna_DataOver            BIT4            //Transmit Data Over Flow IRQ

/*中断标志定义*/
#define SPI_IF_TranDone                BIT2            //Transmit Done IF
#define SPI_IF_SSErr                   BIT1            //SS Signal Error IF
#define SPI_IF_DataOver                BIT0            //Transmit Data Over Flow IF

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx);

void    SPI_SendData(SPI_TypeDef *SPIx, uint8_t n);
uint8_t SPI_ReadData(SPI_TypeDef *SPIx);
#endif /*__lks32mc08x_SPI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
