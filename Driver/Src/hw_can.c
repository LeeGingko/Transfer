/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_can.h"
#include "lks32mc08x_DMA.h"
#include "hw_can.h"
/* USER INCLUDE FILES END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */

/* USER DEFINED VARIABLES END */
u8 CAN_Rx[55] = {0};
/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*--------------------------------------------------------------------------------------------*
 函数名称：    void HW_CAN_GPIO_init(void)
 功能描述：    CAN GPIO硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *--------------------------------------------------------------------------------------------*/

void HW_CAN_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct); // 初始化结构体

    // 配置LED1：P0.6  LED2 P0.7 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO输出模式
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    /*P0.7 CAN TX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    /*P0.6 CAN RX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_6, AF10_CAN);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_7, AF10_CAN);
}

/*--------------------------------------------------------------------------------------------*
 函数名称: void HW_CAN_init(void)
 功能描述: CAN初始化函数,采用2.0B协议
 操作的表:  Can波特率      BTR0    	BTR1
            1Mbps 		  0x05      0x14
            800Kbps 	  0x05      0x16
            666Kbps 	  0x85 	    0xB6
            500Kbps 	  0x05      0x1C
            400Kbps 	  0x05	    0xFA
            250Kbps 	  0x0B 	    0x1C
            200Kbps 	  0x05 	    0xFA
            125Kbps 	  0x17      0x1C
            100Kbps 	  0x1D      0x1C
            83.33Kbps 	0x17      0x6F
            80Kbps 		  0x97      0xFF
            66.66Kbps 	0x1D      0x6F
            50Kbps 		  0x3B      0x1C
            40Kbps 		  0xAF      0xFF
 输入参数: 无
 输出参数: 无
 返回值:   无
 其它说明:
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/5/3      V1.0           HuangMG            创建
*--------------------------------------------------------------------------------------------*/
void HW_CAN_Init(void)
{
    CAN_InitTypeDef CAN_InitStruct;

    /*Can波特率:500Kbps*/
    CAN_StructInit(&CAN_InitStruct);
    CAN_InitStruct.CAN_Btr0       = 0x05;
    CAN_InitStruct.CAN_Btr1       = 0x1C;
    CAN_InitStruct.CAN_DMAEn      = ENABLE;            /*DMA搬运数据*/
    CAN_InitStruct.CAN_WorkMode   = CAN_SELFTEST_MODE; /*CAN正常收发工作模式*/
    CAN_InitStruct.CAN_ErrWarThre = 50;                /*错误与警告门限值为50*/
    CAN_InitStruct.IRQEna         = CAN_IE_RFIFONOEMPTY | CAN_IE_TXDONE | CAN_IE_WERR;
    CAN_Init(CAN, &CAN_InitStruct);
    /*接收滤波： ID:0x5A ,标志帧，BYTE1:0x81,BYTE:0x23*/
    SFF_ID_Filter(0X2F, 0x00, 0x0, 0x0, 0x81, 0x00, 0x23, 0x00);
    HW_CAN_GPIO_Init();
    HW_CAN_DMA_RX(0, 2, 8, CAN_Rx);
}

/*--------------------------------------------------------------------------------------------*
 函数名称：    void CAN_DMA_TX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
 功能描述：    CAN_DMA数据发送函数
 输入参数：    ide：0标准/1扩展帧
              frame_number：需要发送的帧个数
              frame_size：每帧发送数据长度（0-8 BYTE）数据，不含ID等。
              memaddr：发送数据缓冲区地址
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/5/4      V1.0           HuangMG            创建
 *--------------------------------------------------------------------------------------------*/
void HW_CAN_DMA_TX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr)
{
    u8 len = 0;
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    if (ide) /*扩展帧*/
    {
        len = 5 + frame_size;
    } else { /*标准帧*/
        len = 3 + frame_size;
    }
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;           /* DMA 传输完成中断使能 */
    DMA_InitStruct.DMA_DIR    = MEMORY2PERI;        /* 内存至外设 */
    DMA_InitStruct.DMA_CIRC   = DISABLE;            /* 关闭循环传输模式 */
    DMA_InitStruct.DMA_PINC   = ENABLE;             /* 外设地址每轮内是否递增,高有效 */
    DMA_InitStruct.DMA_MINC   = ENABLE;             /* 内存地址第二轮是否在第一轮地址的基础上递增（轮内地址一定递增）,高有效 */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;     /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;     /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN; /* CAN DMA请求使能，高有效 */
    DMA_InitStruct.DMA_TIMES  = len;                /* DMA 通道 x 每轮数据搬运次数 1~511 */
    DMA_InitStruct.DMA_ROUND  = frame_number;       /* DMA 通道 x 采样轮数 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&CAN_TXRX0;    /* DMA 通道 x 外设地址 */
    DMA_InitStruct.DMA_CMAR   = (u32)memaddr;       /* DMA 通道 x 内存地址 */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    DMA_CHx_EN(DMA_CH3, ENABLE); /*使能DMA_CH3通道*/
}

/*--------------------------------------------------------------------------------------------*
 函数名称：    void CAN_DMA_RX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
 功能描述：    CAN_DMA数据接收函数
 输入参数：    ide：0标准/1扩展帧
              frame_number：需要接收的帧个数
              frame_size：每帧接收数据长度（0-8 BYTE）数据，不含ID等。
              memaddr：接收数据缓冲区地址
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2022/5/4      V1.0           HuangMG            创建
 *--------------------------------------------------------------------------------------------*/
void HW_CAN_DMA_RX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr)
{
    u8 len = 0, rgsr = 0;
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    if (ide) /*扩展帧*/
    {
        len = 5 + frame_size;
    } else { /*标准帧*/
        len = 3 + frame_size;
    }
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;           /* DMA 传输完成中断使能 */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;        /* 外设至内存 */
    DMA_InitStruct.DMA_CIRC   = DISABLE;            /* DMA传输模式：循环模式，高有效 */
    DMA_InitStruct.DMA_PINC   = ENABLE;             /* 外设地址每轮内是否递增,高有效 */
    DMA_InitStruct.DMA_MINC   = ENABLE;             /* 内存地址第二轮是否在第一轮地址的基础上递增（轮内地址一定递增）,高有效 */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;     /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;     /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN; /* CAN DMA请求使能，高有效 */
    DMA_InitStruct.DMA_TIMES  = len;                /* DMA 通道 x 每轮数据搬运次数 1~511 */
    DMA_InitStruct.DMA_ROUND  = frame_number;       /* DMA 通道 x 采样轮数 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&CAN_TXRX0;    /* DMA 通道 x 外设地址 */
    DMA_InitStruct.DMA_CMAR   = (u32)memaddr;       /* DMA 通道 x 内存地址 */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    /*******************清除触发DMA数据搬运标志位，防止误接收数据触发DMA搬运*****************/
    rgsr = CAN_GetIRQFlag(); /*读取中断状态寄存器*/
    rgsr = rgsr;             /*防止编译报警告*/
    /*--------------------------------------------------------------------------------------------*/
    DMA_CHx_EN(DMA_CH3, ENABLE); /*使能DMA_CH3通道*/
}

/* USER IMPLEMENTED FUNCTIONS END */
