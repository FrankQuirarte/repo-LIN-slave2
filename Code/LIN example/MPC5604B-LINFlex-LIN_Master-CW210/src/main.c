/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without
any warranty. CUSTOMER retains the final decision relative to the total design
and functionality of the end product. FREESCALE neither guarantees nor will be
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING,
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED 
TO THE PROJECT BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE 
SERVICES. IN NO EVENT SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL 
DAMAGES ARISING OUT OF THIS AGREEMENT.
CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands 
or actions by anyone on account of any damage, or injury, whether commercial,
contractual, or tortuous, rising directly or indirectly as a result 
of the advise or assistance supplied CUSTOMER in connection with product, 
services or goods supplied under this Agreement.
********************************************************************************
* File:             main.c
* Owner:            stnp002
* Version:          1.0
* Date:             Sep-22-2014
* Classification:   General Business Information
* Brief:            LinFlex0 Lin Master example
********************************************************************************
* Detailed Description: 
* - send header from a LIN Master
* - either receive data from a LIN Slave or transmit a data
* - no interrupt is used, just SW pooling
* 
*
* ------------------------------------------------------------------------------
* Test HW:  XPC560B 144 LQFP MINIMODULE, XPC56XX EVB MOTHERBOARD, SPC5604B 2M27V
* Target :  internal_RAM, Flash
* LinFlex0: Lin Master, 19200 baudrate
* Fsys:     64 MHz PLL with 8 MHz crystal reference
*
* ------------------------------------------------------------------------------
* EVB connections and jumper configuration
*
* XPC56XX EVB MOTHERBOARD
* for LinFlex0 connection to the MC33661 LIN transceiver:
* - RXDA_SEL (near SCI !!!!) jumper over pins 1-2
* - TXDA_SEL (near SCI) jumper over 1-2
* 
* for LIN Master functionality
* - VSUP (J6) jumper fitted
*   lin xceiver will get +12V from the EVB
* - V_BUS (J14) jumper not fitted
* - MASTER_EN jumper fitted
* - LIN_EN jumper fitted
* 
*
********************************************************************************
Revision History:
1.0     Sep-22-2014     stnp002  Initial Version
*******************************************************************************/
#include "MPC5604B_M27V.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

static vuint32_t lin_bidr = 0;
static vuint32_t  rx_data[8];
/*******************************************************************************
* Constants and macros
*******************************************************************************/

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/
static void InitModesAndClks(void);
static void InitPeriClkGen(void);
static void DisableWatchdog(void);
static void LinFlex0_Init(void);

/*******************************************************************************
* Local variables
*******************************************************************************/ 


/*******************************************************************************
* Local functions
*******************************************************************************/


/*******************************************************************************
Function Name : InitModesAndClks
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initialize Fsys 64 MHz PLL with 8 MHz crystal reference
Issues        : NONE
*******************************************************************************/
static void InitModesAndClks(void) 
{
    ME.MER.R = 0x0000001D;        /* Enable DRUN, RUN0, SAFE, RESET modes */
                                  /* Initialize PLL before turning it on: */
    /* Use 1 of the next 2 lines depending on crystal frequency: */
    CGM.FMPLL_CR.R = 0x02400100; /* 8 MHz xtal: Set PLL0 to 64 MHz */   
    /*CGM.FMPLL[0].CR.R = 0x12400100;*//* 40 MHz xtal: Set PLL0 to 64 MHz */   
    ME.RUN[0].R = 0x001F0074;       /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
    ME.RUNPC[0].R = 0x00000010; 	  /* Peri. Cfg. 0 settings: only run in RUN0 mode */
    /* Use the next lines as needed for MPC56xxB/S: */  	    	
    ME.PCTL[48].R = 0x0000;         /* MPC56xxB LINFlex0: select ME.RUNPC[0] */	
    ME.PCTL[68].R = 0x0000;         /* MPC56xxB/S SIUL:  select ME.RUNPC[0] */	

    /* Mode Transition to enter RUN0 mode: */
    ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */  
    while (ME.IS.B.I_MTC != 1) {}    /* Wait for mode transition to complete */    
    ME.IS.R = 0x00000001;           /* Clear Transition flag */    
}

/*******************************************************************************
Function Name : InitPeriClkGen
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Enable peri set 1 to be fsys div by 1
Issues        : NONE
*******************************************************************************/
static void InitPeriClkGen(void) 
{
    /* Use the following code as required for MPC56xxB or MPC56xxS:*/
    CGM.SC_DC[0].R = 0x80;   /* MPC56xxB/S: Enable peri set 1 sysclk divided by 1 */
}

/*******************************************************************************
Function Name : DisableWatchdog
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : disable the watchdog
Issues        : NONE
*******************************************************************************/
static void DisableWatchdog(void) 
{
    SWT.SR.R = 0x0000c520;     /* Write keys to clear soft lock bit */
    SWT.SR.R = 0x0000d928; 
    SWT.CR.R = 0x8000010A;     /* Clear watchdog enable (WEN) */
} 

/*******************************************************************************
Function Name : GPIO_Init
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : GPIO init for buttons
Issues        : NONE
*******************************************************************************/
static void GPIO_Init(void) 
{	
	SIU.PCR[64].R = 0x0103;
	SIU.PCR[65].R = 0x0103;
	SIU.PCR[66].R = 0x0103;
	SIU.PCR[67].R = 0x0103;
}

/*******************************************************************************
Function Name : InitLinFlex0
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : LinFlex0 Init as a LIN Master
Issues        : NONE
*******************************************************************************/
static void LinFlex0_Init(void) 
{	

    /* enter INIT mode */
    LINFLEX_0.LINCR1.R = 0x0081; /* SLEEP=0, INIT=1 */
	
    /* wait for the INIT mode */
    while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}
		
    /* configure pads */
    SIU.PCR[18].R = 0x0604;     /* Configure pad PB2 for AF1 func: LIN0TX */
    SIU.PCR[19].R = 0x0100;     /* Configure pad PB3 for LIN0RX */
    
    /* configure baudrate 19200 */
    /* assuming 64 MHz peripheral set 1 clock */		
    LINFLEX_0.LINFBRR.R = 5;
    LINFLEX_0.LINIBRR.R = 208;
        
    LINFLEX_0.LINCR2.R = 0x20; /* IOBE=1, Bit error resets LIN state machine */
    
    LINFLEX_0.LINTCSR.R = 0;	/* LIN timeout mode, no idle on timeout */
		
    /* enter NORMAL mode */
    /* CCD bit 16 = 0 ... checksum calculation by hardware
       CFD bit 17 = 0 ... checksum field is sent after the required number 
                    of data bytes is sent
       LASE bit 18 = 0 ... Slave automatic resync disable
       AWUM bit 19 = 0 ... The sleep mode is exited on software request
       MBL bits 20:23 = 0b0011 ... 13-bit LIN Master break length
       BF bit 24 = 1 ... An RX interrupt is generated on identifier not matching any filter
       SFTM bit 25 = 0 ... Self Test mode disable
       LBKM bit 26 = 0 ... Loop Back mode disable
       MME bit 27 = 1 ... Master mode
       SBDT bit 28 = 0 ... 11-bit Slave Mode break threshold
       RBLM bit 29 = 0 ... Receive Buffer not locked on overrun
       SLEEP bit 30 = 0
       INIT bit 31 = 0 ... entering Normal mode
       =
       0x0390 
     */
    LINFLEX_0.LINCR1.R = 0x0390;
    
    //LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */
}

/*******************************************************************************
Function Name : TransmitData
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Master Tx frame - example
Issues        : 
*******************************************************************************/
static void TransmitData(uint16_t bidr_value)
{
    /* store the data in the message buffer BDR */
    LINFLEX_0.BDRL.B.DATA0 = 'H';
    LINFLEX_0.BDRL.B.DATA1 = 'e';
    LINFLEX_0.BDRL.B.DATA2 = 'l';
    LINFLEX_0.BDRL.B.DATA3 = 'l';
    
    LINFLEX_0.BDRM.B.DATA4 = 'o';
    LINFLEX_0.BDRM.B.DATA5 = ' ';
    LINFLEX_0.BDRM.B.DATA6 = ',';
    LINFLEX_0.BDRM.B.DATA7 = ' ';

	/* Master to publish x bytes with ID and CCS from bidr_value */
    LINFLEX_0.BIDR.R = bidr_value; //0x1E35;
    
    /* Trigger Frame transmission */
    LINFLEX_0.LINCR2.B.HTRQ = 1;    
    
    /* wait until Master response to the LIN header has been sent successfully */
    while(0 == LINFLEX_0.LINSR.B.DTF) 
    {
        /* track LIN Status for errors */
  
    }
    
    LINFLEX_0.LINSR.R = 0x0002; /* clear the DTF bit */

}

/*******************************************************************************
Function Name : ReceiveData
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Master Rx frame - example
Issues        : 
*******************************************************************************/
static void ReceiveData(uint16_t bidr_value)
{
    
    /* Master to receive x bytes with ID and CCS from bidr_value */
    LINFLEX_0.BIDR.R = bidr_value; 

    /* Trigger Frame transmission */
    LINFLEX_0.LINCR2.B.HTRQ = 1;    
    
    /* wait until Slave response to the LIN header has been receive successfully */
    while(0 == LINFLEX_0.LINSR.B.DRF) 
    {
        /* track LIN Status for errors */
  
    }
    
    	/* read BDR registers */
    	rx_data[0] = LINFLEX_0.BDRL.B.DATA0;
    	rx_data[1] = LINFLEX_0.BDRL.B.DATA1;
    	rx_data[2] = LINFLEX_0.BDRL.B.DATA2;
    	rx_data[3] = LINFLEX_0.BDRL.B.DATA3;
    	rx_data[4] = LINFLEX_0.BDRM.B.DATA4;
    	rx_data[5] = LINFLEX_0.BDRM.B.DATA5;
    	rx_data[6] = LINFLEX_0.BDRM.B.DATA6;
    	rx_data[7] = LINFLEX_0.BDRM.B.DATA7;
    
    /* clear RMB, HRF, DRF and DTF flags */    
    LINFLEX_0.LINSR.R = 0x0207;
    
}


/*******************************************************************************
Function Name : button_check
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : 0..no button pressed, 1-4..button number
Notes         : Check switches on the EVB
Issues        : NONE
*******************************************************************************/
uint16_t button_check(void)
{
	if(SIU.GPDI[64].R==0)
	{
		while(SIU.GPDI[64].R==0){};
			
		return (1);
	}
	if(SIU.GPDI[65].R==0)
	{
		while(SIU.GPDI[65].R==0){};
			
		return (2);
	}
	if(SIU.GPDI[66].R==0)
	{
		while(SIU.GPDI[66].R==0){};
			
		return (3);
	}
	if(SIU.GPDI[67].R==0)
	{
		while(SIU.GPDI[67].R==0){};
			
		return (4);
	}
}



/*******************************************************************************
* Global functions
*******************************************************************************/
int32_t main(void) 
{
    uint32_t i = 0;
    uint16_t button;

    InitModesAndClks();
    InitPeriClkGen();
    DisableWatchdog(); 
    GPIO_Init();
    
    LinFlex0_Init();

    /* Loop forever */
    for (;;) 
    {
        
        button = button_check();
        
        switch(button)
        {
        	case 1:
    			// SW1 pressed, BIDR=0x1E35, Master sends 8bytes, ID=0x35, CCS=0
				TransmitData(0x1E35);   
    		break;
    	   	case 2:
    			// SW2 pressed, BIDR=0x1F34, Master sends 8bytes, ID=0x34, CCS=1
				TransmitData(0x1F34);
    		break;
    		case 3:
    			// SW3 pressed, BIDR=0x1C37, Master receives 8bytes, ID=0x37, CCS=0
				ReceiveData(0x1C37);
    		break;
    		case 4:
    			// SW4 pressed, BIDR=0x1D36, Master receives 8bytes, ID=0x36, CCS=1
				ReceiveData(0x1D36);
    		break;
        
        }
        
        i++;
    }
}

