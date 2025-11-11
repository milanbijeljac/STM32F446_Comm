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

void UART_v_Init(uint32 clock, uint32 baudRate)
{
    /* Clock initialization */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Set baud rate */
    USART2->BRR = clock / baudRate;

    /* Enable TX and USART */
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}


void UART_v_Write_Message(uint8* data, uint8 size)
{
	uint8 i;

    for (i = 0; i < size; i++)
    {
    	 while (!(USART2->SR & USART_SR_TXE));
    	 USART2->DR = data[i];
    }
}

