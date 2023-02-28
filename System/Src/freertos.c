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
 * ��������  void LKS_FREERTOS_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  FreeRTOS ��ʼ�����������񡢶�ʱ�����ź��������е�
 * ��������� ��
 * ��������� ��
 * ��  ע��   2023��2��14��->����
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
    taskENTER_CRITICAL(); // �����ٽ���
    /* 485�������񴴽� */
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
    /* 485�������񴴽� */
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

    /* CAN�������񴴽� */
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

    /* CAN�������񴴽� */
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

    taskEXIT_CRITICAL(); // �˳��ٽ���
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
/*-------------------------------------------------------------------------------------------*
 * ��������  static void RS485TaskFunc(void *pvParameters)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  Led0����������̬����
 * ��������� void *pvParameters
 * ��������� ��
 * ��  ע��   2023��2��16��->����
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
 * ��������  static void TC_485TxTaskFunc(void *pvParameters)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  Led0����������̬����
 * ��������� void *pvParameters
 * ��������� ��
 * ��  ע��   2023��2��27��->����
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
 * ��������  static void TC_CANRxTaskFunc(void *pvParameters)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  Led0����������̬����
 * ��������� void *pvParameters
 * ��������� ��
 * ��  ע��   2023��2��16��->����
 *------------------------------------------------------------------------------------------*/
static void TC_CANRxTaskFunc(void *pvParameters)
{
    // printf("static void TC_CANRxTaskFunc(void *pvParameters)!\r\n");
    while (1) {
        vTaskDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------------*
 * ��������  static void TC_CANTxTaskFunc(void *pvParameters)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  Led0����������̬����
 * ��������� void *pvParameters
 * ��������� ��
 * ��  ע��   2023��2��27��->����
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
 * ��������  void SysLedTimerCallback(void const *argument)
 * ��д�ߣ�  F.L
 * �ο����ϣ�
 * ��  �ܣ�  �����ʱ������˸��������
 * ��������� void const *argument ����ָ��
 * ��������� ��
 * ��  ע��   2023��2��16��->����
 *-------------------------------------------------------------------------------------------*/
void SysLedTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
