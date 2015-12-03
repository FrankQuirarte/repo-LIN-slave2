/*============================================================================*/
/*                         AEP SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*
 * Source: LIN.h
 * Revision: 1.0
 * Authors: Luis Armando García Valdovinos. Luis Fernando Alvarez Guerrero.
 * Date: Nov 23, 2015
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/* PIT Module Library
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
/*   02     | 23/11/2015 | Added PIT initialisation function | Luis Alvarez   */
/*          |            | and basic functionality           |                */
/*============================================================================*/
#ifndef LIN_H_
#define LIN_H_

/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "StdTypedef.h"
#include "LIN_Signal_Types.h"
/*============================================================================*/

/* Constants and types */
/*============================================================================*/
#define MASTER_CMD_ALL 0x3C   /* ID: CF */ 
#define MASTER_CMD_SLV1 0x02  /* ID: 50 */
#define MASTER_CMD_SLV2 0x22  /* ID: 11 */
#define MASTER_CMD_SLV3 0x12  /* ID: 92 */
#define MASTER_CMD_SLV4 0x32  /* ID: D3 */
#define SLAVE1_RSP 0x01       /* ID: 20 */
#define SLAVE2_RSP 0x21       /* ID: 61 */
#define SLAVE3_RSP 0x11       /* ID: E2 */
#define SLAVE4_RSP 0x31       /* ID: A3 */
#define SLAVE1_ID 0x03        /* ID: F0 */
#define SLAVE2_ID 0x23        /* ID: B1 */
#define SLAVE3_ID 0x13        /* ID: 32 */
#define SLAVE4_ID 0x33        /* ID: 73 */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
void linflex0_init(void);
void LIN_RXFLAG   (void);
void LIN_TXFLAG   (void);
void LIN_ERRFLAG  (void);
void UART_Init    (void);
/*============================================================================*/

#endif /* LIN_H_ */
