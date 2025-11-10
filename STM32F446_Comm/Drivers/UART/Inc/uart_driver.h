
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
 * \param[in]  - uint8* data - pointer to data
 * \param[in]  - uint8 size - size of data
 * \return     - NONE
 * \globals    - NONE
 *
 */
void UART_Write_Message(uint8* message, uint8 size);



#endif /* UART_INC_UART_DRIVER_H_ */
