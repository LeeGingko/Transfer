/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： Main.c
 * 文件标识：
 * 内容摘要： 工程主代码
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： 李鹏
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：2015年11月5日
 * 版 本 号：V 1.0
 * 修 改 人：李鹏
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
#include "lks32mc08x_adc.h"
#include "freemaster.h"
//#include "absacc.h"
#include "lks32mc08x_uart.h"
#include "lks32mc08x_Gpio.h"
#include "lks32mc08x_tim.h"
#include "hardware_config.h"
#include "lks32mc08x_spi.h"
#include "lks32mc08x_i2c.h"
#include "test_spi.h"
void UART1_SENDDATA(UINT8 n);
void Clock_Init(void);
void UART0_init(void);
void ADC1_init(void);
void Hardware_init(void);
void MCPWM_init(void);
void UTimer_init(void);
void GPIO_init(void);
void UART0_SENDDATA(UINT8 n);
void delay(u32 cnt);
void CMP_Init(void);


/*******************************************************************************
 函数名称：    int main(void)
 功能描述：    主程序入口
 操作的表：    无
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 多任务访问:   该函数涉及全局表项操作，不可重入
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          创建
 *******************************************************************************/
int main(void)
{

   Hardware_init();

	 ADC0_CHN0 = 0xb0b;

   for(;;)
   {
		 
		  delay(0x2fffff);

	 }

}
   
void delay(u32 cnt)
{
   u32 t_cnt;
	
   for(t_cnt = 0; t_cnt < cnt; t_cnt++)
   {
      __nop();
   }
	
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
