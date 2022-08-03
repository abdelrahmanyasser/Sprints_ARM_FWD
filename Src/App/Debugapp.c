#if(0)
#include "Port.h"
#include "Dio.h"
#include "Gpt.h"
#include "Mcu_Hw.h"

/* Global App variables */
static volatile const uint8 g_periodTicks = 100;
static volatile uint8 g_dutyCycle = 0;

static volatile uint32 g_debugTicks = 0;

static volatile uint32 g_globalTicks = 0;
static volatile uint8 g_onTicks;
static volatile uint8 g_offTicks;

/* PWM Config */
static volatile uint8 g_onFlag = 0;
static volatile uint8 g_offFlag = 1;

static volatile const uint8 g_periodTime = 100;

static volatile uint8 g_onTime = 0;
static volatile uint8 g_offTime = 0;

void PwmBySoftware(void);
void toggleLed(void);
void PwmBySoft (void);
void IncreaseDutyCycle(void);


extern Port_ConfigType Port_ConfigSet[PORT_NUM_OF_CONFIG_PINS];
extern Gpt_ConfigType Gpt_ConfigSet[GPT_NUM_OF_CONFIG_CHANNELS];


void Nvic_Init(void)
{
	/* Enable Timer0 interrupt */
	SET_BIT(EN0_REG, 19); /* Enable Timer0 interrupt */
	SET_BIT(EN2_REG, 30); /* Enable WTimer0 interrupt */
}

void PwmBySoftware(void)
{
	g_globalTicks++;
	g_debugTicks++;
	if(g_globalTicks == g_offTicks)
	{
			Dio_WriteChannel(PF1, PIN_HIGH);
	}
	else if(g_globalTicks >= g_periodTicks)
	{
		Dio_WriteChannel(PF1, PIN_LOW);
		g_globalTicks = 0;
	}
}


void calculateDutyCycle(uint8 dutyCycle)
{
	g_onTime = dutyCycle;
	g_offTime = g_periodTime - g_onTime;
}

void toggleLed(void)
{
	g_globalTicks++;
	if(	g_globalTicks == 50)
	{
		Dio_WriteChannel(PF1, PIN_HIGH);
	}
	else if (	g_globalTicks >= 100)
	{
		Dio_WriteChannel(PF1, PIN_LOW);
		g_globalTicks = 0;
	}
}

void PwmBySoft (void)
{
	g_globalTicks++;
	if(g_offFlag)
	{
		Dio_WriteChannel(PF1, PIN_HIGH);
		g_onFlag = 1;
		g_offFlag = 0;
	}
	else if (g_onFlag)
	{
		Dio_WriteChannel(PF1, PIN_LOW);
		g_onFlag = 0;
		g_offFlag = 1;	
	}
}
void IncreaseDutyCycle(void)
{
	g_dutyCycle++;
	if(g_dutyCycle <= 100)
	{
	    calculateDutyCycle(g_dutyCycle);
	}
	else
	{
		g_dutyCycle = 0;
	}
}
int main()
{
	Port_Init(Port_ConfigSet);
	Gpt_Init(Gpt_ConfigSet);
  Nvic_Init();
	
	Gpt_SetCallBack(TIMER0, toggleLed);
  //calculateDutyCycle(50);
	Gpt_StartTimer(TIMER0, 1000);  /* 1 Count equals 1 us */

	while(1)
	{
		#if(0)
    if(g_offFlag)
		{
			Gpt_StartTimer(TIMER0, g_offTime);  /* 16 Count cause an overflow at 1 us */
		}
		else if(g_onFlag)
		{
		  Gpt_StartTimer(TIMER0, g_onTime);  /* 16 Count cause an overflow at 1 us */
		}
		#endif
	}
}

#endif