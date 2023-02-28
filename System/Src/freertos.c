/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "tc_uart.h"
#include "tc_485.h"
#include "tc_can.h"
#include "SEGGER_RTT.h"
#include "hardware_config.h"

/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ------------------------------------------------------------------ */
#define RS485_RX_TASK_PRI 5
#define RS485_RX_STK_SIZE 128

#define RS485_TX_TASK_PRI 4
#define RS485_TX_STK_SIZE 64

#define CAN_RX_TASK_PRI   6
#define CAN_RX_STK_SIZE   128

#define CAN_TX_TASK_PRI   4
#define CAN_TX_STK_SIZE   64
/* USER DEFINED MACROS END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */

/* Defined Variables --------------------------------------------------------------------- */
TimerHandle_t sysLedTimer_Handler;

TaskHandle_t tc485RxTask_Handler;
TaskHandle_t tc485TxTask_Handler;
TaskHandle_t tcCanRxTask_Handler;
TaskHandle_t tcCanTxTask_Handler;

BaseType_t sysLedTimerStart;
BaseType_t taskCreateStatus;

u8 Can_TX[8] = {0x81, 0x13, 0x33, 0x44, 0x15, 0x26, 0x37, 0x48};

/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
static void TC_485RxTaskFunc(void *pvParameters);
static void TC_485TxTaskFunc(void *pvParameters);
static void TC_CANRxTaskFunc(void *pvParameters);
static void TC_CANTxTaskFunc(void *pvParameters);
static void SysLedTimerCallback(void const *argument);

extern TC_485Manage_t tc_485_Manage;
/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* *-------------------------------------------------------------------------------------* */
/*-------------------------------------------------------------------------------------------*
 * 函数名：  void LKS_FREERTOS_Init(void)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  FreeRTOS 初始化，创建任务、定时器、信号量、队列等
 * 输入参数： 无
 * 输出参数： 无
 * 备  注：   2023年2月14日->创建
 *------------------------------------------------------------------------------------------*/
void LKS_FREERTOS_Init(void)
{

/* -------------------------------- FreeRTOS Softtimer Initilization ------------------------------ */
#if ((configUSE_TIMERS == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    sysLedTimer_Handler = xTimerCreate("SysLedTimer",
                                       pdMS_TO_TICKS(1000),
                                       pdTRUE,
                                       (void *)0,
                                       (TimerCallbackFunction_t)SysLedTimerCallback);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (NULL == sysLedTimer_Handler) {
        printf("sysLedTimer Create Failed!\r\n");
    } else {
        printf("sysLedTimer Create Succeed!\r\n");
        sysLedTimerStart = xTimerStart(sysLedTimer_Handler, 500);
        if (sysLedTimerStart == pdFAIL) {
            printf("sysLedTimer Start Failed!\r\n");
        } else {
            printf("sysLedTimer Start Succeed!\r\n");
        }
    }
#endif

#endif

    /* -------------------------------- FreeRTOS Tasks Initilization ------------------------------ */
    taskENTER_CRITICAL(); // 进入临界区
    /* 485接收任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)TC_485RxTaskFunc,
                                   (const char *)"TC_485RxTaskFunc",
                                   (uint16_t)RS485_RX_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)RS485_RX_TASK_PRI,
                                   (TaskHandle_t *)&tc485RxTask_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("tc485RxTask Create Succeed!\r\n");
    } else {
        printf("tc485RxTask Create Failed!\r\n");
    }
#endif
    taskCreateStatus = pdFAIL;
    /* 485发送任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)TC_485TxTaskFunc,
                                   (const char *)"TC_485TxTaskFunc",
                                   (uint16_t)RS485_TX_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)RS485_TX_TASK_PRI,
                                   (TaskHandle_t *)&tc485TxTask_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("tc485TxTask Create Succeed!\r\n");
    } else {
        printf("tc485TxTask Create Failed!\r\n");
    }
#endif
    taskCreateStatus = pdFAIL;

    /* CAN接收任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)TC_CANRxTaskFunc,
                                   (const char *)"TC_CANRxTaskFunc",
                                   (uint16_t)CAN_RX_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)CAN_RX_TASK_PRI,
                                   (TaskHandle_t *)&tcCanRxTask_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("tcCanRxTask Create Succeed!\r\n");
    } else {
        printf("tcCanRxTask Create Failed!\r\n");
    }
#endif
    taskCreateStatus = pdFAIL;

    /* CAN发送任务创建 */
    taskCreateStatus = xTaskCreate((TaskFunction_t)TC_CANTxTaskFunc,
                                   (const char *)"TC_CANTxTaskFunc",
                                   (uint16_t)CAN_TX_STK_SIZE,
                                   (void *)NULL,
                                   (UBaseType_t)CAN_TX_TASK_PRI,
                                   (TaskHandle_t *)&tcCanTxTask_Handler);

#if (SEGGER_RTT_PRINTF_EN == 1)
    if (taskCreateStatus == pdPASS) {
        printf("tcCanTxTask Create Succeed!\r\n");
    } else {
        printf("tcCanTxTask Create Failed!\r\n");
    }
#endif
    taskCreateStatus = pdFAIL;

    taskEXIT_CRITICAL(); // 退出临界区
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
/*-------------------------------------------------------------------------------------------*
 * 函数名：  static void RS485TaskFunc(void *pvParameters)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  Led0任务函数，静态类型
 * 输入参数： void *pvParameters
 * 输出参数： 无
 * 备  注：   2023年2月16日->创建
 *------------------------------------------------------------------------------------------*/
static void TC_485RxTaskFunc(void *pvParameters)
{
    // TickType_t xTimerPeriod;
    // printf("static void TC_485RxTaskFunc(void *pvParameters)!\r\n");
    TC_485TransmitFrame();
    while (1) {
        /* Query the period of the timer that expires. */
        // xTimerPeriod = xTimerGetPeriod(sysLedTimer_Handler);
        vTaskDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------------*
 * 函数名：  static void TC_485TxTaskFunc(void *pvParameters)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  Led0任务函数，静态类型
 * 输入参数： void *pvParameters
 * 输出参数： 无
 * 备  注：   2023年2月27日->创建
 *------------------------------------------------------------------------------------------*/
static void TC_485TxTaskFunc(void *pvParameters)
{
    // printf("static void TC_485TxTaskFunc(void *pvParameters)!\r\n");
    TC_485TransmitFrame();
    while (1) {
        vTaskDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------------*
 * 函数名：  static void TC_CANRxTaskFunc(void *pvParameters)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  Led0任务函数，静态类型
 * 输入参数： void *pvParameters
 * 输出参数： 无
 * 备  注：   2023年2月16日->创建
 *------------------------------------------------------------------------------------------*/
static void TC_CANRxTaskFunc(void *pvParameters)
{
    // printf("static void TC_CANRxTaskFunc(void *pvParameters)!\r\n");
    while (1) {
        vTaskDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------------*
 * 函数名：  static void TC_CANTxTaskFunc(void *pvParameters)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  Led0任务函数，静态类型
 * 输入参数： void *pvParameters
 * 输出参数： 无
 * 备  注：   2023年2月27日->创建
 *------------------------------------------------------------------------------------------*/
static void TC_CANTxTaskFunc(void *pvParameters)
{
    // printf("static void TC_CANTxTaskFunc(void *pvParameters)!\r\n");
    while (1) {
        My_CAN_Send_Msg(CAN_TRANSFER_ADDR, CAN_FRAME_STAND, CAN_FUNC_DATA, Can_TX, 8);
        SEGGER_SYSVIEW_Print("My_CAN_Send_Msg\r\n");
        vTaskDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------------*
 * 函数名：  void SysLedTimerCallback(void const *argument)
 * 编写者：  F.L
 * 参考资料：
 * 功  能：  软件定时器，闪烁周期两秒
 * 输入参数： void const *argument 常量指针
 * 输出参数： 无
 * 备  注：   2023年2月16日->创建
 *-------------------------------------------------------------------------------------------*/
void SysLedTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
