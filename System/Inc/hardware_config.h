/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_config.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��8��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2020��8��20��
 * �� �� �ţ� V 2.0
 * �� �� �ˣ� Howlet
 * �޸����ݣ� ����
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
                                                                              
/* ------------------------------ ϵͳָʾLED ------------------------------ */
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
