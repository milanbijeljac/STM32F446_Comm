
#ifndef UART_INC_UART_DRIVER_H_
#define UART_INC_UART_DRIVER_H_

#include <types.h>

void UART_v_Init(uint32 clock, uint32 baudRate);
void UART_Write_Message(const char *str, uint8 size);



#endif /* UART_INC_UART_DRIVER_H_ */
