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
* Brief:            LinFlex0 Lin Slave example
********************************************************************************
* Detailed Description: 
* - receive header from a LIN Master
* - either receive data from a LIN Master or transmit a data
* - Filter can be enabled with the FILT_EN = 1
* - If filter is enabled TX interrupt is used to prepare data to send and 
*	RX interrupt to read received data
* - If filter is disabled SW polling is used
*
* ------------------------------------------------------------------------------
* Test HW:  XPC560B 144 LQFP MINIMODULE, XPC56XX EVB MOTHERBOARD, SPC5604B 2M27V
* Target :  internal_RAM
* LinFlex0: Lin Slave, 19200 baudrate
* Fsys:     64 MHz PLL with 8 MHz crystal reference
*
* ------------------------------------------------------------------------------
* EVB connections and jumper configuration
*
* XPC56XX EVB MOTHERBOARD
* for LinFlex0 connection to the MC33661 LIN transceiver:
* - RXDA_SEL (near SCI !!!!) jumper over pins 1-2
* - TXDA_SEL (near SCI) jumper over pins 1-2
* 
* for LIN Slave functionality
* - VSUP (J6) jumper not fitted ...LIN transceiver will get +12V from the Master
* - V_BUS jumper not fitted
* - MASTER_EN jumper not fitted
* - LIN_EN jumper fitted
*
********************************************************************************
Revision History:
1.0     Sep-22-2014     stnp002  Initial Version
*******************************************************************************/
#include "MPC5604B_M27V.h"
//#include "IntcInterrupts.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Constants and macros
*******************************************************************************/
#define FILT_EN 1
/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/
static void InitModesAndClks(void);
static void InitPeriClkGen(void);
static void DisableWatchdog(void);
static void InitLinFlex0(void);
static void InitHW(void);
static void InitSysclk(void); 
static void LinSlaveDataReception(void);
static void LinSlaveDataTransmission(void);

/*******************************************************************************
* Local variables
*******************************************************************************/ 

static vuint32_t lin_status = 0;
static vuint32_t lin_bidr = 0;
static vuint32_t lin_bidr_ID = 0;
static vuint32_t  rx_data[8];

/*******************************************************************************
* Local functions
*******************************************************************************/

/*******************************************************************************
Function Name : InitHW
Engineer      : b08110
Date          : Feb-11-2010
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initialization of the hw for the purposes of this example
Issues        : NONE
*******************************************************************************/
static void InitHW(void)
{
    InitSysclk();
    InitLinFlex0();    
}

/*******************************************************************************
Function Name : InitSysclk
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initialize Fsys 64 MHz PLL with 8 MHz crystal reference
Issues        : NONE
*******************************************************************************/
static void InitSysclk(void)
{
    InitModesAndClks();
    InitPeriClkGen();
    DisableWatchdog(); 
}

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
	SIU.PCR[68].R = 0x0204;
	SIU.GPDO[68].R = 0x1;
}


/*******************************************************************************
Function Name : InitLinFlex0
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Init LinFlex0 as a LIN Slave
Issues        : NONE
*******************************************************************************/
static void InitLinFlex0(void) 
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
    
    LINFLEX_0.LINTCSR.R = 0; /* LIN timeout mode, no idle on timeout */

	LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */

#if (FILT_EN==0)
    /* enter NORMAL mode  without filters*/
    /* CCD bit 16 = 0 ... checksum calculation by hardware
       CFD bit 17 = 0 ... checksum field is sent after the required number 
                    of data bytes is sent
       LASE bit 18 = 1 ... Slave automatic resync enable
       AWUM bit 19 = 0 ... The sleep mode is exited on software request
       MBL bits 20:23 = 0b0011 ... 13-bit LIN Master break length
       BF bit 24 = 1 ... An RX interrupt is generated on identifier not matching any filter
       SFTM bit 25 = 0 ... Self Test mode disable
       LBKM bit 26 = 0 ... Loop Back mode disable
       MME bit 27 = 0 ... Slave mode
       SBDT bit 28 = 0 ... 11-bit Slave Mode break threshold
       RBLM bit 29 = 0 ... Receive Buffer not locked on overrun
       SLEEP bit 30 = 0
       INIT bit 31 = 0 ... entering Normal mode
       =
       0x2380 

     */
    LINFLEX_0.LINCR1.R = 0x2380;
#else
	/* enter NORMAL mode  with filters*/
	LINFLEX_0.IFER.R = 0xF;		// enable filters 0-3
	LINFLEX_0.IFMR.R = 0x0;		// filters 0 - 3 are in identifier list mode.
	
	LINFLEX_0.IFCR0.R = 0x1E37;	// 8bytes, TX data, ID=0x37, CCS=0 enhanced checksum on current msg
	LINFLEX_0.IFCR1.R = 0x1C35;	// 8bytes, RX data, ID=0x35, CCS=0 enhanced checksum on current msg
	
	LINFLEX_0.IFCR2.R = 0x1F36; // 8bytes, TX data, ID=0x36, CCS=1 classic checksum on current msg
	LINFLEX_0.IFCR3.R = 0x1D34;	// 8bytes, RX data, ID=0x34, CCS=1 classic checksum on current msg
	
	LINFLEX_0.LINIER.R = 0x7;	// enable RX, TX and header interrupt
	
	LINFLEX_0.LINCR1.R = 0x2300; // master break length reg MBL 11b = 13 bit length
	// LASE=1 slave automaticresynch enable
#endif    
    
}

/*******************************************************************************
Function Name : LinSlaveReceiveHeader
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - polled wait for a header and id reception.
Issues        : NONE
*******************************************************************************/
static void LinSlaveReceiveHeader(void)
{
    /* wait for LINSR[HRF] (identifier received) */
    while(0 == LINFLEX_0.LINSR.B.HRF) {};
        
    /* read the received id from BIDR */
    lin_bidr = LINFLEX_0.BIDR.R;
    lin_bidr_ID = lin_bidr&0x3F;
    
    if(lin_bidr_ID==0x34 || lin_bidr_ID==0x35) LinSlaveDataReception();
    
    if(lin_bidr_ID==0x36 || lin_bidr_ID==0x37) LinSlaveDataTransmission();
    
    
}

/*******************************************************************************
Function Name : LinSlaveDataReception
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - data reception (subscriber)
Issues        : up to 8 bytes. for longer frames DFL>7, DBFF have to be used
*******************************************************************************/
static void LinSlaveDataReception(void)
{
    
    /* specify the data field length BIDR[DFL] */
    LINFLEX_0.BIDR.B.DFL = 0x07; /* 8 bytes - 1 */
    LINFLEX_0.BIDR.B.DIR = 0; /* BDR direction read */
    
    if((lin_bidr&0x3F)==0x35) LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */
    if((lin_bidr&0x3F)==0x34) LINFLEX_0.BIDR.B.CCS = 1; /* classic checksum for LIN Slave */
    
    while(0 == LINFLEX_0.LINSR.B.DRF) {};  /* wait till data received */
                
    /* read BDR registers */
    rx_data[0] = LINFLEX_0.BDRL.B.DATA0;
    rx_data[1] = LINFLEX_0.BDRL.B.DATA1;
    rx_data[2] = LINFLEX_0.BDRL.B.DATA2;
    rx_data[3] = LINFLEX_0.BDRL.B.DATA3;
    rx_data[4] = LINFLEX_0.BDRM.B.DATA4;
    rx_data[5] = LINFLEX_0.BDRM.B.DATA5;
    rx_data[6] = LINFLEX_0.BDRM.B.DATA6;
    rx_data[7] = LINFLEX_0.BDRM.B.DATA7;
    
    /* clear RMB, HRF and DRF flags */    
    LINFLEX_0.LINSR.R = 0x0205;
}

/*******************************************************************************
Function Name : LinSlaveDataTransmission
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - data transmission (publisher)
Issues        : up to 8 bytes. for longer frames DFL>7, DBEF have to be used
*******************************************************************************/
static void LinSlaveDataTransmission(void)
{
    static uint32_t toggle = 0;

    LINFLEX_0.BIDR.B.DIR = 1; /* BDR direction - write */
    
    if((lin_bidr&0x3F)==0x37) LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */
    if((lin_bidr&0x3F)==0x36) LINFLEX_0.BIDR.B.CCS = 1; /* classic checksum for LIN Slave */
    
    /* fill the BDR registers */
    LINFLEX_0.BDRL.B.DATA0 = toggle;
    LINFLEX_0.BDRL.B.DATA1 = 0x55;
    LINFLEX_0.BDRL.B.DATA2 = 0xAA;
    LINFLEX_0.BDRL.B.DATA3 = 0x55;
    LINFLEX_0.BDRM.B.DATA4 = 'o';
    LINFLEX_0.BDRM.B.DATA5 = ' ';
    LINFLEX_0.BDRM.B.DATA6 = ',';
    LINFLEX_0.BDRM.B.DATA7 = ' ';
        
    /* specify the data field length BIDR[DFL] */
    LINFLEX_0.BIDR.B.DFL = 0x07; /* 8 bytes - 1 */        
    
    /* trigger the data transmission */
    LINFLEX_0.LINCR2.B.DTRQ = 1;
    
    /* wait for data transmission complete flag */
    while(0 == LINFLEX_0.LINSR.B.DTF) {}
    
    /* clear HRF and DTF flags */    
    LINFLEX_0.LINSR.R = 0x0003;
    
    /* toggle dat for next frame */
    if(toggle==0) toggle = 1;
    else toggle = 0;
}

/*******************************************************************************
Function Name : LINFlex_0_RX_ISR
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : rx_data - received data
Returns       : NONE
Notes         : Rx data ISR on LINFlex_0. 
				Called if there is match with RX filter and data is received 
Issues        : NONE 
*******************************************************************************/
static void LINFlex_0_RX_ISR(void)
{
	lin_status = LINFLEX_0.LINSR.R;
		/* wait for RMB */
	while (1 != LINFLEX_0.LINSR.B.RMB) {}  /* Wait for Release Message Buffer */
	
	/* get the data */
    rx_data[0] = LINFLEX_0.BDRL.B.DATA0;
    rx_data[1] = LINFLEX_0.BDRL.B.DATA1;
    rx_data[2] = LINFLEX_0.BDRL.B.DATA2;
    rx_data[3] = LINFLEX_0.BDRL.B.DATA3;
    rx_data[4] = LINFLEX_0.BDRM.B.DATA4;
    rx_data[5] = LINFLEX_0.BDRM.B.DATA5;
    rx_data[6] = LINFLEX_0.BDRM.B.DATA6;
    rx_data[7] = LINFLEX_0.BDRM.B.DATA7;

	/* clear the DRF and RMB flags by writing 1 to them */
	LINFLEX_0.LINSR.R = 0x0205;



}

/*******************************************************************************
Function Name : LINFlex_0_TX_ISR
Engineer      : stnp002
Date          : Sep-22-2014
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Tx data ISR on LINFlex_0
				Called if tehre is a match with TX filter and header is received or 
				data are transmitted. 
Issues        : NONE 
*******************************************************************************/
static void LINFlex_0_TX_ISR(void)
{
	static uint32_t toggle = 0;

	lin_status = LINFLEX_0.LINSR.R;
	lin_bidr = LINFLEX_0.BIDR.R;
    lin_bidr_ID = lin_bidr&0x3F;
	
	if(lin_status&0x1)		/* if header received */
	{
		SIU.GPDO[68].R = 0x0;
		
	//	LINFLEX_0.LINSR.R = 0x1;
		
		LINFLEX_0.BIDR.B.DIR = 0; /* BDR direction - write */
    
    //	if(lin_bidr_ID==0x37) LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */
    //	if(lin_bidr_ID==0x36) LINFLEX_0.BIDR.B.CCS = 1; /* classic checksum for LIN Slave */
    
	    /* fill the BDR registers */
	    LINFLEX_0.BDRL.B.DATA0 = toggle;
	    LINFLEX_0.BDRL.B.DATA1 = 0x55;
	    LINFLEX_0.BDRL.B.DATA2 = 0xAA;
	    LINFLEX_0.BDRL.B.DATA3 = 0x55;
	    LINFLEX_0.BDRM.B.DATA4 = 'o';
	    LINFLEX_0.BDRM.B.DATA5 = ' ';
	    LINFLEX_0.BDRM.B.DATA6 = ',';
	    LINFLEX_0.BDRM.B.DATA7 = ' ';
	        
	    /* specify the data field length BIDR[DFL] */
	//  LINFLEX_0.BIDR.B.DFL = 0x07; /* 8 bytes - 1 */        
	    
	    /* trigger the data transmission */
	   	LINFLEX_0.LINCR2.B.DTRQ = 1;
		LINFLEX_0.LINSR.R = 0x1;	
	}
	
	if(lin_status&0x2)	/* if data transmitted */
	{
		SIU.GPDO[68].R = 0x1;
		
		LINFLEX_0.LINSR.R = 0x2;
		
		/* toggle dat for next frame */
    	if(toggle==0) toggle = 1;
    	else toggle = 0;
	}
	
}



/*******************************************************************************
* Global functions
*******************************************************************************/
int32_t main(void) 
{
    uint32_t i = 0;

    InitHW(); 
    GPIO_Init();
  
  
#if(FILT_EN)
	/* enable LinFlex0 Rx and TX interrupt in INTC */
    INTC_InstallINTCInterruptHandler(LINFlex_0_RX_ISR, 79, 1);
    INTC_InstallINTCInterruptHandler(LINFlex_0_TX_ISR, 80, 2);
    INTC.CPR.R = 0x0; /* lower current priority - to enable INTC to process interrupts */
    
#endif
    /* Loop forever */
    for (;;) 
    {
#if(FILT_EN==0)						// if filters are disabled 
        LinSlaveReceiveHeader();	// wait for header received
#endif        
        i++;
    }
}








