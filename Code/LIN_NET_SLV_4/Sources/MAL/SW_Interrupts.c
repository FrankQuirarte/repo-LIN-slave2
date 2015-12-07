/*
 * SW_Interrupts.c
 *
 *  Created on: Dec 3, 2015
 *      Author: x
 */

/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: SW_Interrupts.c $
 * $Revision: 18 $
 * $Author: Guillermo Ramírez Vázquez, Óscar Francisco Miranda García
 * $Date: 12/06/2015 $
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
 * $Log: SW_Interrupts.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MAL\SW_Interrupts.h"


/* Constants and types  */
/*============================================================================*/



/* Variables */
/*============================================================================*/



/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/
void INTC_init(void){
	/* ------------------------------------------------------------------------
	 *  Name                 :	INTC_init
	 *  Description          :  Activates the use of interrupts
	 *  Parameters           :  void
	 *  Return               :  void
	 *  -----------------------------------------------------------------------*/
	INTC.CPR.R = 0;
}

/*void Enable_Interrupts()
{
	asm(" wrteei 1");
	INTC.CPR.R = 0;
}*/
/* Private functions */
/*============================================================================*/


/* Exported functions */
/*============================================================================*/

