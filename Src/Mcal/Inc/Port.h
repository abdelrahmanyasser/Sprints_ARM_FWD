/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Port.h
 *       Module:  GPIO module
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef PORT_H
#define PORT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Port_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define PORT_A (0u)
#define PORT_B (1u)
#define PORT_C (2u)
#define PORT_D (3u)
#define PORT_E (4u)
#define PORT_F (5u)


#define PIN_PA0 (0u)
#define PIN_PA1 (1u)
#define PIN_PA2 (2u)
#define PIN_PA3 (3u)
#define PIN_PA4 (4u)
#define PIN_PA5 (5u)
#define PIN_PA6 (6u)
#define PIN_PA7 (7u)

#define PIN_PB0 (0u)
#define PIN_PB1 (1u)
#define PIN_PB2 (2u)
#define PIN_PB3 (3u)
#define PIN_PB4 (4u)
#define PIN_PB5 (5u)
#define PIN_PB6 (6u)
#define PIN_PB7 (7u)

#define PIN_PC0 (0u)
#define PIN_PC1 (1u)
#define PIN_PC2 (2u)
#define PIN_PC3 (3u)
#define PIN_PC4 (4u)
#define PIN_PC5 (5u)
#define PIN_PC6 (6u)
#define PIN_PC7 (7u)

#define PIN_PD0 (0u)
#define PIN_PD1 (1u)
#define PIN_PD2 (2u)
#define PIN_PD3 (3u)
#define PIN_PD4 (4u)
#define PIN_PD5 (5u)
#define PIN_PD6 (6u)
#define PIN_PD7 (7u)

#define PIN_PE0 (0u)
#define PIN_PE1 (1u)
#define PIN_PE2 (2u)
#define PIN_PE3 (3u)
#define PIN_PE4 (4u)
#define PIN_PE5 (5u)

#define PIN_PF0 (0u)
#define PIN_PF1 (1u)
#define PIN_PF2 (2u)
#define PIN_PF3 (3u)
#define PIN_PF4 (4u)


/** Digital Function (GPIOPCTL PMCx Bit Field Encoding)
 */
#define U0Rx_PA0    (1u)
#define CAN1Rx_PA0  (8u)
#define U0Tx_PA1    (1u)
#define CAN1Tx_PA1  (8u)
#define SSI0Clk_PA2 (2u)
#define SSI0Fss_PA3 (2u)
#define SSI0Rx_PA4  (2u)
#define SSI0Tx_PA5  (2u)
#define I2C1SCL_PA6 (3u)
#define M1PWM2_PA6  (5u)
#define I2C1SDA_PA7 (3u)
#define M1PWM3_PA7  (5u)

#define U1Rx_PB0    (1u)
#define T2CCP0_PB0  (7u)
#define U1Tx_PB1    (1u)
#define T2CCP1_PB1  (7u)
#define I2C0SCL_PB2 (3u)
#define T3CCP0_PB2  (7u)
#define I2C0SDA_PB3 (3u)
#define T3CCP1_PB3  (7u)
#define SSI2Clk_PB4 (2u)
#define M0PWM2_PB4  (4u)
#define T1CCP0_PB4  (7u)
#define CAN0Rx_PB4  (8u)
#define SSI2Fss_PB5 (2u)
#define M0PWM3_PB5  (4u)
#define T1CCP1_PB5  (7u)
#define CAN0Tx_PB5  (8u)
#define SSI2Rx_PB6  (2u)
#define M0PWM0_PB6  (4u)
#define T0CCP0_PB6  (7u)
#define SSI2Tx_PB7  (2u)
#define M0PWM1_PB7  (4u)
#define T0CCP1_PB7  (7u)

#define T4CCP0_PC0   (7u)
#define T4CCP1_PC1   (7u)
#define T5CCP0_PC2   (7u)
#define T5CCP1_PC3   (7u)
#define U4Rx_PC4     (1u)
#define U1Rx_PC4     (2u)
#define M0PWM6_PC4   (4u)
#define IDX1_PC4     (6u)
#define WT0CCP0_PC4  (7u)
#define U1RTS_PC4    (8u)
#define U4Tx_PC5     (1u)
#define U1Tx_PC5     (2u)
#define M0PWM7_PC5   (4u)
#define PhA1_PC5     (6u)
#define WT0CCP1_PC5  (7u)
#define U1CTS_PC5    (8u)
#define U3Rx_PC6     (1u)
#define PhB1_PC6     (6u)
#define WT1CCP0_PC6  (7u)
#define USB0EPEN_PC6 (8u)
#define U3Tx_PC7     (1u)
#define WT1CCP1_PC7  (7u)
#define USB0PFLT_PC7 (8u)

#define SSI3Clk_PD0  (1u)
#define SSI1Clk_PD0  (2u)
#define I2C3SCL_PD0  (3u)
#define M0PWM6_PD0   (4u)
#define M1PWM0_PD0   (5u)
#define WT2CCP0_PD0  (7u)
#define SSI3Fss_PD1  (1u)
#define SSI1Fss_PD1  (2u)
#define I2C3SDA_PD1  (3u)
#define M0PWM7_PD1   (4u)
#define M1PWM1_PD1   (5u)
#define WT2CCP1_PD1  (7u)
#define SSI3Rx_PD2   (1u)
#define SSI1Rx_PD2   (2u)
#define M0FAULT0_PD2 (4u)
#define WT3CCP0_PD2  (7u)
#define USB0EPEN_PD2 (8u)
#define SSI3Tx_PD3   (1u)
#define SSI1Tx_PD3   (2u)
#define IDX0_PD3     (6u)
#define WT3CCP1_PD3  (7u)
#define USB0PFLT_PD3 (8u)
#define U6Rx_PD4     (1u)
#define WT4CCP0_PD4  (7u)
#define U6Tx_PD5     (1u)
#define WT4CCP1_PD5  (7u)
#define U2Rx_PD6     (1u)
#define M0FAULT0_PD6 (4u)
#define PhA0_PD6     (6u)
#define WT5CCP0_PD6  (7u)
#define U2Tx_PD7     (1u)
#define PhB0_PD7     (6u)
#define WT5CCP1_PD7  (7u)
#define NMI_PD7      (8u)

#define U7Rx_PE0    (1u)
#define U7Tx_PE1    (1u)
#define U5Rx_PE4    (1u)
#define I2C2SCL_PE4 (3u)
#define M0PWM4_PE4  (4u)
#define M1PWM2_PE4  (5u)
#define CAN0Rx_PE4  (8u)
#define U5Tx_PE5    (1u)
#define I2C2SDA_PE5 (3u)
#define M0PWM5_PE5  (4u)
#define M1PWM3_PE5  (5u)
#define CAN0Tx_PE5  (8u)

#define U1RTS_PF0   (1u)
#define SSI1Rx_PF0  (2u)
#define CAN0Rx_PF0  (3u)
#define M1PWM4_PF0  (5u)
#define PhA0_PF0    (6u)
#define T0CCP0_PF0  (7u)
#define NMI_PF0     (8u)
#define C0o_PF0     (9u)

#define U1CTS_PF1   (1u)
#define SSI1Tx_PF1  (2u)
#define M1PWM5_PF1  (5u)
#define PhB0_PF1    (6u)
#define T0CCP1_PF1  (7u)
#define C1o_PF1     (9u)
#define TRD1_PF1    (14u)

#define SSI1Clk_PF2  (2u)
#define M0FAULT0_PF2 (4u)
#define M1PWM6_PF2   (5u)
#define T1CCP0_PF2   (7u)
#define TRD0_PF2     (14u)

#define SSI1Fss_PF3  (2u)
#define CAN0Tx_PF3   (3u)
#define M1PWM7_PF3   (5u)
#define T1CCP1_PF3   (7u)
#define TRCLK_PF3    (14u)

#define M1FAULT0_PF4 (5u)
#define IDX0_PF4     (6u)
#define T2CCP0_PF4   (7u)
#define USB0EPEN_PF4 (8u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Port_PortType;          /* Port range 0 .. 5 */
typedef uint8 Port_PinType;           /* Pin range 0 .. 7 */

typedef enum
{
    PORT_PIN_IN = 0,                      /* Pin input type */
    PORT_PIN_OUT = 1                      /* Pin output type */
}Port_PinDirectionType;

typedef enum
{
    /* Table of Alternative functions encodings for gpio pins */
	  /* TODO: figure out conflicts */
	DIO,
    U0Rx,
    U0Tx,
    U1Rx,
    U1Tx,
	U2Rx,
    U2Tx,
	U3Rx,
    U3Tx,
    U4Rx,
    U4Tx,
	U5Rx,
    U5Tx,
	U6Rx,
    U6Tx,
	U7Rx,
    U7Tx,

	M0PWM0,
	M0PWM1,
	M0PWM2,
	M0PWM3,
	M0PWM4,
	M0PWM5,
	M0PWM6,
	M0PWM7,
	M1PWM0,
	M1PWM1,
	M1PWM2,
	M1PWM3,
	M1PWM4,
	M1PWM5,
	M1PWM6,
	M1PWM7,

	T0CCP0,
	T0CCP1,
	T1CCP0,
	T1CCP1,
	T2CCP0,
	T2CCP1,
	T3CCP0,
	T3CCP1,
	T4CCP0,
	T4CCP1,
	T5CCP0,
	T5CCP1,
	WT0CCP0,
	WT0CCP1,
	WT1CCP0,
	WT1CCP1,
	WT2CCP0,
	WT2CCP1,
	WT3CCP0,
	WT3CCP1,
	WT4CCP0,
	WT4CCP1,
	WT5CCP0,
	WT5CCP1,

	SSI0Clk,
	SSI0Fss,
	SSI0Rx,
	SSI0Tx,
	SSI1Clk,
	SSI1Fss,
	SSI1Rx,
	SSI1Tx,
	SSI2Clk,
	SSI2Fss,
	SSI2Rx,
	SSI2Tx,   
	SSI3Clk,
	SSI3Fss,
	SSI3Rx,
	SSI3Tx,

	I2C0SCL,
	I2C0SDA,
	I2C1SCL,
	I2C1SDA,
	I2C2SCL,
	I2C2SDA,
	I2C3SCL,
	I2C3SDA,

	CAN0Rx,
	CAN0Tx,
	CAN1Rx,
	CAN1Tx,

	M0FAULT0,
	M1FAULT0,

	IDX0,
	IDX1,

	PhA0,
	PhB0,
	PhA1,
	PhB1,

	NMI,
	C0o,
	C1o,

	TRD0,
	TRD1,
	TRCLK,

	USB0EPEN,
	USB0PFLT,

	U1RTS,
	U1CTS,

	/* Analog functions */
	USB0ID,
	USB0VBUS,	
	USB0DM,
	USB0DP,
	C0_POS,
	C0_NEG,
	C1_POS,
	C1_NEG,
	AIN0,
	AIN1,
	AIN2,
	AIN3,
	AIN4,
	AIN5,
	AIN6,
	AIN7,
	AIN8,
	AIN9,
	AIN10,
	AIN11
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
{   Port_PortType PortID;
    Port_PinType PinID;
    Port_PinModeType PinMode;
    Port_PinLevelType PinLevelValue;
    Port_PinDirectionType PinDirection;
    Port_PinInternalAttachType PinInternalAttach;
    Port_PinOutputCurrentType PinOutputCurrent;
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
