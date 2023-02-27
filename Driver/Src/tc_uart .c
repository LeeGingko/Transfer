/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_DMA.h"
#include "tc_uart.h"

/* USER INCLUDE FILES END */

/* USER DEFINED TYPEDEFINE BEGIN */
/* Defined Typedefine ------------------------------------------------------------------ */
struct __FILE {
    int handle;
};

/* USER DEFINED TYPEDEFINE END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */
FILE __stdout;

u8 DUART_DMA_RX[DUART_RX_LEN];

u8 rs485_TxFlag = 0;
/* USER DEFINED VARIABLES END */

/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ��������  void _sys_exit(int x)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  ��׼��֧�ֺ���
 * �����������
 * �����������
 * ��  ע��  2023��2��24��->����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void _sys_exit(int x)
{
    x = x;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ��������  int fputc(int ch, FILE *f)
 * ��д�ߣ�  F.L
 * �ο����ϣ���
 * ��  �ܣ�  �ض���printf
 * �����������
 * �����������
 * ��  ע��  2023��2��24��->����
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int fputc(int ch, FILE *f)
{
    // SEGGER_RTT_PutChar(0, ch);
    UART_SendData(UART1, ch);
    while ((UART1->STT & BIT0) == FALSE)
        ;
    return ch;
}

/*-------------------------------------------------------------------------------------------*
 �������ƣ�    void GPIO_init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *-------------------------------------------------------------------------------------------*/
void TC_UART_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* -----------���Դ���1 IO����----------- */

    // UART1_RXD  P0.7
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    // UART1_TXD  P0.6
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_7, AF4_UART);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_6, AF4_UART);
}

/*-------------------------------------------------------------------------------------------*
 �������ƣ�    void TC_UART_Init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *-------------------------------------------------------------------------------------------*/
void TC_UART_Init(void)
{
    UART_InitTypeDef UART_InitStruct;

    /* -----------���Դ���1 IO����----------- */
    TC_UART_GPIO_Init();
    /* -----------���Դ���1 �ṹ������----------- */
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate   = 57600;                                      /* ���ò�����9600 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;                         /* �������ݳ���8λ */
    UART_InitStruct.StopBits   = UART_STOPBITS_1b;                           /* ֹͣλ1λ */
    UART_InitStruct.FirstSend  = UART_FIRSTSEND_LSB;                         /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;                             /* ����żУ�� */
    UART_InitStruct.IRQEna     = UART_IRQEna_SendOver | UART_IRQEna_RcvOver; /* �����ж�ʹ�� */
    UART_Init(UART1, &UART_InitStruct);
    UART1_IF = 0xFF;
}

/*-------------------------------------------------------------------------------------------*
 �������ƣ�    void TC_UART_DMA_Init(void)
 ����������    DMA��ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ʣ�����ж�DMA_UART�Ĵ��䷽��
               ��DMA_DIR����DMA_UART�Ĵ��䷽��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *-------------------------------------------------------------------------------------------*/
void TC_UART_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStruct;

    /*UART1 DMA����ʹ�ܣ����յ�10�����ݺ󣬲���DMA�ж�*/
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;          /* �������ڴ� */
    DMA_InitStruct.DMA_CIRC   = DISABLE;              /* DMA����ģʽ��ѭ��ģʽ������Ч */
    DMA_InitStruct.DMA_PINC   = DISABLE;              /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC   = ENABLE;               /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ���,����Ч */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;       /* �������λ���� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;       /* �ڴ����λ���� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_UART1_REQ_EN; /* ͨ�� DMA_UART0_REQ_EN DMA ����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES  = 1;                    /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND  = 10;                   /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&UART1_BUFF;     /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR   = (u32)DUART_DMA_RX;    /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    DMA_CHx_EN(DMA_CH3, ENABLE); /*ʹ��DMA_CH2ͨ��*/
}

/*-------------------------------------------------------------------------------------------*
 * �������ƣ�    TmOpState TC_UARTSendBytes(UART_TypeDef *UARTx, const u8 *pData, u16 uLen)
 * ����������    ���ڶ��ֽڷ��ͺ���
 * ���������    UART_TypeDef *UARTx    ��������
 *              const u8 *pData        ����ָ��
 *              u16 uLen               ���ͳ���
 * ���������    ��
 * �� �� ֵ��    ��
 * ����˵����
 * �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *-------------------------------------------------------------------------------------------*/
TmOpState TC_UARTSendBytes(UART_TypeDef *UARTx, const u8 *pData, u16 uLen)
{
    if (NULL == pData) {
        return tmErr;
    }

    while (UARTx->IF & UART_IF_SendOver)
        ;
    for (int i = 0; i < uLen; i++) { /* C99 */
        UART_SendData(UARTx, pData[i]);
        while (rs485_TxFlag == 0)
            ;
        rs485_TxFlag = 0;
        printf("%02X ", pData[i]);
    }

    return tmOk;
}

/* USER IMPLEMENTED FUNCTIONS END */