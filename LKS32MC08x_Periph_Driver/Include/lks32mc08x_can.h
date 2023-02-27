/*******************************************************************************
 * 版权所有 (C)2018, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc05x_can.h
 * 文件标识：
 * 内容摘要： can外设驱动程序头文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： HuangMG
 * 完成日期： 2022月6日
 *
 *******************************************************************************/
#ifndef __LKS32MC08x_CAN_H_
#define __LKS32MC08x_CAN_H_
/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

typedef struct
{
    __IO uint32_t MOD;
    __IO uint32_t CMR;
    __IO uint32_t SR;
    __IO uint32_t IR;

    __IO uint32_t EIR;
    __IO uint32_t null0;
    __IO uint32_t BTR0;
    __IO uint32_t BTR1;

    __IO uint32_t null1;
    __IO uint32_t null2;
    __IO uint32_t null3;
    __IO uint32_t ALC;

    __IO uint32_t ECC;
    __IO uint32_t EWLR;
    __IO uint32_t RXERR;
    __IO uint32_t TXERR;

    __IO uint32_t TXRX0;
    __IO uint32_t TXRX1;
    __IO uint32_t TXRX2;
    __IO uint32_t TXRX3;

    __IO uint32_t TXRX4;
    __IO uint32_t TXRX5;
    __IO uint32_t TXRX6;
    __IO uint32_t TXRX7;

    __IO uint32_t TXRX8;
    __IO uint32_t TXRX9;
    __IO uint32_t TXRXA;
    __IO uint32_t TXRXB;

    __IO uint32_t TXRXC;
    __IO uint32_t RMC;
    __IO uint32_t RBSA;
} CAN_TypeDef;

#ifndef CAN
#define CAN ((CAN_TypeDef *)CAN_BASE)
#endif

typedef struct
{
    uint32_t CAN_WorkMode;   /*CAN工作模式选择 1：自测模式 2：监听模式 3;正常收发模式*/
    uint32_t CAN_DMAEn;      /*使能CAN的DMA功能*/
    uint32_t CAN_ErrWarThre; /*错误与警告门限值*/
    uint32_t CAN_Btr0;       /*波特率 0*/
    uint32_t CAN_Btr1;       /*波特率 1*/
    uint32_t IRQEna;         /*中断使能*/
} CAN_InitTypeDef;

/*工作模式选择*/
#define CAN_SELFTEST_MODE BIT2 /*自测模式*/
#define CAN_MONITOR_MODE  BIT1 /*监听模式*/
#define CAN_NORMAL_MODE   0x0  /*正常收发模式*/

/*中断使能定义*/
#define CAN_IE_BUSERR       BIT7 /*总线错误中断使能*/
#define CAN_IE_LOSTARB      BIT6 /*丢失仲裁中断使能*/
#define CAN_IE_PASSIVEERR   BIT5 /*被动错误错误中断使能*/
#define CAN_IE_WAKE         BIT4 /*CAN模块从休眠中唤醒中断使能*/
#define CAN_IE_RFIFOOV      BIT3 /*RFIFO数据发送溢出中断使能*/
#define CAN_IE_WERR         BIT2 /*错误报警中断使能*/
#define CAN_IE_TXDONE       BIT1 /*发送完毕当前帧中断使能*/
#define CAN_IE_RFIFONOEMPTY BIT0 /*RFIFI0有新的数据被接收到中断使能*/

/*中断标志定义,注意CAN_IR为读清除寄存器,只有 BIT0--RFIFO_N_EMPTY_IF 无法读清除，
此位只能通过对CAN_CMR BIT2 写 1 清除，若 RX FIFO 有多帧接收的数据，CAN_CMR BIT2
写 1 也无法清除，除非RX FIFO 全部读完，即读一帧数据写 CAN_CMR BIT2，反复迭代。*/
#define CAN_IF_BUSERR       BIT7 /*总线错误中断标志*/
#define CAN_IF_LOSTARB      BIT6 /*丢失仲裁中断标志*/
#define CAN_IF_PASSIVEERR   BIT5 /*被动错误错误中断标志*/
#define CAN_IF_WAKE         BIT4 /*CAN模块从休眠中唤醒中断标志*/
#define CAN_IF_RFIFOOV      BIT3 /*RFIFO数据发送溢出中断标志*/
#define CAN_IF_WERR         BIT2 /*错误报警中断标志*/
#define CAN_IF_TXDONE       BIT1 /*发送完毕当前帧中断标志*/
#define CAN_IF_RFIFONOEMPTY BIT0 /*RFIFI0有新的数据被接收到中断标志*/

/*选择读取CAN_SR状态定义*/
#define CAN_ONBUS BIT7      /*1：CAN 模块，处于 BUS OFF 状态，无数据发送接收动作 \
                              0：CAN 模块，可以发送数据到 CAN 总线，或接收 CAN 总线数据*/
#define CAN_ERROV BIT6      /*1：CAN 传输产生的错误总数达到或超过 CAN_EWL 规定值 \
                              0：CAN 传输产生的错误总数低于 CAN_EWL 规定值*/
#define CAN_TXING BIT5      /*1：CAN 模块正在发送一帧数据 \
                              0：CAN 模块没有发送数据的动作*/
#define CAN_RXING BIT4      /*1：CAN 模块正在接收一帧数据 \
                              0：CAN 模块没有接收数据的动作 */
#define CAN_TRNASDONE BIT3  /*1：最近一次传输已完成 \
                              0：最近一次传输未完成*/
#define CAN_TFIFOEMPTY BIT2 /*1：TFIFO 空，可以写入发送数据 \
                              0：TFIFO 非空，内部数据没有发送完毕*/
#define CAN_RFIFOEMPTY BIT1 /*1：RFIFO 存入的帧太多，已满，导致数据丢失 \
                              0：RFIFO 未满*/
#define CAN_DATAAVAIL BIT0  /*1：RFIFO 存有一帧或多帧数据，可以通过 RFIFO 寄存器读取 \
                              0：RFIFO 没有有效帧数据*/
#define CAN_ALLSR 0xff      /*读取全部状态*/

/*记录总线仲裁丢失的具体位置定义*/
typedef enum {
    CAN_LOST_ID0,  /*ID的第一位*/
    CAN_LOST_ID1,  /*ID的第二位*/
    CAN_LOST_ID2,  /*ID的第三位*/
    CAN_LOST_ID3,  /*ID的第四位*/
    CAN_LOST_ID4,  /*ID的第五位*/
    CAN_LOST_ID5,  /*ID的第六位*/
    CAN_LOST_ID6,  /*ID的第七位*/
    CAN_LOST_ID7,  /*ID的第八位*/
    CAN_LOST_ID8,  /*ID的第九位*/
    CAN_LOST_ID9,  /*ID的第十位*/
    CAN_LOST_IDA,  /*ID的第十一位*/
    CAN_LOST_SRTR, /*ID的SRTR位*/
    CAN_LOST_IDE,  /*ID的IDE位*/
} enumLossposition;

/*总线错误类型*/
typedef enum {
    CAN_BITERROR,    /*bit错误*/
    CAN_FORMATERROR, /*格式错误*/
    CAN_FILLERROR,   /*填充错误*/
    CAN_OTHERERROR,  /*其它类型错误*/
} enumErrortype;

/*总线错误发生时刻*/
typedef enum {
    CAN_RECE_PROCESS, /*总线错误发生在发送过程中*/
    CAN_SEND_PROCESS, /*总线错误发生在接收过程中*/
} enumErrortime;

/*总线错误发生的位置定义*/
typedef enum {
    null0,
    null1,
    CAN_ID28_ID21,      /*ID.28 到 ID.21*/
    CAN_START,          /*起始帧*/
    CAN_SRTR,           /*SRTR 位*/
    CAN_IDE,            /*IDE 位*/
    CAN_ID20_ID18,      /*ID.20 到 ID.18*/
    CAN_ID17_ID13,      /*ID.17 到 ID.13*/
    CAN_CRC,            /*CRC 数据段*/
    CAN_KEEP0,          /*保留位，固定为 0*/
    CAN_DATA_FIELD,     /*Data Field*/
    CAN_DATALENGTHCODE, /*Data Length Code*/
    CAN_RTR,            /*RTR 位*/
    CAN_KEEP1,          /*保留位，固定为 1*/
    CAN_ID4_ID0,        /*ID.4 到 ID.0*/
    CAN_ID15_ID12,      /*ID.12 到 ID.5*/
    null2,
    CAN_ACTIVE_ERROR_FLAG, /*active error flag*/
    CAN_INTERMISSION,      /*Intermission*/
    CAN_TOLERATE_DOMINANT, /*Tolerate dominant位*/
    null3,
    null4,
    CAN_PASSIVE_ERROR,      /*passive error flag*/
    CAN_ERROR_DELIMITER,    /*Error delimiter*/
    CAN_CRC_DELIMITER,      /*CRC delimiter*/
    CAN_CONNFIRM,           /*确认位*/
    CAN_FRAMEEND,           /*帧结束*/
    CAN_CONNFIRM_DELIMITER, /*确认 delimiter*/
    CAN_OVERLOAD_FLAG,      /*Overload flag*/
} enumErrposition;

void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct);
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct);
void SFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr, u8 Byte1_acr, u8 Byte1_amr, u8 Byte2_acr, u8 Byte2_amr);
void EFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr);
void ID1_Filter_Dual(u32 acr1, u32 amr1, u8 rtr_acr1, u8 rtr_amr1, u8 Byte_acr1, u8 Byte_amr1, u8 ide);
void ID2_Filter_Dual(u32 acr2, u32 amr2, u8 rtr_acr2, u8 rtr_amr2, u8 ide);
u8 My_CAN_Send_Msg(u32 id, u8 ide, u8 rtr, u8 *msg, u8 len);
u8 CAN_Receive_Msg(u32 *id, u8 *ide, u8 *rtr, u8 *buf);
uint8_t CAN_ReadState(CAN_TypeDef *CANx, uint32_t sr);
uint32_t CAN_GetIRQFlag(void);
void CAN_Sleep(u32 Baud);
void CAN_Manual_Awake(void);

#endif
