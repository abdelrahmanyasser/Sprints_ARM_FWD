/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IntCtrl.c
 *        \brief  Implementation for interrupt control driver for TM4C123GH6PM
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IntCtrl.h"
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
static void IntCtrl_configureFaultExceptions (void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void IntCtrl_configureFaultExceptions (void)        
* \Description     : This function configures Fault handlers priority then
                     Enable or Disable the handlers according to config -
                     structure.                                                                           
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None
* \Parameters (out): None                                                      
* \Return value:   : None  
*                                                                      
*******************************************************************************/
static void IntCtrl_configureFaultExceptions (void)
{
    uint8 i, priority;

    for (i = 0; i < NUM_OF_CONFIG_FAULT_EXCEPTIONS; i++)
    {
        priority = PRIORITY_MACRO (APINT_BINARY_POINT, IntCtrl_FaultExceptionConfigSet[i].groupPri,\
         IntCtrl_FaultExceptionConfigSet[i].subgroupPri);

        switch (IntCtrl_FaultExceptionConfigSet[i].exception)
        {
            case MEM_FAULT_HANDLER:
            SYSPRI1_REG |= (priority << 5);
            if (IntCtrl_FaultExceptionConfigSet[i].state == ENABLE)
            {
                SET_BIT(SYSHNDCTRL_REG, 16);
            }
            else
            {
                CLEAR_BIT(SYSHNDCTRL_REG, 16);
            }
            break;

            case BUS_FAULT_HANDLER:
            SYSPRI1_REG |= (priority << 13);
            if (IntCtrl_FaultExceptionConfigSet[i].state == ENABLE)
            {
                SET_BIT(SYSHNDCTRL_REG, 17);
            }
            else
            {
                CLEAR_BIT(SYSHNDCTRL_REG, 17);
            }
            break;
            
            case USAGE_FAULT_HANDLER:
            SYSPRI1_REG |= (priority << 21);
            if (IntCtrl_FaultExceptionConfigSet[i].state == ENABLE)
            {
                SET_BIT(SYSHNDCTRL_REG, 18);
            }
            else
            {
                CLEAR_BIT(SYSHNDCTRL_REG, 18);
            }
            break;

            default: /* ERR in Faults exception configuration */
        }
    }
}

/******************************************************************************
* \Syntax          : void IntCtrl_configureSystemExceptions (void)        
* \Description     : Private function that configures Fault handlers priority,
                     these system handlers does not have NVIC enable\disable
                     gates to configure.                                                                        
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None
* \Parameters (out): None                                                      
* \Return value:   : None  
*                                                                      
*******************************************************************************/
static void IntCtrl_configureSystemExceptions (void) 
{
    uint8 i, priority;
    
    for (i = 0; i < NUM_OF_CONFIG_SYSTEM_EXCEPTIONS; i++)
    {
        priority = PRIORITY_MACRO (APINT_BINARY_POINT, IntCtrl_SystemExceptionConfigSet[i].groupPri,\
         IntCtrl_FaultExceptionConfigSet[i].subgroupPri);

        switch (IntCtrl_FaultExceptionConfigSet[i].exception)
        {
            case SVCALL_HANDLER:
            SYSPRI2_REG |= (priority << 29);
            break;

            case DEBUG_MONITOR_HANDLER:
            SYSPRI3_REG |= (priority << 5);
            break;

            case PENDSV_HANDLER:
            SYSPRI3_REG |= (priority << 21);
            break;

            case SYSTICK_HANDLER:
            SYSPRI3_REG |= (priority << 29);
            break;

            default: /* ERR in System exception configuration */
        }
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void IntCtrl_init(void)        
* \Description     : This function initializes NVIC\SCB module based on
*                    configuration provided by user in Cfg file.                                                                            
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None
* \Parameters (out): None                                                      
* \Return value:   : None  
*                                                                      
*******************************************************************************/
void IntCtrl_init (void)
{
    uint8 i = 0;
    uint8 priority = 0;

    /* Configure grouping\subgrouping system in APINT register in SCB */
    #if (APINT_BINARY_POINT == B_XXX)
    /* CONFIG XXX */
    APINT_REG = APINT_VECTKEY | APINT_XXX_PRIGROUP_MASK;

    #elif (APINT_BINARY_POINT == B_XXY)
    /* CONFIG XXY */
    APINT_REG = APINT_VECTKEY | APINT_XXY_PRIGROUP_MASK;

    #elif (APINT_BINARY_POINT == B_XYY)
    /* CONFIG XYY */
    APINT_REG = APINT_VECTKEY | APINT_XYY_PRIGROUP_MASK;

    #elif (APINT_BINARY_POINT == B_YYY)
    /* CONFIG YYY */
    APINT_REG = APINT_VECTKEY | APINT_YYY_PRIGROUP_MASK;

    #endif


    /* TODO: Assign Group\Subgroup priority in NVIC_PRIx NVIC and SCB_SYSPRIx Registers */ 
    /* TODO: Enable\Disable based on user configurations in NVIC_ENx and SCB_Sys Registers */

    IntCtrl_configureFaultExceptions();

    IntCtrl_configureSystemExceptions();

    /* TODO: Continue interrupt handlers prioriy configuration & En\Disabling */

}

/**********************************************************************************************************************
 *  END OF FILE: IntCtrl.c
 *********************************************************************************************************************/