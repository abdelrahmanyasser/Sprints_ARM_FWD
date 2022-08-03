/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Gpt.h
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef GPT_H
#define GPT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Gpt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define TIMER0  (0u)
#define TIMER1  (1u)  
#define TIMER2  (2u)
#define TIMER3  (3u)
#define TIMER4  (4u)
#define TIMER5  (5u)
#define WTIMER0 (6u)
#define WTIMER1 (7u)
#define WTIMER2 (8u)
#define WTIMER3 (9u)
#define WTIMER4 (10u)
#define WTIMER5 (11u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


typedef uint8 Gpt_ChannelType; /* range 0 .. 11 timer channel */
typedef uint32 Gpt_ValueType;  /* Driver does not support concatenated 64-mode */
typedef enum
{
    GPT_MODE_NORMAL,
    GPT_MODE_SLEEP
} Gpt_ModeType;

typedef enum
{
    GPT_CH_MODE_CONTINUOUS,
    GPT_CH_MODE_ONESHOT
} Gpt_ChannelModeType;

typedef struct
{
    Gpt_ChannelType GptChannelId; 
    uint32 GptChannelTickFrequency;
    uint32 GptChannelTickValueMax;
    Gpt_ChannelModeType GptChannelMode;
    void (*GptNotification)(void);
} Gpt_ConfigType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void Gpt_Init( const Gpt_ConfigType* ConfigPtr );
void Gpt_StartTimer (Gpt_ChannelType GptChannelId, Gpt_ValueType Value);
void Gpt_StopTimer (Gpt_ChannelType GptChannelId);

void Gpt_SetCallBack(Gpt_ChannelType GptChannelId, void(*a_CallBackPtr)(void));

 
#endif  /* GPT_H */

/**********************************************************************************************************************
 *  END OF FILE: Gpt.h
 *********************************************************************************************************************/
