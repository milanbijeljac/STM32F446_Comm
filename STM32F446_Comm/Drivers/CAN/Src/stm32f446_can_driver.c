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

void Can_v_Init(CAN_TypeDef* CANx)
{
	if(CANx == CAN1)
	{
		RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	}
	else
	{
		/* CAN 1 needs to be enabled in order for CAN2 to work */
		RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
		RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
	}
	/* Enter initialization mode */
	CANx->MCR |= CAN_MCR_INRQ;
	while( !(CANx->MSR & CAN_MSR_INAK) );

	/* Loopback Mode. 32 MHz frequency on AHB1 peripheral, 18 quantas in total with baud prescaler set to 4 --> 32MHz/(4 * 18) */
	CANx->BTR = CAN_BTR_LBKM | (1u  << CAN_BTR_SJW_Pos) | (2u  << CAN_BTR_TS1_Pos) | (13u << CAN_BTR_TS2_Pos) | (3u  << CAN_BTR_BRP_Pos);

	/* Exit initialization mode */
	CANx->MCR &= ~CAN_MCR_INRQ;
	while(CANx->MSR & CAN_MSR_INAK);
}


uint8 Can_u_TransmitMessage(CAN_TypeDef* CANx, CANx_IndetifierHandle_t* CANx_Handle, uint8 DLC, uint8* Data)
{
	uint8 i, j, u_shiftLow = 0, u_shiftHigh = 0;

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

	CANx->sTxMailBox[i].TIR |= CANx_Handle->remoteTransmissionRequest << CAN_TI0R_RTR_Pos;
	CANx->sTxMailBox[i].TIR |= CANx_Handle->identifierExtension << CAN_TI0R_IDE_Pos;
	CANx->sTxMailBox[i].TDTR |= DLC << CAN_TDT0R_DLC_Pos;
	if(CANx_Handle->identifierExtension == 0u)
	{
		CANx->sTxMailBox[i].TIR |= CANx_Handle->identifier << CAN_TI0R_STID_Pos;
	}
	else
	{
		CANx->sTxMailBox[i].TIR |= CANx_Handle->identifier << CAN_TI0R_EXID_Pos;
	}

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

	CANx->sTxMailBox[i].TIR |= CAN_TI0R_TXRQ;

	return 1;
}
