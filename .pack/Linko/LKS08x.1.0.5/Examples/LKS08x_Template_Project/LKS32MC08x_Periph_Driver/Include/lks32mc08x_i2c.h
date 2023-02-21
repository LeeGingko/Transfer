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
 
#ifndef __lks32mc08x_I2C_H
#define __lks32mc08x_I2C_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
   __IO uint32_t ADDR;
   __IO uint32_t CFG;
   __IO uint32_t SCR;
   __IO uint32_t DATA;
   __IO uint32_t MSCR;
   __IO uint32_t BCR;
}I2C_TypeDef;

typedef struct
{
   uint8_t    ADRCMP;    //I2C Address Compare by Hardware, 1: Enable, 0: Disable
   uint8_t    ADDR;      //I2C Address, 7-Bit

   uint8_t    INTEN;     //I2C Interrupt Enable, 1: Enable, 0: Disable
   uint8_t    DONEINT;   //I2C Transmit Done Interrupt Enable, 1: Enable, 0: Disable
   uint8_t    BUSERRINT; //I2C Bus Error Interrupt Enable, 1: Enable, 0: Disable
   uint8_t    STOPINT;   //I2C Stop Event Interrupt Enable,1: Enable, 0: Disable
   uint8_t    MASTER;    //I2C Master Mode, 1: Enable, 0: Disable
   uint8_t    SLAVE;     //I2C Slave Mode,  1: Enable, 0: Disable

   uint8_t    BUSERRFLAG;   //I2C Bus Error Flag
   uint8_t    LOSTARBFLAG;  //I2C Lost Arbitration Flag
   uint8_t    STOPFLAG;     //I2C Stop Flag
   uint8_t    ACKCTRL;      //I2C ACK Control
   uint8_t    ADDRFLAG;     //I2C Address Flag, 1: Transmit/Receive Data is Address, 0: not Address
   uint8_t    DIR;          //I2C Transmit/Receive Direction, 1: Transmit, 0: Receive
   uint8_t    RESPFLAG;     //I2C Transmit, Receive Response Flag, 1: Receive NACK, 0: Receive ACK
   uint8_t    TRANSDONEFLAG;//I2C Transmit Done Flag, 1: Finish, 0: not Finish

   uint8_t    BUSY;         //I2C Bus Status, 1: Busy, 0: Idle
   uint8_t    ARBSTATUS;    //I2C Master Get Bus or not, 1: Get, 0: Lost
   uint8_t    RESTART;      //I2C Resend Start Control, 1: Can Trig, 0: Can'table
   uint8_t    START;        //I2C Send Start Trig, 1: Trig, 0: Not Trig
   
   uint8_t    NACKINT;      //I2C NACK Interrupt Enable, 1: Enable, 0: Disable
   uint8_t    ADRCMPINT;    //I2C Address Compare by Hardware Interrupt Enable, 1: Enable, 0: Disable
   uint8_t    DMA;          //I2C DMA Way to Transmit, 1: Enable, 0: Disable
   uint8_t    PREFETCH;     //I2C Slave Prefetch Enable, 1: Enable, 0: Disable
   uint8_t    ByteLength;   //I2C Transmit Byte Length

}I2C_InitTypeDef;

void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);

void    I2C_SendData(I2C_TypeDef *I2Cx, uint8_t n);
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx);

#endif /*__lks32mc08x_I2C_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
