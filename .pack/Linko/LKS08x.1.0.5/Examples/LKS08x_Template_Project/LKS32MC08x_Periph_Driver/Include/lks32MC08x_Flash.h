/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： LKS32MC_061_flash.h
 * 文件标识：
 * 内容摘要： flash外设驱动程序
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2016年3月25日
 *
 * 修改记录1：
 * 修改日期：2016年3月25日
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
#ifndef __lks32mc08x_FLASH__
#define __lks32mc08x_FLASH__

void EraseSector (unsigned long adr);
void ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf);

#endif

