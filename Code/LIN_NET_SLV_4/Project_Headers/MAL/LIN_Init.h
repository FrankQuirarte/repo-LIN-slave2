/*
 * LIN_Init.h
 *
 *  Created on: Dec 2, 2015
 *      Author: x
 */
/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: filename.x $
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
 * $Log: LIN_Init.h  $
  ============================================================================*/
#ifndef LIN_INIT_H_
#define LIN_INIT_H_

/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "driver_channel_MPC5606B.h"
#include "stdtypedef.h"

/* Constants and types */
/*============================================================================*/
typedef enum E_SLV_STATE{DISABLED,ENABLED}t_slv_stat;
typedef enum E_CMD_TYPE{CMD_NONE,CMD_LED_ON, CMD_LED_OFF,
				CMD_LED_TOGGLING, CMD_DISABLE_SLV,
				CMD_ENABLE_SLV}t_cmdType;
typedef enum E_LED_STATE{OFF, ON, TOGGLING}t_LEDstat;

#define INIT_MODE 0x0001
#define ENABLE_LIN0TX 0x0604
#define ENABLE_LIN0RX 0x0100

/*Baud rate = Clock speed/(integerBaudRateReg.fractionalBaudRateReg)
 *Baud rate = 64 * 10 ^ 6 / (208.5)
 *Baud rate = 19184.65 bit / s
 * */
#define INT_BAUD_RATE 416
#define FR_BAUD_RATE 11

#define NO_TIMEOUT 0
/*Enhanced 0, classic 1*/
#define ENHANCED_CHECKSUM 1
#define ENABLE_BIT_ERROR 1
#define ENABLE_FILTERS 0b00000011
#define DISABLE_FILTERS 0b00000000
#define FILTER_LIST_MODE 0x0
/*--------------FILTERS BEGINNING-----------------*/
/*PID = 0xcf
 *ID = 0x0f*/
#define MASTER_CMD_ALL_ID 0x0f
/*dfl = numberofbytes-1
 * 1 byte*/
#define MASTER_CMD_ALL_LENGTH 0

/*PID = 0xd3
 *ID = 0x13*/
#define MASTER_CMD_SLV4_ID 0x13
/*dfl= numberofbytes-1
 * 1 byte*/
#define MASTER_CMD_SLV4_LENGTH 0

/*PID = 0xA3
 *ID = 0x23*/
#define SLAVE4_RSP_ID 0x23
/*dfl= numberofbytes-1
 * 2 bytes*/
#define SLAVE4_RSP_LENGTH 1

/*PID = 0x73
 *ID = 0x33*/
#define SLAVE4_ID_ID 0x33
/*dfl = numberofbytes-1
 * 7 bytes*/
#define SLAVE4_ID_LENGTH 6
/*------------FILTERS END-----------------------*/

#define RECEIVE_DATA 0
#define TRANSMIT_DATA 1

/*Header interrupt*/
#define TX_RX_HEADER_INTPT 7 

#define AUTO_RESYNCH 1
#define MASTER_13BIT_LENGTH 3
#define ENTER_NORMAL_MODE 0
#define TRANSMIT 1
#define RECEIVE 0
/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
void Init_Lin(void);
void LinSlaveDataReception(T_UBYTE lub_Data[]);
void LinSlaveDataTransmission(T_UBYTE lub_Data[8]);
T_ULONG GetLINStateMachineState(void);
void ReceptionComplete(void);
void ClearMessageBuffer(void);
void ReleaseMessageBuffer(void);
void HeaderReceived(void);
void DataTransmissionRequest(void);
T_UBYTE GetBufferId(void);
void SetBufferDirection(T_UBYTE);
#endif /* LIN_INIT_H_ */
