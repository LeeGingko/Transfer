/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_can.c
 * 文件标识：
 * 内容摘要： CAN外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： HuangMG
 * 完成日期： 2022年4月29日
 *
 *******************************************************************************/
#include "lks32mc08x_can.h"

/*******************************************************************************
 函数名称：    void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
 功能描述：    CAN初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG              创建
 *******************************************************************************/
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct)
{
    SYS_ModuleClockCmd(SYS_Module_CAN, ENABLE);
    CAN->MOD |= 1 << 0; // 复位模式
    if (CAN_InitStruct->CAN_DMAEn)
    {
        CANx->CMR |= BIT5;
    }
    else
    {
        CANx->CMR &= ~BIT5;
    }
    CANx->EWLR = CAN_InitStruct->CAN_ErrWarThre;
    CANx->BTR0 = CAN_InitStruct->CAN_Btr0;
    CANx->BTR1 = CAN_InitStruct->CAN_Btr1;
    CANx->MOD  = CAN_InitStruct->CAN_WorkMode;
    CANx->EIR  = CAN_InitStruct->IRQEna;
}

/*******************************************************************************
 函数名称：    void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct)
 功能描述：    CAN结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG            创建
 *******************************************************************************/
void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct)
{
    CAN_InitStruct->CAN_DMAEn      = DISABLE;
    CAN_InitStruct->CAN_WorkMode   = CAN_NORMAL_MODE;
    CAN_InitStruct->CAN_ErrWarThre = 120;
    /*波特率选择500K*/
    CAN_InitStruct->CAN_Btr0 = 0x05;
    CAN_InitStruct->CAN_Btr1 = 0x1C;
    CAN_InitStruct->IRQEna   = 0x0;
}

/*******************************************************************************
 函数名称：    void CAN_Sleep(u32 Baud)
 功能描述：    CAN开启休眠函数
 操作的表：    无
 输入参数：    Baud：波特率，单位Kbps
 输出参数：    无
 返 回 值：    无
 其它说明：    休眠唤醒有两种：1、软件清零CAN_MOD[4]，即自动退出休眠模式。，2、一旦检
               测到CAN总线的活动，即自动退出休眠模式。
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG              创建
 *******************************************************************************/
void CAN_Sleep(u32 Baud)
{
    u32 rate = 0, t_cnt = 0;
    CAN_MODE &= ~BIT0;                     // 正常输出模式
    rate = 1152000 / Baud;                 // 此值为主频96MHz进行计算，主频48MHz写入的Baud适当加大
    for (t_cnt = 0; t_cnt < rate; t_cnt++) // 等待11个波特率周期,进入正常工作模式
    {
        __NOP();
    }
    CAN_MODE |= BIT4;
    rate = 1632000 / Baud; // 此值为主频96MHz进行计算，主频48MHz写入的Baud适当加大
    // 等待16个波特率周期，正式进入休眠等待16个波特率周期，正式进入休眠
    for (t_cnt = 0; t_cnt < rate; t_cnt++)
    {
        __NOP();
    }
}

/*******************************************************************************
 函数名称：    void CAN_Manual_Awake(void)
 功能描述：    CAN手动休眠唤醒函数
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG              创建
 *******************************************************************************/
void CAN_Manual_Awake(void)
{
    CAN_MODE &= ~BIT0;
    CAN_MODE &= ~BIT4;
}

/*******************************************************************************
函数名称:  void  SFF_ID_Filter(u32 acr,u32 amr,u8 rtr_acr,u8 rtr_amr,
                           u8 Byte1_acr,u8 Byte1_amr,u8 Byte2_acr,u8 Byte2_amr)
功能描述:  CAN标准帧单ID过滤,采用2.0B协议
操作的表:  无
输入参数:  acr: ID值。
           amr：屏蔽位
           标准帧  11位ID长度
           rtr_acr:   接收（1远程/0标志帧）帧类型选择
           rtr_amr：  远程/数据帧滤波屏蔽位，0表示只接收rtr_acr类型帧，为1表示远程/0标志帧均接收
           Byte1_acr：数据第一个字节
           Byte1_amr：第一个字节屏蔽位
           Byte2_acr：数据第二个字节
           Byte2_amr：第二个字节屏蔽位
输出参数: 无
返回值:   无
其它说明:
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG            创建
*******************************************************************************/
void SFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr, u8 Byte1_acr, u8 Byte1_amr, u8 Byte2_acr, u8 Byte2_amr)
{
    CAN->MOD |= 1 << 0; // 复位模式
    CAN->MOD |= 1 << 3; // 单滤波ID,一个 32 位长的滤波ID

    acr &= 0x7ff;
    amr &= 0x7ff;

    rtr_acr &= 0x1;
    rtr_amr &= 0x1;

    CAN_ACR0 = (u8)(acr >> 3);
    CAN_ACR1 = (u8)(acr << 5) | (rtr_acr << 4);
    CAN_ACR2 = Byte1_acr;
    CAN_ACR3 = Byte2_acr;

    CAN_AMR0 = (u8)(amr >> 3);
    CAN_AMR1 = (u8)(amr << 5) | (rtr_amr << 4) | 0x0f;
    CAN_AMR2 = Byte1_amr;
    CAN_AMR3 = Byte2_amr;

    CAN->MOD &= ~(1 << 0); // 正常输出模式
}
/*******************************************************************************
函数名称:  void EFF_ID_Filter(u32 acr,u32 amr,u8 rtr_acr,u8 rtr_amr)
功能描述:  CAN扩展帧单ID过滤,采用2.0B协议
操作的表:  无
输入参数:  acr：ID值。
           amr：屏蔽位

           rtr_acr： 接收（1远程/0数据帧）帧类型选择
           rtr_amr： 远程/数据帧滤波屏蔽位，0表示只接收rtr_acr类型帧，为1表示远程/0标志帧均接收

           ide：0标准/1扩展帧
输出参数:  无
返回值:    无
其它说明:
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG            创建
*******************************************************************************/
void EFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr)
{
    CAN->MOD |= 1 << 0; // 复位模式
    CAN->MOD |= 1 << 3; // 单滤波ID,一个 32 位长的滤波ID

    acr &= 0x1fffffff;
    amr &= 0x1fffffff;
    rtr_acr &= 0x1;
    rtr_amr &= 0x1;

    /*可接收的ID寄存器，当输入帧的 ID 同此匹配，则被 CAN 模块接受
      配合 AMR 寄存器，可同一类 ID 匹配。*/
    CAN_ACR3 = (u8)(acr << 3) | (rtr_acr << 2);
    CAN_ACR2 = (u8)(acr >> 5);
    CAN_ACR1 = (u8)(acr >> 13);
    CAN_ACR0 = (u8)(acr >> 21);
    /*配合 ACR 寄存器，同输入 ID 进行匹配。AMR 寄存器某一位为 0，标识
   ID 对应位需要同 ACR 匹配；为 1，标识 ID 对应位不需要同 ACR 匹配*/
    CAN_AMR3 = (u8)((amr << 3) | (rtr_amr << 2) | 0x03);
    CAN_AMR2 = (u8)(amr >> 5);
    CAN_AMR1 = (u8)(amr >> 13);
    CAN_AMR0 = (u8)(amr >> 21);

    CAN->MOD &= ~(1 << 0); // 正常输出模式
}

/*******************************************************************************
函数名称:  void  ID1_Filter_Dual(u32 acr1 ,u32 amr1,u8 rtr_acr1,
                               u8 rtr_amr1,u8 Byte_acr1,u8 Byte_amr1,u8 ide)

功能描述: CAN双ID过滤中ID1滤波设置 采用2.0B协议
操作的表: 无
输入参数: acr1：ID1值（范围1D28~ID13共ID高16位有效）   amr1：ID1屏蔽位  ide：0标准/1扩展帧

              rtr_acr：  接收（1远程/0数据帧）帧类型选择（只支持SFF模式）
              rtr_amr：  远程/标志帧滤波屏蔽位，0表示只接收rtr_acr类型帧，为1表示远程/0标志
                    帧均接收（只支持SFF模式）

              Byte_acr1：数据帧第1个Byte低4位滤波选择（只支持SFF模式）
              Byte_amr1：数据帧第1个Byte低4位屏蔽位（只支持SFF模式）
输出参数: 无
返回值:   0：发送成功 !0：发送失败
多任务访问:
其它说明: 扩展帧时，只对ID的bit13- bit28 进行筛选过滤
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG            创建
*******************************************************************************/
void ID1_Filter_Dual(u32 acr1, u32 amr1, u8 rtr_acr1, u8 rtr_amr1, u8 Byte_acr1, u8 Byte_amr1, u8 ide)
{
    CAN->MOD |= 1 << 0;    // RESET MODE
    CAN->MOD &= ~(1 << 3); // 双滤波ID
    rtr_acr1 &= 0x1;
    rtr_amr1 &= 0x1;

    if (ide) /*扩展帧*/
    {
        acr1 &= 0x1fffffff;
        CAN_ACR1 = (u8)(acr1 >> 13);
        CAN_ACR0 = (u8)(acr1 >> 21);

        CAN_AMR1 = (u8)(amr1 >> 13);
        CAN_AMR0 = (u8)(amr1 >> 21);
    }
    else /*标准帧*/
    {
        acr1 &= 0x7ff;
        CAN_ACR0 = (u8)(acr1 >> 3);
        CAN_ACR1 = (u8)(acr1 << 5) | (rtr_acr1 << 4) | (Byte_acr1 >> 4);
        CAN_ACR3 &= 0xF0;
        CAN_ACR3 |= (u8)(Byte_acr1 & 0x0F);

        CAN_AMR0 = (u8)(amr1 >> 3);
        CAN_AMR1 = (u8)(amr1 << 5) | (rtr_amr1 << 4) | (Byte_amr1 >> 4);
        CAN_AMR3 &= 0xF0;
        CAN_AMR3 |= (u8)(Byte_amr1 & 0x0F);
    }
    CAN->MOD &= ~(1 << 0); // 正常输出模式
}

/*******************************************************************************
函数名称:  void ID2_Filter_Dual(u32 acr2 ,u32 amr2,u8 rtr_acr2,u8 rtr_amr2 ,u8 ide)
功能描述:  CAN双ID过滤中ID2滤波设置 采用2.0B协议
操作的表:  无
输入参数:  acr2：ID2值（范围1D28~ID13共ID高16位有效）   amr2：ID2屏蔽位  ide：0标准/1扩展帧

          rtr_acr2: 接收（1远程/0数据帧）帧类型选择（只支持SFF标准帧模式）
          rtr_amr2：远程/标志帧滤波屏蔽位，0表示只接收rtr_acr类型帧，为1表示远程/0标志
                    帧均接收（只支持SFF标准帧模式）
输出参数: 无
返回值:   0：发送成功 !0：发送失败
多任务访问:
其它说明: 扩展帧时，只对ID的bit13- bit28 进行筛选过滤
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/1/16       V1.0         HuangMG            创建
*******************************************************************************/
void ID2_Filter_Dual(u32 acr2, u32 amr2, u8 rtr_acr2, u8 rtr_amr2, u8 ide)
{
    CAN->MOD |= 1 << 0;    // RESET MODE
    CAN->MOD &= ~(1 << 3); // 双滤波ID
    rtr_acr2 &= 0x1;
    rtr_amr2 &= 0x1;
    if (ide) // externed
    {
        acr2 &= 0x1fffffff;
        CAN_ACR3 = (u8)(acr2 >> 13);
        CAN_ACR2 = (u8)(acr2 >> 21);

        CAN_AMR3 = (u8)(amr2 >> 13);
        CAN_AMR2 = (u8)(amr2 >> 21);
    }
    else
    {
        acr2 &= 0x7ff;
        CAN_ACR3 &= 0x0f;
        CAN_ACR2 = (u8)(acr2 >> 3);
        CAN_ACR3 |= (u8)(acr2 << 5) | (rtr_acr2 << 4);

        CAN_AMR3 &= 0x0f;
        CAN_AMR2 = (u8)(amr2 >> 3);
        CAN_AMR3 |= (u8)(amr2 << 5) | (rtr_amr2 >> 4);
    }
    CAN->MOD &= ~(1 << 0); // 正常输出模式
}

/*******************************************************************************
函数名称: u8 My_CAN_Send_Msg(u32 id,u8 ide,u8 rtr,const u8 *msg,u8 len)
功能描述: CAN轮询发送数据程序  采用2.0B协议
操作的表: 无
输入参数: id: 发送id ide: 标准/扩展帧 rtr：远程帧/数据帧 *msg: 发送的数据  len:发送字节长度
输出参数: 无
返回值:   0：发送成功 !0：发送失败
多任务访问:
其它说明:  id:标准ID(11位)/扩展ID(11位+18位)
          ide:0,标准帧;1,扩展帧
          rtr:0,数据帧;1,远程帧
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
2018/08/10     V1.0            ZKW               创建
*******************************************************************************/
u8 My_CAN_Send_Msg(u32 id, u8 ide, u8 rtr, u8 *msg, u8 len)
{
    u8  frame_inf = 0;
    u16 i         = 0;

    if (CAN->SR & 0X04) // TFIFO 空
    {
        CAN->MOD &= ~(1 << 0);  // 正常输出模式
        frame_inf |= ide << 7;  // 0标准/1扩展帧
        frame_inf |= rtr << 6;  // 0,数据帧;1,遥控帧
        frame_inf |= len << 0;  // 发送数据长度
        CAN->TXRX0 = frame_inf; // 发送TX帧信息
        if (0 == ide)
        { // 标准帧
            id &= 0x7ff;
            // 发送接收寄存器1
            CAN->TXRX1 = id >> 3;          // TX ID0
            CAN->TXRX2 = (id & 0X07) << 5; // TX ID1
            CAN->TXRX3 = msg[0];           // TX DATA0
            CAN->TXRX4 = msg[1];           // TX DATA1
            CAN->TXRX5 = msg[2];           // TX DATA2
            CAN->TXRX6 = msg[3];           // TX DATA3
            CAN->TXRX7 = msg[4];           // TX DATA4
            CAN->TXRX8 = msg[5];           // TX DATA5
            CAN->TXRX9 = msg[6];           // TX DATA6
            CAN->TXRXA = msg[7];           // TX DATA7
        }
        else
        { // 扩展帧
            id &= 0X1FFFFFFF;
            CAN->TXRX1 = (u8)(id >> 21);          // TX ID0
            CAN->TXRX2 = (u8)((id >> 13) & 0xFF); // TX ID1
            CAN->TXRX3 = (u8)((id >> 5) & 0xFF);  // TX ID2
            CAN->TXRX4 = (u8)((id & 0xff) << 3);  // TX ID3
            CAN->TXRX5 = msg[0];                  // TX DATA0
            CAN->TXRX6 = msg[1];                  // TX DATA1
            CAN->TXRX7 = msg[2];                  // TX DATA2
            CAN->TXRX8 = msg[3];                  // TX DATA3
            CAN->TXRX9 = msg[4];                  // TX DATA4
            CAN->TXRXA = msg[5];                  // TX DATA5
            CAN->TXRXB = msg[6];                  // TX DATA6
            CAN->TXRXC = msg[7];                  // TX DATA7
        }
        if ((CAN->MOD) & CAN_SELFTEST_MODE) /*CAN工作在自测模式*/
        {
            CAN->CMR |= 1 << 4; // CAN发送数据的同时也将数据接收回来
        }
        else
        {
            CAN->CMR |= 1 << 0; // CAN发送传输请求
        }
        while (!(CAN->SR & 0X08)) // 最近一次是否传输完成
        {
            if ((i++) >= 0xfff)
            {
                return 0xff; // 发送失败
            }
        };
        return 0; // 发送完成
    }
    else // TFIFO非空
    {
        return 0xff; // 发送失败
    }
}

/*******************************************************************************
 函数名称: u8 CAN_Receive_Msg(uint32_t* id,u8* ide,u8* rtr,u8 *buf)
 功能描述: CAN轮询接收数据程序
 操作的表: 无
 输入参数: id: 发送id ide: 标准/扩展帧 rtr：数据帧/远程帧 *msg: 发送的数据  len:发送字节长度
 输出参数: 无
 返回值:   0：接收成功 !0：接收失败
 多任务访问:
 其它说明: id:标准ID(11位)/扩展ID(11位+18位)
          ide:0,标准帧;1,扩展帧
          rtr:0,数据帧;1,远程帧
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
2018/08/10     V1.0            ZKW               创建
*******************************************************************************/
u8 CAN_Receive_Msg(uint32_t *id, u8 *ide, u8 *rtr, u8 *buf)
{
    u8 reg_inf;

    if (CAN->SR & 0X01) // RFIFO 存有一帧或多帧数据，可以通过 RFIFO 寄存器读取
    {
        CAN->MOD &= ~(1 << 0); // 正常输出模式
        reg_inf = CAN->TXRX0;  // 读取的一帧或多帧数据
        if (reg_inf & 0x80)    // EFF（扩展）帧
        {
            *ide = 1;
            *id  = (CAN->TXRX1 << 21) | (CAN->TXRX2 << 13) | (CAN->TXRX3 << 5) | (CAN->TXRX4 >> 3); // 获取ID
            if (reg_inf & 0x40)
            {
                *rtr = 1; // 遥控帧
            }
            else
            {
                *rtr = 0; // 数据帧
            }
            buf[0] = CAN->TXRX5; // 获取数据
            buf[1] = CAN->TXRX6;
            buf[2] = CAN->TXRX7;
            buf[3] = CAN->TXRX8;
            buf[4] = CAN->TXRX9;
            buf[5] = CAN->TXRXA;
            buf[6] = CAN->TXRXB;
            buf[7] = CAN->TXRXC;
        }
        else // SFF（标准）帧
        {
            *ide = 0;
            if (reg_inf & 0x40)
            {
                *rtr = 1;
            }
            else
            {
                *rtr = 0;
            }

            *id    = ((CAN->TXRX1 << 3) | (CAN->TXRX2 >> 5));
            buf[0] = CAN->TXRX3;
            buf[1] = CAN->TXRX4;
            buf[2] = CAN->TXRX5;
            buf[3] = CAN->TXRX6;
            buf[4] = CAN->TXRX7;
            buf[5] = CAN->TXRX8;
            buf[6] = CAN->TXRX9;
            buf[7] = CAN->TXRXA;
        }

        CAN->CMR |= 0x04; // 释放接收缓冲区

        return 0; // 接收完成
    }             // fifo is empty
    else
    {
        return 0xff; // 接收异常
    }
}

/*******************************************************************************
 函数名称：    uint8_t CAN_ReadState(CAN_TypeDef* CANx , uint32_t sr)
 功能描述：    获取CAN状态
 操作的表：    无
 输入参数：    sr:读取指定状态或者所有状态结果
 输出参数：    无
 返 回 值：    返回指定状态结果或者所有状态结果
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
uint8_t CAN_ReadState(CAN_TypeDef *CANx, uint32_t sr)
{
    return (uint8_t)(CANx->SR & sr);
}

/*******************************************************************************
 函数名称：    enumLossposition CAN_ReadALC(void)
 功能描述：    获取仲裁丢失的具体位置
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    返回仲裁丢失具体位置
 其它说明：    无

 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
enumLossposition CAN_ReadALC(void)
{
    return (enumLossposition)CAN_ALC;
}

/*******************************************************************************
 函数名称：    uint8_t CAN_ReadRXERRCNT(void)
 功能描述：    获取接收错误计数值
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    返回接收错误计数值
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
uint8_t CAN_ReadRXERRCNT(void)
{
    return CAN_RXERR;
}

/*******************************************************************************
 函数名称：    uint8_t CAN_ReadTXERRCNT(void)
 功能描述：    获取发送错误计数值
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    返回发送错误计数值
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
uint8_t CAN_ReadTXERRCNT(void)
{
    return CAN_TXERR;
}

/*******************************************************************************
 函数名称：    void CAN_ReceFramInfor(uint8_t* cnt , uint8_t* rbsa )
 功能描述：    获取接收缓冲区有效接收帧数与第一帧有效接收数据在 RFIFO的位置
 操作的表：    无
 输入参数：    cnt：有效帧个数
               rbsa：RFIFO 的大小为 32 字节，rbsa存储第一帧有效接收数据在 RFIFO的位置0-31
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
void CAN_ReceFramInfor(uint8_t *cnt, uint8_t *rbsa)
{
    *cnt  = CAN_RMC;
    *rbsa = CAN_RBSA;
}

/*******************************************************************************
 函数名称：    void CAN_ReadErrorCode(enumErrortype* errtype , enumErrortime* errtim
                                      , enumErrposition* errpos)
 功能描述：    获取总线错误类型和错误位置
 操作的表：    无
 输入参数：    errtype：总线错误类型
               errtim： 总线错误发生时刻
               errpos： 错误发生的位置
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/29       V1.0         HuangMG            创建
 *******************************************************************************/
void CAN_ReadErrorCode(enumErrortype *errtype, enumErrortime *errtim, enumErrposition *errpos)
{
    *errtype = (enumErrortype)((CAN_ECC & 0xC0) >> 6);
    *errtim  = (enumErrortime)((CAN_ECC & 0x20) >> 5);
    *errpos  = (enumErrposition)(CAN_ECC & 0x1f);
}

/*******************************************************************************
 函数名称：    uint32_t CAN_GetIRQFlag(void)
 功能描述：    获取中断标志与清除中断标志位
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    TIM中断标志
 其它说明：    CAN中断标志位为读清除
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            创建
 *******************************************************************************/
uint32_t CAN_GetIRQFlag(void)
{
    return CAN_INTR;
}
