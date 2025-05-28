#ifndef CAN_INC_STM32F446_CAN_DRIVER_H_
#define CAN_INC_STM32F446_CAN_DRIVER_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <types.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */


/* **************************************************
 *			    GLOBAL VARIABLES 					*
 *************************************************  */

typedef struct
{
	uint8 remoteTransmissionRequest : 1;
	uint8 identifierExtension       : 1;
	uint32 identifier               : 29;
}CANx_IndetifierHandle_t;

#endif /* CAN_INC_STM32F446_CAN_DRIVER_H_ */
