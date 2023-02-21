#include "lks32mc08x.h"
#include "basic.h"

void spi_md0_master_dma(void);
void spi_md0_master_mcu(void);
void spi_md0_slave_dma(void);
void spi_md0_slave_mcu(void);
void test_spi(void);

#define SYS_PROT    REG32(0x400000B0)
#define PSW_PROT    0x7a83
#define SIMDONE     REG32(0x40000020)
#define OK          SYS_PROT = PSW_PROT, SIMDONE = 0xDADE
#define NOTOK       SYS_PROT = PSW_PROT, SIMDONE = 0xFA11
