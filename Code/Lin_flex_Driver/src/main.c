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
* Owner:            b08110
* Version:          1.0
* Date:             Jan-07-2011
* Classification:   General Business Information
* Brief:            LinFlex0 Lin 2.0 Slave example
********************************************************************************
* Detailed Description: 
* - receive header from a LIN Master
* - either receive data from a LIN Master or transmit a data
* 
*
* ------------------------------------------------------------------------------
* Test HW:  XPC560B 144 LQFP MINIMODULE, XPC56XX EVB MOTHERBOARD, PPC5604B 0M27V
* Target :  internal_RAM
* LinFlex0: Lin Slave, 19200 baudrate
* Fsys:     64 MHz PLL with 8 MHz crystal reference
*
* ------------------------------------------------------------------------------
* EVB connections and jumper configuration
*
* XPC56XX EVB MOTHERBOARD
* for LinFlex0 connection to the MC33661 LIN transceiver:
* - RXDA_SEL (near SCI !!!!) shunt over upper two pins
* - TXDA_SEL (near SCI) shunt over upper two pins
* - that is for PB2 connection to U5 and PB3 connection to U5
* 
* for LIN Slave functionality
* - VSUP (J6 on the schematic) shunt disconnected
*   lin xceiver will get +12V from the Master
* - V_BUS shunt disconnected
* - MASTER_EN shunt disconnected
*
* for connection from a LIN Master
* LIN Master        Lin Slave
* +12V (VSUP) ------ VSUP pin 1 (left) (this will connect +12V from Master
*                    directly to the +12V pin of the LIN transceiver
* GND         ------ GND
* LIND        ------ upper right pin of the LIN connector when looking from
*                    outside of the board towards the board
*                           -------
                            |  |D |
                            -------
                            |  |  |
                            -------
*
* NOTE !!! this connector seems to be differently populated than
*          on the MPC5554EVB
* Thus please make sure by a electiral meter that the connecitons
* between Master and Slave are correct.
* 
* 
*
********************************************************************************
Revision History:
1.0     Jan-07-2011     b08110  Initial Version
*******************************************************************************/
#include "MPC5604B_M27V.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

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
static void InitLinFlex0(void);
static void InitHW(void);
static void InitSysclk(void); 

/*******************************************************************************
* Local variables
*******************************************************************************/ 
static vuint32_t lin_status = 0;
static vuint32_t lin_status_old = 0;
static vuint32_t temp = 0;
static vuint32_t lin_esr = 0;
static vuint32_t lin_esr_last = 0;
static vuint32_t lin_status_last = 0;

static vuint32_t lin_bidr = 0;
static vuint32_t  rx_data[4];

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
Function Name : InitLinFlex0
Engineer      : b08110
Date          : Jan-07-2011
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
    //SIU.PCR[18].R = 0x0604;     /* Configure pad PB2 for AF1 func: LIN0TX */
    //SIU.PCR[19].R = 0x0100;     /* Configure pad PB3 for LIN0RX */
    
    SIU.PCR[18].R = 0x0400;         /* MPC56xxB: Configure port B2 as LIN0TX */
    SIU.PCR[19].R = 0x0103;         /* MPC56xxB: Configure port B3 as LIN0RX */	
	
    /* configure baudrate 19200 */
    /* assuming 64 MHz peripheral set 1 clock */		
    LINFLEX_0.LINFBRR.R = 5;
    LINFLEX_0.LINIBRR.R = 208;
        
    LINFLEX_0.LINCR2.R = 0x20; /* clear IOBE */
    
    /* default timeout */
    LINFLEX_0.LINTCSR.R = 0;
		
    /* enter NORMAL mode */
    /* CCD bit 16 = 0 for checksum calculation by hardware
       CFD bit 17 = 0 for checksum field is sent after the required number 
                    of data bytes is sent
       LASE bit 18 = 1 for Slave automatic resync enable
       AWUM bit 19 = 0 for The sleep mode is exited on software request
       MBL bits 20:23 = 0b0011 for 13-bit LIN Master break length
       BF bit 24 = 1 for bypass filter interrupt on ID not matching any filter
       SFTM bit 25 = 0 for Self Test mode disable
       LBKM bit 26 = 0 for Loop Back mode disable
       MME bit 27 = 0 for Slave mode
       SBDT bit 28 = 0 for 11-bit Slave Mode break threshold
       RBLM bit 29 = 0 for Receive Buffer not locked on overrun
       SLEEP bit 30 = 0
       INIT bit 31 = 0 for entering Normal mode
       =
       0x0390 
     */
    LINFLEX_0.LINCR1.R = 0x2380;
    
    LINFLEX_0.BIDR.B.CCS = 0; /* enhanced checksum for LIN Slave */
}

/*******************************************************************************
Function Name : TransmitData
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Master Tx frame - example
Issues        : Not used in this example. Just a relict since the Slave code
                was started from a Master code.
*******************************************************************************/
static void TransmitData(void)
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

#if 1    
    /* Master to publish 8 bytes. ID = 0x35 */
    LINFLEX_0.BIDR.R = 0x1E35;
#else
    /* Master to send header only. ID = 0x35 */
    LINFLEX_0.BIDR.R = 0x0035;
#endif
    
    /* Trigger Frame transmission */
    LINFLEX_0.LINCR2.B.HTRQ = 1;    
    
    /* wait until Master response to the LIN header has been sent successfully */
    while(0 == LINFLEX_0.LINSR.B.DTF) 
    {
        /* track LIN Status */
        temp = LINFLEX_0.LINSR.R;        
        lin_status = temp & 0x000F000;
        lin_esr = LINFLEX_0.LINESR.R;
        
        if((lin_esr & 0x00002000)==0x00002000)
        {
            /* BEF detected */
            lin_esr_last = lin_esr;
            lin_status_last = lin_status;
            
            LINFLEX_0.LINESR.R = 0x00002000;
            
            /* abort transmission */
            LINFLEX_0.LINCR2.B.ABRQ = 1;
            
            /* wait until cleared by hardware */
            while(0 != LINFLEX_0.LINCR2.B.ABRQ) 
            {            
            }
            
            while(1)
            {
                
            }
        }
        
        if(lin_status > lin_status_old)
        {
            lin_status_old = lin_status;
        }
    }
    
    LINFLEX_0.LINSR.R = 0x0002; /* clear the DTF bit */
}

/*******************************************************************************
Function Name : LinSlaveReceiveHeader
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - polled wait for a header and id reception.
Issues        : NONE
*******************************************************************************/
static void LinSlaveReceiveHeader(void)
{
    /* wait for LINSR[HRF] (identifier received) */
    while(0 == LINFLEX_0.LINSR.B.HRF) {}
        
    /* read the received id from BIDR */
    lin_bidr = LINFLEX_0.BIDR.R;
}

/*******************************************************************************
Function Name : LinSlaveDataReception
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - data reception (subscriber)
Issues        : up to 8 bytes. for longer frames DFL>7, DBFF have to be used
*******************************************************************************/
static void LinSlaveDataReception(void)
{
    LinSlaveReceiveHeader();
    
    /* specify the data field length BIDR[DFL] */
    LINFLEX_0.BIDR.B.DFL = 0x03; /* 4 bytes - 1 */
    LINFLEX_0.BIDR.B.DIR = 0; /* BDR direction read */
    
    while(0 == LINFLEX_0.LINSR.B.DRF) {}
                
    /* read BDR registers */
    rx_data[0] = LINFLEX_0.BDRL.B.DATA0;
    rx_data[1] = LINFLEX_0.BDRL.B.DATA1;
    rx_data[2] = LINFLEX_0.BDRL.B.DATA2;
    rx_data[3] = LINFLEX_0.BDRL.B.DATA3;
    
    /* clear HRF and DRF flags */    
    LINFLEX_0.LINSR.R = 0x0005;
}

/*******************************************************************************
Function Name : LinSlaveDataTransmission
Engineer      : b08110
Date          : Jan-07-2011
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Lin Slave - data transmission (publisher)
Issues        : up to 8 bytes. for longer frames DFL>7, DBEF have to be used
*******************************************************************************/
static void LinSlaveDataTransmission(void)
{
    static uint32_t toggle = 0;

    LinSlaveReceiveHeader();
    
    LINFLEX_0.BIDR.B.DIR = 1; /* BDR direction - write */
    
    /* fill the BDR registers */
    LINFLEX_0.BDRL.B.DATA0 = toggle;
    LINFLEX_0.BDRL.B.DATA1 = 0x55;
    LINFLEX_0.BDRL.B.DATA2 = 0xAA;
    LINFLEX_0.BDRL.B.DATA3 = 0x55;
        
    /* specify the data field length BIDR[DFL] */
    LINFLEX_0.BIDR.B.DFL = 0x03; /* 4 bytes - 1 */        
    
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
* Global functions
*******************************************************************************/
int32_t main(void) 
{
    uint32_t i = 0;

    InitHW(); 
  

    /* Loop forever */
    for (;;) 
    {
        //LinSlaveDataReception();
        LinSlaveDataTransmission();
        
        i++;
    }
}




