/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Gpt.c
 *        \brief  Implementation file for Gp driver for TM4C123GH6PM
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Gpt.h"
#include "Mcu_Hw.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
static void(* volatile CallBackPtr[12])(void) = {NULL};

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
volatile uint32* Gpt_GetBaseAddress(Gpt_ChannelType GptChannelId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
volatile uint32* Gpt_GetBaseAddress(Gpt_ChannelType GptChannelId)
{
    switch (GptChannelId)
    {
        case TIMER0:
        return (volatile uint32*)TIMER0_BASE_ADDRESS;
        case TIMER1:
        return (volatile uint32*)TIMER1_BASE_ADDRESS;
        case TIMER2:
        return (volatile uint32*)TIMER2_BASE_ADDRESS;
        case TIMER3:
        return (volatile uint32*)TIMER3_BASE_ADDRESS;
        case TIMER4:
        return (volatile uint32*)TIMER4_BASE_ADDRESS;
        case TIMER5:
        return (volatile uint32*)TIMER5_BASE_ADDRESS;
        case WTIMER0:
        return (volatile uint32*)WTIMER0_BASE_ADDRESS;
        case WTIMER1:
        return (volatile uint32*)WTIMER1_BASE_ADDRESS;
        case WTIMER2:
        return (volatile uint32*)WTIMER2_BASE_ADDRESS;
        case WTIMER3:
        return (volatile uint32*)WTIMER3_BASE_ADDRESS;
        case WTIMER4:
        return (volatile uint32*)WTIMER4_BASE_ADDRESS;
        case WTIMER5:
        return (volatile uint32*)WTIMER5_BASE_ADDRESS;

        default: return NULL;
    }
}

void Gpt_EnableModuleClock(Gpt_ChannelType GptChannelId)
{
    if (GptChannelId >= TIMER0 && GptChannelId <= TIMER5)
    {
        SET_BIT(RCGCTIMER_REG, GptChannelId);
    }
    else if (GptChannelId >= WTIMER0 && GptChannelId <= WTIMER5)
    {
        SET_BIT(RCGCWTIMER_REG, (GptChannelId - WTIMER0));
    }
    return;
}

void Gpt_DisableNotification(Gpt_ChannelType GptChannelId)
{
    volatile uint32* regPtr = Gpt_GetBaseAddress(GptChannelId);
    /* Set interrupt bits */
    CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMIMR_OFFSET)), 0); /* Disable Timer A interrupts only */
}

void Gpt_EnableNotification(Gpt_ChannelType GptChannelId)
{
    volatile uint32* regPtr = Gpt_GetBaseAddress(GptChannelId);
    /* Set interrupt bits */
    SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMIMR_OFFSET)), 0);   /* Enable Timer A interrupts only */
}

void Gpt_StartTimer (Gpt_ChannelType GptChannelId, Gpt_ValueType Value)
{
    volatile uint32* regPtr = Gpt_GetBaseAddress(GptChannelId);

    /* Load interval load reg */
    *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAILR_OFFSET) = Value;

    SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMCTL_OFFSET)), 0); /* Enable Timer A */
}

void Gpt_StopTimer (Gpt_ChannelType GptChannelId)
{
        volatile uint32* regPtr = Gpt_GetBaseAddress(GptChannelId);

        /* Disable Timer Channel */
        CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMCTL_OFFSET)), 0); /* Disable Timer A */
        CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMCTL_OFFSET)), 8); /* Disable Timer B */
    
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void Gpt_Init( const Gpt_ConfigType* ConfigPtr )        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Gpt_Init( const Gpt_ConfigType* ConfigPtr )
{
    /** This Function Init array of GPT channels into periodic\one shot mode
     * Sequence is as follow:
     * 1. Ensure the timer is disabled (the TnEN bit in the GPTMCTL register is cleared) before making
     * any changes.
     * 2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
     * 3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
     *      a. Write a value of 0x1 for One-Shot mode.
     *      b. Write a value of 0x2 for Periodic mode.
     * 4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
     * to select whether to capture the value of the free-running timer at time-out, use an external
     * trigger to start counting, configure an additional trigger or interrupt, and count up or down.
     * 5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
     */
	
    uint8 i = 0;

    volatile uint32* regPtr = NULL;
    for (i = 0; i < GPT_NUM_OF_CONFIG_CHANNELS; i++)
    {
        /* Point to Channel Base address */
        regPtr = Gpt_GetBaseAddress(ConfigPtr[i].GptChannelId);
        
        /* Enable Module clock to access registers and avoid Bus fault */
        Gpt_EnableModuleClock(ConfigPtr[i].GptChannelId);

        /* Ensure Timer Channel is disabled */
        Gpt_StopTimer (ConfigPtr[i].GptChannelId);

        
        /* Driver doesnot support concatenated mode, Set Cfg register with mode 0x4 */
        *(volatile uint32*)((volatile uint8*)regPtr + GPTMCFG_OFFSET) = 0x4;
        
        /* Select one-shot mode or periodic mode*/
        if(ConfigPtr[i].GptChannelMode == GPT_CH_MODE_ONESHOT)
        {
            *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAMR_OFFSET) = 0x1;
        }
        else if (ConfigPtr[i].GptChannelMode == GPT_CH_MODE_CONTINUOUS)
        {
            *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAMR_OFFSET) = 0x2;
        }

        /* Set prescaler */
        if ( (ConfigPtr[i].GptChannelTickFrequency) != 0 )
        {
            // *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = (GPT_SYSTEM_CLOCK / (unsigned long)(ConfigPtr[i].GptChannelTickFrequency));
            //*(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = 16;
            if (ConfigPtr[i].GptChannelId <= TIMER5)
            {
                if ( ((uint32)GPT_SYSTEM_CLOCK / (ConfigPtr[i].GptChannelTickFrequency) )  <= 0xFF )
                {
                    *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = ((uint32)GPT_SYSTEM_CLOCK / (uint32)(ConfigPtr[i].GptChannelTickFrequency));
                }
                else
                {
                    /* Set up max prescaler 0xFF */
                    *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = 0xFF;
                }
            }
            else if (ConfigPtr[i].GptChannelId <= WTIMER5)
            {
                if ( ((uint32)GPT_SYSTEM_CLOCK / (ConfigPtr[i].GptChannelTickFrequency) )  <= 0xFFFF)
                {
                    *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = ((uint32)GPT_SYSTEM_CLOCK / (uint32)(ConfigPtr[i].GptChannelTickFrequency));
                }
                else
                {
                    /* Set up max prescaler 0xFF */
                    *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAPR_OFFSET) = 0xFFFF;
                }
            }
        }
				
        /* Load initial interval load reg */
        if (ConfigPtr[i].GptChannelId <= TIMER5)
        {
            *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAILR_OFFSET) = 0xFFFF;
        }
        else if (ConfigPtr[i].GptChannelId <= WTIMER5)
        {
            *(volatile uint32*)((volatile uint8*)regPtr + GPTMTAILR_OFFSET) = 0xFFFFFFFF;
        }

				Gpt_EnableNotification(ConfigPtr[i].GptChannelId);
				
				#if(0)
        if (ConfigPtr[i].GptNotification != NULL)
        {
            /* Set Callback ptr */
            CallBackPtr[ConfigPtr[i].GptChannelId] = ConfigPtr[i].GptNotification;
        
            /* Enable interrupt for TimerA */
            Gpt_EnableNotification(ConfigPtr[i].GptChannelId);
        }
        else
        {
            Gpt_DisableNotification(ConfigPtr[i].GptChannelId);
        }
        #endif
        //SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPTMCTL_OFFSET)), 0); /* Disable Timer A */

    }
}



void Gpt_SetCallBack(Gpt_ChannelType GptChannelId, void(*a_CallBackPtr)(void))
{
			CallBackPtr[GptChannelId] = a_CallBackPtr;
}

void TIMER0A_Handler (void)
{
    /* Call the callback function */
    CallBackPtr[TIMER0]();
    
    /* Clear interrupt flag */
	  *(volatile uint32*)((volatile uint8*)TIMER0_BASE_ADDRESS + GPTMICR_OFFSET) = 0x1;
}

void WTIMER0A_Handler (void)
{
    /* Call the callback function */
    CallBackPtr[WTIMER0]();
    
    /* Clear interrupt flag */
	  *(volatile uint32*)((volatile uint8*)WTIMER0_BASE_ADDRESS + GPTMICR_OFFSET) = 0x1;
}

/* TODO .. Rest of handlers */

/**********************************************************************************************************************
 *  END OF FILE: Gpt.c
 *********************************************************************************************************************/
