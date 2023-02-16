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

// 任务优先级
#define LED0_TASK_PRIO 2
// 任务堆栈大小
#define LED0_STK_SIZE 50
/* USER DEFINED MACROS END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */

BaseType_t sysLEDTimerStart;

TaskHandle_t led0Task_Handler;

TimerHandle_t sysLEDTimer_Handler;

/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
static void LED0Task(void *pvParameters);

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
    /* RTOS Initialization */
    /* Create Timer */
#if ((configUSE_TIMERS == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    sysLEDTimer_Handler = xTimerCreate("SysLEDTimer",
                                       pdMS_TO_TICKS(1000),
                                       pdTRUE,
                                       (void *)0,
                                       (TimerCallbackFunction_t)SysLEDTimerCallback);
    if (NULL == sysLEDTimer_Handler) {
        printf("xTimerCreate Failed!\r\n");
    } else {
        printf("xTimerCreate Succeed!\r\n");
        sysLEDTimerStart = xTimerStart(sysLEDTimer_Handler, 500);
        if (sysLEDTimerStart == pdFAIL) {
            printf("xTimerStart Failed!\r\n");
        } else {
            printf("xTimerStart Succeed!\r\n");
        }
    }

#endif

    /* Create Tasks */
    taskENTER_CRITICAL(); // 进入临界区
    xTaskCreate((TaskFunction_t)LED0Task,
                (const char *)"LED0Task",
                (uint16_t)LED0_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED0_TASK_PRIO,
                (TaskHandle_t *)&led0Task_Handler);
    taskEXIT_CRITICAL(); // 退出临界区
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// 编写者：  F.L
// 参考资料： 
// 功  能：  接收字节不满足指定长度后继续接收大于等指定长度会出现首字节不是帧头的现象，因此需在此时
//           清除DMA寄存器[8:0]，即重写 DMA_CTMS以重置DMA内部的轮次计数
// 输入参数： DMA_RegTypeDef *DMAx DMA类型寄存器
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
static void LED0Task(void *pvParameters)
{
    while (1) {

        if (RS485_RxFlag) {
            RS485_RxFlag = 0;
            printf("RS485 RxData ");
            for (int i = 0; i < RS485_RX_LEN; i++) {
                printf("%02X ", RS485_RX[i]);
            }
            printf("\r\n");
            HW_485_RxDMAClearCTMS(DMA_CH2);
            memset(RS485_RX, 0, RS485_RX_LEN);
        }
        vTaskDelay(10);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
// 函数名：  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// 编写者：  F.L
// 参考资料： 
// 功  能：  接收字节不满足指定长度后继续接收大于等指定长度会出现首字节不是帧头的现象，因此需在此时
//           清除DMA寄存器[8:0]，即重写 DMA_CTMS以重置DMA内部的轮次计数
// 输入参数： DMA_RegTypeDef *DMAx DMA类型寄存器
// 输出参数： 无
// 备  注：   2023年2月16日->创建
////////////////////////////////////////////////////////////////////////////////////////////
void SysLEDTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
