/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_uart.h"
#include "common.h"
#include "hw_485.h"
/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
u8 RS485_RX[RS485_RX_LEN];

u8 RS485_RxFlag = 0;

static DMA_InitTypeDef DMA_InitStruct;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*******************************************************************************
 �������ƣ�    void HW_485_GPIO_Init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -----------485����0 IO����----------- */
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    // UART0_RXD  P0.15
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    // UART0_TXD  P1.0
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO1, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);

    /* -----------485ʹ�� IO����----------- */
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RS485_EN_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void HW_485_Init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_Init(void)
{
    UART_InitTypeDef UART_InitStruct;

    UART_StructInit(&UART_InitStruct);

    /* -----------485����0 �ṹ������----------- */
    UART_InitStruct.BaudRate   = 115200;                                       /* ���ò�����115200 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;                           /* �������ݳ���8λ */
    UART_InitStruct.StopBits   = UART_STOPBITS_1b;                             /* ֹͣλ1λ */
    UART_InitStruct.FirstSend  = UART_FIRSTSEND_LSB;                           /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;                               /* ����żУ�� */
    UART_InitStruct.IRQEna     = UART_IRQEna_SendOver | UART_IRQEna_RX_DMA_RE; /* �������DMA����ʹ��*/
    UART_Init(UART0, &UART_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void HW_485_DMA_Init(void)
 ����������    DMA��ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ʣ�����ж�DMA_UART�Ĵ��䷽��
               ��DMA_DIR����DMA_UART�Ĵ��䷽��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HW_485_DMA_Init(void)
{
    DMA_StructInit(&DMA_InitStruct);
    /*UART0 DMA����ʹ�ܣ����յ�8�����ݺ󣬲���DMA�ж�*/
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;          /* �������ڴ� */
    DMA_InitStruct.DMA_CIRC   = DISABLE;              /* DMA����ģʽ��ѭ��ģʽ������Ч */
    DMA_InitStruct.DMA_PINC   = DISABLE;              /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC   = ENABLE;               /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ���,����Ч */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;       /* �������λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;       /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH2_UART0_REQ_EN; /* ͨ�� DMA_UART0_REQ_EN DMA ����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES  = 1;                    /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND  = 8;                    /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&UART0_BUFF;     /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR   = (u32)RS485_RX;        /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH2, &DMA_InitStruct);               /* ��ʼ��DMAͨ�� */
    DMA_CHx_EN(DMA_CH2, ENABLE);                      /* ʹ��DMAͨ�� */
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  TmOpState HW_485_DMA_RxCompleteCallback()
// ��д�ߣ�  F.L
// �ο����ϣ���
// ��  �ܣ�  485����DMA�����жϻص�
// ���������
// ���������
// ��  ע��  2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
TmOpState HW_485_DMA_RxCompleteCallback(UART_TypeDef *UARTx)
{
    return tmOk;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������  void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
// ��д�ߣ�  F.L
// �ο����ϣ� 
// ��  �ܣ�  �����ֽڲ�����ָ�����Ⱥ�������մ��ڵ�ָ�����Ȼ�������ֽڲ���֡ͷ������������ڴ�ʱ
//           ���DMA�Ĵ���[8:0]������д DMA_CTMS������DMA�ڲ����ִμ���
// ��������� DMA_RegTypeDef *DMAx DMA���ͼĴ���
// ��������� ��
// ��  ע��   2023��2��16��->����
////////////////////////////////////////////////////////////////////////////////////////////
void HW_485_RxDMAClearCTMS(DMA_RegTypeDef *DMAx)
{
    DMA_CHx_EN(DMAx, DISABLE);
    DMAx->DMA_CTMS &= 0xFFFFFE00;
    DMA_Init(DMAx, &DMA_InitStruct);               /* ��ʼ��DMAͨ�� */
    DMA_CHx_EN(DMAx, ENABLE);                      /* ʹ��DMAͨ�� */
}



/* USER IMPLEMENTED FUNCTIONS END */
