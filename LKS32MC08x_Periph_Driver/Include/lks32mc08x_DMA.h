/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_DMA.h
 * 文件标识：
 * 内容摘要： DMA驱动头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： 
 * 完成日期： 2020年08月1日
 *
 * 修改记录1：
 * 修改日期：2020年08月1日
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
 
#ifndef __LKS32MC08x_DMA_H
#define __LKS32MC08x_DMA_H

#include "lks32mc08x_lib.h"


typedef struct
{
    uint8_t DMA_IRQ_EN;               /* DMA 中断使能 */
    uint8_t DMA_DIR;                  /* DMA传输方向 0:外设至内存， 1:内存至外设 */
    uint8_t DMA_CIRC;                 /* DMA传输模式：循环模式，高有效 */
    uint8_t DMA_PINC;                 /* 外设地址每轮内是否递增,高有效 */
    uint8_t DMA_MINC;                 /* 内存地址第二轮是否在第一轮地址的基础上递增,高有效 */
    uint8_t DMA_PBTW;                 /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    uint8_t DMA_MBTW;                 /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    uint8_t DMA_REQ_EN;               /* 通道 x 三个硬件 DMA 请求使能，高有效 */
    uint16_t DMA_TIMES;               /* DMA 通道 x 每轮数据搬运次数 1~511 */
    uint8_t DMA_ROUND;                /* DMA 通道 x 采样轮数 1~255 */
    uint32_t DMA_CPAR;                /* DMA 通道 x 外设地址 */
    uint32_t DMA_CMAR;                /* DMA 通道 x 内存地址 */

} DMA_InitTypeDef;

#ifndef DMA_CH0
#define DMA_CH0             ((DMA_RegTypeDef *) DMA_BASE)
#endif
#ifndef DMA_CH1
#define DMA_CH1             ((DMA_RegTypeDef *) (DMA_BASE+0x10))
#endif
#ifndef DMA_CH2
#define DMA_CH2             ((DMA_RegTypeDef *) (DMA_BASE+0x20))
#endif
#ifndef DMA_CH3
#define DMA_CH3             ((DMA_RegTypeDef *) (DMA_BASE+0x30))
#endif


typedef struct
{
    __IO uint32_t DMA_CCR;
    __IO uint32_t DMA_CTMS;
    __IO uint32_t DMA_CPAR;
    __IO uint32_t DMA_CMAR;
} DMA_RegTypeDef;

/*DMA中断使能定义*/
#define DMA_TCIE                 BIT0       /* 传输完成中断使能，高有效 */   
#define DMA_TEIE                 BIT1       /* 错误中断使能，高有效 */
/*DMA搬运方向定义*/
#define PERI2MEMORY              0          /* 外设至内存 */
#define MEMORY2PERI              1          /* 内存至外设 */
/*DMA搬运数据位宽定义*/
#define DMA_BYTE_TRANS           0          /* 访问位宽， 0:byte */
#define DMA_HALFWORD_TRANS       1          /* 访问位宽， 1:half-word */
#define DMA_WORD_TRANS           2          /* 访问位宽， 2:word */ 

/*触发DMA搬运源定义*/
#define DMA_CH0_ADC0_REQ_EN      BIT0       /* Channel_0 ADC0 DMA请求使能 */
#define DMA_CH0_TIMER0_REQ_EN    BIT1       /* Channel_0 TIMER0 DMA请求使能 */
#define DMA_CH0_TIMER1_REQ_EN    BIT2       /* Channel_0 TIMER1 DMA请求使能 */ 
#define DMA_CH1_SPI_RX_REQ_EN    BIT0       /* Channel_1 SPI_RX DMA请求使能 */
#define DMA_CH1_MCPWM_REQ_EN     BIT1       /* Channel_1 MCPWM DMA请求使能 */
#define DMA_CH1_TIMER2_REQ_EN    BIT2       /* Channel_1 TIMER2 DMA请求使能 */
#define DMA_CH2_UART0_REQ_EN     BIT0       /* Channel_2 UART0 DMA请求使能 */
#define DMA_CH2_SPI_TX_REQ_EN    BIT1       /* Channel_2 SPI_TX DMA请求使能 */
#define DMA_CH2_TIMER3_REQ_EN    BIT2       /* Channel_2 TIMER3 DMA请求使能 */ 
#define DMA_CH3_UART1_REQ_EN     BIT0       /* Channel_3 UART1 DMA请求使能 */
#define DMA_CH3_CAN_REQ_EN       BIT1       /* Channel_3 SPI_CAN DMA请求使能 */
#define DMA_CH3_I2C_REQ_EN       BIT2       /* Channel_3 I2X DMA请求使能 */

/*DMA中断标志定义*/
#define DMA_CH0_FIF  BIT0    /*通道 0 完成中断标志*/
#define DMA_CH1_FIF  BIT1    /*通道 1 完成中断标志*/
#define DMA_CH2_FIF	 BIT2    /*通道 2 完成中断标志*/
#define DMA_CH3_FIF  BIT3    /*通道 3 完成中断标志*/
#define DMA_CH0_EIF  BIT4    /*通道 0 错误中断标志*/
#define DMA_CH1_EIF  BIT5    /*通道 1 错误中断标志*/
#define DMA_CH2_EIF  BIT6    /*通道 2 错误中断标志*/
#define DMA_CH3_EIF  BIT7    /*通道 3 错误中断标志*/

void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct);
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct);
uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if);
void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag);
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN);

#endif
