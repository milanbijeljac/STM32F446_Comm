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

/****************************************************/

void CAN_v_Init(CAN_TypeDef* CANx)
{
	if(CANx == CAN1)
	{
		RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	}
	else
	{
		/* CAN1 needs to be enabled in order for CAN2 to work */
		RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
		RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
	}

	/* Enter initialization mode */
	CANx->MCR |= CAN_MCR_INRQ;
	while( !(CANx->MSR & CAN_MSR_INAK) );

	/* HW handling of exit from sleep mode */
	CANx->MCR |= CAN_MCR_AWUM;

	/* Loopback Mode. 32 MHz frequency on AHB1 peripheral, 18 quanta in total with baud prescaler set to 4 --> 32MHz/(4 * 18) */
	CANx->BTR |=  (1u  << CAN_BTR_SJW_Pos) | (2u  << CAN_BTR_TS1_Pos) | (13u << CAN_BTR_TS2_Pos) | (3u  << CAN_BTR_BRP_Pos);
	CANx->BTR |= CAN_BTR_LBKM;
	CANx->BTR &= ~CAN_BTR_SILM;
	//CANx->MCR &= ~CAN_MCR_DBF; /* TODO: Configuration handling for debug mode */

	/* Exit initialization mode */
	CANx->MCR &= ~CAN_MCR_INRQ;
	while(CANx->MSR & CAN_MSR_INAK); /* TODO: For now we're leaving init mode */

}

void CAN_v_FiltersInit(CAN_TypeDef* CANx)
{
	/* Filter init mode */
	CANx->FMR |= CAN_FMR_FINIT; // Enter filter init mode

    /* Filter 1 activation */
	CANx->FA1R |= CAN_FFA1R_FFA0;
    CANx->FM1R &= ~(CAN_FM1R_FBM0_Msk);
    CANx->FS1R |= CAN_FS1R_FSC0;

    CANx->sFilterRegister[0].FR1 = 0x00000000;
    CANx->sFilterRegister[0].FR2 = 0x00000000;

    /* Assign filter 0 to FIFO0 */
    CANx->FFA1R &= ~(CAN_FFA1R_FFA0);

    /* Exit filter init */
    CANx->FMR &= ~CAN_FMR_FINIT;
}


uint8 CAN_u_TransmitMessage(CAN_TypeDef* CANx, CANx_IndetifierHandle_t* CANx_Handle, uint8 DLC, uint8* Data)
{
	uint8 i = 0xFFu;
	uint8 j;
	uint8 u_shiftLow = 0;
	uint8 u_shiftHigh = 0;
	uint8 retVal = NOK;

	/* Check which mailbox is empty */
	if( CANx->TSR & (1u << CAN_TSR_TME0_Pos) )
	{
		i = 0;
	}
	else if(CANx->TSR & (1u << CAN_TSR_TME1_Pos) )
    {
		i = 1;
    }
	else if(CANx->TSR & (1u << CAN_TSR_TME1_Pos) )
	{
		i = 2;
	}
	else
	{
		return NOK;
	}

	CAN1->sTxMailBox[i].TIR  = (uint32)0;
	CAN1->sTxMailBox[i].TDTR = (uint32)0;

	CANx->sTxMailBox[i].TIR |= CANx_Handle->remoteTransmissionRequest << CAN_TI0R_RTR_Pos;
	CANx->sTxMailBox[i].TIR |= CANx_Handle->identifierExtension << CAN_TI0R_IDE_Pos;
	CANx->sTxMailBox[i].TDTR |= DLC << CAN_TDT0R_DLC_Pos;

	/* Check if it's CAN 2.0A or 2.0B ID */
	if(CANx_Handle->identifierExtension == 0u)
	{
		CANx->sTxMailBox[i].TIR |= CANx_Handle->identifier << CAN_TI0R_STID_Pos;
	}
	else
	{
		CANx->sTxMailBox[i].TIR |= CANx_Handle->identifier << CAN_TI0R_EXID_Pos;
	}

	/* Empty Mailbox found */
	if(i != 0xFFu)
	{
	    CAN1->sTxMailBox[i].TDLR = (uint32)0;
	    CAN1->sTxMailBox[i].TDHR = (uint32)0;

		/* Set data to empty mailbox */
		for(j = 0; j < DLC; j++)
		{
			if(j <= 3u)
			{
				CANx->sTxMailBox[i].TDLR |= Data[j] << u_shiftLow;
				u_shiftLow+= 8u;
			}
			else
			{
				CANx->sTxMailBox[i].TDHR |= Data[j] << u_shiftHigh;
				u_shiftHigh+= 8u;
			}
		}
		retVal = OK;
	}

	CANx->sTxMailBox[i].TIR |= CAN_TI0R_TXRQ;

	return retVal;
}

uint8 CAN_u_RecieveMessage(CAN_TypeDef* CANx, CANx_RecieveHandle_t* CANx_RecieveHandle, uint8* sizeOfFifo0, uint8* sizeOfFifo1)
{
	uint8  i, j;
	uint8  u_retVal = NOK;
	uint32 u_recieveDataLow = 0;
	uint32 u_recieveDataHigh = 0;

	if( !(CANx->RF0R & CAN_RF0R_FMP0) && !(CANx->RF1R & CAN_RF1R_FMP1) )
	{
		return OK;
	}   /* TODO: Remote frame handling. If this is scaled with CAN-FD and different STM32, no need for this kind of handling */

	*sizeOfFifo0 = CANx->RF0R & 0x3u;
	*sizeOfFifo1 = CANx->RF1R & 0x3u;

	if(*sizeOfFifo0 == 0)
	{
		/* Skip FIFO1 */
	}
	else
	{
		for(i = 0; i < *sizeOfFifo0; i++)
		{
			/* Check if it's CAN 2.0A or 2.0B ID */
			if( (CANx->sFIFOMailBox[0].RIR) & (CAN_RI0R_IDE_Msk) )
			{
				CANx_RecieveHandle->canId[i] = (CANx->sFIFOMailBox[0].RIR >> 3u) & 0x1FFFFFFF;
			}
			else
			{
				CANx_RecieveHandle->canId[i] = (CANx->sFIFOMailBox[0].RIR >> 21u) & 0x7FF;
			}

			CANx_RecieveHandle->DLC[i] = CANx->sFIFOMailBox[0].RDTR & 0xF;

			u_recieveDataLow  = CANx->sFIFOMailBox[0].RDLR;
			u_recieveDataHigh = CANx->sFIFOMailBox[0].RDHR;

			for(j = 0; j < CANx_RecieveHandle->DLC[i]; j++)
			{
				if(j <= 3u)
				{
					CANx_RecieveHandle->Data[i][j] = (u_recieveDataLow >> (8 * j)) & 0xFFu;
				}
				else
				{
					CANx_RecieveHandle->Data[i][j] = (u_recieveDataHigh >> (8 * (j - 4))) & 0xFFu;
				}
			}
			/* Release FIFO0 */
			CANx->RF0R |= CAN_RF0R_RFOM0;
		}

	}

	if(*sizeOfFifo1 == 0)
	{
		/* Skip FIFO1 */
	}
	else
	{
		for(i = 0; i < *sizeOfFifo1; i++)
		{
			/* Check if it's CAN 2.0A or 2.0B ID */
			if( (CANx->sFIFOMailBox[1].RIR) & (CAN_RI1R_IDE_Msk) )
			{
				CANx_RecieveHandle->canId[i + 3] = (CANx->sFIFOMailBox[0].RIR >> 3u) & 0x1FFFFFF;
			}
			else
			{
				CANx_RecieveHandle->canId[i + 3] = (CANx->sFIFOMailBox[0].RIR >> 21u) & 0x7FF;
			}

			CANx_RecieveHandle->DLC[i + 3] = CANx->sFIFOMailBox[1].RDTR & 0xF;

			u_recieveDataLow  = CANx->sFIFOMailBox[1].RDLR;
			u_recieveDataHigh = CANx->sFIFOMailBox[1].RDHR;

			for(j = 0; j < CANx_RecieveHandle->DLC[i + 3]; j++)
			{
				if(j <= 3u)
				{
					CANx_RecieveHandle->Data[i+3][j] = (u_recieveDataLow >> (8 * j)) & 0xFFu;
				}
				else
				{
					CANx_RecieveHandle->Data[i+3][j] = (u_recieveDataHigh >> (8 * (j - 4))) & 0xFFu;
				}
			}
			/* Release FIFO1 */
			CANx->RF0R |= CAN_RF1R_RFOM1;
		}

	}
	u_retVal = OK;

	return u_retVal;
}
