#ifndef INC_MESSAGES_CFG_H_
#define INC_MESSAGES_CFG_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include "stm32f446_can_driver.h"

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */

#define NUMBER_OF_MESSAGES 6u

/* **************************************************
 *			    GLOBAL VARIABLES 					*
 *************************************************  */

/** Structure that is used for configuration of TX messages */
typedef struct
{
	CANx_TxHandle_t CanTxHandle;
	uint16 time;				/** Cyclic time for every message in ms */
}Message_Cfg_t;

#endif /* INC_MESSAGES_CFG_H_ */
