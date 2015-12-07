/*
 * GPIO.h
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
 * $Source: GPIO.h $
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
#ifndef GPIO_H_
#define GPIO_H_

/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "driver_channel_MPC5606B.h"
#include "stdtypedef.h"
/* Constants and types */
/*============================================================================*/
#define OUTPUT 0x0200
#define INPUT 0x0100

/* Exported Variables */
/*============================================================================*/
/* Exported functions prototypes */
/*============================================================================*/
void GPIO_Init(void);
void GPIO_En(T_SWORD, T_UWORD);
void GPIO_SetState (uint16_t ch, uint8_t value);
#endif /* GPIO_H_ */
