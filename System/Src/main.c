/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "hardware_config.h"
/* USER INCLUDE FILES END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */

/* USER DEFINED VARIABLES END */
u8 sumCheck[6] = {0xAA, 0x12, 0x08, 0x08, 0x01, 0x00};
u8 sumCheckParity = 0x00;
/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */
extern void LKS_FREERTOS_Init(void);
/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
int main(void)
{
    /* Temperatory Veriables */
    sumCheckParity = CheckSum(sumCheck, 6);
    /* Debug SEGGER_RTT Initialization */
    SEGGER_RTT_Init();
    /* Hardware Initialization */
    Hardware_Init();
    /* RTOS Initialization */
    LKS_FREERTOS_Init();
    /* Start RTOS Scheduler */
    vTaskStartScheduler();
    /* Main Loop */
    while (1) { // 任务调度开启后永远不会进入主循环
    }
}

/* USER IMPLEMENTED FUNCTIONS END */
