/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_init.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ����ʼ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------- */
#include "SEGGER_RTT.h"
#include "hardware_config.h"
#include "tc_uart.h"
#include "tc_485.h"
#include "tc_can.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine --------------------------------------------------------------------- */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
// u32 time_tick;
u32 SystemCoreClock = 96000000LL;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */
u32 TC_SysTick_Config(uint32_t ticks);

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ��������  void TC_NVIC_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ����NVIC��ʼ��
 * �����������
 * �����������
 * ��  ע��  2023��2��21��->����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TC_NVIC_Init(void)
{
    UART_ClearIRQFlag(UART0, UART_IF_RcvOver);
    NVIC_SetPriority(UART0_IRQn, 0);
    NVIC_EnableIRQ(UART0_IRQn);

    UART_ClearIRQFlag(UART1, UART_IF_SendOver);
    NVIC_SetPriority(UART1_IRQn, 13); // UART0�ж����ȼ�����
    NVIC_EnableIRQ(UART1_IRQn);       // ʹ��UART0��ʱ���ж�

    NVIC_SetPriority(CAN_IRQn, 1);
    NVIC_EnableIRQ(CAN_IRQn);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ��������  static void TC_SysLed_Init(void)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ϵͳָʾLED
 * �����������
 * �����������
 * ��  ע��  2023��2��21��->����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static void TC_SysLed_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -----------ϵͳLED IO����----------- */
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = SYS_LED_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SYS_LED_PORT, &GPIO_InitStruct);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    void Hardware_init(void)
 * ����������    Ӳ�����ֳ�ʼ��
 * ���������    ��
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Hardware_Init(void)
{
    __disable_irq();         /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83; /*ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000; /* enable prefetch ��FLASHԤȡ����ʹ��*/
    TC_SysLed_Init();
    TC_CAN_Init();
    TC_485_Init();
    TC_UART_Init();
    TC_NVIC_Init(); /* NVIC���� */
    SoftDelay(1000);
    //    NVIC_EnableIRQ(DMA_IRQn);      /* ʹ��DMA�ж� */
    //    NVIC_SetPriority(DMA_IRQn, 1); /* ����DMA�ж����ȼ�*/
    SYS_WR_PROTECT = 0x0; /*�ر�ϵͳ�Ĵ���д����*/
    __enable_irq();       /* �����ж� */
    /* ϵͳ��શ�ʱ����ʼ�� */
    if (TC_SysTick_Config(12000) == 0) {
        printf("SysTick Config Ok!\r\n");
    } else {
        printf("SysTick Config Err!\r\n");
    }
    SoftDelay(1000);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    void Clock_Init(void)
 * ����������    ʱ������
 * ���������    ��
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;     /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 |= BIT15;       /* BIT15:PLLPDN*/
    SoftDelay(500);              /* �ȴ�Ӳ����ʼ�����*/
    SYS_CLK_CFG    = 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� ; ��������ʱ�� */
    SYS_WR_PROTECT = 0x0;        /*�ر�ϵͳ�Ĵ���д����*/
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    void SystemInit(void)
 * ����������    Ӳ��ϵͳ��ʼ��������ʱ�ӳ�ʼ������
 * ���������    ��
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SystemInit(void)
{
    Clock_Init(); /* ʱ�ӳ�ʼ�� */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    void SoftDelay(void)
 * ����������    �����ʱ����
 * ���������    ��
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for (t_cnt = 0; t_cnt < cnt; t_cnt++) {
        __NOP();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
 * ����������    GPIO��ƽ��ת����
 * ���������    ��
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0          HuangMG             ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
u8 Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    u16 reg;
    reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); // ��GPIO��ƽSYST_CSR
    if (reg) {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
        return 0;
    } else {
        GPIO_SetBits(GPIOx, GPIO_Pin);
        return 1;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * �������ƣ�    vu32 TC_SysTick_Config(uint32_t ticks)
 * ����������    ��શ�ʱ����ʼ��
 * ���������    ��������
 * ���������    1��ʧ�ܣ����ý��Ĵ��������� 0����ʼ���ɹ�
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/02/12    V1.0          Frank Lee          ����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
u32 TC_SysTick_Config(uint32_t ticks)
{
    if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) {
        return (1UL); /* Reload value impossible */
    }

    SysTick->LOAD = (uint32_t)(ticks - 1UL);                         /* set reload register */
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL = 0UL;                                              /* Load the SysTick Counter Value */
    SysTick->CTRL &= (~SysTick_CTRL_CLKSOURCE_Msk);                  /* MACLK / 8 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;                       /* Enable SysTick IRQ */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                        /* Enable SysTick IRQ and SysTick Timer */

    return (0UL); /* Function successful */
}

/* USER IMPLEMENTED FUNCTIONS END */
