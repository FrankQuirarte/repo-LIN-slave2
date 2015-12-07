/*
 * LED_App.c
 *
 *  Created on: Dec 4, 2015
 *      Author: x
 */


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
 * $Source: LED_App.c $
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
 * $Log: LED_App.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Application\LED_App.h"
/* Constants and types  */
/*============================================================================*/



/* Variables */
/*============================================================================*/
/*Initial state = OFF*/
T_UBYTE rub_LedState = OFF;

/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
void ChangeLedState(T_UBYTE);


/* Exported functions */
/*============================================================================*/
void ChangeLedState(T_UBYTE lul_LedState)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  ChangeLedState
	*  Description          :  Change the state variable depending on the command. 
	*  Parameters           :  T_UBYTE lul_LedState
	*  								Possible states: OFF, ON, and TOGGLING.
	*  								
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
	rub_LedState = lul_LedState;
}

T_UBYTE GetLedState(void)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  GetLedState
	*  Description          :  Get the current state of the led
	*  Parameters           :  void
	*  								
	*  Return               :  T_UBYTE
	*  -------------------------------------------------------------------------
	*/
	return rub_LedState;
}

void Led_StateMachine(void)
{
	/* --------------------------------------------------------------------------
		*  Name                 :  Led_StateMachine
		*  Description          :  This state machine will be called every 1ms.
		*  Initial state is OFF. Led variable state could have three states: OFF, ON, and TOGGLING.
		*  If the node is enabled then the master command is listened. 
		*  When the command is received the rub_LedFlag is changed
		*  depending on the command. The state variable will be changed in the subsequent
		*  tick. If the node is disable, the current state is kept, until a different command, 
		*  when node is enabled, is listened.      							
		*  Parameters           :  void
		*  								
		*  Return               :  void
		*  -------------------------------------------------------------------------
		*/
	static T_UBYTE lub_TickCounter = RESET_COUNTER;
	switch (GetLedState())
		{
	
		case OFF:
			
			if(ENABLED == rub_NodeState)
			{
				GPIO_SetState(LED1,TURN_OFF);
				/*Change state variable into the requested command*/
				ChangeLedState(rub_LedFlag);
			}
			else
			{
			 //do nothing
			}
			break;
		case ON:
			if(ENABLED == rub_NodeState)
			{
				GPIO_SetState(LED1,TURN_ON);
				/*Change state variable into the requested command*/
				ChangeLedState(rub_LedFlag);
			}
			else
			{
			 /*do nothing*/
			}
			break;
		case TOGGLING:
			if(ENABLED == rub_NodeState)
			{
				/*If a cycle has begun turn on the led*/
				if(HOLD_ON == lub_TickCounter )
				{
					GPIO_SetState(LED1,TURN_ON);
				}
				else { /*do nothing*/}
				
				/*If a half cycle has been reached turn off the led*/
				if(HOLD_OFF == lub_TickCounter )
				{
					GPIO_SetState(LED1,TURN_OFF);
				}
				else { /*do nothing*/}
				
				/*If a complete cycle hasn't been reached increase lub_TickCounter
				  otherwise reset it*/
				if(COMPLETE_CYCLE != lub_TickCounter) 	
				{
					lub_TickCounter++;
				}
				else
				{
					lub_TickCounter = RESET_COUNTER;
				}
				/*Change state variable into the requested command*/
				ChangeLedState(rub_LedFlag);
			}
			else
			{
			 /*do nothing*/
			}
			break;
			
		default :
			break;
		}
}
