/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
// #include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "task.h"
#include "tc_uart.h"
#include "hardware_config.h"
/* USER INCLUDE FILES END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */

/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */
extern void LKS_FREERTOS_Init(void);
/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
int main(void)
{
    /* NVIC Priority Group Initialization */
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* SystemView Initialization */
    SEGGER_SYSVIEW_Conf();
    /* Hardware Initialization */
    Hardware_Init();
    /* RTOS Initialization */
    LKS_FREERTOS_Init();
    /* Start RTOS Scheduler */
    vTaskStartScheduler();
    /* Main Loop */
    while (1) /* 任务调度开启后永远不会进入主循环 */
    {
        printf("vTaskStartScheduler();: Not enough RAM!\r\n");
        Invers_GPIO(SYS_LED_PORT, SYS_LED_PIN);
        SoftDelay(6400000);
    }
}

/* USER IMPLEMENTED FUNCTIONS END */
