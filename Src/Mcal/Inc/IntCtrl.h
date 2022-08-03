/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  IntCtrl.h
 *       Module:  NVIC driver
 *
 *  Description:  Interfacing file for NVIC driver for TM4C123GH6PM
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IntCtrl_Cfg.h"
#include "IntCtrl_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define APINT_VECTKEY                            0x05FA0000
#define APINT_XXX_PRIGROUP_MASK                  0X00000000
#define APINT_XXY_PRIGROUP_MASK                  0X00000500
#define APINT_XYY_PRIGROUP_MASK                  0X00000600
#define APINT_YYY_PRIGROUP_MASK                  0X00000700


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
IntCtrl_FaultExceptionConfigType IntCtrl_FaultExceptionConfigSet[NUM_OF_CONFIG_FAULT_EXCEPTIONS];

IntCtrl_SystemExceptionConfigType IntCtrl_SystemExceptionConfigSet[NUM_OF_CONFIG_SYSTEM_EXCEPTIONS];

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  END OF FILE: IntCtrl.h
 *********************************************************************************************************************/