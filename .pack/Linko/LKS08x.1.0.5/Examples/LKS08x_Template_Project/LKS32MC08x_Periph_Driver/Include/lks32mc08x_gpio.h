/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_gpio.h
 * 文件标识：
 * 内容摘要： GPIO外设驱动程序头文件
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
 
#ifndef __lks32mc08x_GPIO_H
#define __lks32mc08x_GPIO_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
   __IO uint32_t PIE;
   __IO uint32_t POE;
   __IO uint32_t PDI;
   __IO uint32_t PDO;
   __IO uint32_t PUE;
   __IO uint32_t PDE;
   __IO uint32_t PODE;
   __IO uint32_t LCKR;
   __IO uint32_t F3210;
   __IO uint32_t F7654;
   __IO uint32_t FBA98;
   __IO uint32_t FFEDC;
} GPIO_TypeDef;

typedef enum
{
   GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode              */
   GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode             */
//   GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
   GPIO_Mode_ANA   = 0x02  /*!< GPIO Analog In/Out Mode,用作ANA时需禁止输入输出*/
}GPIOMode_TypeDef;

typedef enum
{
   GPIO_PuPd_NOPULL = 0x00,
   GPIO_PuPd_UP     = 0x01,  /*上拉*/
   GPIO_PuPd_DOWN   = 0x02,  /*下拉*/
}GPIOPuPd_TypeDef;

typedef enum
{ 
   Bit_RESET = 0,
   Bit_SET
}BitAction;

typedef struct
{
   uint32_t GPIO_Pin;            /*要配置的Pad*/                                      
   GPIOMode_TypeDef GPIO_Mode;   /*GPIO模式:输入、输出、模拟*/
   GPIOPuPd_TypeDef GPIO_PuPd;   /*上拉/下拉*/
   uint32_t GPIO_PODEna;         /*开漏*/
}GPIO_InitTypeDef;

#define AF0_GPIO         0
#define AF1_SYSAF        1
#define AF2_HALL         2
#define AF3_MCPWM        3
#define AF4_UART         4
#define AF5_SPI          5
#define AF6_I2C          6
#define AF7_TIMER01      7
#define AF8_TIMER23      8
#define AF9_DEBUG        9
#define AFF_ADC_OPA_CMP  15

/*GPIO_Pad define*/
#define GPIO_Pin_NONE     0
#define GPIO_Pin_0        BIT0
#define GPIO_Pin_1        BIT1
#define GPIO_Pin_2        BIT2
#define GPIO_Pin_3        BIT3
#define GPIO_Pin_4        BIT4
#define GPIO_Pin_5        BIT5
#define GPIO_Pin_6        BIT6
#define GPIO_Pin_7        BIT7
#define GPIO_Pin_8        BIT8
#define GPIO_Pin_9        BIT9
#define GPIO_Pin_10       BIT10
#define GPIO_Pin_11       BIT11
#define GPIO_Pin_12       BIT12
#define GPIO_Pin_13       BIT13
#define GPIO_Pin_14       BIT14
#define GPIO_Pin_15       BIT15
#define GPIO_Pin_ALL      (uint32_t)0xFFFF

/*GPIO_Pad_sources define*/
#define GPIO_PinSource_0  0
#define GPIO_PinSource_1  1
#define GPIO_PinSource_2  2
#define GPIO_PinSource_3  3
#define GPIO_PinSource_4  4
#define GPIO_PinSource_5  5
#define GPIO_PinSource_6  6
#define GPIO_PinSource_7  7
#define GPIO_PinSource_8  8
#define GPIO_PinSource_9  9
#define GPIO_PinSource_10 10
#define GPIO_PinSource_11 11
#define GPIO_PinSource_12 12
#define GPIO_PinSource_13 13
#define GPIO_PinSource_14 14
#define GPIO_PinSource_15 15

/* 初始化、配置函数 *********************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_LockRegister(GPIO_TypeDef* GPIOx);
void GPIO_UnlockRegister(GPIO_TypeDef* GPIOx);

/* GPIO读写函数 *************************************/
uint8_t  GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint32_t Val);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

/* GPIO功能选择函数 ***************************/
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint32_t GPIO_PinSource, uint32_t GPIO_AF);

#endif /*__lks32mc08x_GPIO_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
