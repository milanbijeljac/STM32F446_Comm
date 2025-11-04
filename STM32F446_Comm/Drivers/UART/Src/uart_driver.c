/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <stm32f446xx.h>
#include "uart_driver.h"
/* **************************************************
 *					DEFINES 					    *
 *************************************************  */

/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */

/**
 *
 * \brief      - Helper function used to sed one byte of data via direct interface to UART peripheral
 * \param[in]  - char ch - one character
 * \return     - NONE
 *
 */
static void UART_v_Write(char ch);

/****************************************************/

static void UART_v_Write(char ch)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (ch & 0xFF);
}

void UART_v_Init(uint32 clock, uint32 baudRate)
{
    /* Clock initialization */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Set baud rate */
    USART2->BRR = clock / baudRate;

    /* Enable TX and USART */
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}


void UART_Write_Message(const char *str, uint8 size)
{
    while (*str)
    {
    	UART_v_Write(*str++);
    }
}

