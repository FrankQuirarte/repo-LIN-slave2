/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Global_Init.c $
 * $Revision: 18 $
 * $Author: Guillermo Ramírez Vázquez, Óscar Francisco Miranda García
 * $Date: 13/11/2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** This file initialize all the resources needed for the project
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
 * $Log: Global_Init.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MAL/Global_Init.h"




/* Constants and types  */
/*============================================================================*/
/*To determine the equivalent value in hexadecimal 
 * to real time use the following formula:
 * Hex value Time = System Clock Freq * Time desired
 *  
 * In this project the system clock frequency is
 * 64MHz,so the procedure should be:
 * T_HEX = (64 * 10 ^ 6)* T_SEC
 * T_HEX ==> Hexadecimal time value
 * T_SEC ==> Real time in seconds*/

/* T_SEC = 10 miliseconds
 * T_HEX = (64 * 10 ^ 6) * (10 * 10 ^(-3))
 * T_HEX = 0x9C400*/
#define T10MS 0xFA00


/* Variables */
/*============================================================================*/


/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/
void Global_Init(void)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  Global_Init
	*  Description          :  Initialise the system resources
	*  Parameters           :  void 
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
	STM_init();
	initModesAndClock();
	GPIO_Init();
	INTC_init();
}

void STM_init(void)
{
	/* --------------------------------------------------------------------------
	*  Name                 :  STM_init
	*  Description          :  Configured the STM options and its channels
	*  Parameters           :  void 
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
    STM.CR.B.TEN        = 0x0;
    STM.CR.B.FRZ        = 0x1;
    STM.CR.B.CPS        = 0;        /* Configure Counter Prescaler, Counter Prescaler is 1     */
        
    STM.CH[0].CMP.R     = T10MS;   	/* Compare with 640KHz = 10 msec*/      
    STM.CH[0].CCR.B.CEN = 0x1;      /* System Timer Channel 0: ENABLE                          */

    STM.CR.B.TEN        = 0x1;      /* Enable System Timer Module                              */
}


/* Exported functions */
/*============================================================================*/


 /* Notice: the file ends with a blank new line to avoid compiler warnings */
