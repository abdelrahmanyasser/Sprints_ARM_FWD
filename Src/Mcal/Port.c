/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Port.c
 *        \brief  Port driver for TMC4123GH6PM
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Port.h"
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
static void PORT_setPinAnalog ( uint8 PinID );
static void PORT_setPinDigital ( uint8 PinID );

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

static void PORT_setPinAnalog ( uint8 PinID )
{
    CLEAR_BIT(GPIODEN_REG, ConfigPtr->PinID);
    SET_BIT(GPIOAMSEL_REG, ConfigPtr->PinID);
}

static void PORT_setPinDigital ( uint8 PinID )
{
    CLEAR_BIT(GPIOAMSEL_REG, ConfigPtr->PinID);
    SET_BIT(GPIODEN_REG, ConfigPtr->PinID);    
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void Port_Init( const Port_ConfigType* ConfigPtr )        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) :                                                           
* \Parameters (out): None                                                      
* \Return value:   : None                                                      
*******************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr )
{
    /* TODO: implement init */
    /*
     * 1. loop through configured pins.
     * 2. put regPtr to GPIOx base address.
     * 3. switch case configurations using specified regPtr.
     */
    uint32 regPtr = NULL;
    uint8 i;

    for (i = 0; i < PORT_NUM_OF_CONFIG_PINS; i++)
    {
        /* Select current register base address */
        switch ( ConfigPtr->PortID )
        {
            case PORT_A:
            regPtr = GPIOA_BASE_ADDRESS;
            break;

            case PORT_B:
            regPtr = GPIOB_BASE_ADDRESS;
            break;

            case PORT_C:
            regPtr = GPIOC_BASE_ADDRESS;
            break;

            case PORT_D:
            regPtr = GPIOD_BASE_ADDRESS;
            break;

            case PORT_E:
            regPtr = GPIOE_BASE_ADDRESS;
            break;

            case PORT_F:
            regPtr = GPIOF_BASE_ADDRESS;
            break;

            default: /* ERR: Skip this pin config */
            continue;
        }

        /* Enable GPIO clock */
        RCGCGPIO_REG |= ( 1 << ConfigPtr->PortID );

        /* Commit protected pins */
        if ( (ConfigPtr->PortID == PORT_C) && (ConfigPtr->PinID <= PIN_PC3) )
        {
            /* This is JTAG pins!! Escape this pin config iteration */
            continue;
        }
        else if ( ((ConfigPtr->PortID == PORT_D) && (ConfigPtr->PinID == PIN_PD7)) ||\
                  ((ConfigPtr->PortID == PORT_F) && (ConfigPtr->PinID == PIN_PF0)) )
        {
            /* This is NMI pins, unlock and commit before pin configurtion */
            *((volatile uint32*)((volatile uint8*)regPtr + GPIOLOCK_OFFSET)) = GPIOLOCK_KEY;
            SET_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOCR_OFFSET)), ConfigPtr->PinID );     
        }
        else
        {
            /* Do nothing! Other Pins are commited by default */
        }
        (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET))
        (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET))

        /* Check Alternative function */
        if ( ConfigPtr->PinMode == DIO )
        {
            SET_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET)), ConfigPtr->PinID );
            (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) &= ~( 0xF << ((ConfigPtr->PinID)*4) );
            PORT_setPinDigital( ConfigPtr->PinID );
        }
        else
        {
            CLEAR_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET)), ConfigPtr->PinID );
            if ( ConfigPtr->PortID == PORT_A )
            {
                switch ( ConfigPtr->PinID )
                {
                    case PIN_PA0:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U0Rx << ((ConfigPtr->PinID)*4) );
                        break;
                        case CAN1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN1Rx << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA1:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U0Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        case CAN1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN1Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA2:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI0Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Clk << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA3:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI0Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Fss << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */
                    }
                    break;

                    case PIN_PA4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Rx << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */
                    }
                    break;

                    case PIN_PA5:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */
                    }
                    break;

                    case PIN_PA6:
                    switch ( ConfigPtr->PinMode )
                    {
                        case I2C1SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C1SCL << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM2 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA7:
                    switch ( ConfigPtr->PinMode )
                    {
                        case I2C1SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C1SDA << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM3 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;
                }

                /* All port_A pinmodes are digital functions */
                /* Set pin as a digital pin */
                Port_setPinDigital(ConfigPtr->PinID);
            }
            else if ( ConfigPtr->PortID == PORT_B )
            {
                switch ( ConfigPtr->PinID )
                {
                    case PIN_PB0:
                    switch ( ConfigPtr->PinMode )
                    {
                        case USB0ID:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0ID << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case T2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB1:
                    switch ( ConfigPtr->PinMode )
                    {
                        case USB0VBUS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0VBUS << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case T2CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB2:
                    switch ( ConfigPtr->PinMode )
                    {
                        case I2C0SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C0SCL << ((ConfigPtr->PinID)*4) );
                        break;
                        case T3CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T3CCP0 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr->PinID); /* All PB2 modes are digital */
                    break;
                    
                    case PIN_PB3:
                    switch ( ConfigPtr->PinMode )
                    {
                        case I2C0SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C0SDA << ((ConfigPtr->PinID)*4) );
                        break;
                        case T3CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T3CCP1 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr->PinID); /* All PB3 modes are digital */
                    break;

                    case PIN_PB4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN10:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN10 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI2Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Clk << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM2 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case T1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB5:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN11:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN11 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI2Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Fss << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM3 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case T1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB6:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI2Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Rx << ((ConfigPtr->PinID)*4) );
                        break;
                        case M0PWM0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP0 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr->PinID); /* All PB6 modes are digital */
                    break;

                    case PIN_PB7:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI2Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        case M0PWM1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM1 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP1 << ((ConfigPtr->PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr->PinID); /* All PB7 modes are digital */
                    break;
                }
            }
            else if ( ConfigPtr->PortID == PORT_C )
            {
                switch ( ConfigPtr->PinID )
                {
                    /* Firts 4 pins PC0 ... PC3 are JTAG pins and are not configurable !! */
                    case PIN_PC4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case C1_NEG:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C1_NEG << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U4Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U4Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case U1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM6 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case IDX1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT0CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case U1RTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1RTS << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC5:
                    switch ( ConfigPtr->PinMode )
                    {
                        case C1_POS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C1_POS << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U4Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U4Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case U1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM7 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case PhA1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT0CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case U1CTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1CTS << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC6:
                    switch ( ConfigPtr->PinMode )
                    {
                        case C0_POS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C0_POS << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U3Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U3Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case PhB1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT1CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC7:
                    switch ( ConfigPtr->PinMode )
                    {
                        case C0_NEG:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C0_NEG << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U3Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U3Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;

                        case WT1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT1CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case USB0PFLT:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0PFLT << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            }
            else if ( ConfigPtr->PortID == PORT_D )
            {
                switch ( ConfigPtr->PinID )
                {
                    case PIN_PD0:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN7 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI3Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Clk << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case SSI1Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Clk << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case I2C3SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C3SCL << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM6 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M1PWM0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT2CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD1:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN6 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI3Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Fss << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case SSI1Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Fss << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case I2C3SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C3SDA << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM7 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M1PWM1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT2CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT2CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD2:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN5:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN5 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI3Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case SSI1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT3CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT3CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD3:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN4:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN4 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case SSI3Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case SSI1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case IDX0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT3CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT3CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case USB0PFLT:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0PFLT << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case USB0DM:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0DM << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U6Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U6Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT4CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT4CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD5:
                    switch ( ConfigPtr->PinMode )
                    {
                        case USB0DP:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0DP << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U6Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U6Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT4CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT4CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD6:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U2Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U2Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case PhA0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT5CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT5CCP0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD7:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U2Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U2Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case PhB0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB0 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case WT5CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT5CCP1 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case NMI:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( NMI << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            }
            else if ( ConfigPtr->PortID == PORT_E )
            {
                switch ( ConfigPtr->PinID )
                {
                    case PIN_PE0:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN3 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U7Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U7Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE1:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN2 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U7Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U7Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE2:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN1 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE3:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN0 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                    
                    case PIN_PE4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN9:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN9 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U5Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U5Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case I2C2SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C2SCL << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM4:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM4 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M1PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM2 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE5:
                    switch ( ConfigPtr->PinMode )
                    {
                        case AIN8:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( AIN8 << ((ConfigPtr->PinID)*4) );
                        PORT_setPinAnalog(ConfigPtr->PinID);
                        break;
                        case U5Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U5Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case I2C2SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C2SDA << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M0PWM5:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM5 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case M1PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM3 << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx << ((ConfigPtr->PinID)*4) );
                        Port_setPinDigital(ConfigPtr->PinID);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            }
            else if ( ConfigPtr->PortID == PORT_F )
            {
                switch ( ConfigPtr->PinID )
                {
                    case PIN_PF0:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U1RTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1RTS << ((ConfigPtr->PinID)*4) );
                        break;
                        case SSI1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Rx << ((ConfigPtr->PinID)*4) );
                        break;

                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM4:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM4 << ((ConfigPtr->PinID)*4) );
                        break;
                        case PhA0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case NMI:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( NMI << ((ConfigPtr->PinID)*4) );
                        break;
                        case C0o:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C0o << ((ConfigPtr->PinID)*4) );
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                    
                    case PIN_PF1:
                    switch ( ConfigPtr->PinMode )
                    {
                        case U1CTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1CTS << ((ConfigPtr->PinID)*4) );
                        break;
                        case SSI1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM5:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM5 << ((ConfigPtr->PinID)*4) );
                        break;
                        case PhB0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP1 << ((ConfigPtr->PinID)*4) );
                        break;
                        case C1o:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C1o << ((ConfigPtr->PinID)*4) );
                        break;
                        case TRD1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRD1 << ((ConfigPtr->PinID)*4) );
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF2:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI1Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Clk << ((ConfigPtr->PinID)*4) );
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM6 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case TRD0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRD0 << ((ConfigPtr->PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF3:
                    switch ( ConfigPtr->PinMode )
                    {
                        case SSI1Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Fss << ((ConfigPtr->PinID)*4) );
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx << ((ConfigPtr->PinID)*4) );
                        break;
                        case M1PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM7 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP1 << ((ConfigPtr->PinID)*4) );
                        break;
                        case TRCLK:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRCLK << ((ConfigPtr->PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF4:
                    switch ( ConfigPtr->PinMode )
                    {
                        case M1FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1FAULT0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case IDX0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case T2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP0 << ((ConfigPtr->PinID)*4) );
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN << ((ConfigPtr->PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;
                }
            /* All port_F pinmodes are digital functions */
            /* Set pin as a digital pin */
            Port_setPinDigital(ConfigPtr->PinID);
            }

        } /* END else */

        /* Set pin drive current as 2mA, 4mA or 8mA */
        switch (ConfigPtr->PinOutputCurrent)
        {
            case PORT_2MA:
            SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODR2R_OFFSET)), ConfigPtr->PinID );
            break;

            case PORT_4MA:
            SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODR4R_OFFSET)), ConfigPtr->PinID );
            break;

            case PORT_8MA:
            SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODR8R_OFFSET)), ConfigPtr->PinID );
            break;

            default: /* ERR */ ;
        }

        /* Check direction if output: set default level
         *                    input : set PUR & PDR
         */
        if (ConfigPtr->PinDirection == PIN_OUTPUT)
        {
            /* Set pin level before direction to avoid glitches on pin */
            if (ConfigPtr->PinLevel)
            {
                SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODATA_OFFSET)), ConfigPtr->PinID );
            }
            else
            {
                CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODATA_OFFSET)), ConfigPtr->PinID );
            }

            /* Set direction to output */
            SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODIR_OFFSET)), ConfigPtr->PinID );
        }
        else if (ConfigPtr->PinDirection == PIN_INPUT)
        {
            /* Set internal pull resistors before direction */
            switch (ConfigPtr->PinInternalAttach)
            {
                case PORT_PIN_PULL_OFF:
                CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr->PinID );
                CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr->PinID );
                break;

                case PORT_PIN_PULL_UP:
                CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr->PinID );
                SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr->PinID );
                break;

                case PORT_PIN_PULL_DOWN:
                CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr->PinID );
                SET_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr->PinID );
                break;

                default: /* ERR */ ;
            }

            /* Set direction */
            CLEAR_BIT(((volatile uint32*)((volatile uint8*)regPtr + GPIODIR_OFFSET)), ConfigPtr->PinID );
        }

    } /* END for loop */

} /* END PORT_Init() */

/**********************************************************************************************************************
 *  END OF FILE: Port.c
 *********************************************************************************************************************/
