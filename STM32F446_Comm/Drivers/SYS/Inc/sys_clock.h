
#ifndef SYS_CLOCK_H_
#define SYS_CLOCK_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <types.h>

/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */

/**
 *
 * \brief      - This function will set clock frequency based on input. Frequency is set by setting PLLN to value that is
 * 				 between 100-432MHz and dividing that value with 2/4/6/8
 * \param[in]  - uint8 PLLN_Multiplication - PLLN_Multiplication is multiplier of reference voltage (PLL_M which is 1MHz) which results in Voltage-Controlled Oscillator (VCO) frequency.
 * 			   - Range: 100 - 432 (suggested range for VCO)
 * \param[in]  - uint8 PLLP_Division - PLLP_Division is divider that is used to get final output clock frequency by dividing VCO (PLL_N).
 * 			   - Range: 2/4/6/8
 * \return     - RetVal (uint8) - OK, NOK
 */
RetVal Sys_v_FrequencyConfig(uint8 PLLN_Multiplication, uint8 PLLP_Division);

#endif /* SYS_CLOCK_H_ */
