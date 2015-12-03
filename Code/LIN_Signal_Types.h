/*============================================================================*/
/*                         AEP SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*
 * Source: LIN_Signal_Types.h
 * Revision: 1.0
 * Authors: Luis Armando García Valdovinos. Luis Fernando Alvarez Guerrero.
 * Date: Nov 25, 2015
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/* Signal Types for Master - Slave LIN Network 
*/
/*============================================================================*/
/* COPYRIGHT (C) CONTINENTAL AUTOMOTIVE 2014                                  */
/* AUTOMOTIVE GROUP, Automotive Entry Program                                 */
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/* REVISION |   DATE     |          COMMENT                  |    AUTHOR      */
/*----------------------------------------------------------------------------*/
/*   04     | 25/11/2015 | Added signal types               | Luis Alvarez    */
/*============================================================================*/
#ifndef LIN_SIGNAL_TYPES_H_
#define LIN_SIGNAL_TYPES_H_

/* Includes */
/*============================================================================*/
#include "StdTypedef.h"
/*============================================================================*/

/* Constants and types */
/*============================================================================*/
/*============================================================================*/


/* Exported Variables */
/*============================================================================*/
typedef enum
{
	DISABLED,
	ENABLED
}t_slv_stat;

typedef enum
{
	cmd_NONE,
	cmd_LED_on,
	cmd_LED_off,
	cmd_LED_toggling,
	cmd_disable_slv,
	cmd_enable_slv
}t_cmdType;

typedef enum
{
	OFF,
	ON,
	TOGGLING
}t_LEDstat;

typedef T_UBYTE scalar;

/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

/*============================================================================*/

#endif /* LIN_SIGNAL_TYPES_H_ */
