/*
 * LED_App.h
 *
 *  Created on: Dec 4, 2015
 *      Author: x
 */
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: LED_App.h $
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
/*                     |                    |                                 */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: GPIO.h  $
  ============================================================================*/
#ifndef LED_APP_H_
#define LED_APP_H_

/* Includes */
/*============================================================================*/

#include "MAL\LIN_Init.h"
#include "MAL\GPIO.h"
/* Constants and types */
/*============================================================================*/
#define TURN_ON 0
#define TURN_OFF 1
/*Hold the led on for 10 ticks*/
#define HOLD_ON 0
/*Hold the led off for 10 ticks*/
#define HOLD_OFF 10
/*Complete cycle = 20 ticks of 10 ms */
#define COMPLETE_CYCLE 20
#define RESET_COUNTER 0 
/* Exported Variables */
/*============================================================================*/
extern T_UBYTE rub_LedFlag;
extern T_UBYTE rub_NodeState;
/* Exported functions prototypes */
/*============================================================================*/
T_UBYTE GetLedState(void);
void Led_StateMachine(void);

#endif /* LED_APP_H_ */
