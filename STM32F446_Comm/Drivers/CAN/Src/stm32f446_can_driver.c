/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */

#include <stm32f446_can_driver.h>
#include <stm32f4xx.h>
#include <core_cm4.h>

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

	/* Enable interrupt for CANx FIFO0 */
	CANx->IER |= CAN_IER_FMPIE0_Msk;
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_SetPriority(CAN1_RX0_IRQn, 5);

	/* Enable interrupt for CANx FIFO1 */
	CANx->IER |= CAN_IER_FMPIE1_Msk;
	NVIC_EnableIRQ(CAN1_RX1_IRQn);
	NVIC_SetPriority(CAN1_RX1_IRQn, 5);

	/* Exit initialization mode */
	CANx->MCR &= ~CAN_MCR_INRQ;
	while(CANx->MSR & CAN_MSR_INAK); /* TODO: For now we're leaving init mode */

}

void CAN_v_FiltersInit(CAN_TypeDef* CANx)
{
	/* Filter init mode */
	CANx->FMR |= CAN_FMR_FINIT; // Enter filter init mode

    /* Filter 0 for FIFO0 */
    CANx->FM1R &= ~(CAN_FM1R_FBM0_Msk);  /* Mask mode */
    CANx->FS1R |= CAN_FS1R_FSC0;         /* 32-bit scale */
    CANx->FFA1R &= ~(CAN_FFA1R_FFA0);    /* Assign to FIFO0 */
    CANx->FA1R  |=  CAN_FFA1R_FFA0;      /* Activate filter 0 */

    CANx->sFilterRegister[0].FR1 = ((0x00000000 << 3) | (1 << 2));  // IDE=1, RTR=0
    CANx->sFilterRegister[0].FR2 = ((0x10000000 << 3));             // Mask bit28

    /* Filter 1 for FIFO1 */
    CANx->FM1R &= ~(CAN_FM1R_FBM1_Msk);
    CANx->FS1R |= CAN_FS1R_FSC1;
    CANx->FFA1R |= CAN_FFA1R_FFA1;   /* Assign to FIFO1 */
    CANx->FA1R  |= CAN_FFA1R_FFA1;   /* Activate filter 1 */

    CANx->sFilterRegister[1].FR1 = ((0x10000000 << 3) | (1 << 2));  // IDE=1, RTR=0
    CANx->sFilterRegister[1].FR2 = ((0x10000000 << 3));

    /* Exit filter init */
    CANx->FMR &= ~CAN_FMR_FINIT;
}

void CAN_v_TransmitMessage(CAN_TypeDef* CANx, CANx_TxHandle_t* CANx_Handle)
{
	uint8 i = 0xFFu;
	uint8 j;
	uint8 u_shiftLow = 0;
	uint8 u_shiftHigh = 0;

	/* Check which mailbox is empty */
	if( CANx->TSR & (1u << CAN_TSR_TME0_Pos) )
	{
		i = 0;
	}
	else if(CANx->TSR & (1u << CAN_TSR_TME1_Pos) )
    {
		i = 1;
    }
	else if(CANx->TSR & (1u << CAN_TSR_TME2_Pos) )
	{
		i = 2;
	}
	else
	{

	}

	CAN1->sTxMailBox[i].TIR  = (uint32)0;
	CAN1->sTxMailBox[i].TDTR = (uint32)0;

	CANx->sTxMailBox[i].TIR |= CANx_Handle->remoteTransmissionRequest << CAN_TI0R_RTR_Pos;
	CANx->sTxMailBox[i].TIR |= CANx_Handle->identifierExtension << CAN_TI0R_IDE_Pos;
	CANx->sTxMailBox[i].TDTR |= CANx_Handle->DLC << CAN_TDT0R_DLC_Pos;

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
		for(j = 0; j < CANx_Handle->DLC; j++)
		{
			if(j <= 3u)
			{
				CANx->sTxMailBox[i].TDLR |= CANx_Handle->Data[j] << u_shiftLow;
				u_shiftLow+= 8u;
			}
			else
			{
				CANx->sTxMailBox[i].TDHR |= CANx_Handle->Data[j] << u_shiftHigh;
				u_shiftHigh+= 8u;
			}
		}
		CANx->sTxMailBox[i].TIR |= CAN_TI0R_TXRQ;
	}
}
