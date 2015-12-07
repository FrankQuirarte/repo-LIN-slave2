/*
 * GPIO.c
 *
 *  Created on: Dec 3, 2015
 *      Author: x
 */

#include "MAL\GPIO.h"


void GPIO_En(T_SWORD lsw_ch, T_UWORD luw_state)
{	
	/* ------------------------------------------------------------------------
     *  Name                 :	Init_GPIO_En
     *  Description          :  Program the drive enable pin
     *  Parameters           :  T_SWORD ch, T_ULONG state
     *  Return               :  void
     *  -----------------------------------------------------------------------
	 * state = 0x0200 -> enable as output                   
	 * state = 0x0100 -> enable as input                    
	 * state = 0x0103 -> enable as input, Pull - up
	 * state = 0x2000 -> enable as APC "Analog Pad Control" or ANP "Analog Port"
	 */
	 
	SIU.PCR[lsw_ch].R = luw_state;				
}

void GPIO_Init(void)
{
	/* ------------------------------------------------------------------------
	 *  Name                 :	GPIO_Init
	 *  Description          :  Call GPIO_En for the ports that will be used
	 *  Parameters           :  void
	 *  Return               :  void
	 *  -----------------------------------------------------------------------*/	
	GPIO_En(LED1, OUTPUT);	 /* Program the drive enable pin of LED3 (PE6) as output       */
	/*For test purposes*/
	GPIO_En(SW_A_PINCH, INPUT);
}


void GPIO_SetState (T_UWORD ch, T_UBYTE value)
{/* --------------------------------------------------------------------------
	*  Name                 :  GPIO_SetState
	*  Description          :  Set the value received to the pin specified in ch
	*  Parameters           :  T_UWORD, T_UBYTE
	*  Return               :  void
	*  -------------------------------------------------------------------------
	*/
    SIU.GPDO[ch].B.PDO = value;
}


/* Notice: the file ends with a blank new line to avoid compiler warnings */
