/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： interrupt.c
 * 文件标识：
 * 内容摘要： 中断服务程序文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：2015年11月5日
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

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

void SleepTimer_IRQHandler(void);
void GPIO_IRQHandler(void);
void CSX_IRQHandler(void);
void I2C0_IRQHandler(void);

void SleepTimer_IRQHandler(void);
void GPIO_IRQHandler(void);
void VC3_IRQHandler(void);
void CSX_IRQHandler(void);
void NSX_IRQHandler(void);
void I2C0_IRQHandler(void);
void SPI0_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void HALL_IRQHandler(void);
void UTIMER0_IRQHandler(void);
void UTIMER1_IRQHandler(void);
void UTIMER2_IRQHandler(void);
void UTIMER3_IRQHandler(void);
void ENCODER0_IRQHandler(void);
void ENCODER1_IRQHandler(void);
void ADC1_IRQHandler(void);
void ADC0_IRQHandler(void);
void MCPWM_IRQHandler(void);

#endif

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
