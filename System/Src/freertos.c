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

// �������ȼ�
#define LED0_TASK_PRIO 2
// �����ջ��С
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
    taskENTER_CRITICAL(); // �����ٽ���
    xTaskCreate((TaskFunction_t)LED0Task,
                (const char *)"LED0Task",
                (uint16_t)LED0_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED0_TASK_PRIO,
                (TaskHandle_t *)&led0Task_Handler);
    taskEXIT_CRITICAL(); // �˳��ٽ���
}

/* *-------------------------------------------------------------------------------------* */
/* -------------------------------- FreeRTOS Task Functions ------------------------------ */
////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// ��д�ߣ�  F.L
// �ο����ϣ� 
// ��  �ܣ�  �����ֽڲ�����ָ�����Ⱥ�������մ��ڵ�ָ�����Ȼ�������ֽڲ���֡ͷ������������ڴ�ʱ
//           ���DMA�Ĵ���[8:0]������д DMA_CTMS������DMA�ڲ����ִμ���
// ��������� DMA_RegTypeDef *DMAx DMA���ͼĴ���
// ��������� ��
// ��  ע��   2023��2��16��->����
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
// ��������  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// ��д�ߣ�  F.L
// �ο����ϣ� 
// ��  �ܣ�  �����ֽڲ�����ָ�����Ⱥ�������մ��ڵ�ָ�����Ȼ�������ֽڲ���֡ͷ������������ڴ�ʱ
//           ���DMA�Ĵ���[8:0]������д DMA_CTMS������DMA�ڲ����ִμ���
// ��������� DMA_RegTypeDef *DMAx DMA���ͼĴ���
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
void SysLEDTimerCallback(void const *argument)
{
    Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
}

/* USER IMPLEMENTED FUNCTIONS END */
