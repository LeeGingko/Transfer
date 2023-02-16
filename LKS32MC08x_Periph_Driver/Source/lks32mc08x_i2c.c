/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc08x_i2c.c
 * 文件标识：
 * 内容摘要： I2C外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者：
 * 完成日期：
 *
 * 修改记录1：
 * 修改日期：
 * 版 本 号：V 1.0
 * 修 改 人：
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/
#include "lks32mc08x_i2c.h"

static volatile u8 i2c_dma_state = 0;   // I2C发送状态
/*******************************************************************************
 函数名称：    void I2C_Init(I2C_TypeDef* I2Cx, EI2C_InitTypeDef* I2C_InitStruct)
 功能描述：    I2C初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{

    SYS_ModuleClockCmd(SYS_Module_I2C, ENABLE);
    SYS_SoftResetModule(SYS_Module_I2C);

    {
        u32 BaudRate;
        BaudRate = ((u64)96000000+((I2C_InitStruct->BaudRate*17)>>1)) / (I2C_InitStruct->BaudRate*17) - 1;
        SYS_WR_PROTECT = 0x7A83;    //解除系统寄存器写保护
        SYS_CLK_DIV0 = BaudRate;
        SYS_WR_PROTECT = 0;    //解除系统寄存器写保护
    }
    
    I2Cx->ADDR =   I2C_InitStruct->ADRCMP << 7;

    I2Cx->CFG  =     (I2C_InitStruct->IE            <<  7 ) | (I2C_InitStruct->TC_IE    << 6)
                   | (I2C_InitStruct->BUS_ERR_IE    <<  5 ) | (I2C_InitStruct->STOP_IE  << 4)
                   | (I2C_InitStruct->MST_MODE      <<  1 ) | (I2C_InitStruct->SLV_MODE << 0);

    I2Cx->SCR  =   0;

    I2Cx->MSCR =   0;

    I2Cx->BCR  =   I2C_InitStruct->BURST_NACK << 7  | (I2C_InitStruct->BURST_ADDR_CMP  << 6)
                   | (I2C_InitStruct->DMA     << 5);
    if(I2C_InitStruct->DMA)
    {
        i2c_dma_init();
    }

}


/*******************************************************************************
 函数名称：    void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
 功能描述：    I2C结构体初始化
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
    I2C_InitStruct->ADRCMP           =   DISABLE    ;   //  I2C 硬件地址比较使能开关，只有在 DMA 模式下开启才有效。
    I2C_InitStruct->MST_MODE         =   DISABLE    ;   //  I2C 主模式使能
    I2C_InitStruct->SLV_MODE         =   DISABLE    ;   //  I2C 从模式使能
    I2C_InitStruct->DMA              =   DISABLE    ;   //  I2C DMA传输使能
    I2C_InitStruct->BaudRate         =   100000     ;   //  I2C 波特率
    I2C_InitStruct->IE               =   DISABLE    ;   //  I2C 中断使能
    I2C_InitStruct->TC_IE            =   DISABLE    ;   //  I2C 数据传输完成中断使能
    I2C_InitStruct->BUS_ERR_IE       =   DISABLE    ;   //  I2C 总线错误事件中断使能
    I2C_InitStruct->STOP_IE          =   DISABLE    ;   //  I2C STOP 事件中断使能
    I2C_InitStruct->BURST_NACK       =   DISABLE    ;   //  I2C 传输，NACK 事件中断使能
    I2C_InitStruct->BURST_ADDR_CMP   =   DISABLE    ;   //  I2C 传输，硬件地址匹配中断使能
}

/*******************************************************************************
 函数名称：    void I2C_SENDDATA(I2C_TypeDef *I2Cx, uint32_t n)
 功能描述：    I2C发送数据
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t n)
{
    I2Cx->DATA = n;
}

/*******************************************************************************
 函数名称：    uint32_t I2C_ReadData(I2C_TypeDef *I2Cx)
 功能描述：    I2C读缓冲区数据
 操作的表：    无
 输入参数：    I2C_TypeDef *I2Cx
 输出参数：    无
 返 回 值：    缓冲区数据
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21     V1.0           cfwu              创建
 *******************************************************************************/
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx)
{
    return I2Cx->DATA;
}



/*******************************************************************************
 函数名称：    static void i2c_dma_delay_over()
 功能描述：    I2C发送接收等待函数
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
static void i2c_dma_delay_over()
{
	u32 t = 100000000;
	while (i2c_dma_state)       // 等待I2C发送完成
	{
		t--;
		if(t==0)
		{// 避免程序卡死
			return;
		}
	}
    i2c_dma_state = 0;
    while(I2C0_MSCR & BIT3)     // 等待I2C总线空闲
    {
		t--;
		if(t==0)
		{// 避免程序卡死
			return;
		}
    }
}

void i2c_dma_state_over()  // I2C发送完成
{
    i2c_dma_state = 0;
}

static void i2c_dma_state_start() // I2C开始发送
{
    i2c_dma_state = 1;
}

/*******************************************************************************
 函数名称：    void i2c_dma_tx(u8 addr, u8 *data, u8 len)
 功能描述：    I2C数据发送函数
 操作的表：    无
 输入参数：    addr：IIC发送地址
              data：发送数据缓冲器
              len：发送数据长度
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0         cfwu               创建
 *******************************************************************************/
void i2c_dma_tx(u8 addr, u8 *data, u8 len)
{
	len = (len)?len:1;
	len = (len>16)?16:len;	//输入限制，1-16
	i2c_dma_delay_over();								  //等待I2C完成
	I2C->ADDR = addr & 0x7f;					    // 从设备地址
  I2C->BCR = (I2C->BCR &0xf0)|((len-1)&0xf); // 设定数据长度
	I2C->SCR |= BIT2;							        // I2C传输方向
  I2C->DATA = 0;
	DMA_CH3->DMA_CCR &=~BIT0;
	DMA_CH3->DMA_CMAR = (u32)data;			  // DMA发送数据地址
	DMA_CH3->DMA_CTMS = ((len+1)<<16)|1;  // DMA搬运的数据长度
	DMA_CH3->DMA_CCR |= BIT4;
  DMA_CH3->DMA_CCR |= BIT0;				      // 配置DMA传输方向并使能DMA
  i2c_dma_state_start();
	I2C->MSCR = 1;								        // 触发I2C发送数据
    
}
/*******************************************************************************
 函数名称：    void i2c_dma_rx(u8 addr, u8 *data, u8 len)
 功能描述：    I2C数据接收函数
 操作的表：    无
 输入参数：    addr：IIC接收地址
              data：发送接收缓冲器
              len：发送接收长度
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2016/05/21      V1.0           cfwu             创建
 *******************************************************************************/
void i2c_dma_rx(u8 addr, u8 *data, u8 len)
{
	len = (len)?len:1;
	len = (len>16)?16:len;	            //输入限制，1-16
	i2c_dma_delay_over();							  //等待I2C完成
	DMA_CH3->DMA_CCR &=~BIT0;
	DMA_CH3->DMA_CMAR = (u32)data;			// DMA发送数据地址
	DMA_CH3->DMA_CTMS = ((len)<<16)|1;  // DMA搬运的数据长度
	DMA_CH3->DMA_CCR &= ~BIT4;				  // DMA传输方向
	DMA_CH3->DMA_CCR |= BIT0;					  // 使能DMA
	I2C->ADDR = addr & 0x7f;					  // 从设备地址
  I2C->BCR = (I2C->BCR &0xf0)|((len-1)&0xf);    // 设定数据长度
	I2C->SCR |= BIT4;							      // 收到数据后进行ACK回应
	I2C->SCR &= ~BIT2;							    // I2C传输方向
  i2c_dma_state_start();
	I2C->MSCR = 1;								      // 触发I2C发送数据
}
/*******************************************************************************
 函数名称：    void DMA_init(void)
 功能描述：    DMA硬件初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void i2c_dma_init(void)
{
    DMA_InitTypeDef DMA_InitStruct;
	
    DMA_InitStruct.DMA_IRQ_EN = DISABLE;       /* DMA 中断使能 */
    DMA_InitStruct.DMA_DIR = 1;                /* DMA传输方向 0:外设至内存， 1:内存至外设 */
    DMA_InitStruct.DMA_CIRC = DISABLE;         /* DMA传输模式：循环模式，高有效 */
    DMA_InitStruct.DMA_PINC = DISABLE;         /* 外设地址每轮内是否递增,高有效 */
    DMA_InitStruct.DMA_MINC = ENABLE;          /* 内存地址第二轮是否在第一轮地址的基础上递增,高有效 */
    DMA_InitStruct.DMA_PBTW = 0;               /* 外设访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW = 0;               /* 内存访问位宽， 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_I2C_REQ_EN;/* 通道 x 三个硬件 DMA 请求使能，高有效 */
    DMA_InitStruct.DMA_TIMES = 1;              /* DMA 通道 x 每轮数据搬运次数 1~511 */
    DMA_InitStruct.DMA_ROUND = 0;              /* DMA 通道 x 采样轮数 1~255 */
    DMA_InitStruct.DMA_CPAR = (u32)&I2C0_DATA; /* DMA 通道 x 外设地址 */
    DMA_InitStruct.DMA_CMAR = 0;               /* DMA 通道 x 内存地址 */
    DMA_Init(DMA_CH3, &DMA_InitStruct); 
    DMA_IF=0xff;
}




