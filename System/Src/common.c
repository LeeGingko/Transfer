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

/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
u8 CheckSum(const u8 *pBuf, u8 uLen)
{
    // u8 i;
    u8 sumTmp = 0x00;

    if (NULL == pBuf) {
        return tmErr;
    }
    if (uLen > 0) {
        do {
            sumTmp += *pBuf;
            pBuf++;
        } while (uLen--);
        // for (i = 0; i < uLen; i++) {
        //     sumTmp += *pBuf;
        //     pBuf++;
        // }
        return sumTmp;
    } else {
        return 0xFF;
    }
}

/*******************************************************************************
 函数名称：    u8 CheckCrc8(u8 *pData,u32 uLen)
 功能描述：    crc8计算程序
 输入参数：    *pData指向待计算crc的数组
                    uLen数组长度
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 *******************************************************************************/
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
/* USER IMPLEMENTED FUNCTIONS END */
