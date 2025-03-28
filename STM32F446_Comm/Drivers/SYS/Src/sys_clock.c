/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */
#include <sys_clock.h>
#include <stm32f4xx.h>
#include <system_stm32f4xx.h>


void Sys_v_ClockConfig(void)
{
	/* Clock configuration */

	SystemCoreClockUpdate();
}

