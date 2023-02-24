/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x.h"
#include "common.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
extern unsigned int SEGGER_SYSVIEW_TickCnt;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
#pragma GCC push_options
#pragma GCC optimize (0)

/*--------------------------------------------------------------------------------------------*
 * 函数名：  s16 CheckSum(u8 *pBuf, u8 uLen)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  校验和计算
 * 输入参数：*pBuf指向待校验数据头，uLen为校验字节数
 * 输出参数：校验值
 * 备  注：  2023年2月21日->创建
 *          为了能够体现校验错误，引入有符号整型，整形不能和枚举类型混用,因此不能返回枚举。
 *--------------------------------------------------------------------------------------------*/
s16 CheckSum(u8 *pBuf, u8 uLen)
{ 
    u8 sumTmp = 0x00;
    
    if (NULL == pBuf)
    {
        return -1;
    }

    if (uLen > 0) {
        for (int i = 0; i < uLen; i++) { /* C99 */
            sumTmp += pBuf[i];
        }
        /*
        // do
        // {
        //     sumTmp += pBuf[uLen];  
        // } while (uLen--); 
            1 此种写法调试会进入错误，无法调试
            2 不调试，和校验只能正确一次，应该是写法导致指针异常
        */
        return sumTmp;
    } else {
        return -1;
    }
}

#pragma GCC pop_options

/*--------------------------------------------------------------------------------------------*
 * 函数名：  u8 CheckCrc8(u8 *pData,u32 uLen)
 * 编写者：  F.L
 * 参考资料：无
 * 功  能：  crc8计算程序
 * 输入参数：*pData指向待计算crc的数组，uLen数组长度
 * 输出参数：校验值
 * 备  注：  2023年2月23日->注释
 *--------------------------------------------------------------------------------------------*/
u8 CheckCrc8(u8 *pData, u32 uLen)
{
    u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
    u8 out_XOR;
    out_XOR      = 0x00;
    REV_OUT_TYPE = DISABLE; // 输出数据反转使能
    REV_IN_TYPE  = 0;
    /*
        REV_IN_TYPE
        待编码数据反转类型
        0:不反转
        1:按字节反转，即 b[31]=b[24], b[30]=b[25], …, b[24]=b[31], …, b[7]=b[0], b[6]=b[1], …, b[0]=b[7]
        2:按半字（ 16bit ） 反 转 ， 即 b[31]=b[16], b[30]=b[17], …, b[16]=b[31], …, b[15]=b[0], b[14]=b[1], …, b[0]=b[15]
        3:按字反转，即 b[31]=b[0], b[30]=b[1],… [b0]=b[31]
    */
    POLY_SIZE = 2;
    /*
        输出编码（多项式）位宽
        0: 32bits
        1: 16bits
        2: 8bits
        3: 7bits
    */
    CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
    CRC_CR |= BIT0;  // 复位
    CRC_POL  = 0x07; // crc8计算公式
    CRC_INIT = 0x00;
    if (uLen == 0)
        return 0;
    else
        while (uLen--) {
            REG8(&CRC_DR) = *pData;
            pData++;
        }
    if (REV_OUT_TYPE == ENABLE) {
        return ((u8)(CRC_DR >> 24)) ^ out_XOR;
    } else {
        return (REG8(&CRC_DR)) ^ out_XOR;
    }
}


u32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
   
    u32 Cycles = 0;   
    u32 TickCount;
    u32 CyclesPerTick;
   
    //
    // Get the cycles of the current system tick.
    // SysTick is down-counting, subtract the current value from the number of cycles per tick.
    //   
    CyclesPerTick = SysTick->LOAD + 1;
    Cycles = (CyclesPerTick - SysTick->VAL);
   
    //
    // Get the system tick count.
    //   
    TickCount = SEGGER_SYSVIEW_TickCnt;
   
    //
    // If a SysTick interrupt is pending, re-read timer and adjust result
    //   
    if ( NVIC_GetPendingIRQ(SysTick_IRQn) != 0)
    {   
        TickCount++;  
    }
        
    Cycles += TickCount * CyclesPerTick;
        
    return Cycles;

}
/* USER IMPLEMENTED FUNCTIONS END */
