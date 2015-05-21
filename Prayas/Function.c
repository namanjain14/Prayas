/****************************************************************************************/
/*  Data Acquisition System Project */
/* File Name 	: Function.c */
/* Purpose		: This file contains the declarations for global variables */
/* Author		: Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "TCPIP Stack/TCPIP.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "VariableDef.h"
#include "i2c_master.h"
#include "diskio.h"
#include "ff.h"
#include "Aliotgateway.h"
#include "Compiler.h"


/***************************************************************************************
 * Function			:void Initialization(void)
 *
 * PreCondition		:None
 *
 * Input			:None
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Initialize the application releated functionality
 *					 
 *
 * Note				:None
 **************************************************************************************/
void Initialization(void)
{
	BYTE temp = 0;
	Toatal_Value=0;
	ADC_Read_Value=0;
	min=0;
	max=0;
	uc_GPRSFailed = FALSE;							// GPRS Failed	
	uc_GSMRestart = FALSE;
	SetGSMUARTParameter();						// Uart parrameter and Bridge Init
	GSM_UARTInit();								//initialize the GSM UART
	TickInit();										//Tick initialization

}

/*********************************************************************************
 * Function			:void PowerONGSM(unsigned char mode)
 *
 * PreCondition		:Hardware GSM module 
 *
 * Input			:None
 *					 
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Power ON the GSM module
 *					
 *
 * Note				:None
*********************************************************************************/
void PowerONGSM(unsigned char mode)
{
	static BOOL GSMis = FALSE;

	if (GSMis == FALSE)
	{
		if (mode == TRUE)
		{
			GSM_VBAT_EN_IO = 1;
			DelayMs(100);

			O_POWERKEY = 1;						//power button set to high	
			
			DelayMs(3000);								//wait for 2 sec
		
			O_POWERKEY = 0;						//and set to low
			GSMis = mode;
		}
	}
	else if (GSMis == TRUE)
	{
		/*if (mode == FALSE)
		{
			O_POWERKEY = 1;						//power button set to high	
			
			DelayMs(3000);								//wait for 2 sec
		
			O_POWERKEY = 0;						//and set to low
			GSMis = mode;
		}*/
		GSM_VBAT_EN_IO = 0;
		GSMis = mode;		
	}
	
	#if defined(TEST)
		memset(Err_String,0x00,sizeof(Err_String));
		memcpypgm2ram(Err_String,"***POWER_KEY***",15) ;
	#endif
}










/*********************************************************************************
 * Function			:void EmergancyOFF_GSM(void)
 *
 * PreCondition		:Hardware GSM module 
 *
 * Input			:None
 *					 
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Power OFF the GSM module. Just make Emergancy Pin(RB12) of pic32 
					 high for 2sec and then make it low.
 *					
 *
 * Note				:None
*********************************************************************************/
void EmergancyOFF_GSM(void)
{
	O_EmergancyOff = 1;						//Emergancy button set to high	
	
	DelayMs(2000);								//wait for 2 sec

	O_EmergancyOff = 0;						//and set to low
	
}



/********************************************************************
;*      Function Name:  SendGSMComm                                 *
;*      Return Value:   void                                        *
;*      Parameters:     Sring to be copied to array                 *
;*      Description:    This routine copies sring to write array    *
*********************************************************************/
void SendGSMComm(char* ATcommand, unsigned char Type)
{
	
	unsigned int uc_LoopVar;
				
	if(Type)
	{	
			memset(GSM_UART_TX_FIFO, 0x00, sizeof(GSM_UART_TX_FIFO));
			uc_LoopVar = strlen(ATcommand);
			memcpypgm2ram(GSM_UART_TX_FIFO,ATcommand, uc_LoopVar);				//copy the string and increment the pointer
		
			GSM_UART_TX_FIFO[strlen(GSM_UART_TX_FIFO)] = '\r';					//attached the \r\n at end
			GSM_UART_TX_FIFO[strlen(GSM_UART_TX_FIFO)] = '\n';
		
			OUART_TXTailPtr = OUART_TXHeadPtr = GSM_UART_TX_FIFO;	
			OUART_TXTailPtr = OUART_TXTailPtr + strlen(GSM_UART_TX_FIFO);		//increment pointer	
			uc_Gsm_Cammand = TRUE;
			GSM_TX_ENABLE = 1;															//enable interrupt //so that it can transmit data(transmission enable)
	}
	else
	{
		memset(GSM_UART_TX_FIFO, 0x00, sizeof(GSM_UART_TX_FIFO));				
		memset(HTTP_POST_RX_FIFO, 0x00,sizeof(HTTP_POST_RX_FIFO));
		OUART_RXHeadPtr = OUART_RXTailPtr = HTTP_POST_RX_FIFO;				//reset pointer
		uc_URLSent = TRUE;
		memcpypgm2ram(GSM_UART_TX_FIFO,ATcommand, strlen(ATcommand));			//copy URL
		uc_LoopVar = strlen(GSM_UART_TX_FIFO);
		GSM_UART_TX_FIFO[uc_LoopVar] = '\r';
		GSM_UART_TX_FIFO[uc_LoopVar+1] = '\n';
		OUART_TXTailPtr = OUART_TXHeadPtr = GSM_UART_TX_FIFO;
		OUART_TXTailPtr = OUART_TXTailPtr + strlen(GSM_UART_TX_FIFO);			//increment pointer
		uc_Gsm_Cammand = 1;
		GSM_TX_ENABLE = 1;																//enable pointer 
	}
}


/*********************************************************************************
 * Function			:void Ascii2Bin(BYTE *DATA, WORD *Bin_DATA)
 *
 * PreCondition		:None
 *
 * Input			:*DATA - byte pointer of the source data
 *					 *Bin_DATA - word pointer of the destination data
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Converts the Ascii data into WORD formated Binary data.
 *					
 *
 * Note				:None
*********************************************************************************/	
void Ascii2Bin(BYTE *DATA, WORD *Bin_DATA)
{
	char Hex_Buffer[5];								//Hex Buffer to hold converted Hex data
	unsigned char loop_var=0;
	unsigned int tempdata = 0;														

	for(loop_var = 0; loop_var<5;loop_var++)
	{
		Hex_Buffer[loop_var] = *DATA;
		DATA++;
	}
	if(loop_var==5)
	{
		sscanf(Hex_Buffer,"%u",&tempdata);		//the group of five Ascii hex value will 
		*Bin_DATA = tempdata;					//convert into respective decimal number 
			
	}

}

/*********************************************************************************
 * Function			:void Bin2Ascii(WORD *DATA , BYTE *Ascii_DATA)
 *
 * PreCondition		:None
 *
 * Input			:*DATA - word pointer of the source data
 *					 *Ascii_DATA - byte pointer of the destination data
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Converts the WORD formated data into Ascii Hex vale
 *					
 *
 * Note				:None
*********************************************************************************/
void Bin2Ascii(WORD *DATA , BYTE *Ascii_DATA)
{
	char AsciiBuffer[5];							//temporary buffer to hold ascii value
	BYTE arry_elmnt,loop_var;
	
	AsciiBuffer[0]= 0x00;
	AsciiBuffer[1]=	0x00;
	AsciiBuffer[2]=	0x00;
	AsciiBuffer[3]=	0x00;
	AsciiBuffer[4]=	0x00;
	
	sprintf(AsciiBuffer,"%d",*DATA);			//will convert WORD value into respective
	loop_var = 4;								//ascii and copy to local buffer
		
	while(AsciiBuffer[loop_var]==0x00)			//OverWrite 0x00 value with 0x30 in the 
	{											//local buffer in reverse order and then 
		*Ascii_DATA= 0x30;						//copied to destination memory
		Ascii_DATA++;	
		loop_var--;
	}
	for(arry_elmnt = 0; AsciiBuffer[arry_elmnt] != 0x00; arry_elmnt++)				
													//otherwise copied the local buffer into
	{											//destination 
		*Ascii_DATA=AsciiBuffer[arry_elmnt];									
		Ascii_DATA++;
		AsciiBuffer[arry_elmnt]=0x00;
	}
}


/*--------------------------------------------------------------------------------------*/
// Function Name: ConvertBCDDateToDec
// Purpose: Converts BCD RTC data to Decimal
// Variables Passed: RTCDT *dt, Pointer to structure RTCDT 
// Variables Returned: None
/*--------------------------------------------------------------------------------------*/		
void ConvertBCDDateToDec(RTCDT *dt)
{
	
    t_currdtDec.date.Date = ((dt->date.Date >> NIBBLE) * BASE_10) + dt->date.Date % BASE_16;
	t_currdtDec.date.Month = ((dt->date.Month >> NIBBLE) * BASE_10) + dt->date.Month % BASE_16;
	t_currdtDec.date.Year = ((dt->date.Year >> NIBBLE) * BASE_10) + dt->date.Year % BASE_16;
	t_currdtDec.date.Day = ((dt->date.Day >> NIBBLE) * BASE_10) + dt->date.Day % BASE_16;
	t_currdtDec.date.hh = ((dt->date.hh >> NIBBLE) * BASE_10) + dt->date.hh % BASE_16;
	t_currdtDec.date.mm = ((dt->date.mm >> NIBBLE) * BASE_10) + dt->date.mm % BASE_16;
	t_currdtDec.date.ss = ((dt->date.ss >> NIBBLE) * BASE_10) + dt->date.ss % BASE_16;
	
	dt->date.Date = t_currdtDec.date.Date ;
	dt->date.Month = t_currdtDec.date.Month ;
	dt->date.Year = t_currdtDec.date.Year ;
	dt->date.Day = t_currdtDec.date.Day ;
	dt->date.hh = t_currdtDec.date.hh ;
	dt->date.mm = t_currdtDec.date.mm ;
	dt->date.ss = t_currdtDec.date.ss ;
}
/*--------------------------------------------------------------------------------------*/
// Function Name: ConvertDECDateToBCD
// Purpose: Converts Decimal data to BCD   
// Variables Passed: RTCDT *dt, Pointer to structure RTCDT
// Variables Returned: None
/*--------------------------------------------------------------------------------------*/		
void ConvertDECDateToBCD(RTCDT *dt)
{
	t_currdtDec.date.Date = ((dt->date.Date / BASE_10) << NIBBLE) + (dt->date.Date % BASE_10);
	t_currdtDec.date.Month = ((dt->date.Month / BASE_10) << NIBBLE) + 
                            (dt->date.Month % BASE_10);
	t_currdtDec.date.Year = ((dt->date.Year / BASE_10) << NIBBLE) + (dt->date.Year % BASE_10);
	t_currdtDec.date.Day = ((dt->date.Day / BASE_10) << NIBBLE) + (dt->date.Day % BASE_10);
	t_currdtDec.date.hh = ((dt->date.hh / BASE_10) << NIBBLE) + (dt->date.hh % BASE_10);
	t_currdtDec.date.mm = ((dt->date.mm / BASE_10) << NIBBLE) + (dt->date.mm % BASE_10);
	t_currdtDec.date.ss = ((dt->date.ss /BASE_10) << NIBBLE) + (dt->date.ss % BASE_10);
	
	dt->date.Date = t_currdtDec.date.Date ;
	dt->date.Month = t_currdtDec.date.Month ;
	dt->date.Year = t_currdtDec.date.Year ;
	dt->date.Day = t_currdtDec.date.Day ;
	dt->date.hh = t_currdtDec.date.hh ;
	dt->date.mm = t_currdtDec.date.mm ;
	dt->date.ss = t_currdtDec.date.ss ;
}

/***************************************************************************************
 * Function			:void GetTime(void)
 *
 * PreCondition		:None
 *
 * Input			:None
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Read Current time & write in buffer
 *
 * Note				:None
 **************************************************************************************/
void GetTime(void)
{
	BYTE buff[2];
	//	struct tm t;
	WORD Year = 0;

	GetCurrentTime();										//Get the current Time
	ConvertBCDDateToDec(&t_currdt);						//convert BCD date into Decimal

	sprintf(buff,"%d",t_currdt.date.Date);					//again into Ascii

	if(strlen(buff)==1)
	{
		uc_DateString[0] = '0';
		uc_DateString[1] = buff[0];						//filename
	}
	else
	{
		uc_DateString[0] = buff[0];						//Write it into the File Name
		uc_DateString[1] = buff[1];
	}
	uc_DateString[2] = '-';

	sprintf(buff,"%d",t_currdt.date.Month);
	if(strlen(buff)==1)
	{
		uc_DateString[3] = '0';
		uc_DateString[4] = buff[0];
	}
	else
	{
		uc_DateString[3] = buff[0];
		uc_DateString[4] = buff[1];
	}
	uc_DateString[5] = '-';
	sprintf(buff,"%d",t_currdt.date.Year);
	uc_DateString[6] = buff[0];
	uc_DateString[7] = buff[1];


	sprintf(buff,"%d",t_currdt.date.hh);			//along with time

	if(strlen(buff) == 1)
	{
		uc_TimeString[0] = '0';
		uc_TimeString[1] = buff[0];
	}
	else
	{
		uc_TimeString[0] = buff[0];
		uc_TimeString[1] = buff[1];
	}
	uc_TimeString[2] = ':';
	sprintf(buff,"%d",t_currdt.date.mm);
	if(strlen(buff) == 1)
	{
		uc_TimeString[3] = '0';
		uc_TimeString[4] = buff[0];
	}
	else
	{
		uc_TimeString[3] = buff[0];
		uc_TimeString[4] = buff[1];
	}
	uc_TimeString[5] = ':';
	sprintf(buff,"%d",t_currdt.date.ss);
	if(strlen(buff) == 1)
	{
		uc_TimeString[6] = '0';
		uc_TimeString[7] = buff[0];
	}
	else
	{
		uc_TimeString[6] = buff[0];
		uc_TimeString[7] = buff[1];
	}

}

/***************************************************************************************
 * Function			:void GetTime(void)
 *
 * PreCondition		:None
 *
 * Input			:None
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Read Current time &Convert it in epoch format
 *
 * Note				:None
 **************************************************************************************/
void GetTimeStamp(void)											//gets timestamp
{
	struct tm t;
	WORD Year = 0;

	GetCurrentTime();										//Get the current Time
	ConvertBCDDateToDec(&t_currdt);						//convert BCD date into Decimal
	
	Year =  Year + 2000 ;									//
	Year = ( Year + t_currdt.date.Year) ;

	t.tm_year	= Year - 1900;								//subtract 1900 from year
	t.tm_mon	= t_currdt.date.Month - 1;
	t.tm_mday	= t_currdt.date.Date;
	t.tm_hour	= t_currdt.date.hh;
	t.tm_min	= t_currdt.date.mm;
	t.tm_sec	= t_currdt.date.ss;
	t.tm_isdst	= -1;										//isdt should set to -1 as unknown

	t_day = mktime(&t);										//creats the TIMESTAMP

	t_day = t_day  - SEC_OFFSET;
	
}
/***************************************************************************************
 * Function			:void Build_Remote_URL(BYTE url)
 *
 * PreCondition		:Server details should have come already
 *
 * Input			:URL number
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:Buids the next URL to send to server
 *
 *
 * Note				:None
 **************************************************************************************/
void Build_Remote_URL(BYTE URL)
{
	
	switch(URL)
	{
		case GATEWAY_REGISTER_URL:									//if URL is for Device
		{
				memset(RemoteURL, 0x00 , sizeof(RemoteURL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],Gateway_Access_Info.URL, strlen(Gateway_Access_Info.URL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)], "/gateway/register.json?",23);
				char* hash = Get_Hmac(DATA);
				strcpy(&RemoteURL[strlen(RemoteURL)], "X-Access-Key=");
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s&X-Dtn-Signature=", Gateway_Access_Info.Access_Key);
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s",hash);
				break;
		}
		case GATEWAY_HEARTBEAT_URL:									//URL is for Parameter
		{
				memset(RemoteURL, 0x00 , sizeof(RemoteURL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],Gateway_Access_Info.URL, strlen(Gateway_Access_Info.URL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)], "/gateway/heartbeat.json?",24);
				char* hash = Get_Hmac(DATA);
				strcpy(&RemoteURL[strlen(RemoteURL)], "X-Access-Key=");
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s&X-Dtn-Signature=",Gateway_Access_Info.Access_Key);
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s",hash);

				break;
		}
		case GET_DEVICE_REGISTER_URL:										//URL is for Variable
		{	
				memset(RemoteURL, 0x00 , sizeof(RemoteURL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],Gateway_Access_Info.URL, strlen(Gateway_Access_Info.URL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],"/device/register.json?",22);
				char* hash = Get_Hmac(DATA);
				strcpy(&RemoteURL[strlen(RemoteURL)], "X-Access-Key=");
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s&X-Dtn-Signature=", Gateway_Access_Info.Access_Key);
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s",hash);
				
				break;
		}
		case GET_SENSOR_HEARTBEAT_URL:										//to send status
		{
				memset(RemoteURL, 0x00 , sizeof(RemoteURL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],Gateway_Access_Info.URL, strlen(Gateway_Access_Info.URL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)], "/sensor/heartbeat.json?",23);
				char* hash = Get_Hmac(DATA);
				strcpy(&RemoteURL[strlen(RemoteURL)], "X-Access-Key=");
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s&X-Dtn-Signature=", Gateway_Access_Info.Access_Key);
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s",hash);

				break;
		}
		case GET_DATA_URL:							//for set current values
		{
				memset(RemoteURL, 0x00 , sizeof(RemoteURL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)],Gateway_Access_Info.URL, strlen(Gateway_Access_Info.URL));
				memcpypgm2ram(&RemoteURL[strlen(RemoteURL)], "/sensor/event.json?",19);
				char* hash = Get_Hmac(DATA);
				strcpy(&RemoteURL[strlen(RemoteURL)], "X-Access-Key=");
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s&X-Dtn-Signature=", Gateway_Access_Info.Access_Key);
				sprintf(&RemoteURL[strlen(RemoteURL)], "%s",hash);
				break;
		}
	}
}	


/*************************************************************************************
* Function			:void ERR_LOG(char *)
 *
 * PreCondition		:
 *
 * Input			: string tobe saved
 *					 
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:It writes the error condtions in file.
 *					
 *
 * Note				:None
*************************************************************************************/
void ERR_LOG (char *Err)
{
		
	res = f_open(&hFile, ERR_LOG_FILE,FA_OPEN_ALWAYS | FA_WRITE);
		
	
	if(!res)
	{
		if ((t_currdt.date.Day == 1) && (t_currdt.date.hh == 0) && (t_currdt.date.mm > 0) && (t_currdt.date.mm < 24))
		{
			f_close(&hFile);
			res = f_unlink(ERR_LOG_FILE);
			return;
		}	
		f_lseek(&hFile, hFile.fsize);	
		f_write(&hFile,Err,strlen(Err),&Readcnt1);
		f_write(&hFile, "\t",1, &Readcnt1);
		GetTime();
		f_write(&hFile,uc_DateString,18,&Readcnt1);
		f_write(&hFile, "\n",1, &Readcnt1);
	}	

	else LED6_IO = 1;	

	f_close(&hFile);
}

/*************************************************************************************
* Function			:void FILE NAME_LOG(char *)
 *
 * PreCondition		:
 *
 * Input			: string tobe saved
 *					 
 *
 * Output			:None
 *					
 * Side Effects		:None
 *
 * Overview			:It writes the File name which is cuurrently transmitted by the GPRS_FTP.
 *					
 *
 * Note				:None
*************************************************************************************/
void FILE_NAME_LOG (char *FILE_NAME_STRING)
{
		
//	res = f_open(&hFile, FILE_NAME,FA_OPEN_ALWAYS | FA_WRITE);
//	
//
//	if(!res)
//	{
//		if ((t_currdt.date.Day == 1) && (t_currdt.date.hh == 0) && (t_currdt.date.mm > 0) && (t_currdt.date.mm < 24))
//		{
//			f_close(&hFile);
//			res = f_unlink(FILE_NAME);
//			return;	
//		}	
//		f_lseek(&hFile, hFile.fsize);	
//		f_write(&hFile,FILE_NAME_STRING,strlen(FILE_NAME_STRING),&Readcnt1);
//		f_write(&hFile, "\n",1, &Readcnt1);  
//	}
//
//	else if(res) LED6_IO = 1;
//
//	f_close(&hFile);
}


/****************************************************************************
  Function:
    static void InitializeBoard(void)

  Description:
    This routine initializes the hardware.  It is a generic initialization
    routine for many of the Microchip development boards, using definitions
    in HardwareProfile.h to determine specific initialization.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
 void InitializeBoard(void)
{	
	LED4_TRIS=0;
	POWERKEY_TRIS = 0;				//makes powerkey pin for GSM as output
	RESET_SWCH_TRIS = 1;			//makes reset switch as input(Soft RESET)
	TRISEbits.TRISE8 = 1;				//makes RI pin as port pin not as INT 

 	HRTBIT_LED_TRIS = 0;			//make Heart bit  LED pin as input and make it off
 	HRTBIT_LED_IO = 0;

	GSM_VBAT_EN_TRIS = 0;
	GSM_VBAT_EN_IO = 0;

	#if defined(__18CXX)
		Nop();
	#else	// 16-bit C30 and and 32-bit C32		

	//From here the PIC32 Section will be implemented 
	#if defined(__PIC32MX__)
	{
		// Enable multi-vectored interrupts
		INTEnableSystemMultiVectoredInt();
		
		// Enable optimal performance
		SYSTEMConfigPerformance(GetSystemClock());
		mOSCSetPBDIV(OSC_PB_DIV_1);				// Use 1:1 CPU Core:Peripheral clocks
		
		// Disable JTAG port so we get our I/O pins back, but first
		// wait 50ms so if you want to reprogram the part with 
		// JTAG, you'll still have a tiny window before JTAG goes away.
		// The PIC32 Starter Kit debuggers use JTAG and therefore must not 
		// disable JTAG.
		DelayMs(50);
		#if !defined(__MPLAB_DEBUGGER_PIC32MXSK) && !defined(__MPLAB_DEBUGGER_FS2)
			DDPCONbits.JTAGEN = 0;
		#endif
		LED_PUT(0x00);				// Turn the LEDs off
		
		CNPUESET = 0x00098000;		// Turn on weak pull ups on CN15, CN16, CN19 (RD5, RD7, RD13), which is connected to buttons on PIC32 Starter Kit boards
	}
	#endif

	#if defined(__dsPIC33F__) || defined(__PIC24H__)
	//		// Crank up the core frequency
	//		PLLFBD = 38;				// Multiply by 40 for 160MHz VCO output (8MHz XT oscillator)
	//		CLKDIV = 0x0000;			// FRC: divide by 2, PLLPOST: divide by 2, PLLPRE: divide by 2
	//	
	//		// Port I/O
	//		AD1PCFGHbits.PCFG23 = 1;	// Make RA7 (BUTTON1) a digital input
	//		AD1PCFGHbits.PCFG20 = 1;	// Make RA12 (INT1) a digital input for MRF24WB0M PICtail Plus interrupt
	//
	//		// ADC
	//	    AD1CHS0 = 0;				// Input to AN0 (potentiometer)
	//		AD1PCFGLbits.PCFG5 = 0;		// Disable digital input on AN5 (potentiometer)
	//		AD1PCFGLbits.PCFG4 = 0;		// Disable digital input on AN4 (TC1047A temp sensor)

	#else	//defined(__PIC24F__) || defined(__PIC32MX__)

		#if defined(__PIC24F__)
			CLKDIVbits.RCDIV = 0;		// Set 1:1 8MHz FRC postscalar
		#endif

	    #if defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
		Nop();
		#else
		
		    AD1CHS = 0;					// Input to AN0 (potentiometer)			for test
	
			#if defined(__32MX460F512L__) || defined(__32MX795F512L__)	// PIC32MX460F512L and PIC32MX795F512L PIMs has different pinout to accomodate USB module
				// Disable digital input on AN2 (potentiometer)
				ADC_CHANNEL_1 = 0;
				ADC_CHANNEL_2 = 0;
				ADC_CHANNEL_3 = 0;
				ADC_CHANNEL_4 = 0;
				ADC_CHANNEL_5 = 0;
				ADC_CHANNEL_6 = 0;
				ADC_CHANNEL_7 = 0;
				ADC_CHANNEL_8 = 0;

			#else
				AD1PCFGbits.PCFG5 = 0;		// Disable digital input on AN5 (potentiometer)
			#endif
		#endif
	#endif

	// ADC
	AD1CON1 = 0x80E4;			// Turn on, auto sample start, auto-convert
	AD1CON2 = 0x041C;	//04;	// AVdd, AVss, int every 2 conversions, MUXA only, scan
	AD1CON3 = 0x1003;			// 16 Tad auto-sample, Tad = 3*Tcy
	#if defined(__32MX460F512L__) || defined(__32MX795F512L__)	// PIC32MX460F512L and PIC32MX795F512L PIMs has different pinout to accomodate USB module
	
		AD1CSSL = 0x0F1D;			//1<<0;				// Scan pot

	#else
		Nop()
	#endif

#endif

}