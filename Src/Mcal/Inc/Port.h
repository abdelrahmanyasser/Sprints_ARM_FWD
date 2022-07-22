/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Port.h
 *       Module:  GPIO module
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef <PORT_H>
#define <PORT_H>

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Port_PinType;           /* Pin range 0 .. 7 */

typedef enum
{
    PORT_PIN_IN,                      /* Pin input type */
    PORT_PIN_OUT                      /* Pin output type */
}Port_PinDirectionType;

typedef enum
{
    /* TODO: Table of Alternative functions for gpio pins */

}Port_PinModeType;

typedef enum
{
    PORT_PIN_LOW,                     /* Pin level low */
    PORT_PIN_HIGH                     /* Pin level high */
}Port_PinLevelType;

typedef enum
{
    PORT_PIN_PULL_OFF,                /* Pin internal pull resistors off */
    PORT_PIN_PULL_UP,                 /* Pin internal pull up resistor */
    PORT_PIN_PULL_DOWN                /* Pin internal pull down resistor */
}Port_PinInternalAttachType;

typedef enum
{
    PORT_2MA,                         /* Pin current 2mA */
    PORT_4MA,                         /* Pin current 4mA */
    PORT_8MA                          /* Pin current 8mA */
}Port_PinOutputCurrentType;

typedef struct
{
    Port_PinModeType PortPinMode;
    Port_PinLevelType PortPinLevelValue;
    Port_PinDirectionType PortPinDirection;
    Port_PinInternalAttachType PortPinInternalAttach;
    Port_PinOutputCurrentType PortPinOutputCurrent;
}Port_ConfigType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr );
 
#endif  /* PORT_H */

/**********************************************************************************************************************
 *  END OF FILE: Port.h
 *********************************************************************************************************************/
