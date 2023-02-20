/*******************************************************************************
 * 版权所有 (C)2018, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc05x_cmp.h
 * 文件标识：
 * 内容摘要： ADC外设驱动程序头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： William Zhang 
 * 完成日期： 2018月25日
 *
 * 修改记录1：
 * 修改日期：2018年7月25日
 * 版 本 号：V 1.0
 * 修 改 人：William Zhang 
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：20220426
 * 版 本 号：V2.0
 * 修 改 人：YangZJ
 * 修改内容：增加开窗配置、增加比较器中断相关函数
 *
 *******************************************************************************/
#ifndef __LKS32MC08x_CMP_H
#define __LKS32MC08x_CMP_H
/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"
#include <string.h>
 
// 比较器回差选择
#define CMP_HYS_20mV            0
#define CMP_HYS_0mV             1
//比较器N端
#define CMP_SELN_IN             0         /* N端输入 */
#define CMP_SELN_REF            1         /* N端内部参考REF输入 */
#define CMP_SELN_DAC            2         /* N端内部DAC输入 */
#define CMP_SELN_HALL_MID       3         /* N端反电势过零中点输入 */

//比较器P端
#define CMP_SELP_IP0            0        /* P端IP0输入 */
#define CMP_SELP_OPA_IP         1        /* OPA IP输入 OPA0_IP/OPA1_IP */
#define CMP_SELP_OPA_OUT        2        /* OPA OUT OPA0_OUT, OPA1_OUT */
#define CMP_SELP_IP1            3        /* P端IP1输入 */
#define CMP_SELP_IP2            4        /* P端IP2输入 */
#define CMP_SELP_IP3            5        /* P端IP3输入 */
#define CMP_SELP_IP4            6        /* P端IP4输入 */

// 为保证库兼容性，保留修改的宏定义
#define SELN_CMP_IN             0        /* N端输入 */
#define SELN_REF                1        /* N端内部参考REF输入 */
#define SELN_DAC                2        /* N端内部DAC输入 */
#define SELN_HALL_MID           3        /* N端反电势过零中点输入 */
#define SELP_CMP_OPA0IP         1        /* P端OPA0_IP输入 */
#define SELP_CMP_OPA0OUT        2        /* P端OPA0_OUT输入 */
#define SELP_CMP_OPA1OUT        3        /* P端OPA1_OUT输入 */

#define SELP_CMP_OPA3IP         1        /* P端OPA3_IP输入 */
#define SELP_CMP_OPA2OUT        2        /* P端OPA2_OUT输入 */
#define SELP_CMP_OPA3OUT        3        /* P端OPA3_OUT输入 */

#define SELP_CMP_IP0            0        /* P端IP0输入 */
#define SELP_CMP_IP1            4        /* P端IP1输入 */
#define SELP_CMP_IP2            5        /* P端IP2输入 */
#define SELP_CMP_IP3            6        /* P端IP3输入 */
#define SELP_CMP_IP4            7        /* P端IP4输入 */

typedef struct
{
    __IO uint32_t IE;       // 比较器中断使能寄存器
    __IO uint32_t IF;       // 比较器中断标志寄存器
    __IO uint32_t TCLK;     // 比较器分频时钟控制寄存器
    __IO uint32_t CFG;      // 比较器控制寄存器
    __IO uint32_t BLCWIN;   // 比较器开窗控制寄存器
    __IO uint32_t DATA;     // 比较器输出数值寄存器
} CMP_TypeDef;
#ifndef CMP
#define CMP             ((CMP_TypeDef *) CMP_BASE)
#endif
#define CMP0            0
#define CMP1            1

typedef struct
{
    u32         FIL_CLK10_DIV16;    // 比较器 1/0 滤波系数
    FuncState   CLK10_EN;           // 比较器 1/0 滤波时钟使能
    u32         FIL_CLK10_DIV2;     // 比较器 1/0 滤波系数移位 0-3
    FuncState   CMP_FT;             // 比较器快速比较使能
    u32         CMP_HYS;            // 比较器回差选择
    
    // 比较器0
    FuncState   CMP0_EN;            // 比较器 0 使能
    u32         CMP0_SELP;          // 比较器 0 信号正端选择
    u32         CMP0_SELN;          // 比较器 0 信号负端选择
    FuncState   CMP0_IE;            // 比较器 0 中断使能
    u32         CMP0_W_PWM_POL;     // 比较器 0 开窗 PWM 信号极性选择
    FuncState   CMP0_IRQ_TRIG;      // 比较器 0 边沿触发使能
    FuncState   CMP0_IN_EN;         // 比较器 0 信号输入使能
    u32         CMP0_POL;           // 比较器 0 极性选择
    FuncState   CMP0_CHN3P_WIN_EN;  // MCPWM 模块 CHN3_P 通道使能比较器 0 开窗
    FuncState   CMP0_CHN2P_WIN_EN;  // MCPWM 模块 CHN2_P 通道使能比较器 0 开窗
    FuncState   CMP0_CHN1P_WIN_EN;  // MCPWM 模块 CHN1_P 通道使能比较器 0 开窗
    FuncState   CMP0_CHN0P_WIN_EN;  // MCPWM 模块 CHN0_P 通道使能比较器 0 开窗
    
    // 比较器1
    FuncState   CMP1_EN;            // 比较器 1 使能
    u32         CMP1_SELP;          // 比较器 1 信号正端选择
    u32         CMP1_SELN;          // 比较器 1 信号负端选择
    FuncState   CMP1_IE;            // 比较器 1 中断使能
    u32         CMP1_W_PWM_POL;     // 比较器 1 开窗 PWM 信号极性选择
    FuncState   CMP1_IRQ_TRIG;      // 比较器 1 边沿触发使能
    FuncState   CMP1_IN_EN;         // 比较器 1 信号输入使能
    u32         CMP1_POL;           // 比较器 1 极性选择
    FuncState   CMP1_CHN3P_WIN_EN;  // MCPWM 模块 CHN3_P 通道使能比较器 1 开窗
    FuncState   CMP1_CHN2P_WIN_EN;  // MCPWM 模块 CHN2_P 通道使能比较器 1 开窗
    FuncState   CMP1_CHN1P_WIN_EN;  // MCPWM 模块 CHN1_P 通道使能比较器 1 开窗
    FuncState   CMP1_CHN0P_WIN_EN;  // MCPWM 模块 CHN0_P 通道使能比较器 1 开窗
}CMP_InitTypeDef;

void CMP_Init(CMP_InitTypeDef *);           // 比较器初始化
void CMP_StructInit(CMP_InitTypeDef *);     // 比较器配置结构体初始化
void CMP_ClearIRQFlag(u8 CMPx);             // 清CMP中断
u8   CMP_GetIRQFlag(u8 CMPx);               // 读CMP中断

#endif /*__LKS32MC05x_CMP_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/

