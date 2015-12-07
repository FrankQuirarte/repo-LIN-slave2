/*
 * Slave4_Rsp_Rx_Tx.c
 *
 *  Created on: Dec 4, 2015
 *      Author: x
 */

/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: filename.c $
 * $Revision: version $
 * $Author: Guillermo Ramírez Vázquez, Óscar Francisco Miranda García
 * $Date: date $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    short description in one sentence end with dot.
    detailed
    multiline
    description of the file
*/
/*============================================================================*/
/* COPYRIGHT (C) CONTINENTAL AUTOMOTIVE 2014                                  */
/* AUTOMOTIVE GROUP, Interior Division, Body and Security                     */
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/
/*============================================================================*/
/*                    REUSE HISTORY - taken over from                         */
/*============================================================================*/
/*  DATABASE           |        PROJECT     | FILE VERSION (AND INSTANCE)     */
/*----------------------------------------------------------------------------*/
/*                     |LIN NETWORK 1 SLAVE |                                 */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: filename.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Basic Software\Slave4_Rsp_Rx_Tx.h"
/* Constants and types  */
/*============================================================================*/

#define SWITCH_INTERR_FLAG SIU.ISR.B.EIF21

/* Variables */
/*============================================================================*/



/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/



/* Exported functions */
/*============================================================================*/

void LINFlex_0_RX_ISR(void)
{
	/*******************************************************************************
	Function Name : LINFlex_0_RX_ISR
	Parameters    : NONE
	Modifies      : rx_data - received data
	Returns       : NONE
	Notes         : Rx data ISR on LINFlex_0. 
					Called if there is match with RX filter and data is received 
	*******************************************************************************/
	T_UBYTE lin_bidr;
	T_UBYTE laub_RxData[8];
	T_ULONG lub_LinStatus;
	
	lin_bidr = GetBufferId();
	HeaderReceived();
	
	    switch(lin_bidr)
	    {
			case MASTER_CMD_ALL_ID:
				
				//LINFLEX_0.BIDR.B.DFL = 0; one byte verify page 526
				lub_LinStatus = GetLINStateMachineState();
					/* wait for RMB */
				ReleaseMessageBuffer();
				
				/* get the data */
				LinSlaveDataReception(laub_RxData);
				ProcessCommand(laub_RxData);
				ReceptionComplete();
				ClearMessageBuffer();
				//HeaderReceived();
				
				break;
			case MASTER_CMD_SLV4_ID:
				
				//LINFLEX_0.BIDR.B.DFL = 0; one byte verify page 526
				lub_LinStatus = GetLINStateMachineState();
					/* wait for RMB */
				ReleaseMessageBuffer();
				
				/* get the data */
				LinSlaveDataReception(laub_RxData);
				ProcessCommand(laub_RxData);
				ReceptionComplete();
				ClearMessageBuffer();
				//HeaderReceived();
				
				break;
			default:
				break;
	    }

}

void LINFlex_0_TX_ISR(void)
{

	T_UBYTE lub_lin_bidr;
	lub_lin_bidr = GetBufferId();
	/* if  interrupt generated when header received */
	if(GetLINStateMachineState()&0x1)
	{
		
		switch(lub_lin_bidr)
		{
		case SLAVE4_RSP_ID:
			
				
				SetBufferDirection(TRANSMIT); /* BDR direction - write */
			
				/* fill the BDR registers */
				/*Return Led status*/
				LINFLEX_0.BDRL.B.DATA0 = GetLedState();
				/*Return Node status*/
				LINFLEX_0.BDRL.B.DATA1 = GetNodeState();    
				
				//LINFLEX_0.BIDR.B.DFL = 1;  Verify page 525, sec. 24.7.2.1
				
				/* trigger the data transmission */
				DataTransmissionRequest();
				HeaderReceived();
			
			break;
		case SLAVE4_ID_ID:

				SetBufferDirection(TRANSMIT); /* BDR direction - write probably not necessary*/
			
				/* fill the BDR registers */
				LINFLEX_0.BDRL.B.DATA0 = 4;
				LINFLEX_0.BDRL.B.DATA1 = 'G';
				LINFLEX_0.BDRL.B.DATA2 = 'R';
				LINFLEX_0.BDRL.B.DATA3 = 'V';
				LINFLEX_0.BDRM.B.DATA4 = 'O';
				LINFLEX_0.BDRM.B.DATA5 = 'M';
				LINFLEX_0.BDRM.B.DATA6 = 'G';       
				
				/* trigger the data transmission */
				DataTransmissionRequest();
				HeaderReceived();
			
				break;
		default:
			break;
		}
	}	
	else if(GetLINStateMachineState()&0x2)	/* if interrupt generated when data transmitted */
	{
		 		
		LINFLEX_0.LINSR.R = 0x2;
	}	else {/*Do nothing*/}

	
}

void SendMessage(void)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  Send Message
	*  Description          :  Emulates the master commands of function ProcessCommand
	*  by using the third switch of the
	*  board.   When switch is pressed and interrupt take place. Each command correspond to 
	*  a value of counter variable. 
	*  This function was created for testing purposes to verify the correct functionality 
	*  of the state machines.
	*  							
	*  Parameters           :  void
	*  								
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
	static int counter = 1;
	if (SWITCH_INTERR_FLAG)
	{
		switch (counter)
			{

			case 2:
				SendLedCommand(ON);
				break;
			case 3:
				SendLedCommand(OFF);
				break;
			case 4:
				SendLedCommand(TOGGLING);
				break;
			case 5:
				SendNodeCommand(DISABLED);
				counter = 0;
					break;
			case 1:
				SendNodeCommand(ENABLED);
				break;
			default :
				break;
			}
		
		SWITCH_INTERR_FLAG = 1;
	}
	else
	{
	  /*DO NOTHING*/
	}
	
	counter++;
}

void ProcessCommand(T_UBYTE laub_Msg[1])
{
	/* --------------------------------------------------------------------------
	*  Name                 :  ProcessCommand
	*  Description          :  Reads the response of the master and determines which command
	*  							the slave will execute: whether to do nothing, request the Led_StateMachine
	*  							 to turn on/off or toggle, or request the Node_StateMachine to disable or enable 
	*  							 the node.
	*  							
	*  Parameters           :  T_UBYTE laub_Msg[1]
	*  								Possible values: CMD_LED_ON, CMD_LED_OFF or CMD_LED_TOGGLING,
	*  								 CMD_DISABLE_SLV, CMD_ENABLE_SLV.
	*  								
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
					 
				
	switch (laub_Msg[0])
	{
	case CMD_NONE:
		break;
	case CMD_LED_ON:
		SendLedCommand(ON);
		break;
	case CMD_LED_OFF:
		SendLedCommand(OFF);
		break;
	case CMD_LED_TOGGLING:
		SendLedCommand(TOGGLING);
		break;
	case CMD_DISABLE_SLV:
		SendNodeCommand(DISABLED);
			break;
	case CMD_ENABLE_SLV:
		SendNodeCommand(ENABLED);
		break;
	default :
		break;
	}
}

void SendLedCommand(T_UBYTE lub_LedFlg)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  SendLedCommand
	*  Description          :  Change the value of the global variable led flag 
	*  							that will be used by the Led_StateMachine to change its
	*  							state variable. 
	*  Parameters           :  T_UBYTE lub_LedFlg
	*  								Possible values: CMD_LED_ON, CMD_LED_OFF or CMD_LED_TOGGLING.
	*  								
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
	rub_LedFlag = lub_LedFlg;
	
}

void SendNodeCommand(T_UBYTE lub_NodeFlg)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  SendNodeCommand
	*  Description          :  Change the value of the global variable node flag 
	*  							that will be used by the Node_StateMachine to change its
	*  							state variable. 
	*  Parameters           :  T_UBYTE lub_NodeFlg
	*  								Possible values: CMD_DISABLE_SLV, CMD_ENABLE_SLV.
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
	rub_NodeFlag = lub_NodeFlg;
	
}


