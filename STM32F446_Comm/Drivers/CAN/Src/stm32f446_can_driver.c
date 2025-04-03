/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <stm32f446_can_driver.h>
#include <stm32f4xx.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */


/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */

void Can_v_Init(CAN_TypeDef* CANx)
{
	/* Enter initialization mode */
	CANx->MCR |= CAN_MCR_INRQ;
	while( !(CANx->MSR & CAN_MSR_INAK) );

	/* CANx->BTR =| */


	/* Exit initialization mode */
	CANx->MCR &= ~CAN_MCR_INRQ;
	while(CANx->MSR & CAN_MSR_INAK);

}
