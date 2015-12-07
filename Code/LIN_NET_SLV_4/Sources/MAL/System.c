/*
 * System.c
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
 * $Source: System.c $
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
 * $Log: System.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MAL\System.h"


/* Constants and types  */
/*============================================================================*/



/* Variables */
/*============================================================================*/



/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/
void initModesAndClock(void) 
{
	/* --------------------------------------------------------------------------
	*  Name                 :  initModesAndClock
	*  Description          :  Initialize the modes and clocks of the MPC5606B
	*  Parameters           :  void 
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
  	ME.MER.R = 0x0000001D;               /* Enable DRUN, RUN0, SAFE, RESET modes              */
    
  	/* Initialize PLL before turning it on:              */
   	CGM.FMPLL_CR.R = 0x02400100;         /* 8 MHz xtal: Set PLL0 to 64 MHz                    */   
   	
   	ME.RUN[0].R = 0x001F0074;            /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL0     */
   	
   	ME.RUNPC[1].R = 0x00000010; 	     /* Peri. Cfg. 1 settings: only run in RUN0 mode      */
   	
   	ME.PCTL[LED1].R = 0x01;
   	ME.PCTL[LED2].R = 0x01;
   	ME.PCTL[LED3].R = 0x01;
   	ME.PCTL[LED4].R = 0x01;
   	
                                         
   	/* Mode Transition to enter RUN0 mode:               */
   	ME.MCTL.R = 0x40005AF0;              /* Enter RUN0 Mode & Key                             */
   	ME.MCTL.R = 0x4000A50F;              /* Enter RUN0 Mode & Inverted Key                    */  
   	while (ME.GS.B.S_MTRANS) {}          /* Wait for mode transition to complete              */    
    
   	/* Note: could wait here using timer and/or I_TC IRQ */
   	while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode                   */
}



/* Private functions */
/*============================================================================*/


/* Exported functions */
/*============================================================================*/


