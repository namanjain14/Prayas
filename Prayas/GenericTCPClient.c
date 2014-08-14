/*********************************************************************
 *
 *  Generic TCP Client Example Application
 *  Module for Microchip TCP/IP Stack
 *   -Implements an example HTTP client and should be used as a basis 
 *	  for creating new TCP client applications
 *	 -Reference: None.  Hopefully AN833 in the future.
 *
 *********************************************************************
 * FileName:        GenericTCPClient.c
 * Dependencies:    TCP, DNS, ARP, Tick
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
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     8/01/06	Original
 ********************************************************************/


#include "TCPIP Stack/TCPIP.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "VariableDef.h"

static SM_GPRS_POST smGPRSPOST;
BYTE VBuffer[50];
static BYTE len;
static BYTE TemporaryBuff[100];
static BYTE uc_GprsDactFlg = 0;
unsigned int BufferSize;
extern BYTE Attendance_Data[700];
BYTE uc_GPRS_Done;
BYTE vBuffer[300], VBuffer[50],vDataBuffer[1500], LogBuffer[1500];

/************************************************************************
* Function Name:  SendURL(void)                                       	*
* Return Value:   TRUE  - Sending successful 		                    *
*  				  FALSE - Sending unsuccessful
* Parameters:     						                		        *
* Description:    This routine sends Request to Server using GET or
*				   POST method.   								   		*
************************************************************************/
BOOL SendURL(METHOD method)
{
	if(uc_GSMRestart==3)
	{
		smGPRSPOST=SM_GPRS_POST_HOME;
		uc_GSMRestart=0;
	}
	
	switch(smGPRSPOST)
	{
	
		case SM_GPRS_POST_HOME:

			#if defined(TEST)
			{
				sprintf(TESTING,"%d:%d:%d",t_currdt.date.hh,t_currdt.date.mm,t_currdt.date.ss);
				ERR_LOG (TESTING);
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL HOME",8);							//testing
				ERR_LOG (Err_String);
			}
			#endif

			uc_GPRS_Receive	= FALSE;
			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

			SendGSMComm("AT",CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_HOME_RES;

			break;
		case SM_GPRS_POST_HOME_RES:

			if((HTTP_POST_RX_FIFO[0] == 'O' && HTTP_POST_RX_FIFO[1] == 'K')
					|| (HTTP_POST_RX_FIFO[2] == 'O' && HTTP_POST_RX_FIFO[3] == 'K'))
			{

				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL HOME OK",11);							//testing
					ERR_LOG (Err_String);
				}
				#endif

				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_ECHO;
			}
			else if(ui_SMSCount > SMS_DELAY_LOW)
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL HOME ERR",12);							//testing
					ERR_LOG (Err_String);
					sprintf(TESTING,"uc_RetriesGPRS  %d ",uc_RetriesGPRS);
					ERR_LOG (TESTING);
				}
				#endif
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_HOME;
				if(uc_RetriesGPRS >= 1)
				{
					uc_RetriesGPRS = 0;
				   	uc_RTR = uc_RTR +1;
					if(uc_RTR >= 5)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						uc_GSMRestart = TRUE;
						//PowerONGSM(FALSE);
						//GSM_Timer = TickGet();
						smGPRSPOST = SM_GPRS_POST_ECHO;
						
					}
				}
				return FALSE;					//return false and retries
			}
		break;

		case SM_GPRS_POST_ECHO:

			#if defined(TEST)
			{
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL ECHO",8);							//testing
				ERR_LOG (Err_String);
			}
			#endif
			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

			SendGSMComm("ATE0",CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_ECHO_RES;

		break;
		case SM_GPRS_POST_ECHO_RES:

			if((HTTP_POST_RX_FIFO[0] == 'O'  && HTTP_POST_RX_FIFO[1] == 'K')
				||(HTTP_POST_RX_FIFO[4] == 'O'  && HTTP_POST_RX_FIFO[5] == 'K'))
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL ECHO OK",11);							//testing
					ERR_LOG (Err_String);
				}
				#endif

				smGPRSPOST =SM_GPRS_POST_SIGNAL_QUALITY; 
				uc_Gsm_Cammand = 0;
			}
			else if(ui_SMSCount > SMS_DELAY_LOW)
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL ECHO ERR",12);							//testing
					sprintf(TESTING,"uc_RetriesGPRS  %d ",uc_RetriesGPRS);
					ERR_LOG (TESTING);
				}
				#endif
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_HOME;
				uc_RetriesGPRS++;
				if(uc_RetriesGPRS >= 5)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
					smGPRSPOST = SM_GPRS_POST_HOME;
					
				}
				return FALSE;
			}
			else if(HTTP_POST_RX_FIFO[0] =='+' || HTTP_POST_RX_FIFO[0] =='E')
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL ECHO +EE",12);							//testing
					ERR_LOG (Err_String);
					ERR_LOG (HTTP_POST_RX_FIFO);
				}
				#endif

				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_HOME;
				return FALSE;
			}
		break;

		case SM_GPRS_POST_SIGNAL_QUALITY:

		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

		SendGSMComm("AT+CSQ",CMD);
		ui_SMSCount = 0;
		smGPRSPOST = SM_GPRS_POST_SIGNAL_QUALITY_RES;
		#if defined(TEST)
		{
			memset(Err_String,0x00,sizeof(Err_String));
			memcpypgm2ram(Err_String,"SIGNAL",6) ;
			ERR_LOG (Err_String);
		}
		#endif
		break;

	case SM_GPRS_POST_SIGNAL_QUALITY_RES:

    	if(HTTP_POST_RX_FIFO[10] == 'O' && HTTP_POST_RX_FIFO[11] == 'K'  )		//If reply is "OK" then proceed
		{
			if(HTTP_POST_RX_FIFO[0] == '+' && HTTP_POST_RX_FIFO[1] == 'C' &&
				HTTP_POST_RX_FIFO[2] == 'S' && HTTP_POST_RX_FIFO[3] == 'Q' &&
				HTTP_POST_RX_FIFO[4] == ':' && HTTP_POST_RX_FIFO[5] == ' ' &&
				HTTP_POST_RX_FIFO[8] == ',' )
				{
					
					uc_Gsm_Cammand = 0;
					smGPRSPOST = SM_GPRS_POST_ACT;

 					#if defined(TEST)
					{
						memset(Err_String,0x00,sizeof(Err_String));
						memcpypgm2ram(Err_String,"SIGNAL OK",9);
						Err_String[9]=HTTP_POST_RX_FIFO[6];		//test
						Err_String[10]=HTTP_POST_RX_FIFO[7];
						ERR_LOG (Err_String);
						ERR_LOG (HTTP_POST_RX_FIFO);
					}
					#endif
				}
				else if (ui_SMSCount > 1000)
				{
					#if defined(TEST)
					{
						memset(Err_String,0x00,sizeof(Err_String));
						memcpypgm2ram(Err_String,"SIGNAL PROBLEM",14);
						ERR_LOG (Err_String);
						ERR_LOG (HTTP_POST_RX_FIFO);
					}
					#endif
					uc_RetriesGPRS++;
					if(uc_RetriesGPRS >= 3)
					{
						smGPRSPOST = SM_GPRS_POST_SIGNAL_QUALITY;
						uc_RetriesGPRS = 0;
					   	uc_RTR = uc_RTR +1;
						if(uc_RTR >= 10)
						{
							uc_GPRSFailed = TRUE;
							uc_RTR = 0;
							uc_RetriesGPRS = 0;
							uc_GPRSTried = TRUE;
							
							smGPRSPOST = SM_GPRS_POST_HOME;
						
						}
					}
					return FALSE;
  				}
		}
		else if ((HTTP_POST_RX_FIFO[9] == 'O') && (HTTP_POST_RX_FIFO[10] == 'K') && (ui_SMSCount > 1000))		//If reply is "OK" then proceed
		{
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 3)
			{
				smGPRSPOST = SM_GPRS_POST_SIGNAL_QUALITY;
				uc_RetriesGPRS = 0;
			   	uc_RTR = uc_RTR +1;
				if(uc_RTR >= 10)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
                    smGPRSPOST = SM_GPRS_POST_HOME;
					
				}
			}
			return FALSE;
		}
		else if(ui_SMSCount > SMS_DELAY_LOW)
		{
            uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;
			if(uc_RetriesGPRS >= 5)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
			
				smGPRSPOST = SM_GPRS_POST_HOME;
			
			}
			#if defined(TEST)
			{
				memset(Err_String,0x00,sizeof(Err_String));
				memcpypgm2ram(Err_String,"SIGNAL ERR",10);
				Err_String[10]=HTTP_POST_RX_FIFO[6];		//test
				Err_String[11]=HTTP_POST_RX_FIFO[7];
				ERR_LOG (Err_String);
			}
			#endif
			return FALSE;
		}
		break;

		case SM_GPRS_POST_ACT:

		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

		SendGSMComm("AT+QIFGCNT=0",CMD);
		ui_SMSCount = 0;

		smGPRSPOST = SM_GPRS_POST_ACT_RES;
		break;

		case SM_GPRS_POST_ACT_RES:

		if(HTTP_POST_RX_FIFO[0] == 'O'
				&& HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
		{
			#if defined(TEST)
			{
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL ACTIVE OK",13);							//testing
				ERR_LOG (Err_String);

			}
			#endif
			smGPRSPOST = SM_GPRS_POST_STAT;
			uc_Gsm_Cammand = 0;
		}
		else if(ui_SMSCount > SMS_DELAY_MID)
		{
			uc_Gsm_Cammand = 0; 
			smGPRSPOST = SM_GPRS_POST_HOME;
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 3)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			return FALSE;
		}
		else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
		{

			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;

			if(uc_RetriesGPRS >= 10)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			return FALSE;
		}
		break;

		case SM_GPRS_POST_STAT:

			uc_GPRS_Receive = FALSE;
			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

			SendGSMComm("AT+QISTAT",CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_STAT_RES;

		break;

		case SM_GPRS_POST_STAT_RES:

		if(HTTP_POST_RX_FIFO[9] == 'I' &&  HTTP_POST_RX_FIFO[10] == 'P' &&  HTTP_POST_RX_FIFO[17] == 'A'
				&& HTTP_POST_RX_FIFO[18] == 'L')		//If reply is "OK" then proceed
		{
			uc_GprsDactFlg = FALSE;
			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_APN;
		}
		else if(HTTP_POST_RX_FIFO[9] == 'I' && HTTP_POST_RX_FIFO[10] == 'P' &&  HTTP_POST_RX_FIFO[16] == 'A'
						&& HTTP_POST_RX_FIFO[17] == 'C' && HTTP_POST_RX_FIFO[18] == 'T')		//If reply is "OK" then proceed
		{
			uc_GprsDactFlg = FALSE;
			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_HTTP_VER;
		}
		else if(HTTP_POST_RX_FIFO[9] == 'P' &&  HTTP_POST_RX_FIFO[10] == 'D' &&  HTTP_POST_RX_FIFO[11] == 'P'
					&&  HTTP_POST_RX_FIFO[13] == 'D' &&  HTTP_POST_RX_FIFO[14] == 'E' &&  HTTP_POST_RX_FIFO[15] == 'A' &&  HTTP_POST_RX_FIFO[16] == 'C' &&  HTTP_POST_RX_FIFO[17] == 'T')
		{

			uc_Gsm_Cammand = 0;
			uc_GprsDactFlg = TRUE;
			smGPRSPOST = SM_GPRS_POST_APN;
		}
		else if(HTTP_POST_RX_FIFO[9] == 'I' &&  HTTP_POST_RX_FIFO[10] == 'P' &&  HTTP_POST_RX_FIFO[12] == 'I'
					&&  HTTP_POST_RX_FIFO[13] == 'N' &&  HTTP_POST_RX_FIFO[14] == 'D')
		{

			uc_Gsm_Cammand = 0;
			uc_GprsDactFlg = TRUE;
			smGPRSPOST = SM_GPRS_POST_DACTIVATE;
		}
		else if(ui_SMSCount > SMS_DELAY_HIGH)			//makes change in this
		{
			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_HOME;
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 1)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
			
			}
				#if defined(TEST)
			{
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL ACTIVE FAIL",15);							//testing
				ERR_LOG (Err_String);

			}
			#endif
			return FALSE;
		}
		else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
		{

			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;
			if(uc_RetriesGPRS >= 10)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
					#if defined(TEST)
			{
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL ACTIVE FAIL",15);							//testing
				ERR_LOG (Err_String);

			}
			#endif
			return FALSE;
		}
		break;

	case SM_GPRS_POST_APN:

		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

		memset(VBuffer,0x00,sizeof(VBuffer));
		strcpypgm2ram(VBuffer,"AT+QICSGP=1,");
		VBuffer[strlen(VBuffer)] = '"';
	//	strcpypgm2ram(&VBuffer[strlen(VBuffer)],"internet");
		strcpypgm2ram(&VBuffer[strlen(VBuffer)],"www");
	//	strcpypgm2ram(&VBuffer[strlen(VBuffer)],"TATA.DOCOMO.INTERNET");
	//	memcpypgm2ram(&VBuffer[strlen(VBuffer)],GPRS_APN,sizeof(GPRS_APN));
		VBuffer[strlen(VBuffer)] = '"';
		SendGSMComm(VBuffer,CMD);

		ui_SMSCount = 0;
		smGPRSPOST = SM_GPRS_POST_APN_RES;
		break;

		case SM_GPRS_POST_APN_RES:

		if(HTTP_POST_RX_FIFO[0] == 'O'
				&& HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
		{
			if(uc_GprsDactFlg == TRUE)								//if GPRS is Dactivated
			{
				uc_Gsm_Cammand = 0;
				uc_GprsDactFlg = FALSE;
				smGPRSPOST = SM_GPRS_POST_ACTIVATE;			//go to Activate state directly

			 #if defined(TEST)
                            {
                                    memset(Err_String,0x00,sizeof(Err_String));							//testing
                                    memcpypgm2ram(Err_String,"POST APN OK",11);							//testing
                                    ERR_LOG (Err_String);
                                    ERR_LOG (HTTP_POST_RX_FIFO);

                            }
                            #endif


			}
			else
			{
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_REG_TCPIP;										//otherwise next state
			}
		}
		else if(ui_SMSCount > SMS_DELAY_HIGH)
		{
			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_HOME;
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 1)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}

			#if defined(TEST)
               {
                       memset(Err_String,0x00,sizeof(Err_String));							//testing
                       memcpypgm2ram(Err_String,"POST APN Failed",14);							//testing
                       ERR_LOG (Err_String);
                       ERR_LOG (HTTP_POST_RX_FIFO);

               }
               #endif


			return FALSE;
		}
		else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
		{

			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;
			if(uc_RetriesGPRS >= 10)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
				#if defined(TEST)
                            {
                                    memset(Err_String,0x00,sizeof(Err_String));							//testing
                                    memcpypgm2ram(Err_String,"POST APN Failed",14);							//testing
                                    ERR_LOG (Err_String);
                                    ERR_LOG (HTTP_POST_RX_FIFO);

                            }
                            #endif
			return FALSE;
		}
		break;

		case SM_GPRS_POST_REG_TCPIP:

		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

		SendGSMComm("AT+QIREGAPP\r\n",CMD);
		ui_SMSCount = 0;

		smGPRSPOST = SM_GPRS_POST_REG_TCPIP_RES;

		break;
		case SM_GPRS_POST_REG_TCPIP_RES:

		if(HTTP_POST_RX_FIFO[0] == 'O'
				&& HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
		{
			smGPRSPOST = SM_GPRS_POST_ACTIVATE;
			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS = 0;								// clear this counter as we will reset module only after successive error
            uc_RTR = 0;
	        if(uc_GPRSFailed == TRUE)
            {
               uc_GPRSFailed = FALSE;
				smGPRSPOST = SM_GPRS_POST_HOME;
				break;
				
	      	}
		}
		else if(ui_SMSCount > SMS_DELAY_HIGH)
		{

			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_HOME;
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 1)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			#if defined(TEST)
           {
              memset(Err_String,0x00,sizeof(Err_String));							//testing
              memcpypgm2ram(Err_String,"POST TCPIP RES Failed",20);							//testing
              ERR_LOG (Err_String);
              ERR_LOG (HTTP_POST_RX_FIFO);

           }
           #endif
			return FALSE;
		}
		else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
		{
			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;
			if(uc_RetriesGPRS >= 10)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			#if defined(TEST)
           {
              memset(Err_String,0x00,sizeof(Err_String));							//testing
              memcpypgm2ram(Err_String,"POST TCPIP RES Failed",20);							//testing
              ERR_LOG (Err_String);
              ERR_LOG (HTTP_POST_RX_FIFO);

           }
           #endif
			return FALSE;
		}

		break;
		case SM_GPRS_POST_ACTIVATE:

		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

		SendGSMComm("AT+QIACT",CMD);
		ui_SMSCount = 0;

		smGPRSPOST = SM_GPRS_POST_ACTIVATE_RES;

		break;
		case SM_GPRS_POST_ACTIVATE_RES:

		if(HTTP_POST_RX_FIFO[0] == 'O'
				&& HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
		{
			smGPRSPOST = SM_GPRS_POST_HTTP_VER;
			uc_Gsm_Cammand = 0;

			#if defined(TEST)
            {
               memset(Err_String,0x00,sizeof(Err_String));							//testing
               memcpypgm2ram(Err_String,"GPRS Activated",14);							//testing
               ERR_LOG (Err_String);
               ERR_LOG (HTTP_POST_RX_FIFO);

            }
            #endif


		}
		else if(ui_SMSCount > SMS_DELAY_HIGH)
		{

			uc_Gsm_Cammand = 0;
			smGPRSPOST = SM_GPRS_POST_HOME;
			uc_RetriesGPRS++;
			if(uc_RetriesGPRS >= 1)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			#if defined(TEST)
             {
                 memset(Err_String,0x00,sizeof(Err_String));							//testing
                 memcpypgm2ram(Err_String,"GPRS Activation Failed",22);							//testing
                 ERR_LOG (Err_String);
                 ERR_LOG (HTTP_POST_RX_FIFO);

             }
             #endif


		
			return FALSE;
		}
		else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
		{
			uc_Gsm_Cammand = 0;
			uc_RetriesGPRS++;
			smGPRSPOST = SM_GPRS_POST_HOME;
			if(uc_RetriesGPRS >= 10)
			{
				uc_GPRSFailed = TRUE;
				uc_RTR = 0;
				uc_RetriesGPRS = 0;
				uc_GPRSTried = TRUE;
				#if defined(TEST)
                  {
                      memset(Err_String,0x00,sizeof(Err_String));							//testing
                      memcpypgm2ram(Err_String,"GPRS Activation Failed",20);							//testing
                      ERR_LOG (Err_String);
                      ERR_LOG (HTTP_POST_RX_FIFO);

                  }
                  #endif
				
				smGPRSPOST = SM_GPRS_POST_HOME;
				
			}
			return FALSE;
		}

		break;


	case SM_GPRS_POST_HTTP_VER:

			#if defined(TEST)
			{
				sprintf(TESTING,"%d:%d:%d",t_currdt.date.hh,t_currdt.date.mm,t_currdt.date.ss);
				ERR_LOG (TESTING);
				memset(Err_String,0x00,sizeof(Err_String));							//testing
				memcpypgm2ram(Err_String,"URL HOME",8);							//testing
				ERR_LOG (Err_String);
			}
			#endif

			uc_GPRS_Receive	= FALSE;
			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

			SendGSMComm("AT+QHTTPVRS=0",CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_HTTP_VER_RES;

			break;
	case SM_GPRS_POST_HTTP_VER_RES:

				if((HTTP_POST_RX_FIFO[0] == 'O'  && HTTP_POST_RX_FIFO[1] == 'K')
				||(HTTP_POST_RX_FIFO[4] == 'O'  && HTTP_POST_RX_FIFO[5] == 'K'))
			{

				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL HOME OK",11);							//testing
					ERR_LOG (Err_String);
				}
				#endif

				uc_Gsm_Cammand = 0;
				smGPRSPOST =SM_GPRS_POST_URL; 
			}
			else if(ui_SMSCount > SMS_DELAY_LOW)
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL HOME ERR",12);							//testing
					ERR_LOG (Err_String);
					sprintf(TESTING,"uc_RetriesGPRS  %d ",uc_RetriesGPRS);
					ERR_LOG (TESTING);
				}
				#endif
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_HTTP_VER;
				if(uc_RetriesGPRS >= 1)
				{
					uc_RetriesGPRS = 0;
				   	uc_RTR = uc_RTR +1;
					if(uc_RTR >= 5)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						uc_GSMRestart = TRUE;
						//PowerONGSM(FALSE);
						//GSM_Timer = TickGet();
						smGPRSPOST = SM_GPRS_POST_HTTP_VER;
						
					}
				}
				return FALSE;					//return false and retries
			}
		break;

		case SM_GPRS_POST_URL:

			
			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer                   
			memset(VBuffer,0x00,sizeof(VBuffer));
			strcpypgm2ram(VBuffer,"AT+QHTTPURL=");
			sprintf(&VBuffer[strlen(VBuffer)],"%d", strlen(RemoteURL));
			VBuffer[strlen(VBuffer)] = ',';
			sprintf(&VBuffer[strlen(VBuffer)],"%d", 30);

			SendGSMComm(VBuffer,CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_URL_RES;
		break;

		case SM_GPRS_POST_URL_RES:

			if(HTTP_POST_RX_FIFO[0] == 'C' && HTTP_POST_RX_FIFO[1] == 'O' && HTTP_POST_RX_FIFO[2] == 'N'
			 && HTTP_POST_RX_FIFO[3] == 'N' && HTTP_POST_RX_FIFO[4] == 'E'
			&& HTTP_POST_RX_FIFO[5] == 'C' && HTTP_POST_RX_FIFO[6] == 'T' )		//If reply is "OK" then proceed
			{
				uc_Gsm_Cammand = 0;
				memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
				OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer
				SendGSMComm(RemoteURL,SREG);
				ui_SMSCount = 0;
				smGPRSPOST = SM_GPRS_POST_URL_OK;
				#if defined(TEST)
                 {
                     memset(Err_String,0x00,sizeof(Err_String));							//testing
                     memcpypgm2ram(Err_String,"POST URL CONNECT",16);							//testing
                     ERR_LOG (Err_String);
                     ERR_LOG (HTTP_POST_RX_FIFO);

                 }
                 #endif

			}
			else if(ui_SMSCount > SMS_DELAY_MID)
			{
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_STAT;
				uc_RetriesGPRS++;
				if(uc_RetriesGPRS >= 3)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
					
					smGPRSPOST = SM_GPRS_POST_HOME;
					
				}
					#if defined(TEST)
                     {
                         memset(Err_String,0x00,sizeof(Err_String));							//testing
                         memcpypgm2ram(Err_String,"POST URL CONN FAILED",19);							//testing
                         ERR_LOG (Err_String);
                         ERR_LOG (HTTP_POST_RX_FIFO);

                     }
                     #endif

				return FALSE;
			}
			else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
			{
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_HOME;
				if(uc_RetriesGPRS >= 10)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
					smGPRSPOST = SM_GPRS_POST_HOME;
					
				}

					#if defined(TEST)
                   {
                       memset(Err_String,0x00,sizeof(Err_String));							//testing
                       memcpypgm2ram(Err_String,"POST URL CONN FAILED",19);							//testing
                       ERR_LOG (Err_String);
                       ERR_LOG (HTTP_POST_RX_FIFO);

                   }
                   #endif

				return FALSE;
			}
		break;
		case SM_GPRS_POST_URL_OK:

			if((HTTP_POST_RX_FIFO[0] == 'O' && HTTP_POST_RX_FIFO[1] == 'K')
				|| (HTTP_POST_RX_FIFO[1] == 'O' && HTTP_POST_RX_FIFO[2] == 'K'))		//If reply is "OK" then proceed
			{
				smGPRSPOST = SM_GPRS_POST_GET;
				uc_Gsm_Cammand = 0;
				#if defined(TEST)
                 {
                     memset(Err_String,0x00,sizeof(Err_String));							//testing
                     memcpypgm2ram(Err_String,"POST_URL OK",12);							//testing
                     ERR_LOG (Err_String);
                     ERR_LOG (HTTP_POST_RX_FIFO);

                 }
	             #endif

			}
			else if(ui_SMSCount > SMS_DELAY_MID)
			{
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_STAT;
				uc_RetriesGPRS++;
				if(uc_RetriesGPRS >= 3)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
					
					smGPRSPOST = SM_GPRS_POST_HOME;
					
				}
				#if defined(TEST)
                 {
                     memset(Err_String,0x00,sizeof(Err_String));							//testing
                     memcpypgm2ram(Err_String,"POST_URL FAIL",13);							//testing
                     ERR_LOG (Err_String);
                     ERR_LOG (HTTP_POST_RX_FIFO);

                 }
                 #endif
				return FALSE;
			}
			else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
			{
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_HOME;
				if(uc_RetriesGPRS >= 10)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;
					
					smGPRSPOST = SM_GPRS_POST_HOME;
					
				}
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"POST_URL FAIL",13);							//testing
					ERR_LOG (Err_String);
					ERR_LOG (HTTP_POST_RX_FIFO);
				
				}
				#endif
				return FALSE;
			}
		break;
		case SM_GPRS_POST_GET:

			memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

			memset(VBuffer,0x00,sizeof(VBuffer));
			if(method == GET)
			{
				strcpypgm2ram(VBuffer,"AT+QHTTPGET=60");
			}
			else if (method == POST)
			{
                BufferSize=strlen(DATA);
				//	strcpypgm2ram(VBuffer,"AT+QHTTPPOST=75,60,60");
				strcpypgm2ram(VBuffer,"AT+QHTTPPOST=");//75,60,60");
	            sprintf(&VBuffer[strlen(VBuffer)],"%d", BufferSize);
				strcpypgm2ram(&VBuffer[strlen(VBuffer)],",30,30");
				
			}
			SendGSMComm(VBuffer,CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_GET_RES;

		break;
		case SM_GPRS_POST_GET_RES:
			if(method == GET)
			{
				if(HTTP_POST_RX_FIFO[0] == 'O' && HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
				{
					smGPRSPOST = SM_GPRS_POST_READ;
					uc_Gsm_Cammand = 0;
				}
				else if(ui_SMSCount > SMS_DELAY_MID)
				{
					uc_Gsm_Cammand = 0;
					smGPRSPOST = SM_GPRS_POST_STAT;
					uc_RetriesGPRS++;
					if(uc_RetriesGPRS >= 3)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						smGPRSPOST = SM_GPRS_POST_HOME;
						
					}
					return FALSE;
				}
				else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
				{
					uc_Gsm_Cammand = 0;
					uc_RetriesGPRS++;
					smGPRSPOST = SM_GPRS_POST_HOME;
					if(uc_RetriesGPRS >= 10)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						smGPRSPOST = SM_GPRS_POST_HOME;
						
					}
					return FALSE;
				}
			}
			else if(method == POST)
			{	
				if(HTTP_POST_RX_FIFO[0] == 'C' && HTTP_POST_RX_FIFO[1] == 'O' && HTTP_POST_RX_FIFO[2] == 'N'
			 			&& HTTP_POST_RX_FIFO[3] == 'N' && HTTP_POST_RX_FIFO[4] == 'E' 
						&& HTTP_POST_RX_FIFO[5] == 'C' && HTTP_POST_RX_FIFO[6] == 'T' )	
				{
						uc_Gsm_Cammand = 0;
						memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
						OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer
						SendGSMComm(DATA,SREG);
						ui_SMSCount = 0;
						smGPRSPOST = SM_GPRS_POST_READ;
						#if defined(TEST)
		                 {
		                         memset(Err_String,0x00,sizeof(Err_String));							//testing
		                         memcpypgm2ram(Err_String,"POST GET OK",11);							//testing
		                         ERR_LOG (Err_String);
		                         ERR_LOG (HTTP_POST_RX_FIFO);
		
		                 }
		                 #endif
				}
				else if(ui_SMSCount > SMS_DELAY_MID)
				{
					uc_Gsm_Cammand = 0;
					smGPRSPOST = SM_GPRS_POST_STAT;
					uc_RetriesGPRS++;
					if(uc_RetriesGPRS >= 3)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						smGPRSPOST = SM_GPRS_POST_HOME;
						
					}
					#if defined(TEST)
                     {
                        memset(Err_String,0x00,sizeof(Err_String));							//testing
                        memcpypgm2ram(Err_String,"POST GET FAIL",13);							//testing
                        ERR_LOG (Err_String);
                        ERR_LOG (HTTP_POST_RX_FIFO);

                     }
                     #endif
					return FALSE;
				}
				else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
				{
					uc_Gsm_Cammand = 0;
					uc_RetriesGPRS++;
					smGPRSPOST = SM_GPRS_POST_HOME;
					if(uc_RetriesGPRS >= 10)
					{
						uc_GPRSFailed = TRUE;
						uc_RTR = 0;
						uc_RetriesGPRS = 0;
						uc_GPRSTried = TRUE;
						
						smGPRSPOST = SM_GPRS_POST_HOME;
						
					}
					#if defined(TEST)
					{
				       memset(Err_String,0x00,sizeof(Err_String));							//testing
				       memcpypgm2ram(Err_String,"POST GET FAIL",13);							//testing
				       ERR_LOG (Err_String);
				       ERR_LOG (HTTP_POST_RX_FIFO);
					
					}
					#endif
					return FALSE;
				}
			}
		break;
		case SM_GPRS_POST_READ:
		if((HTTP_POST_RX_FIFO[0] == 'O' && HTTP_POST_RX_FIFO[1] == 'K')
				|| (HTTP_POST_RX_FIFO[1] == 'O' && HTTP_POST_RX_FIFO[2] == 'K'))
				{
					uc_Gsm_Cammand = 0;
					memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
					OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;					//Initialize the pointer

					memset(VBuffer,0x00,sizeof(VBuffer));
					strcpypgm2ram(VBuffer,"AT+QHTTPREAD=04");

					SendGSMComm(VBuffer,CMD);
					ui_SMSCount = 0;
					smGPRSPOST = SM_GPRS_POST_READ_RES;

				#if defined(TEST)
                {
                    memset(Err_String,0x00,sizeof(Err_String));							//testing
                    memcpypgm2ram(Err_String,"POST READ OK",12);							//testing
                    ERR_LOG (Err_String);
                    ERR_LOG (HTTP_POST_RX_FIFO);

                }
				  #endif
			}
			else if(ui_SMSCount > SMS_DELAY_MID)
			{
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_STAT;
				uc_RetriesGPRS++;
				if(uc_RetriesGPRS >= 3)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;

					smGPRSPOST = SM_GPRS_POST_HOME;

				}

				#if defined(TEST)
				{
				  memset(Err_String,0x00,sizeof(Err_String));							//testing
				  memcpypgm2ram(Err_String,"POST READ FAIL",12);							//testing
				  ERR_LOG (Err_String);
				  ERR_LOG (HTTP_POST_RX_FIFO);
				
				}
				#endif
				return FALSE;
			}
			else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
			{
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_READ;
				if(uc_RetriesGPRS >= 10)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;

					smGPRSPOST = SM_GPRS_POST_HOME;

				}
				#if defined(TEST)
                 {
                     memset(Err_String,0x00,sizeof(Err_String));							//testing
                     memcpypgm2ram(Err_String,"POST READ FAIL",12);							//testing
                     ERR_LOG (Err_String);
                     ERR_LOG (HTTP_POST_RX_FIFO);

                 }
				  #endif
				return FALSE;
			}


		break;
		case SM_GPRS_POST_READ_RES:

			if(HTTP_POST_RX_FIFO[0] == 'C' && HTTP_POST_RX_FIFO[1] == 'O' && HTTP_POST_RX_FIFO[2] == 'N'
				 && HTTP_POST_RX_FIFO[3] == 'N' && HTTP_POST_RX_FIFO[4] == 'E'
				&& HTTP_POST_RX_FIFO[5] == 'C' && HTTP_POST_RX_FIFO[6] == 'T' )		//If reply is "OK" then proceed
			{

				memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
				OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;
				ui_SMSCount = 0;
				smGPRSPOST = SM_GPRS_POST_DONE;

					#if defined(TEST)
                     {
                        memset(Err_String,0x00,sizeof(Err_String));							//testing
                        memcpypgm2ram(Err_String,"POST READ RES OK",16);							//testing
                        ERR_LOG (Err_String);
                        ERR_LOG (HTTP_POST_RX_FIFO);

                     }
					 #endif
			}
			else if(HTTP_POST_RX_FIFO[0] == 'O' && HTTP_POST_RX_FIFO[1] == 'K')		//If reply is "OK" then proceed
			{

				memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
				OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;
				ui_SMSCount = 0;
				smGPRSPOST = SM_GPRS_POST_DONE;
				#if defined(TEST)
                {
                    memset(Err_String,0x00,sizeof(Err_String));							//testing
                    memcpypgm2ram(Err_String,"POST READ POST OK",16);							//testing
                    ERR_LOG (Err_String);
                    ERR_LOG (HTTP_POST_RX_FIFO);

                }
			  #endif
			}
			else if(ui_SMSCount > SMS_DELAY_MID)
			{
				uc_Gsm_Cammand = 0;
				smGPRSPOST = SM_GPRS_POST_STAT;
				memset(RemoteURL, 0x00,sizeof(RemoteURL));
				uc_RetriesGPRS++;
				if(uc_RetriesGPRS >= 3)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;

					smGPRSPOST = SM_GPRS_POST_HOME;

				}
			#if defined(TEST)
            {
                memset(Err_String,0x00,sizeof(Err_String));							//testing
                memcpypgm2ram(Err_String,"POST READ RES FAIL",16);							//testing
                ERR_LOG (Err_String);
                ERR_LOG (HTTP_POST_RX_FIFO);

            }
			  #endif
				return FALSE;
			}
			else if(((HTTP_POST_RX_FIFO[0] =='+') || (HTTP_POST_RX_FIFO[0] =='E')) && (ui_SMSCount > 1000))
			{
				uc_Gsm_Cammand = 0;
				uc_RetriesGPRS++;
				smGPRSPOST = SM_GPRS_POST_READ;
				if(uc_RetriesGPRS >= 10)
				{
					uc_GPRSFailed = TRUE;
					uc_RTR = 0;
					uc_RetriesGPRS = 0;
					uc_GPRSTried = TRUE;

					smGPRSPOST = SM_GPRS_POST_HOME;

				}
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"POST READ RES FAIL",18);							//testing
					ERR_LOG (Err_String);
					ERR_LOG (HTTP_POST_RX_FIFO);
				
				}
				 #endif
				return FALSE;
			}
		break;
		case SM_GPRS_POST_DONE:

			if(((HTTP_POST_RX_FIFO[strlen(HTTP_POST_RX_FIFO) - 2] == 'O') && (HTTP_POST_RX_FIFO[strlen(HTTP_POST_RX_FIFO) -1] == 'K')) || ((HTTP_POST_RX_FIFO[strlen(HTTP_POST_RX_FIFO) - 3] == '1') && (HTTP_POST_RX_FIFO[strlen(HTTP_POST_RX_FIFO)-1] == '0')))
			{
				uc_RetriesGPRS = 0;
				uc_Gsm_Cammand = 0;
				
				uc_GPRS_Receive = TRUE;
				uc_GPRS_Done=TRUE;
				*(OUART_RXHeadPtr-1) = 0;
				*(OUART_RXHeadPtr-2) = '\n';
				//DelayMs(10);
				smGPRSPOST = SM_GPRS_POST_STAT;
				// DelayMs(10);
				uc_GPRS_Done=TRUE;
				//DelayMs(100);
				#if defined(TEST)
				{
				     memset(Err_String,0x00,sizeof(Err_String));							//testing
				     memcpypgm2ram(Err_String,"URL POST_DONE OK",13);							//testing
				     ERR_LOG (Err_String);
				     ERR_LOG (HTTP_POST_RX_FIFO);
				
				}
				#endif

			}

			else if(ui_SMSCount > SMS_DELAY_LOW/20)
			{
				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL POST_DONE DELAY_ERR",13);							//testing
					ERR_LOG (Err_String);
					ERR_LOG (HTTP_POST_RX_FIFO);
					sprintf(TESTING,"uc_RetriesGPRS  %d ",uc_RetriesGPRS);
					ERR_LOG (TESTING);

				}
				#endif
			//	uc_GPRS_Done=TRUE;
			//	uc_RetriesGPRS++;
			//	if(uc_RetriesGPRS >= 3)
//				{
//					uc_GPRSFailed = TRUE;
//					uc_RTR = 0;
//					uc_RetriesGPRS = 0;
//					uc_GPRSTried = TRUE;
//					uc_GSMRestart=TRUE;
//					smGPRSPOST = SM_GPRS_POST_STAT;
//
//				}	
			}

		break;
		case SM_GPRS_POST_DACTIVATE:

				#if defined(TEST)
				{
					memset(Err_String,0x00,sizeof(Err_String));							//testing
					memcpypgm2ram(Err_String,"URL DEACTIVATE",14);							//testing
					ERR_LOG (Err_String);
					ERR_LOG (HTTP_POST_RX_FIFO);

				}
				#endif
			memset(GSM_UART_RX_FIFO, 0x00,sizeof(GSM_UART_RX_FIFO));
			OUART_RXHeadPtr = OUART_RXTailPtr = GSM_UART_RX_FIFO;					//Initialize the pointer

			memset(VBuffer,0x00,sizeof(VBuffer));
			strcpypgm2ram(VBuffer,"AT+QIDEACT");

			SendGSMComm(VBuffer,CMD);
			ui_SMSCount = 0;
			smGPRSPOST = SM_GPRS_POST_DACTIVATE_RES;

		break;
		case SM_GPRS_POST_DACTIVATE_RES:

			if(GSM_UART_RX_FIFO[6] == 'O' && GSM_UART_RX_FIFO[7] == 'K')		//If reply is "OK" then proceed
			{

				uc_GPRS_Receive = TRUE;
				DelayMs(50);
				smGPRSPOST = SM_GPRS_POST_STAT;
				uc_Gsm_Cammand = 0;
				return TRUE;
			}
			else if(ui_SMSCount > SMS_DELAY_MID)
			{
				uc_Gsm_Cammand = 0;
				uc_GPRS_Receive = TRUE;
				DelayMs(50);
				smGPRSPOST = SM_GPRS_POST_STAT;
				return TRUE;
			}
		break;

	}

}

