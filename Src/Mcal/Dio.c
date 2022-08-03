/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  FileName.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dio.h"
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

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
volatile uint32* Dio_GetBaseAddress(Dio_ChannelType ChannelId);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
volatile uint32* Dio_GetBaseAddress(Dio_ChannelType ChannelId)
{
    if (ChannelId >= PA0 && ChannelId <= PA7)
    {
        return (volatile uint32*)GPIOA_BASE_ADDRESS;
    }
    else if (ChannelId >= PB0 && ChannelId <= PB7)
    {
        return (volatile uint32*)GPIOB_BASE_ADDRESS;
    }
    else if (ChannelId >= PC0 && ChannelId <= PC7)
    {
        return (volatile uint32*)GPIOC_BASE_ADDRESS;
    }
    else if (ChannelId >= PD0 && ChannelId <= PD7)
    {
        return (volatile uint32*)GPIOD_BASE_ADDRESS;
    }
    else if (ChannelId >= PE0 && ChannelId <= PE5)
    {
        return (volatile uint32*)GPIOE_BASE_ADDRESS;
    }
    else if (ChannelId >= PF0 && ChannelId <= PF4)
    {
        return (volatile uint32*)GPIOF_BASE_ADDRESS;
    }
    else
    {
        return NULL;
    }
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId) 
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId)
{
    volatile uint32* regPtr = Dio_GetBaseAddress(ChannelId);
    if (ChannelId >= PA0 && ChannelId <= PA7)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOA_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));
    }
    else if (ChannelId >= PB0 && ChannelId <= PB7)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOB_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));
    }
    else if (ChannelId >= PC0 && ChannelId <= PC7)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOC_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));   
    }
    else if (ChannelId >= PD0 && ChannelId <= PD7)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOD_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));
    }
    else if (ChannelId >= PE0 && ChannelId <= PE5)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOE_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));
    }
    else if (ChannelId >= PF0 && ChannelId <= PF4)
    {
        return (*(volatile uint32*)((volatile uint8*)GPIOF_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC));
    }
		
		return 0;
}

void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level)
{

    volatile uint32* regPtr = Dio_GetBaseAddress(ChannelId);

    if (ChannelId >= PA0 && ChannelId <= PA7)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOA_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PA0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOA_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PA0));
        }
    }
    else if (ChannelId >= PB0 && ChannelId <= PB7)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOB_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PB0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOB_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PB0));
        }    }
    else if (ChannelId >= PC0 && ChannelId <= PC7)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOC_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PC0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOC_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PC0));
        }    }
    else if (ChannelId >= PD0 && ChannelId <= PD7)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOD_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PD0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOD_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PD0));
        }    }
    else if (ChannelId >= PE0 && ChannelId <= PE5)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOE_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PE0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOE_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PE0));
        }    }
    else if (ChannelId >= PF0 && ChannelId <= PF4)
    {
        if (Level)
        {
            SET_BIT((*(volatile uint32*)((volatile uint8*)GPIOF_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PF0));
        }
        else
        {
            CLEAR_BIT((*(volatile uint32*)((volatile uint8*)GPIOF_BASE_ADDRESS + GPIODATA_OFFSET + 0x3FC)), (ChannelId-PF0));
        }    
    }
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
