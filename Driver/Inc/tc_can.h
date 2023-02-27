#ifndef __TC_CAN_H
#define __TC_CAN_H

/* USER INCLUDE FILES BEGIN */
/* Included Files ---------------------------------------------------------------------- */
#include "basic.h"
/* USER INCLUDE FILES END */

/* USER DEFINED MACROS BEGIN */
/* Defined Macros ---------------------------------------------------------------------- */
#define CAN_STB_PORT       GPIO1
#define CAN_STB_PIN        GPIO_Pin_3

#define CAN_RX_LEN         128

#define CAN_FRAME_STAND    0
#define CAN_FRAME_EXTEND   1
#define CAN_FUNC_DATA      0
#define CAN_FUNC_REMOTE    1
#define CAN_COMMANDER_ADDR 0x2F
#define CAN_TRANSFER_ADDR  0x08

/* USER DEFINED MACROS END */

/* USER DEFINED ENUMERATION BEGIN */
/* Defined Enumeration ----------------------------------------------------------------- */

/* USER DEFINED ENUMERATION END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
typedef struct
{
    u32 f_id;         /* 设备ID */
    u8 f_ide;         /* 0：标准帧 1：扩展帧 */
    u8 f_rtr;         /* 0：数据帧 1：远程帧 */
    u8 f_txdata[8];   /* 接收数据缓冲 */
    u8 f_rxdata[8];   /* 发送数据缓冲 */
    u8 f_error_alarm; /* 错误报警 */
} TC_CANManage_t;

/* USER DEFINED TYPEDEFINE END */

/* USER STATEMENTS BEGIN */
/* Defined Statements ------------------------------------------------------------------ */

void TC_CAN_GPIO_Init(void);
void TC_CAN_Init(void);
void TC_CAN_DMA_TX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr);
void TC_CAN_DMA_RX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr);

extern u8 CAN_Rx[55];

extern TC_CANManage_t tc_CAN_Manage_t;

/* USER STATEMENTS END */

#endif
