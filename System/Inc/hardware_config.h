/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_config.h
 * 文件标识：
 * 内容摘要： 硬件相关文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2020年8月20日
 *
 * 修改记录1：
 * 修改日期： 2020年8月20日
 * 版 本 号： V 2.0
 * 修 改 人： Howlet
 * 修改内容： 创建
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion -------------------*/
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "hardware_init.h"

/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define NVIC_PRIORITYGROUP_0 0x00000007U /*!< 0 bits for pre-emption priority \
                                              4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1 0x00000006U /*!< 1 bits for pre-emption priority \
                                              3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2 0x00000005U /*!< 2 bits for pre-emption priority \
                                              2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3 0x00000004U /*!< 3 bits for pre-emption priority \
                                              1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4 0x00000003U /*!< 4 bits for pre-emption priority \
                                                                              
/* ------------------------------ 系统指示LED ------------------------------ */
#define SYS_LED_PORT GPIO0
#define SYS_LED_PIN  GPIO_Pin_0
/* USER DEFINED MACROS END */

/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */
extern u8 Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
extern void HW_NVIC_Init(void);
// extern void TimeTickIncrenment(void);

/* USER STATEMENTS END */

#endif
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
