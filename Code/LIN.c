/*============================================================================*/
/*                         AEP SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*
 * $Source: LIN.c 
 * $Revision: 1.0 
 * Authors: Luis Armando García Valdovinos. Luis Fernando Alvarez Guerrero. 
 * $Date: November 23, 2015 
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/* LINFLEX Initialization, configuration and ISR Functionality    
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
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/* REVISION |   DATE     |          COMMENT                  |    AUTHOR      */
/*----------------------------------------------------------------------------*/
/*   02     | 23/11/2015 | Added LIN initialization function | Luis Alvarez   */
/*          |            | and basic functionality           |                */
/*----------------------------------------------------------------------------*/
/*   04     | 25/11/2015 | Added LIN_TXFLAG functionality    | Luis Alvarez   */
/*          |            | Added LIN_RXFLAG functionality    |                */
/*          |            | Added LIN_ERRFLAG functionality   |                */
/*============================================================================*/

/* Includes */
/*============================================================================*/
#include "HAL\LIN.h"
/*============================================================================*/

/* Constants and types  */
/*============================================================================*/
/*============================================================================*/

/* Variables */
/*============================================================================*/

T_UBYTE		lub_RXID = 0;			/*Variable for Received ID      */
t_cmdType	le_RXCMD = cmd_NONE;	/*variable for Received Command */

/*============================================================================*/

/* Private functions prototypes */
/*============================================================================*/
/*============================================================================*/

/* Inline functions */
/*============================================================================*/
/*============================================================================*/

/* Private functions */
/*============================================================================*/
/*============================================================================*/

/* Exported functions */
/*============================================================================*/

/* -----------------------------------------------------------------------
*	Name			:	linflex0_init  
*	Description		:	LINFlex channel 0 initialization Function
*	Parameters		:	void 
*	Return			:	void
*  -----------------------------------------------------------------------
*/
void linflex0_init(void) 
{

	/* Check pag. 478 of MPC5606B Datasheet for LIN control register 1 (LINFLEX_0_LINCR1)*/
	LINFLEX_0.LINCR1.B.INIT  = 1;     /* Put LINFlex hardware in initialization mode     */    
    while(LINFLEX_0.LINSR.B.LINS != 1)
    	{
    		/*Wait for INIT state*/
    	} 
    
    LINFLEX_0.UARTCR.B.UART = 0;	  /* UART working on LIN mode                        */
    
    LINFLEX_0.LINCR1.B.MBL   = 0x03;  /* LIN Master Break Length: 13 bits                */ 
    LINFLEX_0.LINCR1.B.BF    = 1;     /* Bypass Filter: Enabled                          */ 
    LINFLEX_0.LINCR1.B.MME   = 0;     /* Master Mode Enable: Slave                       */    
    LINFLEX_0.LINCR1.B.SBDT  = 0;     /* Slave Mode Break Detection Threshold:  11 bits  */
    LINFLEX_0.LINCR1.B.RBLM  = 0;     /* Receive Buffer: Not Locked on overrun           */
    LINFLEX_0.LINCR1.B.CCD   = 0;     /* Checksum Calculation is done by hardware.       */
    LINFLEX_0.LINCR1.B.CFD   = 0;     /* Checksum field: Enabled                         */ 
    LINFLEX_0.LINCR1.B.LASE  = 1;     /* LIN Slave Automatic Resynchronization: enabled   */
	    
    /* Check pag. 481 of MPC5606B Datasheet for LIN interrupt enable register  (LINFLEX_0_LINIER)                                            */
    LINFLEX_0.LINIER.B.SZIE  = 0;  /*Stuck at Zero Interrupt: Disabled, No interrupt when SZF bit in LINESR or UARTSR is set.                */
    LINFLEX_0.LINIER.B.OCIE  = 0;  /*Output Compare Interrupt: Disabled, No interrupt when OCF bit in LINESR or UARTSR is set.               */	
    LINFLEX_0.LINIER.B.BEIE  = 0;  /*Bit error Interrupt generated when BEF bit in LINESR is set.                                            */
    LINFLEX_0.LINIER.B.CEIE  = 1;  /*Checksum error, Interruption generated when checksum error flag (CEF) in LINESR is set.                 */
    LINFLEX_0.LINIER.B.HEIE  = 1;  /*Header error, Interrupt generated on Break Delimiter error, Synch Field error, Identifier field error.  */
    LINFLEX_0.LINIER.B.FEIE  = 1;  /*Frame Error Interrupt: Enabled, interrupt on Framing error.                                         */
    LINFLEX_0.LINIER.B.BOIE  = 0;  /*Buffer Overrun Error Interrupt: Disabled,No interrupt on Buffer overrun.                                */
    LINFLEX_0.LINIER.B.LSIE  = 0;  /*LIN State Interrupt: Disabled, No interrupt on LIN state change.                                        */
    LINFLEX_0.LINIER.B.WUIE  = 0;  /*Wake up Interrupt: Disabled,No interrupt when WUF bit in LINSR or UARTSR is set.                        */  
    LINFLEX_0.LINIER.B.DBFIE = 0;  /*Data Buffer Full Interrupt: Disabled, No interrupt when buffer data register is full.                   */ 
    LINFLEX_0.LINIER.B.DBEIE = 0;  /*Data Buffer Empty Interrupt: Disabled, Data Buffer Empty Interrupt Enable                               */
    LINFLEX_0.LINIER.B.DRIE  = 1;  /*Interruption generated when data received flag (DRF) in LINSR or UARTSR is set.                         */	
    LINFLEX_0.LINIER.B.DTIE  = 1;  /*Interruption generated when data transmitted flag (DTF) is set in LINSR or UARTSR.                      */
    LINFLEX_0.LINIER.B.HRIE  = 1;  /*Interrupt generated when a valid LIN header has been received, that is, HRF bit in LINSR is set.        */
         
    /* Check pag. 493 of MPC5606B Datasheet for LIN Control Register 2 (LINCR2)                                           */
    LINFLEX_0.LINCR2.B.IOBE = 0; /*Idle on bit error: Disabled, Bit error does not reset state machine                    */
    LINFLEX_0.LINCR2.B.IOPE = 0; /*Idle on Bit Error: Disabled, Identifier Parity error does not reset LIN state machine. */  
    LINFLEX_0.LINCR2.B.WURQ = 0; /*Wake-up Generation Request: Disabled                                                   */   
    LINFLEX_0.LINCR2.B.DDRQ = 0; /*Data Discard Request: Disabled                                                          */
    LINFLEX_0.LINCR2.B.DTRQ = 0; /*Data Transmission Request: Disabled                                                     */
    LINFLEX_0.LINCR2.B.ABRQ = 0; /*Abort Request: Disabled                                                                */
    LINFLEX_0.LINCR2.B.HTRQ = 0; /*Header Transmission Request: Disabled                                                 */

    /* Check pag. 473,474 and 492 of MPC5606B Datasheet for LIN integer baudrate register  (LINFLEX_0_LINIBRR)  */
    LINFLEX_0.LINIBRR.R = 0x00D0;  
    /* Baudrate : 19201.9 Symbols/sec, Integer Baud Rate Factor: 208   */
    /* Tx/Rx baud =(fperiph_set_1_clk /(16 × LFDIV)  with 0.01% Error  */
    
    /* Check pag. 473,474 and 491 of MPC5606B Datasheet for LIN fractional baudrate register (LINFLEX_0_LINFBRR)*/
    LINFLEX_0.LINFBRR.R = 0x0005;  /* Fractional Baud Rate Factor: 5 */
    
    /* Check pag. 499 of MPC5606B Datasheet for Identifier filter mode register  (LINFLEX_0_IFMR)   */
    LINFLEX_0.IFMR.R = 0x0000;  /*Disable filters to be able to set them manually                   */

    /* Check pag. 497 of MPC5606B Datasheet for Identifier filter enable register  (LINFLEX_0_IFER) */
    LINFLEX_0.IFER.R = 0x0003;  /*Disable filters to be able to set them manually                   */
        
    //TODO: Set the IDfor the next 4 filters
    /* Check pag. 500 of MPC5606B Datasheet for Identifier filter control register  (LINFLEX_0_IFCR0)           */
    /* MASTER_CMD_ALL */
    LINFLEX_0.IFCR[0].R = 0x043C; 
    /* Data Field Length (Bytes): 1              */
    /* Direction:                 Receive        */
    /* Checksum:                  Enhanced       */
    /* Identifier:                CF             */

    LINFLEX_0.IFCR[1].R = 0x0402;
    /* MASTER_CMD_SLV1                           */
    /* Data Field Length (Bytes): 1              */
    /* Direction:                 Receive        */
    /* Checksum:                  Enhanced       */
    /* Identifier:                50             */

    LINFLEX_0.IFCR[2].R = 0x0A01;
    /* SLAVE1_RSP                                */
    /* Data Field Length (Bytes): 2              */
    /* Direction:                 Transmit       */
    /* Checksum:                  Enhanced       */
    /* Identifier:                20             */

    LINFLEX_0.IFCR[3].R = 0x1E03;
    /* SLAVE1_ID                                 */
    /* Data Field Length (Bytes): 7              */
    /* Direction:                 Transmit       */
    /* Checksum:                  Enhanced       */
    /* Identifier:                F0             */

    LINFLEX_0.IFER.R         = 0x3;  /* Activate Filters                        */
    LINFLEX_0.LINCR1.B.INIT = 0; /* LIN operational */
    while(LINFLEX_0.LINSR.B.LINS != 2)
    	{
    		/*Wait for IDLE state*/
    	}
}

/* -----------------------------------------------------------------------
*	Name			:	LIN_RXFlag  
*	Description		:	Check LINFlex Rx flag and Receive Data
*	Parameters		:	void 
*	Return			:	void
*  -----------------------------------------------------------------------
*/
void LIN_RXFLAG(void)
{
	if(LINFLEX_0.LINSR.B.RMB == 1)     /* Buffer ready to be read by software */
		{
			LINFLEX_0.LINSR.B.RMB = 0; /*set Flag as Buffer is free           */
		}
		
	if((LINFLEX_0.BIDR.B.ID == MASTER_CMD_ALL ) || (LINFLEX_0.BIDR.B.ID ==  MASTER_CMD_SLV1))
		{   /* Check ID for Master Command to all or Master Command to Slave 01 */
			lub_RXID = LINFLEX_0.BIDR.B.ID;    /* Save ID in lub_RxID      */
			le_RXCMD = LINFLEX_0.BDRL.B.DATA0; /* Save Command in le_RXCMD */
		}
	else if((LINFLEX_0.BIDR.B.ID == SLAVE1_RSP) || (LINFLEX_0.BIDR.B.ID == SLAVE1_ID))
		{
			/* Check ID for Slave 01 Response or Slave 01 ID and Do Nothing*/
		}
	else
		{
			/* Do nothing */
		}
}

/* -----------------------------------------------------------------------
*	Name			:	LIN_TXFlag  
*	Description		:	Check LINFlex Tx flag and Transfer Data
*	Parameters		:	void 
*	Return			:	void
*  -----------------------------------------------------------------------
*/
void LIN_TXFLAG(void)
{
	if(LINFLEX_0.LINSR.B.DTF == 1)
		{
			LINFLEX_0.LINSR.B.DTF = 0;
		}
		
	if((LINFLEX_0.BIDR.B.ID == SLAVE1_RSP) || (LINFLEX_0.BIDR.B.ID == SLAVE1_ID))
		{
			/* Check ID for Slave 01 Response or Slave 01 ID */
			lub_RXID = LINFLEX_0.BIDR.B.ID;    /* Save ID in lub_RxID      */
			le_RXCMD = LINFLEX_0.BDRL.B.DATA0; /* Save Command in le_RXCMD */
		}
	else if((LINFLEX_0.BIDR.B.ID == MASTER_CMD_ALL) || (LINFLEX_0.BIDR.B.ID == MASTER_CMD_SLV1))
		{
		/* Check ID for Master Command to all or Master Command to Slave 01 and Do Nothing*/
		}
	else
		{
			/* Do nothing */
		}
	if(LINFLEX_0.LINSR.B.RMB == 1)
		{
			LINFLEX_0.LINSR.B.RMB = 0;
		}
		
}

/* -----------------------------------------------------------------------
*	Name			:	LIN_ERRFlag  
*	Description		:	Interrupt function when error flag is detected
*	Parameters		:	void 
*	Return			:	void
*  -----------------------------------------------------------------------
*/
void LIN_ERRFLAG(void)
{
	LINFLEX_0.LINESR.B.BEF = 1; /* Clearing Bit Error Flag */
}

/*============================================================================*/

/* Notice: the file ends with a blank new line to avoid compiler warnings */
