/*!
*-----------------------------------------------------------------------------
* The confidential and proprietary information contained in this file may
* only be used by a person authorised under and to the extent permitted
* by a subsisting licensing agreement from ARM Limited.
*
* 		   (C) COPYRIGHT 2009-2010 ARM Limited.
* 			   ALL RIGHTS RESERVED
*
* This entire notice must be reproduced on all copies of this file
* and copies of this file may only be made by a person if such person is
* permitted to do so under the terms of a subsisting license agreement
* from ARM Limited.
*
* 	 SVN Information
*    @file boot.c
*    @brief	define the confidential and proprietary information for ARM Core
*    @data  2008-12-31 10:59:44 +0000 (Wed, 31 Dec 2008)
*    @revision  Revision: 97564 
*
* 	 Release Information : Cortex-M0-AT510-r0p0-03rel0
*-----------------------------------------------------------------------------
*/

// Boot (vectors) file for Cortex-M0 Integration Kit

/*-----------------------------------------------------------------------------*/
/* Including Files											                     		     */
/*-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>
#include <stdint.h>
#include <basic.h>

/*!
* Build a stand-alone image \n
*

*#pragma import(__use_no_semihosting) \n


* Dummy Interrupt Handlers	\n
*
* The following functions are defined weakly to allow the user \n
* to override them at link time simply by declaring their own \n
* function of the same name. \n
*
* If no user function is provided, the weak function is used. \n

* Interrupt Handlers Declaration \n
*/

// Interrupt Handlers Declaration
extern void TIMER0_IRQHandler(void);
extern void TIMER1_IRQHandler(void);
extern void TIMER2_IRQHandler(void);
extern void TIMER3_IRQHandler(void);
extern void ENCODER0_IRQHandler(void);
extern void ENCODER1_IRQHandler(void);
extern void I2C0_IRQHandler(void);
extern void GPIO_IRQHandler(void);
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void HALL_IRQHandler(void);
extern void ADC0_IRQHandler(void);
extern void ADC1_IRQHandler(void);
extern void MCPWM_IRQHandler(void);

void NMI_Handler(void)
{
  while(1);
}

void HardFault_Handler(void)
{
  SIM_DONE = 0x8A4D;
  while(1);
}

void SVC_Handler(void)
{
  while(1);
}

void PendSV_Handler(void)
{
  while(1);
}

void SysTick_Handler(void)
{
  while(1);
}

void Default_IRQHandler(void)
{
  while(1);
}

//
// Reset Handler
//
extern void main(void);

void Reset_Handler(void)
{
	main();
}

//
//
// Set up Vector Table
//

vect_t __Vectors[]
__attribute__ ((section("vectors"))) = {
  (vect_t)(0x20000FFC),     // Top of Stack - Allowing 4 words for DEBUGDRIVERDATA ???? 8Kbyte SRAM
  (vect_t)Reset_Handler,    // Reset Handler
  (vect_t)NMI_Handler,      // NMI Handler
  (vect_t)HardFault_Handler,// Hard Fault Handler
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  (vect_t)SVC_Handler,      // SV Call Handler
  0,                        // Reserved
  0,                        // Reserved
  (vect_t)PendSV_Handler,   // PendSV Handler
  (vect_t)SysTick_Handler,  // SysTick Handler

  // External Interrupts
  
  // 0-3
  (vect_t)TIMER0_IRQHandler,
  (vect_t)TIMER1_IRQHandler,
  (vect_t)TIMER2_IRQHandler,
  (vect_t)TIMER3_IRQHandler,

  //4-7
  (vect_t)ENCODER0_IRQHandler,
  (vect_t)ENCODER1_IRQHandler,
  (vect_t)I2C0_IRQHandler,
  (vect_t)GPIO_IRQHandler,

  //8-11
  (vect_t)UART0_IRQHandler,
  (vect_t)HALL_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)ADC0_IRQHandler,

  //12-15
  (vect_t)ADC1_IRQHandler,
  (vect_t)MCPWM_IRQHandler,
  (vect_t)UART1_IRQHandler,
  // External Interrupts 15 - 32
  // These are essentially unused, so will all
  // take the same default handler if invoked.
  //(vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,

  //16-19
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,

  //20-23
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,

  //24-27
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,

  //28-31
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler,
  (vect_t)Default_IRQHandler
};

/*!
* Set up initial stack and heap  ------------------------------------------------------------------
*/

__value_in_regs struct __initial_stackheap
__user_initial_stackheap(unsigned hb, unsigned sb, unsigned hl, unsigned sl)
{	
  struct __initial_stackheap s;
	
  s.heap_base   = hb;
  s.stack_base  = sb;
  s.heap_limit  = s.stack_base;
  s.stack_limit = s.heap_base;
  return s;
}
