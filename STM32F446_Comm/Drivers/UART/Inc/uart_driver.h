
#ifndef UART_INC_UART_DRIVER_H_
#define UART_INC_UART_DRIVER_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <types.h>

/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */
/**
 *
 * \brief      - Initialization function for UART
 * \param[in]  - uint32 clock - clock speed in HZ
 * \param[in]  - uint32 baudRate - desired baud rate
 * \return     - NONE
 * \globals    - NONE
 *
 */
void UART_v_Init(uint32 clock, uint32 baudRate);

/**
 *
 * \brief      - Function that is used for sending messages over UART
 * \param[in]  - const char *str - pointer to string that holds message
 * \param[in]  - uint8 size - size of message
 * \return     - NONE
 * \globals    - NONE
 *
 */
void UART_Write_Message(const char *str, uint8 size);



#endif /* UART_INC_UART_DRIVER_H_ */
