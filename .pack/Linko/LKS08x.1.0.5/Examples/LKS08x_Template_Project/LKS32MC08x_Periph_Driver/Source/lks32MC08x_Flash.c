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
#include "basic.h"
#include "lks32mc08x.h"

void EraseSector(unsigned long adr)
{


	  FLASH_CFG &=~0x88008800;
	  FLASH_ADDR   = adr;
    FLASH_CFG   |= 0x88000000;//enable erase & prog
    FLASH_ERASE  = 0x7654DCBA;//trig sector erase
	  FLASH_CFG &=~0x88008800;

}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{

    /* Add your Code */
    volatile unsigned int Addr, size;
    volatile unsigned char *p;
    volatile unsigned t_rlt, t_Cnt;


    t_Cnt = 0;


ReErase:
    p = buf;
    size = sz;
    Addr = adr;

    EraseSector(adr);

    FLASH_CFG &=~0x88008800;
    FLASH_CFG   |= 0x08000000;

    size = (size + 3) & ~3;                     // align the word
    while(size)
    {
        FLASH_ADDR = (Addr & ~0x00000003);      // Address alignment
        FLASH_WDATA= *((u32 *)p);             // Write Data to Flash
			

        p += 4;
        size -= 4;                       // Go to next Word
        Addr += 4;
    }

		for(Addr=0; Addr<0xffff; Addr++);
    p = buf;
    size = sz;
    Addr = adr;
    t_rlt = 0;
    while(size)
    {

        if(*((u32 *)p) != *((u32 *)Addr))
        {
            t_rlt = 1;
        }

        p += 4;
        size -= 4;
        Addr+=4;
    }

    t_Cnt++;

    if(t_Cnt < 3)
    {
        if(t_rlt)
        {
            goto ReErase;
        }
    }

    FLASH_CFG &=~0x88008800;
    return (0);                             // Finished without Errors
}



