/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: main.c $
 * $Revision: 1.0 $
 * $Author: Guillermo Ramirez $
 * $Date: 13/11/2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** Main file that calls the scheduler.
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
/*                     |                    |                                 */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: main.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MAL\Global_Init.h"

/* Constants and types  */
/*============================================================================*/
#define SWITCH_VEC 43
#define TX_VEC 80
#define RX_VEC 79
#define STM0_VEC 30

/* Variables */
/*============================================================================*/

/* Exported functions */
/*============================================================================*/
	extern void kernel(void);
	extern void Tick_Flag(void);
	extern void LINFlex_0_RX_ISR(void);
	extern void LINFlex_0_TX_ISR(void);
	extern void SendMessage(void);

int main(void) {
	

  Global_Init();
  GPIO_SetState (LED1, 1);
  asm(" wrteei 1");
  SIU.IFEER.B.IFEE21 = 1;	        /* Enable falling edge event on EIRQ21    */
  SIU.IRER.B.IRE21   = 1;		    /* Enable interrupt Results EIRQ21        */
  INTC_InstallINTCInterruptHandler(Tick_Flag,STM0_VEC,4); /* vector 30 for STM[0]*/
  INTC_InstallINTCInterruptHandler(LINFlex_0_RX_ISR, RX_VEC, 1); /* vector 79 for LINFlex Rx*/
  INTC_InstallINTCInterruptHandler(LINFlex_0_TX_ISR, TX_VEC, 2); /* vector 79 for LINFlex Tx*/
  INTC_InstallINTCInterruptHandler(SendMessage, SWITCH_VEC, 3); /* vector 43 for 3rd switch interrupt*/
 
  kernel();



}



