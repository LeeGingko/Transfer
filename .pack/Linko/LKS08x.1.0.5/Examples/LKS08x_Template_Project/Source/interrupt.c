/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： interrupt.c
 * 文件标识：
 * 内容摘要： 中断服务程序文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2015年11月5日
 *
 * 修改记录1：
 * 修改日期：2015年11月5日
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
 
#include "lks32mc08x.h"
#include "interrupt.h"
#include "freemaster.h"
#include "hardware_config.h"
#include "lks32mc08x_spi.h"
void UART1_SENDDATA(UINT8 n);

extern volatile UINT8 test_adc;

volatile UINT32 adc1_irq_flag, gpio_IRQflg, irqTestFlg;
volatile UINT8 RX_buffer[33];
volatile UINT8 RX_point;
volatile UINT16 ADC_data[9],ADC0_data[9];

volatile u16 Rec_Cnt, Rec_Cnt_Lim;

void SleepTimer_IRQHandler(void)
{
  while(1);
}

void GPIO_IRQHandler(void)
{

}

void VC3_IRQHandler(void)
{
  while(1);
}

void CSX_IRQHandler(void)
{
  while(1);
}

void NSX_IRQHandler(void)
{
  while(1);
}

void I2C0_IRQHandler(void)
{
  while(1);
}

void SPI0_IRQHandler(void)
{
//  while(1);
    switch (SPI_IE){
        case 0xF4://done
            SPI_IE |= 0x04;
            break;
        case 0xF2://error
            SPI_IE |= 0x02;
            FAIL;
            break;
        case 0xF1: //ovfl
            SPI_IE |= 0x01;
            FAIL;
            break;
        default:
            break;
    }
if (SPI->DIV & 0x80)
{
REG32(0x20001000) = 0x00112233;
}
else	
{
//wait DMA done, SPI done means spi transfer done but only after DMA transfer done, all data are stored in ram.
while(DMA_IF & 0x04)break;
DMA_IF  |= 0x04;
DMA_CTRL = 0x0000;//disable dma
REG32(0x20001000) = 0x00112233;
}

}



void HALL_IRQHandler(void)
{
   HALL_INFO = BIT16|BIT17;
 
}



void UTIMER0_IRQHandler(void)
{
   UTIMER_IF = 0x07;
	
	 GPIO0_PDO ^= BIT11;
}

void UTIMER1_IRQHandler(void)
{
    irqTestFlg = UTIMER_IF;
}

void UTIMER2_IRQHandler(void)
{
   irqTestFlg = UTIMER_IF;
}

void UTIMER3_IRQHandler(void)
{
   irqTestFlg = UTIMER_IF;
}

void ENCODER0_IRQHandler(void)
{
    while(1);
}

void ENCODER1_IRQHandler(void)
{
    while(1);
}



void ADC1_IRQHandler(void)
{
 

}

void ADC0_IRQHandler(void)
{
    ADC0_IF |= BIT1 | BIT0;


}

void MCPWM_IRQHandler(void)
{

   

}

volatile u16 Cmp_IRQ_flg;
void CMP_IRQHandler(void)
{
   if(MCPWM_EIF & (BIT6|BIT7))
   {
      Cmp_IRQ_flg = MCPWM_EIF;
      MCPWM_EIF = BIT6;
   }
}

volatile u16 uartTestCnt, uartDelay = 10; 
void UART1_IRQHandler(void)
{
	FMSTR_Isr();
   uartTestCnt++;
   if(UART1_IF & 0x10)
	{
		 UART1_IF |= 0x10;
	}
   else if(UART1_IF & 0x20)
	{
		 UART1_IF |= 0x20;
	}
   
   GPIO1_PDO ^= 0x04;
	 
}


void UART0_IRQHandler(void)
{

  if(UART0_IF & 0x08)
	{
		 UART0_IF |= 0x08;
	}
	else if(UART0_IF & 0x10)
	{
		 UART0_IF |= 0x10;
	}
   else if(UART0_IF & 0x20)
	{
		 UART0_IF |= 0x20;
	}

}


void SysTick_Handler(void)
{

}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/


