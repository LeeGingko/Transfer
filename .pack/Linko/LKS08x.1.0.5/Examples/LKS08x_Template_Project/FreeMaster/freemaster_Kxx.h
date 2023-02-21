/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2011 Freescale Semiconductor
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file   freemaster_Kxx.h
*
* @brief  FreeMASTER Driver hardware dependent stuff
*
* @version 1.0.7.0
* 
* @date Aug-18-2011
* 
*******************************************************************************/

#ifndef __FREEMASTER_KXX_H
#define __FREEMASTER_KXX_H

/******************************************************************************
* platform-specific default configuration
******************************************************************************/

/* use 32-bit (EX) commands by default */
#ifndef FMSTR_USE_EX_CMDS
#define FMSTR_USE_EX_CMDS 1
#endif

/* do not use 16-bit (no-EX) commands by default */
#ifndef FMSTR_USE_NOEX_CMDS
#define FMSTR_USE_NOEX_CMDS 0
#endif

/* at least one of EX or no-EX command handling must be enabled */
#if !FMSTR_USE_EX_CMDS && !FMSTR_USE_NOEX_CMDS
    #error At least one of EX or no-EX command handling must be enabled (please set FMSTR_USE_EX_CMDS)
    #undef  FMSTR_USE_EX_CMDS
    #define FMSTR_USE_EX_CMDS 1
#endif

/*****************************************************************************
* Board configuration information 
******************************************************************************/

#define FMSTR_PROT_VER           3      /* protocol version 3 */
#define FMSTR_CFG_FLAGS          0      /* board info flags */
#define FMSTR_CFG_BUS_WIDTH      1      /* data bus width */
#define FMSTR_GLOB_VERSION_MAJOR 2      /* driver version */
#define FMSTR_GLOB_VERSION_MINOR 0
#define FMSTR_IDT_STRING "Kxx FreeMASTER"
#define FMSTR_TSA_FLAGS          0

/******************************************************************************
* platform-specific types
******************************************************************************/

typedef unsigned char  FMSTR_U8;         /* smallest memory entity */
typedef unsigned short FMSTR_U16;        /* 16bit value */
typedef unsigned long  FMSTR_U32;        /* 32bit value */

typedef signed char    FMSTR_S8;         /* signed 8bit value */
typedef signed short   FMSTR_S16;        /* signed 16bit value */
typedef signed long    FMSTR_S32;        /* signed 32bit value */

#if FMSTR_REC_FLOAT_TRIG
typedef float          FMSTR_FLOAT;        /* float value */
#endif

typedef unsigned char  FMSTR_FLAGS;      /* type to be union-ed with flags (at least 8 bits) */
typedef unsigned char  FMSTR_SIZE8;      /* one-byte size value */
typedef signed short   FMSTR_INDEX;      /* general for-loop index (must be signed) */

typedef unsigned char  FMSTR_BCHR;       /* type of a single character in comm.buffer */
typedef unsigned char* FMSTR_BPTR;       /* pointer within a communication buffer */

typedef unsigned char  FMSTR_SCISR;      /* data type to store SCI status register */

/******************************************************************************
* communication buffer access functions
******************************************************************************/

void FMSTR_CopyMemory(FMSTR_ADDR nDestAddr, FMSTR_ADDR nSrcAddr, FMSTR_SIZE8 nSize);
FMSTR_BPTR FMSTR_CopyToBuffer(FMSTR_BPTR pDestBuff, FMSTR_ADDR nSrcAddr, FMSTR_SIZE8 nSize);
FMSTR_BPTR FMSTR_CopyFromBuffer(FMSTR_ADDR nDestAddr, FMSTR_BPTR pSrcBuff, FMSTR_SIZE8 nSize);
void FMSTR_CopyFromBufferWithMask(FMSTR_ADDR nDestAddr, FMSTR_BPTR pSrcBuff, FMSTR_SIZE8 nSize);

/* mixed EX and non-EX commands may occur */
#if (FMSTR_USE_EX_CMDS) && (FMSTR_USE_NOEX_CMDS) || (FMSTR_BYTE_BUFFER_ACCESS)
void FMSTR_SetExAddr(FMSTR_BOOL bNextAddrIsEx);
#else
/* otherwise, we always know what addresses are used, (ignore FMSTR_SetExAddr) */
#define FMSTR_SetExAddr(bNextAddrIsEx) 
#endif

#if (FMSTR_BYTE_BUFFER_ACCESS)
FMSTR_BPTR FMSTR_ValueFromBuffer16(FMSTR_U16* pDest, FMSTR_BPTR pSrc);
FMSTR_BPTR FMSTR_ValueFromBuffer32(FMSTR_U32* pDest, FMSTR_BPTR pSrc);
FMSTR_BPTR FMSTR_ValueToBuffer16(FMSTR_BPTR pDest, FMSTR_U16 src);
FMSTR_BPTR FMSTR_ValueToBuffer32(FMSTR_BPTR pDest, FMSTR_U32 src);
#endif


/*********************************************************************************
* communication buffer access functions. Most of them are trivial simple on KXX
*********************************************************************************/

#define FMSTR_ValueFromBuffer8(pDest, pSrc) \
    ( (*((FMSTR_U8*)(pDest)) = *(FMSTR_U8*)(pSrc)), (((FMSTR_BPTR)(pSrc))+1) )

#if !(FMSTR_BYTE_BUFFER_ACCESS)
#define FMSTR_ValueFromBuffer16(pDest, pSrc) \
    ( (*((FMSTR_U16*)(pDest)) = *(FMSTR_U16*)(pSrc)), (((FMSTR_BPTR)(pSrc))+2) )

#define FMSTR_ValueFromBuffer32(pDest, pSrc) \
    ( (*((FMSTR_U32*)(pDest)) = *(FMSTR_U32*)(pSrc)), (((FMSTR_BPTR)(pSrc))+4) )
#endif

#define FMSTR_ValueToBuffer8(pDest, src) \
    ( (*((FMSTR_U8*)(pDest)) = (FMSTR_U8)(src)), (((FMSTR_BPTR)(pDest))+1) )

#if !(FMSTR_BYTE_BUFFER_ACCESS)
#define FMSTR_ValueToBuffer16(pDest, src) \
    ( (*((FMSTR_U16*)(pDest)) = (FMSTR_U16)(src)), (((FMSTR_BPTR)(pDest))+2) )

#define FMSTR_ValueToBuffer32(pDest, src) \
    ( (*((FMSTR_U32*)(pDest)) = (FMSTR_U32)(src)), (((FMSTR_BPTR)(pDest))+4) )
#endif

#define FMSTR_SkipInBuffer(pDest, nSize) \
    ( ((FMSTR_BPTR)(pDest)) + (nSize) )


#define FMSTR_ConstToBuffer8  FMSTR_ValueToBuffer8
#define FMSTR_ConstToBuffer16 FMSTR_ValueToBuffer16

/* EX address used only: fetching 32bit word */
#if (FMSTR_USE_EX_CMDS) && !(FMSTR_USE_NOEX_CMDS) && !(FMSTR_BYTE_BUFFER_ACCESS)
    #define FMSTR_AddressFromBuffer(pDest, pSrc) \
        FMSTR_ValueFromBuffer32(pDest, pSrc)
    #define FMSTR_AddressToBuffer(pDest, nAddr) \
        FMSTR_ValueToBuffer32(pDest, nAddr)

/* no-EX address used only: fetching 16bit word  */
#elif !(FMSTR_USE_EX_CMDS) && (FMSTR_USE_NOEX_CMDS) && !(FMSTR_BYTE_BUFFER_ACCESS)
    #define FMSTR_AddressFromBuffer(pDest, pSrc) \
        FMSTR_ValueFromBuffer16(pDest, pSrc)
    #define FMSTR_AddressToBuffer(pDest, nAddr) \
        FMSTR_ValueToBuffer16(pDest, nAddr)

/* mixed addresses used, need to process it programatically */
#else
    FMSTR_BPTR FMSTR_AddressFromBuffer(FMSTR_ADDR* pAddr, FMSTR_BPTR pSrc);
    FMSTR_BPTR FMSTR_AddressToBuffer(FMSTR_BPTR pDest, FMSTR_ADDR nAddr);
#endif

#define FMSTR_GetS8(addr)  ( *(FMSTR_S8*)(addr) )
#define FMSTR_GetU8(addr)  ( *(FMSTR_U8*)(addr) )
#define FMSTR_GetS16(addr) ( *(FMSTR_S16*)(addr) )
#define FMSTR_GetU16(addr) ( *(FMSTR_U16*)(addr) )
#define FMSTR_GetS32(addr) ( *(FMSTR_S32*)(addr) )
#define FMSTR_GetU32(addr) ( *(FMSTR_U32*)(addr) )

#if FMSTR_REC_FLOAT_TRIG
#define FMSTR_GetFloat(addr) ( *(FMSTR_FLOAT*)(addr) )
#endif

/****************************************************************************************
* Other helper macros
*****************************************************************************************/

/* This macro assigns C pointer to FMSTR_ADDR-typed variable */
#define FMSTR_PTR2ADDR(tmpAddr,ptr) ( tmpAddr = (FMSTR_ADDR) (FMSTR_U8*) ptr )
#define FMSTR_ARR2ADDR FMSTR_PTR2ADDR

/****************************************************************************************
* Platform-specific configuration
*****************************************************************************************/


/****************************************************************************************
* General peripheral space access macros
*****************************************************************************************/

/****************************************************************************************
* SCI module constants
*****************************************************************************************/

/* SCI module registers */
#define FMSTR_SCIBDH_OFFSET 0
#define FMSTR_SCIBDL_OFFSET 1
#define FMSTR_SCIC1_OFFSET 2
#define FMSTR_SCIC2_OFFSET 3
#define FMSTR_SCIS1_OFFSET 4
#define FMSTR_SCIS2_OFFSET 5
#define FMSTR_SCIC3_OFFSET 6
#define FMSTR_SCIDR_OFFSET 7

/* SCI Control Register bits */
#define UART_IF_FLG            UART1_IF

#define USART_TX_EN_FLG()      uart_tx_en
#define USART_ReceiveData()    UART1_BUFF
void UART1_SENDDATA(unsigned char n);
#define USART_SendData         UART1_SENDDATA
#define USART_FLAG_TX_EMPT     0x05 /* 发送缓冲空 */
#define USART_FLAG_TC          0x01
#define USART_FLAG_RXNE        0x02 /* 接收到数据 */



/*******************************************************************************************
* SCI access macros
*****************************************************************************************/
#define TXD_IE_DISABLE()       {UART1_IE &= (~0x05);}
#define TXD_IE_ENABLE()        {UART1_IE |= 0x05;}
#define RXD_IE_DISABLE()       {}
#define RXD_IE_ENABLE()        {UART1_IE |= 0x02;}

#define CLEAR_RXNE()           {UART1_IF |= USART_FLAG_RXNE;}
#define CLEAR_TX_EMPT()        {UART1_IF |= USART_FLAG_TX_EMPT;}
#define CLEAR_TX_TC()          {UART1_IF |= USART_FLAG_TC;}

#define USART_TX_ENABLE()      {uart_tx_en = 1;}
#define USART_TX_DISABLE()     {uart_tx_en = 0;}

#endif /* __FREEMASTER_KXX_H */

