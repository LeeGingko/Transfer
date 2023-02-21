 
#include "test_spi.h"
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_spi.h"
#include "lks32mc08x_sys.h"
#include "hardware_config.h"

UINT8 data_tbl[64]=
{
    0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
    0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
    0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
    0x55,0xaa,0x80,0x01,0xff,0x00,0x00,0xff,
    0x0f,0xf0,0x55,0xaa,0x80,0x01,0xff,0x00,
    0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
    0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
    0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0 
};

UINT8 data_receive[64]=
{
    0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
    0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
    0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
    0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
    0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
    0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
    0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
    0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f
};

void spi_md0_master_dma()
{

    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;
    register UINT32 i = 0;
    UINT8 data_receive_init[64]=
    {
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f
    };

    REG32(0x20001000) = 0x00000000;

    SYS_WR_PROTECT    = PSW_PROT;
    SYS_CLK_CFG  = 0x01FF;
    SYS_CLK_FEN  = 0x01FF; // clock enable
    SYS_CLK_DIV0 = 0x0000; // spi clock div

    NVIC_EnableIRQ(SPI0_IRQn); //enable the SPI interrupt
    __enable_irq();           //clr PRIMASK
    SCB->SCR &= ~0x00000010;  //clr SEVONPEND

    //DMA SPI2RAM
    DMA_CTMS2       = 0x00200001;//Send 32 times, one byte per time
    DMA_CPAR2       = 0x4001000C;//SPI_TX_DATA
    DMA_CMAR2       = ( UINT32 )data_tbl;//data_receive;
    // enable chn2 at last, or chn regs cannot be written
    // MINC = 0, PINC = 0
    DMA_CCR2        = 0x2091;

    //DMA RAM2SPI
    DMA_CTMS1       = 0x00200001;//Send 32 times, one byte per time
    DMA_CPAR1       = 0x40010010;//SPI_RX_DATA
    DMA_CMAR1       = ( UINT32 )data_tbl;
    // enable chn1 at last, or chn regs cannot be written
    // MINC = 0, PINC = 0
    DMA_CCR1        = 0x1083;

    DMA_CTRL        = 0x0001;//enable dma, mcu has higher priority

//  //Release Outside SPI Slave, Only This Test Use
//  GPIO_StructInit(&GPIO_InitStruct);
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
//  GPIO_Init(GPIO2,&GPIO_InitStruct);
//  GPIO_Write(GPIO2, BIT0);

    //Set SPI GPIO
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIO0,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_14,AF5_SPI);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_15,AF5_SPI);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_0,AF5_SPI);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_1,AF5_SPI);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.Mode       = SPI_Master;
    SPI_InitStruct.EN         = 1;
    SPI_InitStruct.IRQEna     = SPI_IRQEna_Enable | SPI_IRQEna_TranDone | SPI_IRQEna_SSErr | SPI_IRQEna_DataOver;
    SPI_InitStruct.ByteLength = 0x20;
    SPI_Init(SPI,&SPI_InitStruct);
    SPI->TX_DATA              = 0x10;//Trig Transfer, Write Data can be any value.

    while(1){if (REG32(0x20001000) == 0x00112233)break;}

    for (i = 0x0; i < 32; i++){
      if (data_tbl[i] != data_receive_init[i])NOTOK;//only flag
    }
    OK;//only flag
  //next data is ready, config dma again & SPI->TX_DATA = any value -> next transfer
}

void spi_md0_slave_dma()
{

    register UINT32 i;
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;

    // DO NOT use keyword "const" in array declaration 
    // otherwise array will be stored in flash instead of SRAM
    // DMA cannot access data in flash
    // It seems the global array is not properly initialized during declaration
    // so we manually initialize them below
    UINT8 data_receive_init[64]=
    {
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f
    };
    //inter spi (DUT) transmit data table  MISO
    UINT8 data_tbl_init[64]=
    {
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
        0x55,0xaa,0x80,0x01,0xff,0x00,0x00,0xff,
        0x0f,0xf0,0x55,0xaa,0x80,0x01,0xff,0x00,
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
    };

    REG32(0x20001000) = 0x00000000;

    SYS_WR_PROTECT    = PSW_PROT;
    SYS_CLK_CFG  = 0x01FF;
    SYS_CLK_FEN  = 0x01FF; // clock enable
    SYS_CLK_DIV0 = 0x0000; // spi clock div

    for(i=0; i<64; i++){
        data_tbl[i]     = data_tbl_init[i];
        data_receive[i] = data_receive_init[i];
    }

    //Init DMA
    NVIC_EnableIRQ(SPI0_IRQn); //enable the SPI interrupt
    __enable_irq();           //clr PRIMASK
    SCB->SCR &= ~0x00000010;  //clr SEVONPEND
    
    //DMA SPI2RAM
    DMA_CTMS2       = 0x00080001;//Send 8 times, one byte per time
    DMA_CPAR2       = 0x4001000C;//SPI_TX_DATA
    DMA_CMAR2       = ( UINT32 )data_tbl;
    // enable chn2 at last, or chn regs cannot be written
    // MINC = 0, PINC = 0
    DMA_CCR2        = 0x2091;
    
    //DMA RAM2SPI
    DMA_CTMS1       = 0x00080001;//Send 8 times, one byte per time
    DMA_CPAR1       = 0x40010010;//SPI_RX_DATA
    DMA_CMAR1       = ( UINT32 )data_tbl;
    // enable chn1 at last, or chn regs cannot be written
    // MINC = 0, PINC = 0
    DMA_CCR1        = 0x1083;
    
    DMA_CTRL        = 0x0001;//enable dma, mcu has higher priority
    
    //Set SPI GPIO
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIO0,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_14,AF5_SPI);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_15,AF5_SPI);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_0,AF5_SPI);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_1,AF5_SPI);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.Mode       = SPI_Slave;
    SPI_InitStruct.EN         = 1;
    SPI_InitStruct.IRQEna     = SPI_IRQEna_Enable | SPI_IRQEna_TranDone | SPI_IRQEna_SSErr | SPI_IRQEna_DataOver;
    SPI_InitStruct.ByteLength = 0x08;
    SPI_Init(SPI,&SPI_InitStruct);

    SPI->TX_DATA              = 0x10;//Trig Transfer, This Means SPI Prefetch 1-Byte
    
//  //Release Outside SPI Slave, Only This Test Use
//  GPIO_StructInit(&GPIO_InitStruct);
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
//  GPIO_Init(GPIO2,&GPIO_InitStruct);
//  GPIO_Write(GPIO2, BIT0);
    
    while(1){if (REG32(0x20001000) == 0x00112233)break;}
    //Check Receive Data is Right or Not
    for (i = 0x0; i < 8; i++){
      if (data_tbl[i] != data_receive_init[i])NOTOK;//only flag
    }
    OK;//only flag
  //next data is ready, config dma again & SPI->TX_DATA = any value -> next transfer
}

void spi_md0_master_mcu()
{
    register UINT32 i;

    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;
    // DO NOT use keyword "const" in array declaration 
    // otherwise array will be stored in flash instead of SRAM
    // DMA cannot access data in flash
    // It seems the global array is not properly initialized during declaration
    // so we manually initialize them below
    UINT8 data_receive_init[64]=
    {
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f
    };
    //inter spi (DUT) transmit data table  MISO
    UINT8 data_tbl_init[64]=
    {
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
        0x55,0xaa,0x80,0x01,0xff,0x00,0x00,0xff,
        0x0f,0xf0,0x55,0xaa,0x80,0x01,0xff,0x00,
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
    };

    REG32(0x20001000) = 0x00000000;

    SYS_WR_PROTECT    = PSW_PROT;
    SYS_CLK_CFG  = 0x01FF;
    SYS_CLK_FEN  = 0x01FF; // clock enable
    SYS_CLK_DIV0 = 0x0000; // spi clock div

    for(i=0; i<64; i++){
        data_tbl[i]     = data_tbl_init[i];
        data_receive[i] = data_receive_init[i];
    }

    NVIC_EnableIRQ(SPI0_IRQn); //enable the SPI interrupt
    __enable_irq();           //clr PRIMASK
    SCB->SCR &= ~0x00000010;  //clr SEVONPEND
    
//  //Release Outside SPI Slave, Only This Test Use
//  GPIO_StructInit(&GPIO_InitStruct);
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
//  GPIO_Init(GPIO3,&GPIO_InitStruct);
//  GPIO_Write(GPIO3, BIT0);
    
    //group 2 : p1_13/14/15 p2_0
    //Set SPI GPIO
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_13,AF5_SPI);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_14,AF5_SPI);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_15,AF5_SPI);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIO2,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO2,GPIO_PinSource_0,AF5_SPI);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.Mode       = SPI_Master;
    SPI_InitStruct.EN         = 1;
    SPI_InitStruct.IRQEna     = SPI_IRQEna_Enable | SPI_IRQEna_TranDone | SPI_IRQEna_SSErr | SPI_IRQEna_DataOver;
    SPI_InitStruct.ByteLength = 0x08;
    SPI_Init(SPI,&SPI_InitStruct);

    SPI->DIV   |= 0x80;// non-dma 
    SPI->SIZE   = 0x01;
    SPI->TX_DATA= 0x00;
    while(1){if (REG32(0x20001000) == 0x00112233)break;}
    if (SPI->RX_DATA != 0x0000000f)NOTOK;


    REG32(0x20001000) = 0x00000000;
    SPI->SIZE   = 0x01;
    SPI->TX_DATA= 0xff;
    while(1){if (REG32(0x20001000) == 0x00112233)break;}
    if (SPI->RX_DATA != 0x000000aa)NOTOK;

    OK;
}

void spi_md0_slave_mcu()
{

    register UINT32 i;

    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;
    // DO NOT use keyword "const" in array declaration 
    // otherwise array will be stored in flash instead of SRAM
    // DMA cannot access data in flash
    // It seems the global array is not properly initialized during declaration
    // so we manually initialize them below
    UINT8 data_receive_init[64]=
    {
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,
        0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,
        0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f,0xaa,
        0x55,0x0f,0xaa,0x55,0x0f,0xaa,0x55,0x0f
    };
    //inter spi (DUT) transmit data table  MISO
    UINT8 data_tbl_init[64]=
    {
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
        0x55,0xaa,0x80,0x01,0xff,0x00,0x00,0xff,
        0x0f,0xf0,0x55,0xaa,0x80,0x01,0xff,0x00,
        0x00,0xff,0x0f,0xf0,0x55,0xaa,0x80,0x01,
        0xff,0x00,0x00,0xff,0x0f,0xf0,0x55,0xaa,
        0x80,0x01,0xff,0x00,0x00,0xff,0x0f,0xf0,
    };

    for(i=0; i<64; i++){
        data_tbl[i]     = data_tbl_init[i];
        data_receive[i] = data_receive_init[i];
    }

    REG32(0x20001000) = 0x00000000;

    SYS_WR_PROTECT    = PSW_PROT;
    SYS_CLK_CFG  = 0x01FF;
    SYS_CLK_FEN  = 0x01FF; // clock enable
    SYS_CLK_DIV0 = 0x0000; // spi clock div

    NVIC_EnableIRQ(SPI0_IRQn); //enable the SPI interrupt
    __enable_irq();           //clr PRIMASK
    SCB->SCR &= ~0x00000010;  //clr SEVONPEND
    
    //Set SPI GPIO
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIO0,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_14,AF5_SPI);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_15,AF5_SPI);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIO1,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_0,AF5_SPI);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_1,AF5_SPI);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.Mode       = SPI_Slave;
    SPI_InitStruct.EN         = 1;
    SPI_InitStruct.IRQEna     = SPI_IRQEna_Enable | SPI_IRQEna_TranDone | SPI_IRQEna_SSErr | SPI_IRQEna_DataOver;
    SPI_InitStruct.ByteLength = 0x08;
    SPI_Init(SPI,&SPI_InitStruct);


    SPI->DIV   |= 0x80;// non-dma 
    SPI->SIZE   = 0x01;
    SPI->TX_DATA= 0x55;

//  //Release Outside SPI Slave, Only This Test Use
//  GPIO_StructInit(&GPIO_InitStruct);
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
//  GPIO_Init(GPIO2,&GPIO_InitStruct);
//  GPIO_Write(GPIO2, BIT0);

    while(1){if (REG32(0x20001000) == 0x00112233)break;}
    if (SPI->RX_DATA != 0x0000000f)NOTOK;


    REG32(0x20001000) = 0x00000000;
    SPI->SIZE   = 0x01;
    SPI->TX_DATA= 0xff;
    while(1){if (REG32(0x20001000) == 0x00112233)break;}
    if (SPI->RX_DATA != 0x000000aa)NOTOK;

    OK;
}


void test_spi()
{
//spi_md0_master_dma();//test spi master, md == 0, dma way
//spi_md0_master_mcu();//test spi master, md == 0, mcu way
//spi_md0_slave_dma(); //test spi slave,  md == 0, dma way
  spi_md0_slave_mcu(); //test spi slave,  md == 0, mcu way
}
