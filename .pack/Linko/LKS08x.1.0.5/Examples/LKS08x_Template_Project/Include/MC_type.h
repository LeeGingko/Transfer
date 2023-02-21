

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_TYPE_H
#define __MC_TYPE_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

		
typedef struct 
{
  s16 qI_Component1;
  s16 qI_Component2;
} Curr_Components;

typedef struct 
{
  s16 qV_Component1;
  s16 qV_Component2;
} Volt_Components;

typedef struct
{
  s16 hCos;
  s16 hSin;
} Trig_Components;

typedef struct 
{  
  s16 hKp_Gain;
  u16 hKp_Divisor;
  s16 hKi_Gain;
  u16 hKi_Divisor;  
  s16 hLower_Limit_Output;     //Lower Limit for Output limitation
  s16 hUpper_Limit_Output;     //Lower Limit for Output limitation
  s32 wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wIntegral;
  // Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled in
  s16 hKd_Gain;
  u16 hKd_Divisor;
  s32 wPreviousError;
} PID_Struct_t;

typedef struct 
{  
  s16 hKp_Gain;
  u16 hKp_Divisor;
  s16 hKi_Gain;
  u16 hKi_Divisor;  

  s16 hLower_Limit_Output;     //Lower Limit for Output limitation
  s16 hUpper_Limit_Output;     //Lower Limit for Output limitation
  s32 wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  // Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled in
  s32 wIntegral;
  s16 hKd_Gain;
  u16 hKd_Divisor;
  s32 wPreviousError;
} PD_Struct_t;

typedef struct
{
   s16 ProportionalGain;
   s16 ProportionalGainScale;
   s16 IntegralGain;
   s16 IntegralGainScale;
   s16 PositivePILimit;
   s16 NegativePILimit;
   s32 IntegralPortionK_1;
   s16 integral_separate_point;
   s32 wLower_Limit_Integral;   /* Lower Limit for Integral term limitation */
   s32 wUpper_Limit_Integral;   /* Lower Limit for Integral term limitation */

   u16 FF_voltage;
   u16 PI_flg;

}Dump_PIparams;

typedef struct 
{
	 s32 yk_1;
	 u16 coef;
}RC_t;


typedef struct {
	u32 hCapture;
        s8 bDirection;
	} SpeedMeas_s;

typedef enum 
{
IDLE, INIT, START, RUN, STOP, BRAKE, WAIT, FAULT
} SystStatus_t;

typedef enum 
{
NO_FAULT, OVER_VOLT, UNDER_VOLT
} BusV_t;

/* Exported constants --------------------------------------------------------*/
#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#define S16P80     ((s16)26213)
#define S16P65     ((s16)25000)
#define S16P50     ((s16)13000)




#define HAlL_ERROR           BIT0  /* »ô¶û¹ÊÕÏ */
#define SHORT_ERROR          BIT1  /* ¶ÌÂ·¹ÊÕÏ */
#define LOW_VOL_ERROR        BIT2  /* µÍÑ¹¹ÊÕÏ */
#define HIG_VOL_ERROR        BIT3  /* ¹ýÑ¹¹ÊÕÏ */
#define OV_TEMPER_ERROR      BIT4  /* ¹ýÎÂ¹ÊÕÏ */
#define MOSFET_ERROR         BIT5  /* MOSFET¹ÊÕÏ */
#define OVER_SPEED_ERR       BIT6  /* ¹ýËÙ¶È¹ÊÕÏ */
#define AGL_ROLL_ERROR       BIT8  /* ´«¸ÐÆ÷Î»ÖÃÆ«²îÌ«´ó */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MC_TYPE_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
