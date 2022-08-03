#include "Port.h"
#include "Dio.h"
#include "Gpt.h"
#include "Mcu_Hw.h"

/* PWM resolution, you can lower resolution by changing this number */
#define PWM_SAMPLES (50u)
#define g_LED (PF3)
#define g_LED1 (PF2)
#define g_LED2 (PF3)
/* PWM Task variables Config
 * WARNING: Do not change this variables!!
 * NOTE: If you want to change the on and off time use the function:*/
 
static volatile uint32 g_globalTicks = 0;
static volatile uint8 g_onTime = 0;
static volatile uint8 g_offTime = 0;
static volatile uint8 g_dutyCycle = 0;

static volatile uint8 g_countUpFlag = 1;


/* App function prototypes */
void PwmBySoftware(void);
void calculateDutyCycle(uint8 dutyCycle);

/* App external configurations */
extern Port_ConfigType Port_ConfigSet[PORT_NUM_OF_CONFIG_PINS];
extern Gpt_ConfigType Gpt_ConfigSet[GPT_NUM_OF_CONFIG_CHANNELS];


void Nvic_Init(void)
{
	/* Enable Timer0 interrupt */
	SET_BIT(EN0_REG, 19); /* Enable Timer0 interrupt */
	SET_BIT(EN2_REG, 30); /* Enable WTimer0 interrupt */

}

/* Service function */
void calculateDutyCycle(uint8 dutyCycle)
{
	if (dutyCycle >= 100)
	{
		/* Avoid led turn off locking */
		dutyCycle = 99;
	}
	g_onTime = (dutyCycle * PWM_SAMPLES) / 100;
	g_offTime = PWM_SAMPLES - g_onTime;
}

/* PWM Task */
void PwmBySoftware(void)
{
	g_globalTicks++;
	if(	g_globalTicks == g_offTime)
	{
		Dio_WriteChannel(g_LED, PIN_HIGH);
	}
	else if (	g_globalTicks >= PWM_SAMPLES)
	{
		Dio_WriteChannel(g_LED, PIN_LOW);
		g_globalTicks = 0;
	}
}
void IncreaseDutyCycle(void)
{
	if(g_countUpFlag)
	{
		g_dutyCycle += 1;
	}
	else
	{
		g_dutyCycle -= 1;
	}
	
	if(g_dutyCycle >= 100)
	{
	    g_countUpFlag = 0;
	}
	else if(g_dutyCycle <= 1)
	{
		g_countUpFlag = 1;
	}
	calculateDutyCycle(g_dutyCycle);
}

int main()
{
	Port_Init(Port_ConfigSet);
	Gpt_Init(Gpt_ConfigSet);
	Gpt_SetCallBack(TIMER0, PwmBySoftware);
	Gpt_SetCallBack(WTIMER0, IncreaseDutyCycle);

	/* Enable interrupts in NVIC */
	Nvic_Init();
	
  /* This function calculate the on & off time required for the software PWM function */
	calculateDutyCycle(0);
	
	/* 1 count of TIMER0 equals 1 us */
	/** To Achieve a frequency of 500 HZ
	 *  that means 1/500 = 2 ms
	 *  ;; Divide that by PWM_SAMPLES which is 100
	 *  ;; Thus we need an over flow every 20 count or 20 us 
	 */
	Gpt_StartTimer(TIMER0, 20);
  
	/* 1 count of WTIMER0 equals 1 ms */
	/** To Achieve a frequency of 0.5 HZ
	 *  that means 500 ms
	 *  ;; Thus we need an over flow every 500 count or 500 us 
	 */
	Gpt_StartTimer(WTIMER0, 20);
	
	while(1);
}

