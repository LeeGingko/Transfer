/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： lks32mc081_flash.c
 * 文件标识：
 * 内容摘要： Flash外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2019年3月5日
 *
 * 修改记录1：
 * 修改日期：2019年3月5日
 * 版 本 号：V 1.0
 * 修 改 人：Howlet
 * 修改内容：创建
 *
 * 修改记录2：
 * 修改日期：
 * 版 本 号：
 * 修 改 人：
 * 修改内容：
 *
 *******************************************************************************/
#include "lks32MC08x_Flash.h"

volatile u32 erase_flag = 0;
volatile u32 progm_flag = 0;



/*******************************************************************************
 函数名称：    void EraseSector(unsigned long adr, unsigned long nvr)
 功能描述：    Flash按扇区擦除操作
 操作的表：    无
 输入参数：    adr：       扇区地址  (一个扇区512字节)
               nvr：       为0x800的时候擦除NVR, 为0时擦除主Flash 
               erase_flag：调用函数前必须赋值0x9A0D361F，否则不执行擦除，用来防程序跑飞
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/12/15      V1.2           DengT            创建
 *******************************************************************************/
void EraseSector(u32 adr, u16 nvr)
{
    u32 key1,key2,key3;

    key1 = 0x0;key2 = 0x0;key3 = 0x0;
    if(erase_flag == 0x9A0D361F)
    { //get flash_cfg addr
      key1   = 0xB00C060A;
      key2   = 0x6A013415;
      key3   = erase_flag ^ key1 ^ key2;
      REG32(key3) &=~0x80008000;
      FLASH_ADDR = adr;     
      REG32(key3) |=(0x80000000|nvr);
      key1 = 0x0;key2 = 0x0;key3 = 0x0;
    }

    if(erase_flag == 0x9A0D361F)
    { //get flash_erase addr
      key1   = 0xB001341A;
      key2   = 0x6A0C0615;
      key3   = erase_flag ^ key1 ^ key2;
      REG32(key3)= 0x7654DCBA;//trig sector erase, FLASH_ERASE
      FLASH_CFG &=~0x80000000;
      erase_flag = 0x00000000;
      key1 = 0x0;key2 = 0x0;key3 = 0x0;
    }
    FLASH_CFG &=~0x80000000;
    erase_flag = 0x00000000;
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
}


/*******************************************************************************
 函数名称：    ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
 功能描述：    Flash编程操作
 操作的表：    无
 输入参数：     adr：扇区地址  
               sz : 编程字节数量(0~512字节) 
               buf: 要编程的数据数组指针；
               nvr：       为0x800的时候编程NVR, 为0时编程主Flash 
               progm_flag：调用函数前必须赋值0x9AFDA40C，否则不执行编程，用来防程序跑飞
 输出参数：    无
 返 回 值：    1：编程成功；  0: 编程失败
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/12/15      V1.2           DengT            创建
 *******************************************************************************/
int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr)
{

    /* Add your Code */
    volatile u32 Addr, size;
    volatile u8 *p;
    volatile u8 t_rlt = 1;

    u32 key1,key2,key3;
  
    key1 = 0x0;key2 = 0x0;key3 = 0x0;

    if(progm_flag == 0x9AFDA40C)
    {
      //get flash_cfg addr
      key1   = 0x6A5C040F;
      key2   = 0xB0A1A403;

      key3   = progm_flag ^ key1 ^ key2;
      REG32(key3) &=~0x80008000;
      FLASH_ADDR = adr;     
      REG32(key3) |=(0x08000000|nvr);
      progm_flag = 0;
      key1 = 0x0;key2 = 0x0;key3 = 0x0;

      p    = buf;
      size = (sz + 3) & ~3;                  // align the word
      Addr = adr;
      while(size)
      {
        FLASH_ADDR = (Addr & ~0x00000003);   // address alignment
        FLASH_WDATA= *p + (*(p+1)<<8) + (*(p+2)<<16) + (*(p+3)<<24);            // write data to flash

        p    += 4;
        size -= 4;                           // Go to next word
        Addr += 4;
      }

      p    = buf;
      size = (sz + 3) & ~3;                  // align the word
      Addr = adr;
      while(size)
      {
        u32 t_RData;
        FLASH_ADDR = (Addr & ~0x00000003);   // address alignment
        t_RData = *p + (*(p+1)<<8) + (*(p+2)<<16) + (*(p+3)<<24);
        if(t_RData != FLASH_RDATA)
        {
            t_rlt = 0;
        }

        p    += 4;
        size -= 4;
        Addr += 4;
      }

    }
    FLASH_CFG &= ~0x08000800;
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
    progm_flag = 0;
    return (t_rlt);
}


/*******************************************************************************
 函数名称：    void MCPWM0_RegUpdate(void)
 功能描述：    读取FLASH数据函数
 输入参数：    adr：要读取数据的地址  
               nvr：为0，读取MAIN数据，为1，读取NVR数据
 输出参数：    读取的数据值
 返 回 值：    data
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/3/5      V1.0                                创建
 *******************************************************************************/
u32 Read_Flash(uint32_t adr , u16 nvr)
{
   uint32_t data;
	 if(nvr == Flash_MAIN) //读取MIAN区域
	 {
	   FLASH_CFG &= ~BIT11;
	   FLASH_ADDR = adr;
	   data = FLASH_RDATA;
     FLASH_CFG &= ~BIT11;  //恢复MIAN操作		 
	 }else{
	   data =  Read_NVR(adr);
	 } 
	 return data;
}

/*******************************************************************************
 函数名称：    void MCPWM0_RegUpdate(void)
 功能描述：    读取FLASH多地址数据函数
 输入参数：    adr：要读取数据的地址
               nvr：为0x800，读取MAIN数据，为0x0，读取NVR数据
							 buf:读取数据存储的地址
               sz :读取sz个字
 输出参数：    读取的数据值
 返 回 值：    data
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2021/3/5      V1.0                                创建
 *******************************************************************************/
void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr)
{
	 volatile u32 Addr = 0;
   volatile u32	size = 0;
   volatile u32 *p;
	 size = sz;
	 Addr = adr;
	 p  = buf;
	 while(size)
	 {	 
		 *p = Read_Flash(Addr , nvr);
		 while(!(FLASH_READY&BIT0)){};
	   size -= 1;  
		 Addr += 4;
		 p += 1;		 
	 }
}




