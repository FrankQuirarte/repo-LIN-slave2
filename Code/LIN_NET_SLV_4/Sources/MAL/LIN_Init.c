/*
 * LIN_Init.c
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
 * $Source: LIN_Init.c $
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
 * $Log: LIN_Init.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MAL\LIN_Init.h"
#include "MAL\GPIO.h"
/* Constants and types  */
/*============================================================================*/



/* Variables */
/*============================================================================*/



/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/
void Init_Lin(void)
{
	/* Send INIT mode request */
	LINFLEX_0.LINCR1.R = INIT_MODE; /* SLEEP=0, INIT=1 */	
	LINFLEX_0.UARTCR.B.UART = 0;	  /* UART working on LIN mode 
	/* wait for the INIT mode VERIFY!!!!!!!!!!!!!!!*/
	while (INIT_MODE != LINFLEX_0.LINSR.B.LINS) {}
	/*Enable LIN transmission channel 0*/
	GPIO_En(LIN0TX,ENABLE_LIN0TX);
	/*Enable LIN transmission channel 0*/
	GPIO_En(LIN0RX,ENABLE_LIN0RX);
	
	/*Baud rate = Clock speed/(integerBaudRateReg.fractionalBaudRateReg)
	*Baud rate = 64 * 10 ^ 6 / (208.5)
	*Baud rate = 19184.65 bit / s
	*/
	LINFLEX_0.LINFBRR.R = FR_BAUD_RATE; 
	LINFLEX_0.LINIBRR.R = INT_BAUD_RATE;
	
	LINFLEX_0.LINCR2.B.IOBE = ENABLE_BIT_ERROR; /* Bit error resets LIN state machine */
	LINFLEX_0.LINTCSR.R = NO_TIMEOUT; /* LIN timeout mode, no idle on timeout */
	LINFLEX_0.BIDR.B.CCS = ENHANCED_CHECKSUM; /* enhanced checksum for LIN Slave */
	
	/* enter NORMAL mode  with filters*/
	LINFLEX_0.IFER.B.FACT = DISABLE_FILTERS;	    /* Disable filters: this must be done before defining filters*/ 
	LINFLEX_0.IFMR.R = FILTER_LIST_MODE;		/* filters 0 - 3 are in identifier list mode. */
	LINFLEX_0.IFER.B.FACT = ENABLE_FILTERS;		/* enable filters 0-3*/  
	
	/*Master_cmdForAll filter*/
	LINFLEX_0.IFCR[0].B.CCS = ENHANCED_CHECKSUM;
	LINFLEX_0.IFCR[0].B.DFL = MASTER_CMD_ALL_LENGTH;
	LINFLEX_0.IFCR[0].B.DIR = RECEIVE_DATA; 
	LINFLEX_0.IFCR[0].B.ID =  MASTER_CMD_ALL_ID;
	
	/*Master_cmdForSlave4 filter*/
	LINFLEX_0.IFCR[1].B.CCS = ENHANCED_CHECKSUM;
	LINFLEX_0.IFCR[1].B.DFL = MASTER_CMD_SLV4_LENGTH;
	LINFLEX_0.IFCR[1].B.DIR = RECEIVE_DATA;
	LINFLEX_0.IFCR[1].B.ID = MASTER_CMD_SLV4_ID;
	
	/*slave4_LEDstat and slave4_enabled filter*/
	LINFLEX_0.IFCR[2].B.CCS = ENHANCED_CHECKSUM;
	LINFLEX_0.IFCR[2].B.DFL = SLAVE4_RSP_LENGTH;
	LINFLEX_0.IFCR[2].B.DIR = TRANSMIT_DATA;
	LINFLEX_0.IFCR[2].B.ID =  SLAVE4_RSP_ID;
	
	/*slave4_supplier and slave4_serial filter*/
	LINFLEX_0.IFCR[3].B.CCS = ENHANCED_CHECKSUM;
	LINFLEX_0.IFCR[3].B.DFL = SLAVE4_ID_LENGTH;
	LINFLEX_0.IFCR[3].B.DIR = TRANSMIT_DATA;
	LINFLEX_0.IFCR[3].B.ID = SLAVE4_ID_ID;
	
	LINFLEX_0.LINIER.R = TX_RX_HEADER_INTPT;	// enable RX, TX and header interrupt
	
	// LASE=1 slave automatic resynch enable
	LINFLEX_0.LINCR1.B.LASE = AUTO_RESYNCH;
	/*master break length reg MBL 11b = 13 bit length */
	LINFLEX_0.LINCR1.B.MBL = MASTER_13BIT_LENGTH;
	
	/*Enter normal mode */
	LINFLEX_0.LINCR1.B.INIT = ENTER_NORMAL_MODE;
}

void LinSlaveDataReception(T_UBYTE laub_Data[8])
{

    laub_Data[0] = (T_UBYTE)LINFLEX_0.BDRL.B.DATA0;
    laub_Data[1] = (T_UBYTE)LINFLEX_0.BDRL.B.DATA1;
    laub_Data[2] = (T_UBYTE)LINFLEX_0.BDRL.B.DATA2;
    laub_Data[3] = (T_UBYTE)LINFLEX_0.BDRL.B.DATA3;
    laub_Data[4] = (T_UBYTE)LINFLEX_0.BDRM.B.DATA4;
    laub_Data[5] = (T_UBYTE)LINFLEX_0.BDRM.B.DATA5;
    laub_Data[6] = (T_UBYTE)LINFLEX_0.BDRM.B.DATA6;
    laub_Data[7] = (T_UBYTE)LINFLEX_0.BDRM.B.DATA7;
}

T_ULONG GetLINStateMachineState()
{

	T_ULONG lul_LinStatus;
	lul_LinStatus = LINFLEX_0.LINSR.R;
	return lul_LinStatus;
}

void ReceptionComplete(void)
{
	/* ------------------------------------------------------------------------
			     *  Name                 :	ReceptionComplete
			     *  Description          :  Program the drive enable pin
			     *  Parameters           :  void
			     *  Return               :  void
			     *  -----------------------------------------------------------------------*/
	LINFLEX_0.LINSR.B.DRF = 1;
}

void ClearMessageBuffer(void)
{
	/* ------------------------------------------------------------------------
		     *  Name                 :	ClearMessageBuffer
		     *  Description          : Prepares the buffer for new data
				0: Buffer is free
				1: Buffer ready to be read by software. 
				This bit must be cleared by software after reading data received
				in the buffer.
				This bit is cleared by hardware in Initialization mode.
		     *  Parameters           : void
		     *  Return               :  void
		     *  -----------------------------------------------------------------------*/
	LINFLEX_0.LINSR.B.RMB = 1;
}

void ReleaseMessageBuffer(void)
{
	/* ------------------------------------------------------------------------
			     *  Name                 :	ReleaseMessageBuffer
			     *  Description          :  Reads the buffer for new data
					0: Buffer is free
					1: Buffer ready to be read by software. 
			     *  Parameters           :  void
			     *  Return               :  void
			     *  -----------------------------------------------------------------------*/
	while (1 != LINFLEX_0.LINSR.B.RMB) {}  /* Wait for Release Message Buffer */
}
void HeaderReceived(void)
{
	/* ------------------------------------------------------------------------
	     *  Name                 :	HeaderReceived
	     *  Description          :  This bit is set by hardware and indicates a valid 
	     *  header reception is completed. This bit must be cleared by software.
	     *  This bit is reset by hardware in Initialization mode and at end of
	     *  completed or aborted frame.
	     *  Note: If filters are enabled, this bit is set only when identifier
	     *   software filtering is required, that is to say:
	     *   • all filters are inactive and BF bit in LINCR1 is set
	     *   • no match in any filter and BF bit in LINCR1 is set
	     *   • TX filter match
	     *  Parameters           :  void
	     *  Return               :  void
	     *  -----------------------------------------------------------------------*/
	LINFLEX_0.LINSR.B.HRF = 1;
}

void DataTransmissionRequest(void)
{
	/* ------------------------------------------------------------------------
		     *  Name                 :	DataTransmissionRequest
		     *  Description          :  Set by software in Slave mode to request the transmission
		     *  of the LIN Data field stored in the Buffer data register.
		     *  This bit can be set only when HRF bit in LINSR is set.
		     *  Cleared by hardware when the request has been 
		     *  completed or aborted or on an error condition.
		     *  Parameters           :  void
		     *  Return               :  void
		     *  -----------------------------------------------------------------------*/
	LINFLEX_0.LINCR2.B.DTRQ = 1;
}

T_UBYTE GetBufferId(void)
{
	/* ------------------------------------------------------------------------
		     *  Name                 :	GetBufferId
		     *  Description          :  Returns the id (without parity identifier)
		     *   received in the buffer when an Tx or Rx interrupt
		     *  take place.
		     *  Parameters           :  void
		     *  Return               :  T_UBYTE
		     *  -----------------------------------------------------------------------*/
	return (T_UBYTE)LINFLEX_0.BIDR.B.ID;
}

void SetBufferDirection(T_UBYTE lub_Dir)
{
	/* ------------------------------------------------------------------------
		     *  Name                 :	SetBufferDirection
		     *  Description          : Direction
				This bit controls the direction of the data field.
				0: LINFlexD receives the data and copy them in the BDR registers.
				1: LINFlexD transmits the data from the BDR registers.
		     *  Parameters           :  T_UBYTE
		     *  Return               :  void
		     *  -----------------------------------------------------------------------*/
	LINFLEX_0.BIDR.B.DIR = lub_Dir;
}
/* Private functions */
/*============================================================================*/



/* Exported functions */
/*============================================================================*/


