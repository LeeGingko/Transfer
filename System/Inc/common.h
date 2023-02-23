#ifndef __COMMON__H
#define __COMMON__H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define SEGGER_RTT_PRINTF_EN 1
/* USER DEFINED MACROS END */

/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
typedef enum {
    tmErr = -1,
    tmOk,
    tmRspErr,
    tmRspOK,
    tmChErr,
    tmChOk
} TmOpState;
/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */
extern s16 CheckSum(u8 *pBuf, u8 uLen);
extern u8 CheckCrc8(u8 *pData, u32 uLen);
/* USER STATEMENTS END */

#endif
