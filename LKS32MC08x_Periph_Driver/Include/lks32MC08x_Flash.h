/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： LKS32MC_08x_flash.h
 * 文件标识：
 * 内容摘要： flash外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2016年3月25日
 *
 * 修改记录1：
 * 修改日期：2020年12月15日
 * 版 本 号：V 1.2
 * 修 改 人：DengT
 * 修改内容：增加程序跑飞时，对Flash的保护
 *
 *******************************************************************************/
#ifndef __LKS32MC08x_FLASH__
#define __LKS32MC08x_FLASH__

#include "lks32mc08x_lib.h"

#define Flash_MAIN 0x000
#define Flash_NVR  0x800

extern volatile u32 erase_flag;
extern volatile u32 progm_flag;
int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr);
extern void EraseSector(u32 adr, u16 nvr);
void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr);

#endif

