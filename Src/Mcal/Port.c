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
static void Port_setPinAnalog ( const Port_ConfigType* ConfigPtr );
static void Port_setPinDigital ( const Port_ConfigType* ConfigPtr );

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

static void Port_setPinAnalog ( const Port_ConfigType* ConfigPtr )
{
    volatile uint32* regPtr = NULL;

    /* Select current register base address */
    switch ( ConfigPtr->PortID )
    {
        case PORT_A:
        regPtr = (volatile uint32*)GPIOA_BASE_ADDRESS;
        break;

        case PORT_B:
        regPtr = (volatile uint32*)GPIOB_BASE_ADDRESS;
        break;

        case PORT_C:
        regPtr = (volatile uint32*)GPIOC_BASE_ADDRESS;
        break;

        case PORT_D:
        regPtr = (volatile uint32*)GPIOD_BASE_ADDRESS;
        break;

        case PORT_E:
        regPtr = (volatile uint32*)GPIOE_BASE_ADDRESS;
        break;

        case PORT_F:
        regPtr = (volatile uint32*)GPIOF_BASE_ADDRESS;
        break;

        default: /* ERR */;
    }
    CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIODEN_OFFSET)), ConfigPtr->PinID);
    SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIOAMSEL_OFFSET)), ConfigPtr->PinID);
}

static void Port_setPinDigital ( const Port_ConfigType* ConfigPtr )
{
    volatile uint32* regPtr = NULL;

    /* Select current register base address */
    switch ( ConfigPtr->PortID )
    {
        case PORT_A:
        regPtr = (volatile uint32*)GPIOA_BASE_ADDRESS;
        break;

        case PORT_B:
        regPtr = (volatile uint32*)GPIOB_BASE_ADDRESS;
        break;

        case PORT_C:
        regPtr = (volatile uint32*)GPIOC_BASE_ADDRESS;
        break;

        case PORT_D:
        regPtr = (volatile uint32*)GPIOD_BASE_ADDRESS;
        break;

        case PORT_E:
        regPtr = (volatile uint32*)GPIOE_BASE_ADDRESS;
        break;

        case PORT_F:
        regPtr = (volatile uint32*)GPIOF_BASE_ADDRESS;
        break;

        default: /* ERR */;
    }
    CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIOAMSEL_OFFSET)), ConfigPtr->PinID);
    SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIODEN_OFFSET)), (ConfigPtr->PinID));    
		//*(volatile uint32*)((volatile uint8*)regPtr + GPIODEN_OFFSET) |= (1 << (ConfigPtr->PinID));
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
    volatile uint32* regPtr = NULL;
    uint8 i = 0;
	  uint32 delay_3clocks;

    for (i = 0; i < PORT_NUM_OF_CONFIG_PINS; i++)
    {
        /* Select current register base address */
        switch ( ConfigPtr[i].PortID )
        {
            case PORT_A:
            regPtr = (volatile uint32*)GPIOA_BASE_ADDRESS;
            break;

            case PORT_B:
            regPtr = (volatile uint32*)GPIOB_BASE_ADDRESS;
            break;

            case PORT_C:
            regPtr = (volatile uint32*)GPIOC_BASE_ADDRESS;
            break;

            case PORT_D:
            regPtr = (volatile uint32*)GPIOD_BASE_ADDRESS;
            break;

            case PORT_E:
            regPtr = (volatile uint32*)GPIOE_BASE_ADDRESS;
            break;

            case PORT_F:
            regPtr = (volatile uint32*)GPIOF_BASE_ADDRESS;
            break;

            default: /* ERR: Skip this pin config */
            continue;
        }

        /* Enable GPIO clock */
        RCGCGPIO_REG |= ( 1 << ConfigPtr[i].PortID );
				delay_3clocks = RCGCGPIO_REG;

        /* Commit protected pins */
        if ( ((ConfigPtr[i].PortID) == PORT_C) && ((ConfigPtr[i].PinID) <= PIN_PC3) )
        {
            /* This is JTAG pins!! Escape this pin config iteration */
            continue;
        }
        else if ( ((ConfigPtr[i].PortID == PORT_D) && (ConfigPtr[i].PinID == PIN_PD7)) ||\
                  ((ConfigPtr[i].PortID == PORT_F) && (ConfigPtr[i].PinID == PIN_PF0)) )
        {
            /* This is NMI pins, unlock and commit before pin configurtion */
            *(volatile uint32*)((volatile uint8*)regPtr + GPIOLOCK_OFFSET) = GPIOLOCK_KEY;
            SET_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOCR_OFFSET)), ConfigPtr[i].PinID );     
        }
        else
        {
            /* Do nothing! Other Pins are commited by default */
        }


        /* Check Alternative function */
        if ( ConfigPtr[i].PinMode == DIO )
        {
            CLEAR_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET)), ConfigPtr[i].PinID );
            (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) &= ~( 0xF << ((ConfigPtr[i].PinID)*4) );
            Port_setPinDigital(ConfigPtr + i);
        }
        else
        {
            SET_BIT( (*(volatile uint32*)((volatile uint8*)regPtr + GPIOAFSEL_OFFSET)), ConfigPtr[i].PinID );
            if ( ConfigPtr[i].PortID == PORT_A )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    case PIN_PA0:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U0Rx_PA0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case CAN1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN1Rx_PA0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA1:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U0Tx_PA1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case CAN1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN1Tx_PA1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA2:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI0Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Clk_PA2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA3:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI0Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Fss_PA3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */;
                    }
                    break;

                    case PIN_PA4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Rx_PA4 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */;
                    }
                    break;

                    case PIN_PA5:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI0Tx_PA5 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */;
                    }
                    break;

                    case PIN_PA6:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case I2C1SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C1SCL_PA6 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM2_PA6 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PA7:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case I2C1SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C1SDA_PA7 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM3_PA7 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    break;
                }

                /* All port_A pinmodes are digital functions */
                /* Set pin as a digital pin */
                Port_setPinDigital(ConfigPtr + i);
            } /* End PortA alternative functions */
						
            else if ( ConfigPtr[i].PortID == PORT_B )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    case PIN_PB0:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case USB0ID:
						/* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Rx_PB0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case T2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP0_PB0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB1:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case USB0VBUS:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Tx_PB1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case T2CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP1_PB1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB2:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case I2C0SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C0SCL_PB2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T3CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T3CCP0_PB2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr + i); /* All PB2 modes are digital */
                    break;
                    
                    case PIN_PB3:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case I2C0SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C0SDA_PB3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T3CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T3CCP1_PB3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr + i); /* All PB3 modes are digital */
                    break;

                    case PIN_PB4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN10:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI2Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Clk_PB4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM2_PB4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case T1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP0_PB4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx_PB4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB5:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN11:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI2Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Fss_PB5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM3_PB5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case T1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP1_PB5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx_PB5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PB6:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI2Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Rx_PB6 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M0PWM0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM0_PB6 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP0_PB6 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr + i); /* All PB6 modes are digital */
                    break;

                    case PIN_PB7:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI2Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI2Tx_PB7 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M0PWM1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM1_PB7 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP1_PB7 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        default: /* ERR */ ;
                    }
                    Port_setPinDigital(ConfigPtr + i); /* All PB7 modes are digital */
                    break;
                }
            } /* End PortB alternative functions */

            else if ( ConfigPtr[i].PortID == PORT_C )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    /* Firts 4 pins PC0 ... PC3 are JTAG pins and are not configurable !! */
                    case PIN_PC4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case C1_NEG:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U4Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U4Rx_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case U1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Rx_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM6_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case IDX1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX1_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT0CCP0_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case U1RTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1RTS_PC4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC5:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case C1_POS:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U4Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U4Tx_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case U1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1Tx_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM7_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case PhA1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA1_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT0CCP1_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case U1CTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1CTS_PC5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC6:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case C0_POS:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U3Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U3Rx_PC6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case PhB1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB1_PC6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT1CCP0_PC6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN_PC6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PC7:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case C0_NEG:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U3Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U3Tx_PC7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;

                        case WT1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT1CCP1_PC7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case USB0PFLT:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0PFLT_PC7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            } /* End PortC alternative functions */

            else if ( ConfigPtr[i].PortID == PORT_D )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    case PIN_PD0:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN7:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI3Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Clk_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case SSI1Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Clk_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case I2C3SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C3SCL_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM6_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M1PWM0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM0_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT2CCP0_PD0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD1:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN6:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI3Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Fss_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case SSI1Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Fss_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case I2C3SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C3SDA_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM7_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M1PWM1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM1_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT2CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT2CCP1_PD1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD2:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN5:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI3Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Rx_PD2 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case SSI1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Rx_PD2 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0_PD2 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT3CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT3CCP0_PD2 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN_PD2 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD3:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN4:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case SSI3Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI3Tx_PD3 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case SSI1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Tx_PD3 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case IDX0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX0_PD3 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT3CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT3CCP1_PD3 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case USB0PFLT:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0PFLT_PD3 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case USB0DM:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U6Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U6Rx_PD4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT4CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT4CCP0_PD4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;

                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD5:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case USB0DP:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U6Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U6Tx_PD5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT4CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT4CCP1_PD5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD6:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U2Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U2Rx_PD6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0_PD6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case PhA0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA0_PD6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT5CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT5CCP0_PD6 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PD7:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U2Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U2Tx_PD7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case PhB0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB0_PD7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case WT5CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( WT5CCP1_PD7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case NMI:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( NMI_PD7 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            } /* End PortD alternative functions */

            else if ( ConfigPtr[i].PortID == PORT_E )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    case PIN_PE0:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN3:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U7Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U7Rx_PE0 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE1:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN2:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U7Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U7Tx_PE1 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE2:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN1:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE3:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN0:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                    
                    case PIN_PE4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN9:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U5Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U5Rx_PE4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case I2C2SCL:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C2SCL_PE4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM4:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM4_PE4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M1PWM2:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM2_PE4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx_PE4 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PE5:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case AIN8:
                        /* Analog Pin */
                        Port_setPinAnalog(ConfigPtr);
                        break;
                        case U5Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U5Tx_PE5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case I2C2SDA:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( I2C2SDA_PE5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M0PWM5:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0PWM5_PE5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case M1PWM3:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM3_PE5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx_PE5 << ((ConfigPtr[i].PinID)*4) );
                        Port_setPinDigital(ConfigPtr + i);
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                }
            } /* End PortE alternative functions */

            else if ( ConfigPtr[i].PortID == PORT_F )
            {
                switch ( ConfigPtr[i].PinID )
                {
                    case PIN_PF0:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U1RTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1RTS_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case SSI1Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Rx_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;

                        case CAN0Rx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Rx_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM4:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM4_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case PhA0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhA0_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T0CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP0_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case NMI:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( NMI_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case C0o:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C0o_PF0 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;
                    
                    case PIN_PF1:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case U1CTS:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( U1CTS_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case SSI1Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Tx_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM5:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM5_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case PhB0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( PhB0_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T0CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T0CCP1_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case C1o:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( C1o_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case TRD1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRD1_PF1 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF2:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI1Clk:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Clk_PF2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M0FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M0FAULT0_PF2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM6:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM6_PF2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T1CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP0_PF2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case TRD0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRD0_PF2 << ((ConfigPtr[i].PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF3:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case SSI1Fss:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( SSI1Fss_PF3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case CAN0Tx:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( CAN0Tx_PF3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case M1PWM7:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1PWM7_PF3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T1CCP1:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T1CCP1_PF3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case TRCLK:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( TRCLK_PF3 << ((ConfigPtr[i].PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;

                    case PIN_PF4:
                    switch ( ConfigPtr[i].PinMode )
                    {
                        case M1FAULT0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( M1FAULT0_PF4 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case IDX0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( IDX0_PF4 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case T2CCP0:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( T2CCP0_PF4 << ((ConfigPtr[i].PinID)*4) );
                        break;
                        case USB0EPEN:
                        (*((volatile uint32*)((volatile uint8*)regPtr + GPIOPCTL_OFFSET))) |= ( USB0EPEN_PF4 << ((ConfigPtr[i].PinID)*4) );
                        break;
                     
                        default: /* ERR */ ;
                    }
                    break;
                }
            /* All port_F pinmodes are digital functions */
            /* Set pin as a digital pin */
            Port_setPinDigital(ConfigPtr + i);
            } /* End PortF alternative functions */

        } /* END else */

        /* Set pin drive current as 2mA, 4mA or 8mA */
        switch (ConfigPtr[i].PinOutputCurrent)
        {
            case PORT_2MA:
            SET_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIODR2R_OFFSET)), ConfigPtr[i].PinID );
            break;

            case PORT_4MA:
            SET_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIODR4R_OFFSET)), ConfigPtr[i].PinID );
            break;

            case PORT_8MA:
            SET_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIODR8R_OFFSET)), ConfigPtr[i].PinID );
            break;

            default: /* ERR */ ;
        }

        /* Check direction if output: set default level
         *                    input : set PUR & PDR
         */
        if ((ConfigPtr[i].PinDirection) == PORT_PIN_OUT)
        {
            

            /* Set direction to output */
            //*(volatile uint32*)((volatile uint8*)regPtr + GPIODIR_OFFSET) |= (1 << ConfigPtr[i].PinID );
			SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIODIR_OFFSET)), (ConfigPtr[i].PinID));

            /* Set pin level before direction to avoid glitches on pin */
            if (ConfigPtr[i].PinLevelValue)
            {
                SET_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIODATA_OFFSET + 0x3FC)), ConfigPtr[i].PinID );
							//*(volatile uint32*)((volatile uint8*)regPtr + GPIODATA_OFFSET + 0x3FC) = 0xFF;
            }
            else
            {
                CLEAR_BIT((*(volatile uint32*)((volatile uint8*)regPtr + GPIODATA_OFFSET + 0x3FC)), ConfigPtr[i].PinID );
            }
        }
        else if ((ConfigPtr[i].PinDirection) == PORT_PIN_IN)
        {
            /* Set direction */
            CLEAR_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIODIR_OFFSET)), ConfigPtr[i].PinID );

            /* Set internal pull resistors before direction */
            switch (ConfigPtr[i].PinInternalAttach)
            {
                case PORT_PIN_PULL_OFF:
                CLEAR_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr[i].PinID );
                CLEAR_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr[i].PinID );
                break;

                case PORT_PIN_PULL_UP:
                CLEAR_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr[i].PinID );
                SET_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr[i].PinID );
                break;

                case PORT_PIN_PULL_DOWN:
                CLEAR_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPUR_OFFSET)), ConfigPtr[i].PinID );
                SET_BIT(*((volatile uint32*)((volatile uint8*)regPtr + GPIOPDR_OFFSET)), ConfigPtr[i].PinID );
                break;

                default: /* ERR */ ;
            }


        }

    } /* END for loop */

} /* END PORT_Init() */

/**********************************************************************************************************************
 *  END OF FILE: Port.c
 *********************************************************************************************************************/
