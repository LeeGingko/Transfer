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
// ��������  void LKS_FREERTOS_Init(void)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  FreeRTOS ��ʼ�����������񡢶�ʱ�����ź��������е�
// ��������� ��
// ��������� ��
// ��  ע��   2023��2��14��->����
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
    taskENTER_CRITICAL(); // �����ٽ���
    /* 485�������񴴽� */
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
    /* CAN�������񴴽� */
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

    taskEXIT_CRITICAL(); // �˳��ٽ���
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
///////////////////////////////////////////

/////////////////////////////////////////////////
// ��������  static void RS485TaskFunc(void *pvParameters)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  LED0����������̬����
// ��������� void *pvParameters
// ��������� ��
// ��  ע��   2023��2��16��->����
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
// ��������  static void CANTaskFunc(void *pvParameters)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  LED0����������̬����
// ��������� void *pvParameters
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
static void CANTaskFunc(void *pvParameters)
{
    while (1) {
        vTaskDelay(100);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void SysLEDTimerCallback(void const *argument)
// ��д�ߣ�  F.L
// �ο����ϣ�
// ��  �ܣ�  �����ʱ������˸��������
// ��������� void const *argument ����ָ��
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
void SysLEDTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
