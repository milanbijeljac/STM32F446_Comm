#ifndef CAN_INC_STM32F446_CAN_DRIVER_H_
#define CAN_INC_STM32F446_CAN_DRIVER_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <types.h>
#include <stm32f446xx.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */


/* **************************************************
 *			    GLOBAL VARIABLES 					*
 *************************************************  */

/* Structure used for sending CAN messages */
typedef struct
{
	uint32 identifier;					/** CAN ID */
	uint8  remoteTransmissionRequest;	/** Data (0) or remote (1) frame */
	uint8  identifierExtension;			/** Identifier extension for - CAN 2.0A or CAN 2.0B */
	uint8  DLC;							/** Data length code */
	uint8  Data[8];						/** Data to be sent */

}CANx_TxHandle_t;

/** Structure for receiving CAN messages */
typedef struct
{
	uint32 canId[6];	/** CAN ID */
    uint8  Data[6][8];	/** Received data */
	uint8  DLC[6];		/** Data length code */
}CANx_RxHandle_t;

/* **************************************************
 *			    FUNCTION PROTOTYPES					*
 *************************************************  */

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void CAN_v_Init(CAN_TypeDef* CANx);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void CAN_v_FiltersInit(CAN_TypeDef* CANx);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void CAN_v_TransmitMessage(CAN_TypeDef* CANx, CANx_TxHandle_t* CANx_Handle);

#endif /* CAN_INC_STM32F446_CAN_DRIVER_H_ */
