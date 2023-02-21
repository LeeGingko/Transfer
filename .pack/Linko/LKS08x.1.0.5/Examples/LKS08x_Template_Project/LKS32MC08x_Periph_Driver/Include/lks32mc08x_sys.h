/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_sys.h
 * 文件标识：
 * 内容摘要： sys驱动头文件
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
 
#ifndef __lks32mc08x_SYS_H
#define __lks32mc08x_SYS_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef struct
{
	uint32_t PLL_SrcSel;                      /*PLL信号输入源选择，RC时钟或外部晶体*/
	uint32_t PLL_DivSel;                      /*选择8个时钟周期中，哪个周期输出时钟*/
	uint32_t PLL_ReDiv;                       /*PLL再分频，1分频或4分频*/

	uint32_t Clk_DivSPI;                      /*SPI I2C时钟分频*/
	uint32_t Clk_DivUART;                     /*UART时钟分频*/
	uint32_t Clk_FEN;                         /*模块时钟使能*/

	uint32_t WDT_Ena;                         /*使能看门狗*/
	uint32_t PORFilter_Ena;                   /*使能POR滤波*/
}SYS_InitTypeDef;

/*PLL信号输入源选择*/
#define SYS_PLLSRSEL_RCH        0x00          /*使用4MHz RC时钟*/
#define SYS_PLLSRSEL_CRYSTAL    0x01          /*使用晶体时钟*/

/*PLL再分频定义*/
#define SYS_PLLREDIV_4			0x00          /*在PLL分频基础上再/4*/
#define SYS_PLLREDIV_1			0x01          /*在PLL分频基础上不再分频*/

/*SPI时钟分频*/
#define SYS_Clk_SPIDiv1          0
#define SYS_Clk_SPIDiv2          1
#define SYS_Clk_SPIDiv4          2
#define SYS_Clk_SPIDiv8          3

/*UART时钟分频*/
#define SYS_Clk_UARTDiv1         0
#define SYS_Clk_UARTDiv2         1
#define SYS_Clk_UARTDiv4         2
#define SYS_Clk_UARTDiv8         3

/*模块位定义*/
#define SYS_Module_I2C           BIT0
#define SYS_Module_HALL          BIT1
#define SYS_Module_TIMER         BIT2
#define SYS_Module_MCPWM         BIT3
#define SYS_Module_UART0         BIT4
#define SYS_Module_UART1         BIT5
#define SYS_Module_DSP           BIT6
#define SYS_Module_CAN           BIT7
#define SYS_Module_SPI           BIT8

/*看门狗超时时间*/
#define SYS_WD_TimeOut2s  0         /*2s复位*/
#define SYS_WD_TimeOut4s  1         /*4s复位*/
#define SYS_WD_TimeOut8s 2         /*8s复位*/
#define SYS_WD_TimeOut64s 3         /*64s复位*/

/*复位信号源定义*/
#define SYS_RstSrc_LPOR        0x01
#define SYS_RstSrc_HPOR        0x02
#define SYS_RstSrc_KEYBOARD    0x04       /*按键复位*/
#define SYS_RstSrc_WDT         0x08       /*WDT复位*/

/*休眠唤醒间隔时间*/
#define SYS_WakeInterval_025s  0x00     /*0.25s*/    
#define SYS_WakeInterval_05s   0x01     /*0.5s*/
#define SYS_WakeInterval_1s    0x02     /*1s*/
#define SYS_WakeInterval_2s    0x03     /*2s*/
#define SYS_WakeInterval_4s    0x04     /*4s*/
#define SYS_WakeInterval_8s    0x05     /*8s*/
#define SYS_WakeInterval_16s   0x06     /*16s*/
#define SYS_WakeInterval_32s   0x07     /*32s*/

/*系统设置*/
void SYS_Init(SYS_InitTypeDef* SYS_InitStruct);
void SYS_StructInit(SYS_InitTypeDef* SYS_InitStruct);

/*复位源*/
uint32_t SYS_GetRstSource(void);
void SYS_ClearRst(void);

/*看门狗*/
// void SYS_EnableWatchDog(void);
// void SYS_DisableWatchDog(void);
void SYS_WatchDogCmd(FuncState state);
void SYS_FeedDog(uint32_t timeOut);

/*POR滤波*/
// void SYS_EnablePORFilter(void);
// void SYS_DisablePORFilter(void);
void SYS_PORFilterCmd(FuncState state);

/*休眠、唤醒*/
void SYS_FallSleep(void);
void SYS_SetWakeInterval(uint32_t nInterval);

/*控制8路时钟使能*/
// void SYS_Enable_ModuleClock(uint32_t nModule);
// void SYS_Disable_ModuleClock(uint32_t nModule);
void SYS_ModuleClockCmd(uint32_t nModule, FuncState state);

/*软复位各模块*/
void SYS_SoftResetModule(uint32_t nModule);

/*IAP_MODE*/
//允许进入IAP_MODE，在软复位以后



#endif /*__lks32mc08x_SYS_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
