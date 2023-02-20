/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "common.h"
#include "hw_uart.h"
#include "hw_485.h"
#include "hardware_config.h"

/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ------------------------------------------------------------------ */
#define CAN_TASK_PRI   3
#define CAN_STK_SIZE   64

#define RS485_TASK_PRI 4

#define RS485_STK_SIZE 64
/* USER DEFINED MACROS END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */

/* Defined Variables --------------------------------------------------------------------- */
TaskHandle_t rs485Task_Handler;
TaskHandle_t canTask_Handler;
TimerHandle_t sysLEDTimer_Handler;

BaseType_t sysLEDTimerStart;
BaseType_t taskCreateStatus;

/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
static void RS485TaskFunc(void *pvParameters);
static void CANTaskFunc(void *pvParameters);

static void SysLEDTimerCallback(void const *argument);

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* *-------------------------------------------------------------------------------------* */
/* --------------------------------- FreeRTOS Task Creation ------------------------------ */
////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void LKS_FREERTOS_Init(void)
// 编写者：  F.L
// 参考资料：
// 功  能：  FreeRTOS 初始化，创建任务、定时器、信号量、队列等
// 输入参数： 无
// 输出参数： 无
// 备  注：   2023年2月14日->创建
////////////////////////////////////////////////////////////////////////////////////////////
void LKS_FREERTOS_Init(void)
{

    /* -------------------------------- FreeRTOS Softtimer Initilization ------------------------------ */
#if ((configUSE_TIMERS == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    sysLEDTimer_Handler = xTimerCreate("SysLEDTimer",
                                       pdMS_TO_TICKS(1000),
                                       pdTRUE,
                                       (void *)0,
                                       (TimerCallbackFunction_t)SysLEDTimerCallback);
#if (SEGGER_RTT_PRINTF_EN == 1)
    if (NULL == sysLEDTimer_Handler) {
        printf("sysLEDTimer Create Failed!\r\n");
    } else {
        printf("sysLEDTimer Create Succeed!\r\n");
        sysLEDTimerStart = xTimerStart(sysLEDTimer_Handler, 500);
        if (sysLEDTimerStart == pdFAIL) {
            printf("sysLEDTimer Start Failed!\r\n");
        } else {
            printf("sysLEDTimer Start Succeed!\r\n");
        }
    }
#endif

#endif

    /* -------------------------------- FreeRTOS Tasks Initilization ------------------------------ */
    taskENTER_CRITICAL(); // 进入临界区
    /* 485处理任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)RS485TaskFunc,
                                   (const char *)"RS485TaskFunc",
                                   (uint16_t)RS485_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)RS485_TASK_PRI,
                                   (TaskHandle_t *)&rs485Task_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("RS485Task Create Succeed!\r\n");
    } else {
        printf("RS485Task Create Failed!\r\n");
    }
#endif

    taskCreateStatus = pdFAIL;
    /* CAN处理任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)CANTaskFunc,
                                   (const char *)"CANTaskFunc",
                                   (uint16_t)CAN_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)CAN_TASK_PRI,
                                   (TaskHandle_t *)&canTask_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("CANTask Create Succeed!\r\n");
    } else {
        printf("CANTask Create Failed!\r\n");
    }
#endif

    taskEXIT_CRITICAL(); // 退出临界区
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
///////////////////////////////////////////

/////////////////////////////////////////////////
// 函数名：  static void RS485TaskFunc(void *pvParameters)
// 编写者：  F.L
// 参考资料：
// 功  能：  LED0任务函数，静态类型
// 输入参数： void *pvParameters
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
static void RS485TaskFunc(void *pvParameters)
{
    while (1) {
        // if (rs485_RxFlag == f_rec_ok) {
        //     rs485_RxFlag = f_rec_er;
        //     printf("RS485 RxData ");
        //     for (int i = 0; i < RS485_RX_LEN; i++) {
        //         printf("%02X ", rs485_Rx[i]);
        //     }
        //     printf("\r\n");
        //     HW_485_RxDMAClearCTMS(DMA_CH2);
        //     // HW_485_RxDMAClearCPAR_CMAR(DMA_CH2);
        //     memset(rs485_Rx, 0, RS485_RX_LEN);
        // }
        if (rs485_Rx[0] != F485_HEAD) {
            memset(rs485_Rx, 0, RS485_RX_LEN);
        } else if (rs485_Rx[0] == F485_HEAD) {
            printf("RS485 RxData ");
            for (int i = 0; i < RS485_RX_LEN; i++) {
                printf("%02X ", rs485_Rx[i]);
            }
            printf("\r\n");
            memset(rs485_Rx, 0, RS485_RX_LEN);
        }
        else
        {

        }
        vTaskDelay(10);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  static void CANTaskFunc(void *pvParameters)
// 编写者：  F.L
// 参考资料：
// 功  能：  LED0任务函数，静态类型
// 输入参数： void *pvParameters
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
static void CANTaskFunc(void *pvParameters)
{
    while (1) {
        vTaskDelay(100);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void SysLEDTimerCallback(void const *argument)
// 编写者：  F.L
// 参考资料：
// 功  能：  软件定时器，闪烁周期两秒
// 输入参数： void const *argument 常量指针
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
void SysLEDTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
