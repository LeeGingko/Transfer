#ifndef __HW_UART_H
#define __HW_UART_H
/* USER INCLUDE FILES BEGIN */ 
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
#include "common.h"
#include "lks32mc08x_uart.h"
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
extern u8 rs485_TxFlag;
extern u8 duart_TxFlag;
extern void HW_UART_Init(void);
// void HW_UART_DMA_Init(void);
extern TmOpState HW_UARTSendBytes(UART_TypeDef *UARTx, const u8 *pData, u16 uLen);
/* USER STATEMENTS END */
#endif
