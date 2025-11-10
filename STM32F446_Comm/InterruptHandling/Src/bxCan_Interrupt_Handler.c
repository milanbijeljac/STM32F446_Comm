/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */
#include <types.h>
#include <stm32f4xx.h>
#include "stm32f446_can_driver.h"
#include <FreeRTOS.h>
#include <queue.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */

/****************************************************/



/* **************************************************
 *			    GLOBAL VARIABLES 					*
 *************************************************  */

/* TODO: clean - test variables */
uint32 counterRx0 = 0, counterRx1 = 0;

uint8 size1, size2;
CANx_RxHandle_t CanHndlRecieve;

extern QueueHandle_t xQueueCanRx;

/****************************************************/

void CAN1_RX0_IRQHandler(void)
{
	uint8  i, j, numberOfMessagesInFifo0 = 0;
	uint32 u_recieveDataLow = 0;
	uint32 u_recieveDataHigh = 0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if( !(CAN1->RF0R & CAN_RF0R_FMP0) )
	{
		/* TODO: Remote frame handling. If this is scaled with CAN-FD and different STM32, no need for this kind of handling */
	}

	numberOfMessagesInFifo0 = CAN1->RF0R & 0x3u;

	for(i = 0; i < numberOfMessagesInFifo0; i++)
	{
		/* Check if it's CAN 2.0A or 2.0B ID */
		if( (CAN1->sFIFOMailBox[0].RIR) & (CAN_RI0R_IDE_Msk) )
		{
			CanHndlRecieve.canId = (CAN1->sFIFOMailBox[0].RIR >> 3u) & 0x1FFFFFFF;
		}
		else
		{
			CanHndlRecieve.canId = (CAN1->sFIFOMailBox[0].RIR >> 21u) & 0x7FF;
		}

		CanHndlRecieve.DLC = CAN1->sFIFOMailBox[0].RDTR & 0xF;

		u_recieveDataLow  = CAN1->sFIFOMailBox[0].RDLR;
		u_recieveDataHigh = CAN1->sFIFOMailBox[0].RDHR;

		for(j = 0; j < CanHndlRecieve.DLC; j++)
		{
			if(j <= 3u)
			{
				CanHndlRecieve.Data[j] = (u_recieveDataLow >> (8 * j)) & 0xFFu;
			}
			else
			{
				CanHndlRecieve.Data[j] = (u_recieveDataHigh >> (8 * (j - 4))) & 0xFFu;
			}
		}
		/* Release FIFO0 */
		CAN1->RF0R |= CAN_RF0R_RFOM0;
	}

	/* Currently not needed - for possible future use */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	counterRx0++;
}

void CAN1_RX1_IRQHandler(void)
{
	uint8  i, j, numberOfMessagesInFifo1 = 0;
	uint32 u_recieveDataLow = 0;
	uint32 u_recieveDataHigh = 0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if( !(CAN1->RF0R & CAN_RF0R_FMP0) && !(CAN1->RF1R & CAN_RF1R_FMP1) )
	{
		/* TODO: Remote frame handling. If this is scaled with CAN-FD and different STM32, no need for this kind of handling */
	}

	numberOfMessagesInFifo1 = CAN1->RF1R & 0x3u;

	for(i = 0; i < numberOfMessagesInFifo1; i++)
	{
		/* Check if it's CAN 2.0A or 2.0B ID */
		if( (CAN1->sFIFOMailBox[1].RIR) & (CAN_RI1R_IDE_Msk) )
		{
			CanHndlRecieve.canId = (CAN1->sFIFOMailBox[0].RIR >> 3u) & 0x1FFFFFF;
		}
		else
		{
			CanHndlRecieve.canId = (CAN1->sFIFOMailBox[0].RIR >> 21u) & 0x7FF;
		}

		CanHndlRecieve.DLC = CAN1->sFIFOMailBox[1].RDTR & 0xF;

		u_recieveDataLow  = CAN1->sFIFOMailBox[1].RDLR;
		u_recieveDataHigh = CAN1->sFIFOMailBox[1].RDHR;

		for(j = 0; j < CanHndlRecieve.DLC; j++)
		{
			if(j <= 3u)
			{
				CanHndlRecieve.Data[j] = (u_recieveDataLow >> (8 * j)) & 0xFFu;
			}
			else
			{
				CanHndlRecieve.Data[j] = (u_recieveDataHigh >> (8 * (j - 4))) & 0xFFu;
			}
		}
		/* Release FIFO1 */
		CAN1->RF1R |= CAN_RF1R_RFOM1;
	}

	/* Currently not needed - for possible future use */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	counterRx1++;
}
