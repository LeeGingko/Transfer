#ifndef __HW_UART_H
#define __HW_UART_H
/* USER INCLUDE FILES BEGIN */ 
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"

/* USER INCLUDE FILES END */ 

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define DUART_RX_LEN 128

/* USER DEFINED MACROS END */

/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */


/* USER DEFINED ENUMERATION END */


/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */


/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */
void HW_UART_GPIO_Init(void);
void HW_UART_Init(void);
void HW_UART_DMA_Init(void);

/* USER STATEMENTS END */
#endif
