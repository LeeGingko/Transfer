#ifndef __CONNIE_H
#define __CONNIE_H

#include "basic.h"


//-----------------------------------------------------------
#define ROM_TAB_BASE        0xF0000000

//-----------------------------------------------------------
#define MISC_BASE           0x40000000

#define SCR_SYS_BOOT        REG32(MISC_BASE + 0x00) 


#define SYS_WDT_CLR         REG32(MISC_BASE + 0x38) 

#define SYS_AFE_ADC0        REG32(MISC_BASE + 0x10) 
#define SYS_AFE_CMP         REG32(MISC_BASE + 0x18) 
#define SYS_AFE_INFO        REG32(MISC_BASE + 0x1C) 
#define SYS_AFE_REG0        REG32(MISC_BASE + 0x20) 
#define SYS_AFE_REG1        REG32(MISC_BASE + 0x24)  
#define SYS_AFE_REG2        REG32(MISC_BASE + 0x28)  
#define SYS_AFE_REG3        REG32(MISC_BASE + 0x2C)  
#define SYS_AFE_REG4        REG32(MISC_BASE + 0x30)  
#define SYS_AFE_REG5        REG32(MISC_BASE + 0x34)  
#define SYS_AFE_REG6        REG32(MISC_BASE + 0x38)  
#define SYS_AFE_REG7        REG32(MISC_BASE + 0x3C)  
#define SYS_AFE_REG8        REG32(MISC_BASE + 0x40)  
#define SYS_AFE_REG9        REG32(MISC_BASE + 0x44)  
#define SYS_AFE_REGA        REG32(MISC_BASE + 0x48)  
#define SYS_AFE_REGB        REG32(MISC_BASE + 0x4C)  
#define SYS_AFE_REGC        REG32(MISC_BASE + 0x50)

#define SYS_AFE_DAC         REG32(MISC_BASE + 0x7C)  
#define SYS_CLK_CFG         REG32(MISC_BASE + 0x80)  
#define SYS_RST_CFG         REG32(MISC_BASE + 0x84)  
#define SYS_RST_SRC         REG32(MISC_BASE + 0x88)  
#define SYS_CLR_RST         REG32(MISC_BASE + 0x8C)  
#define SYS_CLK_DIV0        REG32(MISC_BASE + 0x90)  // SPI clock div
#define SYS_CLK_DIV1        REG32(MISC_BASE + 0x94)  // I2C clock div
#define SYS_CLK_DIV2        REG32(MISC_BASE + 0x98)  // UART0/1 clock div
#define SYS_CLK_FEN         REG32(MISC_BASE + 0x9C)  // perapheral clock enable
#define SYS_CLK_SLP         REG32(MISC_BASE + 0xA0)  
#define SYS_IAP             REG32(MISC_BASE + 0xA4)  
#define SYS_TRIM            REG32(MISC_BASE + 0xA8)  
#define SYS_SFT_RST         REG32(MISC_BASE + 0xAC)  
#define SYS_WR_PROTECT      REG32(MISC_BASE + 0xB0)
#define SYS_AFE_DAC_AMC     REG32(MISC_BASE + 0xB4)
#define SYS_AFE_DAC_DC      REG32(MISC_BASE + 0xB8)

//#define SYS_AFE_OSCALI      REG32(MISC_BASE + 0xC4)  
//#define SYS_AFE_CAP8CALI    REG32(MISC_BASE + 0xC8)  
//#define SYS_AFE_CAP9CALI    REG32(MISC_BASE + 0xCC)  
//#define SYS_AFE_CAPACALI    REG32(MISC_BASE + 0xD0)  
//#define SYS_AFE_CAPBCALI    REG32(MISC_BASE + 0xD4)  
//#define SYS_AFE_CAPCCALI    REG32(MISC_BASE + 0xD8)  
//#define SYS_AFE_CAPDCALI    REG32(MISC_BASE + 0xDC)  
//#define SYS_AFE_CAPECALI    REG32(MISC_BASE + 0xE0)  
//#define SYS_AFE_CAPSUM_T    REG32(MISC_BASE + 0xE4)  
//#define SYS_AFE_ADC0_CT     REG32(MISC_BASE + 0xE8)  
//#define SYS_AFE_ADC1_CT     REG32(MISC_BASE + 0xEC)  

#define PSW_SYS_CLR_RST     0xCA40
#define PSW_SYS_CLK_SLP     0xDEAD
#define PSW_SYS_PWR_SLP     0xDAD0
#define PSW_SYS_SET_IAP     0x3720 // lsb must be ZERO
#define PSW_SYS_PROTECT     0x7a83

#define FLASH_PASSEN       0xB714AD9C
#define FLASH_PASS_NVR_EN  0x7531FDB9
#define FLASH_PASS_PROG    0x2468ACE0
#define FLASH_PASS_ERASE   0x7654DCBA
#define FLASH_PASS_CONFEN  0x13579BDF

#define FLS_CR_BASE         0x40000400
#define FLASH_CFG           REG32(FLS_CR_BASE + 0x00)
#define FLASH_ADDR          REG32(FLS_CR_BASE + 0x04) 
#define FLASH_WDATA         REG32(FLS_CR_BASE + 0x08)
#define FLASH_RDATA         REG32(FLS_CR_BASE + 0x0C)

#define FLASH_ERASE         REG32(FLS_CR_BASE + 0x10)
#define FLASH_PROTECT       REG32(FLS_CR_BASE + 0x14)
#define FLASH_READY         REG32(FLS_CR_BASE + 0x18)

#define SPI0_BASE 	        0x40010000

#define SPI_CFG             REG32(SPI0_BASE + 0x00)
#define SPI_IE              REG32(SPI0_BASE + 0x04)
#define SPI_DIV             REG32(SPI0_BASE + 0x08)
#define SPI_TX_DATA         REG32(SPI0_BASE + 0x0C)
#define SPI_RX_DATA         REG32(SPI0_BASE + 0x10)
#define SPI_SIZE            REG32(SPI0_BASE + 0x14)

#define SPI                ((SPI_TypeDef *) SPI0_BASE)

#define I2C0_BASE 	        0x40010400

#define I2C0_ADDR 	        REG32(I2C0_BASE + 0x00) 
#define I2C0_CFG 	        REG32(I2C0_BASE + 0x04)
#define I2C0_SCR 	        REG32(I2C0_BASE + 0x08)
#define I2C0_DATA 	        REG32(I2C0_BASE + 0x0C)
#define I2C0_MSCR 	        REG32(I2C0_BASE + 0x10)
#define I2C0_BCR            REG32(I2C0_BASE + 0x14)

#define I2C                ((I2C_TypeDef *) I2C0_BASE)

#define IO_MASK_BASE        0x40010800 
#define IO_MASK_CTRL        REG32(IO_MASK_BASE + 0x00)
#define IO_MASK_STT         REG32(IO_MASK_BASE + 0x04)

#define CMP_BASE            0x40010C00
#define CMP_IE              REG32(CMP_BASE + 0x00)
#define CMP_IF              REG32(CMP_BASE + 0x04)
#define CMP_TCLK            REG32(CMP_BASE + 0x08)
#define CMP_CFG             REG32(CMP_BASE + 0x0C)
#define CMP_BLCWIN          REG32(CMP_BASE + 0x10)

#define HALL_BASE 	        0x40011000
#define HALL_CFG            REG32(HALL_BASE + 0x00) 
#define HALL_INFO           REG32(HALL_BASE + 0x04) 
#define HALL_WIDTH          REG32(HALL_BASE + 0x08) 
#define HALL_TH             REG32(HALL_BASE + 0x0C)
#define HALL_CNT            REG32(HALL_BASE + 0x10) 

#define HALL                ((HALL_TypeDef *) HALL_BASE)

#define ADC0_BASE           0x40011400

#define ADC0                ((ADC_TypeDef *) ADC0_BASE)

#define ADC0_DAT0           REG32(ADC0_BASE + 0x00)
#define ADC0_DAT1           REG32(ADC0_BASE + 0x04)
#define ADC0_DAT2           REG32(ADC0_BASE + 0x08)
#define ADC0_DAT3           REG32(ADC0_BASE + 0x0C)
#define ADC0_DAT4           REG32(ADC0_BASE + 0x10)
#define ADC0_DAT5           REG32(ADC0_BASE + 0x14)
#define ADC0_DAT6           REG32(ADC0_BASE + 0x18)
#define ADC0_DAT7           REG32(ADC0_BASE + 0x1C)
#define ADC0_DAT8           REG32(ADC0_BASE + 0x20)
#define ADC0_DAT9           REG32(ADC0_BASE + 0x24)
#define ADC0_DAT10          REG32(ADC0_BASE + 0x28)
#define ADC0_DAT11          REG32(ADC0_BASE + 0x2C)
#define ADC0_DAT12          REG32(ADC0_BASE + 0x30)
#define ADC0_DAT13          REG32(ADC0_BASE + 0x34)
#define ADC0_DAT14          REG32(ADC0_BASE + 0x38)
#define ADC0_DAT15          REG32(ADC0_BASE + 0x3C)
#define ADC0_DAT16          REG32(ADC0_BASE + 0x40)
#define ADC0_DAT17          REG32(ADC0_BASE + 0x44)
#define ADC0_DAT18          REG32(ADC0_BASE + 0x48)
#define ADC0_DAT19          REG32(ADC0_BASE + 0x4C)

#define ADC0_CHN0           REG32(ADC0_BASE + 0x50)
#define ADC0_CHN1           REG32(ADC0_BASE + 0x54)
#define ADC0_CHN2           REG32(ADC0_BASE + 0x58)
#define ADC0_CHN3           REG32(ADC0_BASE + 0x5C)
#define ADC0_CHN4           REG32(ADC0_BASE + 0x60)
#define ADC0_CHN5           REG32(ADC0_BASE + 0x64)
#define ADC0_CHN6           REG32(ADC0_BASE + 0x68)
#define ADC0_CHN7           REG32(ADC0_BASE + 0x6C)
#define ADC0_CHN8           REG32(ADC0_BASE + 0x70)
#define ADC0_CHN9           REG32(ADC0_BASE + 0x74)

#define ADC0_CHNT0          REG32(ADC0_BASE + 0x78)
#define ADC0_CHNT1          REG32(ADC0_BASE + 0x7C)

#define ADC0_SD0            REG32(ADC0_BASE + 0x80)
#define ADC0_SD1            REG32(ADC0_BASE + 0x84)
#define ADC0_GAIN0          REG32(ADC0_BASE + 0x88)
#define ADC0_GAIN1          REG32(ADC0_BASE + 0x8C)
#define ADC0_DC_A0          REG32(ADC0_BASE + 0x90)
#define ADC0_DC_A1          REG32(ADC0_BASE + 0x94)
#define ADC0_AMC_A0         REG32(ADC0_BASE + 0x98)
#define ADC0_AMC_A1         REG32(ADC0_BASE + 0x9C)
#define ADC0_DC_B0          REG32(ADC0_BASE + 0xA0)
#define ADC0_DC_B1          REG32(ADC0_BASE + 0xA4)
#define ADC0_AMC_B0         REG32(ADC0_BASE + 0xA8)
#define ADC0_AMC_B1         REG32(ADC0_BASE + 0xAC)

#define ADC0_IE             REG32(ADC0_BASE + 0xB0)
#define ADC0_IF             REG32(ADC0_BASE + 0xB4)
#define ADC0_CFG            REG32(ADC0_BASE + 0xB8)
#define ADC0_TRIG           REG32(ADC0_BASE + 0xBC)
#define ADC0_SWT            REG32(ADC0_BASE + 0xC0)
#define ADC0_DAT0_TH        REG32(ADC0_BASE + 0xC4)

#define UTIMER_BASE         0x40011800

#define UTIMER_UNT0_CFG     REG32(UTIMER_BASE + 0x00) 
#define UTIMER_UNT0_TH      REG32(UTIMER_BASE + 0x04) 
#define UTIMER_UNT0_CNT     REG32(UTIMER_BASE + 0x08) 
#define UTIMER_UNT0_CMP0    REG32(UTIMER_BASE + 0x0C) 
#define UTIMER_UNT0_CMP1    REG32(UTIMER_BASE + 0x10) 
#define UTIMER_UNT0_EVT     REG32(UTIMER_BASE + 0x14) 

#define UTIMER_UNT1_CFG     REG32(UTIMER_BASE + 0x20) 
#define UTIMER_UNT1_TH      REG32(UTIMER_BASE + 0x24) 
#define UTIMER_UNT1_CNT     REG32(UTIMER_BASE + 0x28) 
#define UTIMER_UNT1_CMP0    REG32(UTIMER_BASE + 0x2C) 
#define UTIMER_UNT1_CMP1    REG32(UTIMER_BASE + 0x30) 
#define UTIMER_UNT1_EVT     REG32(UTIMER_BASE + 0x34) 

#define UTIMER_UNT2_CFG     REG32(UTIMER_BASE + 0x40) 
#define UTIMER_UNT2_TH      REG32(UTIMER_BASE + 0x44) 
#define UTIMER_UNT2_CNT     REG32(UTIMER_BASE + 0x48) 
#define UTIMER_UNT2_CMP0    REG32(UTIMER_BASE + 0x4C) 
#define UTIMER_UNT2_CMP1    REG32(UTIMER_BASE + 0x50) 
#define UTIMER_UNT2_EVT     REG32(UTIMER_BASE + 0x54) 

#define UTIMER_UNT3_CFG     REG32(UTIMER_BASE + 0x60) 
#define UTIMER_UNT3_TH      REG32(UTIMER_BASE + 0x64) 
#define UTIMER_UNT3_CNT     REG32(UTIMER_BASE + 0x68) 
#define UTIMER_UNT3_CMP0    REG32(UTIMER_BASE + 0x6C) 
#define UTIMER_UNT3_CMP1    REG32(UTIMER_BASE + 0x70) 
#define UTIMER_UNT3_EVT     REG32(UTIMER_BASE + 0x74) 

#define UTIMER_ECD0_CFG     REG32(UTIMER_BASE + 0x80) 
#define UTIMER_ECD0_TH      REG32(UTIMER_BASE + 0x84) 
#define UTIMER_ECD0_CNT     REG32(UTIMER_BASE + 0x88) 

#define UTIMER_ECD1_CFG     REG32(UTIMER_BASE + 0x90) 
#define UTIMER_ECD1_TH      REG32(UTIMER_BASE + 0x94) 
#define UTIMER_ECD1_CNT     REG32(UTIMER_BASE + 0x98) 

#define UTIMER_FLT_TH01     REG32(UTIMER_BASE + 0xA0)
#define UTIMER_FLT_TH23     REG32(UTIMER_BASE + 0xA4)

#define UTIMER_CFG          REG32(UTIMER_BASE + 0xF0) 
#define UTIMER_IE           REG32(UTIMER_BASE + 0xF4) 
#define UTIMER_IF           REG32(UTIMER_BASE + 0xF8) 
#define UTIMER_RE           REG32(UTIMER_BASE + 0xFC) 

#define TIMER0              ((TIM_TimerTypeDef *) UTIMER_BASE)
#define TIMER1              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x20))
#define TIMER2              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x40))
#define TIMER3              ((TIM_TimerTypeDef *) (UTIMER_BASE+0x60))
#define ECD0                ((TIM_ECDTypeDef *) (UTIMER_BASE+0x80))
#define ECD1                ((TIM_ECDTypeDef *) (UTIMER_BASE+0x90))

#define MCPWM_BASE          0x40011C00

#define MCPWM_TH00          REG32(MCPWM_BASE + 0x00)
#define MCPWM_TH01          REG32(MCPWM_BASE + 0x04)
#define MCPWM_TH10          REG32(MCPWM_BASE + 0x08)
#define MCPWM_TH11          REG32(MCPWM_BASE + 0x0C)
#define MCPWM_TH20          REG32(MCPWM_BASE + 0x10)
#define MCPWM_TH21          REG32(MCPWM_BASE + 0x14)
#define MCPWM_TH30          REG32(MCPWM_BASE + 0x18)
#define MCPWM_TH31          REG32(MCPWM_BASE + 0x1C)
#define MCPWM_TMR0          REG32(MCPWM_BASE + 0x20)
#define MCPWM_TMR1          REG32(MCPWM_BASE + 0x24)
#define MCPWM_TMR2          REG32(MCPWM_BASE + 0x28)
#define MCPWM_TMR3          REG32(MCPWM_BASE + 0x2C)
#define MCPWM_TH            REG32(MCPWM_BASE + 0x30)

#define MCPWM_UPDATE        REG32(MCPWM_BASE + 0x34)
#define MCPWM_IE            REG32(MCPWM_BASE + 0x38)
#define MCPWM_IF            REG32(MCPWM_BASE + 0x3C)
#define MCPWM_EIE           REG32(MCPWM_BASE + 0x40)
#define MCPWM_EIF           REG32(MCPWM_BASE + 0x44)
#define MCPWM_RE            REG32(MCPWM_BASE + 0x48)
#define MCPWM_PP            REG32(MCPWM_BASE + 0x4C)
#define MCPWM_IO01          REG32(MCPWM_BASE + 0x50)
#define MCPWM_IO23          REG32(MCPWM_BASE + 0x54)
#define MCPWM_SDCFG         REG32(MCPWM_BASE + 0x58)

#define MCPWM_TCLK          REG32(MCPWM_BASE + 0x60)
#define MCPWM_FAIL          REG32(MCPWM_BASE + 0x64)
#define MCPWM_FILT          REG32(MCPWM_BASE + 0x68)

#define MCPWM_PRT           REG32(MCPWM_BASE + 0x74)
#define MCPWM_CNT           REG32(MCPWM_BASE + 0x78)

#define MCPWM_DTH00         REG32(MCPWM_BASE + 0x80)
#define MCPWM_DTH01         REG32(MCPWM_BASE + 0x84)
#define MCPWM_DTH10         REG32(MCPWM_BASE + 0x88)
#define MCPWM_DTH11         REG32(MCPWM_BASE + 0x8C)
#define MCPWM_DTH20         REG32(MCPWM_BASE + 0x90)
#define MCPWM_DTH21         REG32(MCPWM_BASE + 0x94)
#define MCPWM_DTH30         REG32(MCPWM_BASE + 0x98)
#define MCPWM_DTH31         REG32(MCPWM_BASE + 0x9C)


#define GPIO0               ((GPIO_TypeDef *) GPIO0_BASE)
#define GPIO1               ((GPIO_TypeDef *) GPIO1_BASE)
#define GPIO2               ((GPIO_TypeDef *) GPIO2_BASE)
#define GPIO3               ((GPIO_TypeDef *) GPIO3_BASE)

#define GPIO0_BASE          0x40012000

#define GPIO0_PIE           REG32(GPIO0_BASE + 0x00)
#define GPIO0_POE           REG32(GPIO0_BASE + 0x04)
#define GPIO0_PDI           REG32(GPIO0_BASE + 0x08)
#define GPIO0_PDO           REG32(GPIO0_BASE + 0x0C)
#define GPIO0_PUE           REG32(GPIO0_BASE + 0x10)
//#define GPIO0_PDE           REG32(GPIO0_BASE + 0x14)
#define GPIO0_PODE          REG32(GPIO0_BASE + 0x18)
#define GPIO0_LCKR          REG32(GPIO0_BASE + 0x1C)
#define GPIO0_F3210         REG32(GPIO0_BASE + 0x20)
#define GPIO0_F7654         REG32(GPIO0_BASE + 0x24)
#define GPIO0_FBA98         REG32(GPIO0_BASE + 0x28)
#define GPIO0_FFEDC         REG32(GPIO0_BASE + 0x2C)

#define GPIO1_BASE          0x40012040
#define GPIO1_PIE           REG32(GPIO1_BASE + 0x00)
#define GPIO1_POE           REG32(GPIO1_BASE + 0x04)
#define GPIO1_PDI           REG32(GPIO1_BASE + 0x08)
#define GPIO1_PDO           REG32(GPIO1_BASE + 0x0C)
#define GPIO1_PUE           REG32(GPIO1_BASE + 0x10)
//#define GPIO1_PDE           REG32(GPIO1_BASE + 0x14)
#define GPIO1_PODE          REG32(GPIO1_BASE + 0x18)
#define GPIO1_LCKR          REG32(GPIO1_BASE + 0x1C)
#define GPIO1_F3210         REG32(GPIO1_BASE + 0x20)
#define GPIO1_F7654         REG32(GPIO1_BASE + 0x24)
#define GPIO1_FBA98         REG32(GPIO1_BASE + 0x28)
#define GPIO1_FFEDC         REG32(GPIO1_BASE + 0x2C)

#define GPIO2_BASE          0x40012080
#define GPIO2_PIE           REG32(GPIO2_BASE + 0x00)
#define GPIO2_POE           REG32(GPIO2_BASE + 0x04)
#define GPIO2_PDI           REG32(GPIO2_BASE + 0x08)
#define GPIO2_PDO           REG32(GPIO2_BASE + 0x0C)
#define GPIO2_PUE           REG32(GPIO2_BASE + 0x10)
//#define GPIO2_PDE           REG32(GPIO2_BASE + 0x14)
#define GPIO2_PODE          REG32(GPIO2_BASE + 0x18)
#define GPIO2_LCKR          REG32(GPIO2_BASE + 0x1C)
#define GPIO2_F3210         REG32(GPIO2_BASE + 0x20)
#define GPIO2_F7654         REG32(GPIO2_BASE + 0x24)
#define GPIO2_FBA98         REG32(GPIO2_BASE + 0x28)
#define GPIO2_FFEDC         REG32(GPIO2_BASE + 0x2C)

#define GPIO3_BASE          0x400120C0
#define GPIO3_PIE           REG32(GPIO3_BASE + 0x00)
#define GPIO3_POE           REG32(GPIO3_BASE + 0x04)
#define GPIO3_PDI           REG32(GPIO3_BASE + 0x08)
#define GPIO3_PDO           REG32(GPIO3_BASE + 0x0C)
#define GPIO3_PUE           REG32(GPIO3_BASE + 0x10)
//#define GPIO3_PDE           REG32(GPIO3_BASE + 0x14)
#define GPIO3_PODE          REG32(GPIO3_BASE + 0x18)
#define GPIO3_LCKR          REG32(GPIO3_BASE + 0x1C)
#define GPIO3_F3210         REG32(GPIO3_BASE + 0x20)
#define GPIO3_F7654         REG32(GPIO3_BASE + 0x24)
#define GPIO3_FBA98         REG32(GPIO3_BASE + 0x28)
#define GPIO3_FFEDC         REG32(GPIO3_BASE + 0x2C)

#define EXTI_BASE           0x40012100
#define EXTI_CR0            REG32(EXTI_BASE  + 0x00)
#define EXTI_CR1            REG32(EXTI_BASE  + 0x04)
#define EXTI_IF             REG32(EXTI_BASE  + 0x08)
#define LCKR_PRT            REG32(EXTI_BASE  + 0x0C)
#define GPIO_WAKE_POL       REG32(EXTI_BASE  + 0x10)
#define GPIO_WAKE_EN        REG32(EXTI_BASE  + 0x14)

#define CRC_BASE            0x40012400
#define CRC_DR              REG32(CRC_BASE   + 0x00)
#define CRC_CR              REG32(CRC_BASE   + 0x04)
#define CRC_INIT            REG32(CRC_BASE   + 0x08)
#define CRC_POL             REG32(CRC_BASE   + 0x0C)

#define UART0_BASE          0x40012800
#define UART0_CTRL          REG32(UART0_BASE + 0x00) 
#define UART0_DIVH          REG32(UART0_BASE + 0x04)
#define UART0_DIVL          REG32(UART0_BASE + 0x08)
#define UART0_BUFF          REG32(UART0_BASE + 0x0C)
#define UART0_ADR           REG32(UART0_BASE + 0x10)
#define UART0_STT           REG32(UART0_BASE + 0x14)
#define UART0_IE            REG32(UART0_BASE + 0x18)
#define UART0_IF            REG32(UART0_BASE + 0x1C)
#define UART0_INV           REG32(UART0_BASE + 0x20)

#define UART1_BASE          0x40012C00
#define UART1_CTRL          REG32(UART1_BASE + 0x00) 
#define UART1_DIVH          REG32(UART1_BASE + 0x04)
#define UART1_DIVL          REG32(UART1_BASE + 0x08)
#define UART1_BUFF          REG32(UART1_BASE + 0x0C)
#define UART1_ADR           REG32(UART1_BASE + 0x10)
#define UART1_STT           REG32(UART1_BASE + 0x14)
#define UART1_IE            REG32(UART1_BASE + 0x18)
#define UART1_IF            REG32(UART1_BASE + 0x1C)
#define UART1_INV           REG32(UART1_BASE + 0x20)

#define UART0               ((UART_TypeDef *) UART0_BASE)
#define UART1               ((UART_TypeDef *) UART1_BASE)

#define DMA_BASE            0x40013000
#define DMA_CCR0            REG32(DMA_BASE + 0x00) 
#define DMA_CTMS0           REG32(DMA_BASE + 0x04)
#define DMA_CPAR0           REG32(DMA_BASE + 0x08)
#define DMA_CMAR0           REG32(DMA_BASE + 0x0C)
#define DMA_CCR1            REG32(DMA_BASE + 0x10) 
#define DMA_CTMS1           REG32(DMA_BASE + 0x14)
#define DMA_CPAR1           REG32(DMA_BASE + 0x18)
#define DMA_CMAR1           REG32(DMA_BASE + 0x1C)
#define DMA_CCR2            REG32(DMA_BASE + 0x20) 
#define DMA_CTMS2           REG32(DMA_BASE + 0x24)
#define DMA_CPAR2           REG32(DMA_BASE + 0x28)
#define DMA_CMAR2           REG32(DMA_BASE + 0x2C)
#define DMA_CCR3            REG32(DMA_BASE + 0x30) 
#define DMA_CTMS3           REG32(DMA_BASE + 0x34)
#define DMA_CPAR3           REG32(DMA_BASE + 0x38)
#define DMA_CMAR3           REG32(DMA_BASE + 0x3C)
#define DMA_CTRL            REG32(DMA_BASE + 0x40)
#define DMA_IF              REG32(DMA_BASE + 0x44)

#define CAN_BASE            0x40013400
#define CAN_MODE            REG32(CAN_BASE + 0x0000)
#define CAN_CMR             REG32(CAN_BASE + 0x0004)
#define CAN_SR              REG32(CAN_BASE + 0x0008)
#define CAN_INTR            REG32(CAN_BASE + 0x000C)
#define CAN_INTR_EN         REG32(CAN_BASE + 0x0010)
#define CAN_BTR0            REG32(CAN_BASE + 0x0018)
#define CAN_BTR1            REG32(CAN_BASE + 0x001C)
#define CAN_OCR             REG32(CAN_BASE + 0x0020)
#define CAN_ALC             REG32(CAN_BASE + 0x002C)
#define CAN_ECC             REG32(CAN_BASE + 0x0030)
#define CAN_EWLR            REG32(CAN_BASE + 0x0034)
#define CAN_RXERR           REG32(CAN_BASE + 0x0038)
#define CAN_TXERR           REG32(CAN_BASE + 0x003C)
//Reset Mode
#define CAN_ACR0    		REG32(CAN_BASE + 0x0040)
#define CAN_ACR1    		REG32(CAN_BASE + 0x0044)
#define CAN_ACR2    		REG32(CAN_BASE + 0x0048)
#define CAN_ACR3    		REG32(CAN_BASE + 0x004C)
#define CAN_AMR0    		REG32(CAN_BASE + 0x0050)
#define CAN_AMR1    		REG32(CAN_BASE + 0x0054)
#define CAN_AMR2    		REG32(CAN_BASE + 0x0058)
#define CAN_AMR3    		REG32(CAN_BASE + 0x005C)
//Operation Mode
#define CAN_TXRX0           REG32(CAN_BASE + 0x0040)
#define CAN_TXRX1           REG32(CAN_BASE + 0x0044)
#define CAN_TXRX2           REG32(CAN_BASE + 0x0048)
#define CAN_TXRX3           REG32(CAN_BASE + 0x004C)
#define CAN_TXRX4           REG32(CAN_BASE + 0x0050)
#define CAN_TXRX5           REG32(CAN_BASE + 0x0054)
#define CAN_TXRX6           REG32(CAN_BASE + 0x0058)
#define CAN_TXRX7           REG32(CAN_BASE + 0x005C)
#define CAN_TXRX8           REG32(CAN_BASE + 0x0060)
#define CAN_TXRX9           REG32(CAN_BASE + 0x0064)
#define CAN_TXRXA           REG32(CAN_BASE + 0x0068)
#define CAN_TXRXB           REG32(CAN_BASE + 0x006C)
#define CAN_TXRXC           REG32(CAN_BASE + 0x0070)
#define CAN_RMC             REG32(CAN_BASE + 0x0074)
#define CAN_RBSA            REG32(CAN_BASE + 0x0078)
#define CAN_CDR             REG32(CAN_BASE + 0x007C)
#define CAN_RFIFO00         REG32(CAN_BASE + 0x0080)
#define CAN_RFIFO01         REG32(CAN_BASE + 0x0084)
#define CAN_RFIFO02         REG32(CAN_BASE + 0x0088)
#define CAN_RFIFO03         REG32(CAN_BASE + 0x008C)
#define CAN_RFIFO04         REG32(CAN_BASE + 0x0090)
#define CAN_RFIFO05         REG32(CAN_BASE + 0x0094)
#define CAN_RFIFO06         REG32(CAN_BASE + 0x0098)
#define CAN_RFIFO07         REG32(CAN_BASE + 0x009C)
#define CAN_RFIFO08         REG32(CAN_BASE + 0x00A0)
#define CAN_RFIFO09         REG32(CAN_BASE + 0x00A4)
#define CAN_RFIFO0A         REG32(CAN_BASE + 0x00A8)
#define CAN_RFIFO0B         REG32(CAN_BASE + 0x00AC)
#define CAN_RFIFO0C         REG32(CAN_BASE + 0x00B0)
#define CAN_RFIFO0D         REG32(CAN_BASE + 0x00B4)
#define CAN_RFIFO0E         REG32(CAN_BASE + 0x00B8)
#define CAN_RFIFO0F         REG32(CAN_BASE + 0x00BC)
#define CAN_RFIFO10         REG32(CAN_BASE + 0x00C0)
#define CAN_RFIFO11         REG32(CAN_BASE + 0x00C4)
#define CAN_RFIFO12         REG32(CAN_BASE + 0x00C8)
#define CAN_RFIFO13         REG32(CAN_BASE + 0x00CC)
#define CAN_RFIFO14         REG32(CAN_BASE + 0x00D0)
#define CAN_RFIFO15         REG32(CAN_BASE + 0x00D4)
#define CAN_RFIFO16         REG32(CAN_BASE + 0x00D8)
#define CAN_RFIFO17         REG32(CAN_BASE + 0x00DC)
#define CAN_RFIFO18         REG32(CAN_BASE + 0x00E0)
#define CAN_RFIFO19         REG32(CAN_BASE + 0x00E4)
#define CAN_RFIFO1A         REG32(CAN_BASE + 0x00E8)
#define CAN_RFIFO1B         REG32(CAN_BASE + 0x00EC)
#define CAN_RFIFO1C         REG32(CAN_BASE + 0x00F0)
#define CAN_RFIFO1D         REG32(CAN_BASE + 0x00F4)
#define CAN_RFIFO1E         REG32(CAN_BASE + 0x00F8)
#define CAN_RFIFO1F         REG32(CAN_BASE + 0x00FC)
#define CAN_RFIFO20         REG32(CAN_BASE + 0x0100)
#define CAN_RFIFO21         REG32(CAN_BASE + 0x0104)
#define CAN_RFIFO22         REG32(CAN_BASE + 0x0108)
#define CAN_RFIFO23         REG32(CAN_BASE + 0x010C)
#define CAN_RFIFO24         REG32(CAN_BASE + 0x0110)
#define CAN_RFIFO25         REG32(CAN_BASE + 0x0114)
#define CAN_RFIFO26         REG32(CAN_BASE + 0x0118)
#define CAN_RFIFO27         REG32(CAN_BASE + 0x011C)
#define CAN_RFIFO28         REG32(CAN_BASE + 0x0120)
#define CAN_RFIFO29         REG32(CAN_BASE + 0x0124)
#define CAN_RFIFO2A         REG32(CAN_BASE + 0x0128)
#define CAN_RFIFO2B         REG32(CAN_BASE + 0x012C)
#define CAN_RFIFO2C         REG32(CAN_BASE + 0x0130)
#define CAN_RFIFO2D         REG32(CAN_BASE + 0x0134)
#define CAN_RFIFO2E         REG32(CAN_BASE + 0x0138)
#define CAN_RFIFO2F         REG32(CAN_BASE + 0x013C)
#define CAN_RFIFO30         REG32(CAN_BASE + 0x0140)
#define CAN_RFIFO31         REG32(CAN_BASE + 0x0144)
#define CAN_RFIFO32         REG32(CAN_BASE + 0x0148)
#define CAN_RFIFO33         REG32(CAN_BASE + 0x014C)
#define CAN_RFIFO34         REG32(CAN_BASE + 0x0150)
#define CAN_RFIFO35         REG32(CAN_BASE + 0x0154)
#define CAN_RFIFO36         REG32(CAN_BASE + 0x0158)
#define CAN_RFIFO37         REG32(CAN_BASE + 0x015C)
#define CAN_RFIFO38         REG32(CAN_BASE + 0x0160)
#define CAN_RFIFO39         REG32(CAN_BASE + 0x0164)
#define CAN_RFIFO3A         REG32(CAN_BASE + 0x0168)
#define CAN_RFIFO3B         REG32(CAN_BASE + 0x016C)
#define CAN_RFIFO3C         REG32(CAN_BASE + 0x0170)
#define CAN_RFIFO3D         REG32(CAN_BASE + 0x0174)
#define CAN_RFIFO3E         REG32(CAN_BASE + 0x0178)
#define CAN_RFIFO3F         REG32(CAN_BASE + 0x017C)
#define CAN_TXBUF0          REG32(CAN_BASE + 0x0180)
#define CAN_TXBUF1          REG32(CAN_BASE + 0x0184)
#define CAN_TXBUF2          REG32(CAN_BASE + 0x0188)
#define CAN_TXBUF3          REG32(CAN_BASE + 0x018C)
#define CAN_TXBUF4          REG32(CAN_BASE + 0x0190)
#define CAN_TXBUF5          REG32(CAN_BASE + 0x0194)
#define CAN_TXBUF6          REG32(CAN_BASE + 0x0198)
#define CAN_TXBUF7          REG32(CAN_BASE + 0x019C)
#define CAN_TXBUF8          REG32(CAN_BASE + 0x01A0)
#define CAN_TXBUF9          REG32(CAN_BASE + 0x01A4)
#define CAN_TXBUFA          REG32(CAN_BASE + 0x01A8)
#define CAN_TXBUFB          REG32(CAN_BASE + 0x01AC)
#define CAN_TXBUFC          REG32(CAN_BASE + 0x01B0)

#define SIF_BASE            0x40013800
#define SIF_CFG             REG32(SIF_BASE + 0x00) 
#define SIF_FREQ            REG32(SIF_BASE + 0x04) 
#define SIF_IRQ             REG32(SIF_BASE + 0x08) 
#define SIF_WDATA           REG32(SIF_BASE + 0x0C)

#define DSP_BASE            0x40014000
#define DSP_CODE_MEM        REG32(DSP_BASE + 0x00)
#define DSP_DATA_MEM        REG32(DSP_BASE + 0x0800)
#define DSP_SC              REG32(DSP_BASE + 0x1000)
#define DSP_CORDIC_THETA    REG32(DSP_BASE + 0x1004)
#define DSP_CORDIC_X        REG32(DSP_BASE + 0x1008)
#define DSP_CORDIC_Y        REG32(DSP_BASE + 0x100C)
#define DSP_CORDIC_SIN      REG32(DSP_BASE + 0x1010)
#define DSP_CORDIC_COS      REG32(DSP_BASE + 0x1014)
#define DSP_CORDIC_MOD      REG32(DSP_BASE + 0x1018)
#define DSP_CORDIC_ARCTAN   REG32(DSP_BASE + 0x101C)
#define DSP_DID             REG32(DSP_BASE + 0x1020)
#define DSP_DIS             REG32(DSP_BASE + 0x1024)
#define DSP_QUO             REG32(DSP_BASE + 0x1028)
#define DSP_REM             REG32(DSP_BASE + 0x102C)
#define DSP_RAD             REG32(DSP_BASE + 0x1030)
#define DSP_SQRT            REG32(DSP_BASE + 0x1034)

#endif
