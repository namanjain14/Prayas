/*********************************************************************
 *
 *	UART <-> TCP Bridge Example
 *  Module for Microchip TCP/IP Stack
 *	 -Transmits all incoming TCP bytes on a socket out the UART 
 *    module, all incoming UART bytes out of the TCP socket.
 *	 -Reference: None (hopefully AN833 in the future)
 *
 *********************************************************************
 * FileName:        UART2TCPBridge.c
 * Dependencies:    TCP, Hardware UART module, ARP (optional), 
 *					DNS (optional)
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		06/12/07	Original
 ********************************************************************/
#define __UART2TCPBRIDGE_C

//#include "TCPIPConfig.h"
#include "TCPIP Stack/TCPIP.h"
#include "HardwareProfile.h"
#include "VariableDef.h"

// TCP listen port and UART baud rate settings.  Due to a current C18 
// preprocessor bug (tested on compiler 3.30), the BAUD_RATE definition needs 
// to be placed above the #if defined(STACK_USE_UART2TCP_BRIDGE) test.  
// Otherwise, it won't compile when STACK_USE_UART2TCP_BRIDGE is UNdefined.  
// Also, HardwareProfile.h also needs to be included before the 
// STACK_USE_UART2TCP_BRIDGE test.
#define UART2TCPBRIDGE_PORT	9761
//#define BAUD_RATE			19200

extern BYTE Read_var_cnt;			//just for testing

//#if defined(STACK_USE_UART2TCP_BRIDGE)


// Comment this define out if we are the server.  
// Insert the appropriate address if we are the client.
//#define USE_REMOTE_TCP_SERVER	"192.168.2.50"
		
/*********************************************************************
 * Function:        void SetGSMUARTParameter(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Sets up the GSM UART parameter. 
 *
 * Note:            None
 ********************************************************************/
void SetGSMUARTParameter(void)
{
	if(DE_BAUD_RATE_GSM[0] == '3' && DE_BAUD_RATE_GSM[1] =='0')
	{
		lli_Baud_Rate_GSM = 300;
	}
	else if(DE_BAUD_RATE_GSM[0] == '1' && DE_BAUD_RATE_GSM[1] == '2')
	{
		lli_Baud_Rate_GSM = 1200;
	}
	else if(DE_BAUD_RATE_GSM[0] == '2' && DE_BAUD_RATE_GSM[1] == '4')
	{
		lli_Baud_Rate_GSM = 2400;
	}
	else if(DE_BAUD_RATE_GSM[0] == '4' && DE_BAUD_RATE_GSM[1] == '8')
	{
		lli_Baud_Rate_GSM = 4800;
	}
	else if(DE_BAUD_RATE_GSM[0] == '9' && DE_BAUD_RATE_GSM[1] =='6')
	{
		lli_Baud_Rate_GSM = 9600;
	}
	else if(DE_BAUD_RATE_GSM[0] == '1' && DE_BAUD_RATE_GSM[1] == '9')
	{
		lli_Baud_Rate_GSM = 19200;
	}
	else if(DE_BAUD_RATE_GSM[0] == '3' && DE_BAUD_RATE_GSM[1] == '8')
	{
		lli_Baud_Rate_GSM = 38400;
	}
	else if(DE_BAUD_RATE_GSM[0] == '5' && DE_BAUD_RATE_GSM[1] == '7')
	{
		lli_Baud_Rate_GSM = 57600;
	}
	else if(DE_BAUD_RATE_GSM[0] == '1' && DE_BAUD_RATE_GSM[1] == '1')
	{
		lli_Baud_Rate_GSM = 115200;
	}

	ui_Mode_GSM = 0x8000;				//used as 8 bit communication
}

/*********************************************************************
 * Function:        void GSM_UARTInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Sets up the UART peripheral for the GSM application
 *
 * Note:            Uses interrupts
 ********************************************************************/
void GSM_UARTInit(void)
{


	lli_Closest_UBRG_Value_GSM =((GetPeripheralClock()+8ul*lli_Baud_Rate_GSM)/16/lli_Baud_Rate_GSM-1);
															//Calculates the nearest value of 
															//UBRG register for output UART
	lli_Baud_Actual_GSM =(GetPeripheralClock()/16/(lli_Closest_UBRG_Value_GSM+1));
															//Calculates the actual BAUD Rate

	UMODE_GSM = ui_Mode_GSM;						//Set output UART enable
	IPC6bits.U1IP = 5;										//and interrupt priority to 5.
	USTA_GSM = 0x00001400;								//Set output UART TX and RX enable
	UBRG_GSM = lli_Closest_UBRG_Value_GSM;			//Stored UBRG value in the UBRG 
															//register
	GSM_RX_ENABLE = SET;

}	

/*********************************************************************
 * Function:        void UART2TCPBridgeTask(void)
 *
 * PreCondition:    Stack is initialized()
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void UART2TCPBridgeTask(void)
{
	Nop();
}

/***************************************************************************************
 * Function:        void _ISR _U1RXInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Copies bytes to and from the local UART TX and 
 *					RX FIFOs
 *
 * Note:            None
 *************************************************************************************/
#if __C30_VERSION__ >= 300
void _ISR __attribute__((__no_auto_psv__)) _U1RXInterrupt(void)
#elif defined(__C30__)
void _ISR _U1RXInterrupt(void)
#else
void _U1RXInterrupt(void)
#endif
{
	WORD i;													//Local Variable to stored data 


	IFS0bits.U1RXIF = FALSE;								//Clear the interrupt flag so we 
															//don't keep entering this ISR

	i = U1RXREG;											//copy the byte into local Variable

	if(	uc_Gsm_Cammand == 1)														
	{
													
		if((i != 0 )&& (i != 0x0A ) && (i != 0x0D))
		{
			*OUART_RXHeadPtr = i;
			OUART_RXHeadPtr++;
		}
	
		if(uc_URLSent ==TRUE)									//if data from server contains 0x0A or 0x0D 
		{														//so to received that data this flag is used
			uc_URLSent = FALSE;
			*OUART_RXHeadPtr++ = i;
		}
	
	}
}
/***********************************************************************************
 * Function:        void _ISR _U1TXInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Copies bytes to and from the local UART TX and 
 *					RX FIFOs
 *
 * Note:            None
 *********************************************************************************/
#if __C30_VERSION__ >= 300
void _ISR __attribute__((__no_auto_psv__)) _U1TXInterrupt(void)
#elif defined(__C30__)
void _ISR _U1TXInterrupt(void)
#else
void _U1TXInterrupt(void)
#endif
{

	IFS0bits.U1TXIF = FALSE;														//clear the TX interrupt flag
														// before sending the data clear this flag so that it gets set after data transmission
	
	if(OUART_TXHeadPtr != OUART_TXTailPtr)
	{
		U1TXREG = *OUART_TXHeadPtr;											//transmit data
		OUART_TXHeadPtr++;
	
	}
	else
	{		
		U1TXREG = *OUART_TXHeadPtr;
		OUART_TXHeadPtr++;	
		GSM_TX_ENABLE = 0;														//Disable the TX interrupt
		
	}
}

/***********************************************************************************
 * Function:        void __attribute((interrupt(ipl6), vector(_UART1_VECTOR),
 *													 nomips16)) U1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The ISR for transmit and Recieved will get call from here.
 *					
 *
 * Note:            None
 *********************************************************************************/
#if defined(__C32__)
void __attribute((interrupt(ipl5), vector(_UART1_VECTOR), nomips16)) U1Interrupt(void)
{
	// FOR UART1 WHICH IS OUTPUT UART
	if(IFS0bits.U1RXIF)
		_U1RXInterrupt();
	if(GSM_TX_ENABLE)
	{
		if(IFS0bits.U1TXIF)
			_U1TXInterrupt();
	}
}
#endif



//#endif	//#if defined(STACK_USE_UART2TCP_BRIDGE)
