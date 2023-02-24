/* USER INCLUDE FILES BEGIN */
/* Included Files ------------------------------------------------------------------------ */
#include "lks32mc08x_gpio.h"
#include "lks32mc08x_can.h"
#include "lks32mc08x_DMA.h"
#include "hw_can.h"
/* USER INCLUDE FILES END */

/* USER DEFINED VARIABLES BEGIN */
/* Defined Variables --------------------------------------------------------------------- */

/* USER DEFINED VARIABLES END */
u8 CAN_Rx[55] = {0};
/* USER DEFINED FROTOTYPES BEGIN */
/* Defined Prototypes -------------------------------------------------------------------- */

/* USER DEFINED FROTOTYPES END */

/* USER IMPLEMENTED FUNCTIONS BEGIN */
/* Implemented Functions ----------------------------------------------------------------- */
/*--------------------------------------------------------------------------------------------*
 �������ƣ�    void HW_CAN_GPIO_init(void)
 ����������    CAN GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *--------------------------------------------------------------------------------------------*/

void HW_CAN_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct); // ��ʼ���ṹ��

    // ����LED1��P0.6  LED2 P0.7 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    /*P0.7 CAN TX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    /*P0.6 CAN RX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_6, AF10_CAN);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_7, AF10_CAN);
}

/*--------------------------------------------------------------------------------------------*
 ��������: void HW_CAN_init(void)
 ��������: CAN��ʼ������,����2.0BЭ��
 �����ı�:  Can������      BTR0    	BTR1
            1Mbps 		  0x05      0x14
            800Kbps 	  0x05      0x16
            666Kbps 	  0x85 	    0xB6
            500Kbps 	  0x05      0x1C
            400Kbps 	  0x05	    0xFA
            250Kbps 	  0x0B 	    0x1C
            200Kbps 	  0x05 	    0xFA
            125Kbps 	  0x17      0x1C
            100Kbps 	  0x1D      0x1C
            83.33Kbps 	0x17      0x6F
            80Kbps 		  0x97      0xFF
            66.66Kbps 	0x1D      0x6F
            50Kbps 		  0x3B      0x1C
            40Kbps 		  0xAF      0xFF
 �������: ��
 �������: ��
 ����ֵ:   ��
 ����˵��:
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/5/3      V1.0           HuangMG            ����
*--------------------------------------------------------------------------------------------*/
void HW_CAN_Init(void)
{
    CAN_InitTypeDef CAN_InitStruct;

    /*Can������:500Kbps*/
    CAN_StructInit(&CAN_InitStruct);
    CAN_InitStruct.CAN_Btr0       = 0x05;
    CAN_InitStruct.CAN_Btr1       = 0x1C;
    CAN_InitStruct.CAN_DMAEn      = ENABLE;            /*DMA��������*/
    CAN_InitStruct.CAN_WorkMode   = CAN_SELFTEST_MODE; /*CAN�����շ�����ģʽ*/
    CAN_InitStruct.CAN_ErrWarThre = 50;                /*�����뾯������ֵΪ50*/
    CAN_InitStruct.IRQEna         = CAN_IE_RFIFONOEMPTY | CAN_IE_TXDONE | CAN_IE_WERR;
    CAN_Init(CAN, &CAN_InitStruct);
    /*�����˲��� ID:0x5A ,��־֡��BYTE1:0x81,BYTE:0x23*/
    SFF_ID_Filter(0X2F, 0x00, 0x0, 0x0, 0x81, 0x00, 0x23, 0x00);
    HW_CAN_GPIO_Init();
    HW_CAN_DMA_RX(0, 2, 8, CAN_Rx);
}

/*--------------------------------------------------------------------------------------------*
 �������ƣ�    void CAN_DMA_TX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
 ����������    CAN_DMA���ݷ��ͺ���
 ���������    ide��0��׼/1��չ֡
              frame_number����Ҫ���͵�֡����
              frame_size��ÿ֡�������ݳ��ȣ�0-8 BYTE�����ݣ�����ID�ȡ�
              memaddr���������ݻ�������ַ
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/5/4      V1.0           HuangMG            ����
 *--------------------------------------------------------------------------------------------*/
void HW_CAN_DMA_TX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr)
{
    u8 len = 0;
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    if (ide) /*��չ֡*/
    {
        len = 5 + frame_size;
    } else { /*��׼֡*/
        len = 3 + frame_size;
    }
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;           /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR    = MEMORY2PERI;        /* �ڴ������� */
    DMA_InitStruct.DMA_CIRC   = DISABLE;            /* �ر�ѭ������ģʽ */
    DMA_InitStruct.DMA_PINC   = ENABLE;             /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC   = ENABLE;             /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ��������ڵ�ַһ��������,����Ч */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;     /* �������λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;     /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN; /* CAN DMA����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES  = len;                /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND  = frame_number;       /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&CAN_TXRX0;    /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR   = (u32)memaddr;       /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    DMA_CHx_EN(DMA_CH3, ENABLE); /*ʹ��DMA_CH3ͨ��*/
}

/*--------------------------------------------------------------------------------------------*
 �������ƣ�    void CAN_DMA_RX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
 ����������    CAN_DMA���ݽ��պ���
 ���������    ide��0��׼/1��չ֡
              frame_number����Ҫ���յ�֡����
              frame_size��ÿ֡�������ݳ��ȣ�0-8 BYTE�����ݣ�����ID�ȡ�
              memaddr���������ݻ�������ַ
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/5/4      V1.0           HuangMG            ����
 *--------------------------------------------------------------------------------------------*/
void HW_CAN_DMA_RX(u8 ide, u8 frame_number, u8 frame_size, u8 *memaddr)
{
    u8 len = 0, rgsr = 0;
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    if (ide) /*��չ֡*/
    {
        len = 5 + frame_size;
    } else { /*��׼֡*/
        len = 3 + frame_size;
    }
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;           /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR    = PERI2MEMORY;        /* �������ڴ� */
    DMA_InitStruct.DMA_CIRC   = DISABLE;            /* DMA����ģʽ��ѭ��ģʽ������Ч */
    DMA_InitStruct.DMA_PINC   = ENABLE;             /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC   = ENABLE;             /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ��������ڵ�ַһ��������,����Ч */
    DMA_InitStruct.DMA_PBTW   = DMA_BYTE_TRANS;     /* �������λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW   = DMA_BYTE_TRANS;     /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN; /* CAN DMA����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES  = len;                /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND  = frame_number;       /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR   = (u32)&CAN_TXRX0;    /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR   = (u32)memaddr;       /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
    /*******************�������DMA���ݰ��˱�־λ����ֹ��������ݴ���DMA����*****************/
    rgsr = CAN_GetIRQFlag(); /*��ȡ�ж�״̬�Ĵ���*/
    rgsr = rgsr;             /*��ֹ���뱨����*/
    /*--------------------------------------------------------------------------------------------*/
    DMA_CHx_EN(DMA_CH3, ENABLE); /*ʹ��DMA_CH3ͨ��*/
}

/* USER IMPLEMENTED FUNCTIONS END */
