#ifndef __HW_CAN_H
#define __HW_CAN_H
/* USER INCLUDE FILES BEGIN */ 
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
/* USER INCLUDE FILES END */ 

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define CAN_STB_PORT GPIO1
#define CAN_STB_PIN GPIO_Pin_3

#define CAN_STB_RX_LEN 128
/* USER DEFINED MACROS END */

/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */


/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */

void HW_CAN_GPIO_Init(void);
void HW_CAN_Init(void);
void HW_CAN_DMA_TX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr);
void HW_CAN_DMA_RX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr);

extern u8 CAN_Rx[55];
/* USER STATEMENTS END */

#endif
