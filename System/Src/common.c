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
 �������ƣ�    u8 CheckCrc8(u8 *pData,u32 uLen)
 ����������    crc8�������
 ���������    *pDataָ�������crc������
                    uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 CheckCrc8(u8 *pData, u32 uLen)
{
    u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
    u8 out_XOR;
    out_XOR      = 0x00;
    REV_OUT_TYPE = DISABLE; // ������ݷ�תʹ��
    REV_IN_TYPE  = 0;
    /*
        REV_IN_TYPE
        ���������ݷ�ת����
        0:����ת
        1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
        2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
        3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
    */
    POLY_SIZE = 2;
    /*
        ������루����ʽ��λ��
        0: 32bits
        1: 16bits
        2: 8bits
        3: 7bits
    */
    CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
    CRC_CR |= BIT0;  // ��λ
    CRC_POL  = 0x07; // crc8���㹫ʽ
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
