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

typedef struct
{
	uint32  remoteTransmissionRequest : 1 ;
	uint32  identifierExtension       : 1 ;
	uint32  identifier                 : 29;
}CANx_IndetifierHandle_t;

typedef struct
{
	uint32 canId[6];
    uint8  Data[6][8];
	uint8  DLC[6];
	uint8  reserved[2];
}CANx_RecieveHandle_t;

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
uint8 CAN_u_RecieveMessage(CAN_TypeDef* CANx, CANx_RecieveHandle_t* CANx_RecieveHandle, uint8* sizeOfFifo0, uint8* sizeOfFifo1);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
uint8 CAN_u_TransmitMessage(CAN_TypeDef* CANx, CANx_IndetifierHandle_t* CANx_Handle, uint8 DLC, uint8* Data);

#endif /* CAN_INC_STM32F446_CAN_DRIVER_H_ */
