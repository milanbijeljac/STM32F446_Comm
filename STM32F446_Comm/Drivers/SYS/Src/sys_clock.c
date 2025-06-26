/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */
#include <sys_clock.h>
#include <stm32f4xx.h>
#include <system_stm32f4xx.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */

#define APB1_MAX_FREQUENCY		((uint8)45)
#define APB2_MAX_FREQUENCY		((uint8)90)
#define VCO_MIN_VALUE			((uint8)100)
#define VCO_MAX_VALUE			((uint16)432)
#define PLLP_MAX_DIVISION		((uint8)8)
#define MAX_CLOCK_FREQUENCY     ((uint8)180)

/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */

/**
 *
 * \brief      - Helper function used to set APBx buses frequency in range if configured frequency is too high
 * \param[in]  - uint8 frequency - configured clock frequency
 * \return     - NONE
 *
 */
static void Sys_v_SetAPBxFrequencyInRange(uint8 frequency);

/**
 *
 * \brief      - Helper function used to configure latency based on provided clock frequency
 * \param[in]  - uint8 frequency - configured clock frequency
 * \return     - NONE
 *
 */
static void Sys_v_FlashLatency(uint8 frequency);

/****************************************************/

RetVal Sys_v_FrequencyConfig(uint8 PLLN_Multiplication, uint8 PLLP_Division)
{
	uint8 retVal = NOK;
	uint8 desiredFrequency = 0u;

	if( (PLLN_Multiplication < VCO_MIN_VALUE) || (PLLN_Multiplication > VCO_MAX_VALUE) || !(PLLP_Division % 2u) || (PLLP_Division > PLLP_MAX_DIVISION) )
	{
		desiredFrequency = PLLN_Multiplication / PLLP_Division;
		if(desiredFrequency <= MAX_CLOCK_FREQUENCY)
		{
			/* Enable HSI oscillator */
			RCC->CR |= RCC_CR_HSION;

			/* Wait for HSI oscillator to stabilize */
			while(!(RCC->CR & RCC_CR_HSIRDY));

			/* RCC_PLLCFGR_PLLP - division of factor 2/4/6/8 equals to values in register of 0/1/2/3 */
			PLLP_Division -= 2u;
			if (PLLP_Division == 0u)
			{
				/* Do nothing */
			}
			else
			{
				PLLP_Division = PLLP_Division >> 1u;
			}

			/* PLL configuration */
			RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP );

			RCC->PLLCFGR |= 16u << RCC_PLLCFGR_PLLM_Pos;                  /* PLL_M (PLLin, division, used for setting of referent frequency) needs to be between 1-2 MHz. 16/16 = 1MHz */
			RCC->PLLCFGR |= PLLN_Multiplication << RCC_PLLCFGR_PLLN_Pos;  /* PLL_N (VCO, multiplication, used for multiplication of referent frequency to set Voltage-Controlled Oscillator. 256 MHz */
			RCC->PLLCFGR |= PLLP_Division << RCC_PLLCFGR_PLLP_Pos;        /* PLL_P (SYSCLK, division, produces the final output clock based on PLL_N / division factor. For now, division factor of 2 will always be used */
			RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;                       /* For HIS 0 needs to be selected */

			/* Enable PLL */
			RCC->CR |= RCC_CR_PLLON;

			/* Wait for PLL to lock */
			while(!(RCC->CR & RCC_CR_PLLRDY));

			/* Configure Flash latency based on Vdd 3.3 and desired clock frequency */
			Sys_v_FlashLatency(desiredFrequency);

			/* Switch system clock to PLL */
			RCC->CFGR &= ~RCC_CFGR_SW;
			RCC->CFGR |= RCC_CFGR_SW_PLL;

			/* Wait for system clock switch */
			while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

			/* Set AHB/APB1/APB2 prescalers to go below maximum defined clock frequency (if needed) */
			RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);

			if(desiredFrequency > APB1_MAX_FREQUENCY)
			{
				Sys_v_SetAPBxFrequencyInRange(desiredFrequency);
			}

			SystemCoreClockUpdate();

			retVal = OK;
		}
	}
	return retVal;
}

static void Sys_v_SetAPBxFrequencyInRange(uint8 frequency)
{
	if(frequency <= APB2_MAX_FREQUENCY)
	{
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	}
	else
	{
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	}
}

static void Sys_v_FlashLatency(uint8 frequency)
{
	uint32 latency = 0xDEADDEADu;

	FLASH->ACR &= ~(FLASH_ACR_LATENCY);

	/* Reference Manual for STM32F446xx - Section 3.4.1 Relation between CPU clock frequency and Flash memory read time */

	if(frequency <= 30u)
	{
		latency = FLASH_ACR_LATENCY_0WS;
	}
	else if ( (frequency > 30u) && (frequency <= 60u) )
	{
		latency = FLASH_ACR_LATENCY_1WS;
	}
	else if ( (frequency > 60u) && (frequency <= 90u) )
	{
		latency = FLASH_ACR_LATENCY_2WS;
	}
	else if ( (frequency > 90u) && (frequency <= 120u) )
	{
		latency = FLASH_ACR_LATENCY_3WS;
	}
	else if ( (frequency > 120u) && (frequency <= 150u) )
	{
		latency = FLASH_ACR_LATENCY_4WS;
	}
	else
	{
		latency = FLASH_ACR_LATENCY_5WS;
	}

	FLASH->ACR |= latency;
	while (!(FLASH->ACR & latency));
}
