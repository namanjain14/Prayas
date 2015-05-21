/****************************************************************************************/
/*  Data Acquisition System Project */
/* File Name 	: declearation.h */
/* Purpose		: This file contains the declarations for global variables */
/* Author		: Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/****************************************************************************************/
#include "time.h"
#include "ff.h"
#include "Aliotgateway.h"
// Variables used in the Program
BYTE res = 0;
BYTE RunOne = 0;
UINT Readcnt1 = 0, Readcnt2 = 0;													//used in read file
DWORD ui_SMSCount =0 ;
unsigned char uc_GsmCheck =0;
unsigned char uc_URLSent = 0;
unsigned char uc_Flag_SDInit = 0;
unsigned char uc_GPRS_Receive = 0;				// Site configuration received over GPRS (URL response)
unsigned char uc_URLBuild = 0;
unsigned char uc_GPRSFailed = 0;
DWORD		GSM_Timer = 0;
unsigned char uc_GSMRestart=0;
unsigned char uc_RetriesGPRS=0;
unsigned char uc_RTR=0;
unsigned char uc_GPRSTried = FALSE;
long long int lli_Closest_UBRG_Value_MOD = 0;						//Variable to stored the closest baud rate value 
long long int lli_Baud_Rate_MOD = 0;								//Temparary Variable for BAUD RATE of ModBus UART
long long int lli_Baud_Actual_MOD = 0;								//Actual set BAUD RATE ModBus UART 
unsigned int ui_Mode_MOD = 0;										//Variable to hold the ModBus UART MODE register Value
long long int lli_Closest_UBRG_Value_GSM = 0;						//Variable to stored the closest baud rate value 
long long int lli_Baud_Rate_GSM = 0;								//Temparary Variable for BAUD RATE of GSM UART
long long int lli_Baud_Actual_GSM = 0;								//Actual set BAUD RATE GSM UART 
long long int lli_Baud_Error = 0;										//ERROR value after the calculation 
long long int lli_Baud_Error_Percent = 0;								//ERROR value after the calculation in percent
unsigned int ui_Mode_GSM = 0;										//Variable to hold the GSM UART MODE register Value
unsigned char uc_Gsm_Cammand = 0;			// gsm data to be saved or not (It is set when command is sent)
BYTE TESTING[100];							// to convert data in binary to ascii in error log func //
BYTE HTTP_POST_RX_FIFO[1500];									//Array for the HTTP post receiving buffer
BYTE RemoteURL[200];
unsigned char ReadADC;


//GSM/GPRS Array
BYTE GPRS_APN[26];
BYTE GSM_UART_TX_FIFO[250];
BYTE GSM_UART_RX_FIFO[10];
BYTE *OUART_RXHeadPtr	= HTTP_POST_RX_FIFO;				//pointers of the GSM UART reciever buffer
BYTE *OUART_RXTailPtr 	= HTTP_POST_RX_FIFO;
BYTE *OUART_TXHeadPtr	= GSM_UART_TX_FIFO;				//pointers of the GSM UART transmit buffer
BYTE *OUART_TXTailPtr 	= GSM_UART_TX_FIFO;



//For RTC
RTCDT t_currdt,t_currdtDec;										// structure variable to hold
time_t  t_day;														//structure for Long time stamp
BYTE uc_DateString[18];
BYTE uc_TimeString[18];
BYTE uc_DateString_write[18];
									//Yogesh
//Functions used in this Program
void GenericTCPClient(METHOD Method);
void Initialization(void);
void InitConnection(void);
void SetModBusUARTParameter(void);								//sets the parameter for ModBus UART
void SetGSMUARTParameter(void);									//sets the parameter for GSM UART
void PowerONGSM(unsigned char mode);											//Power ONs the GSM module

void EmergancyOFF_GSM(void);									//emergancy off GSM Module

void SendGSMComm(char* ATcommand,unsigned char Type);			//Copy sms sring to array
//BOOL SendGET(void);												//to request URL from GPRS
void Bin2Ascii(WORD *DATA , BYTE *Ascii_DATA);				//Convert Binary Data into Ascii
void Ascii2Bin(BYTE *DATA, WORD *Bin_DATA);					//convert Ascii data into Binary
void WriteParameter(void);											//Writes parameter in EEPROM
void WriteDefaultParameter(void);									//Writes Default parameter in EREPROM
void ConvertBCDDateToDec(RTCDT *dt);							// function to convert BCD time format to  decimal
void ConvertDECDateToBCD(RTCDT *dt);							// function to convert decimal format to BCD time format.
void GetTime(void);													//Get and Stored current Time
void Build_Remote_URL(BYTE URL);									//Build remote URL									//Receive and Parse server Response
BOOL SendURL(METHOD method);
void GSM_UARTInit(void);											//initialize the GSM UART

void GetTimeStamp(void);											//gets timestamp
void GSM_GPRS_Init(void);
char* Get_Hmac(char* message) ;
//void ERR_LOG (char *Err_String)										//Erroe Log for GPRS FTP
/**************ERROR LOG*******************/
char Err_String[1500];
BYTE ERR_LOG_FILE[]="1:Error.txt";
FILE *ERR_LOG_FILE_PTR;


char FILE_NAME_STRING[100];
BYTE FILE_NAME[]="1:NAME_ER.txt";
FILE *FILE_NAME_PTR;
char boot_variable;
char  message[300];
void InitializeBoard(void);



FILINFO flinfo;											//file info used in the find in files functionality
FIL hFile, tFile;											//file pointer
FILINFO finfo;											//file info structure
FATFS fatfs[_DRIVES];									// file system object

static BYTE POST_DATA;
BYTE DATA[1024];
// Main application entry point.
//
extern BYTE uc_GPRS_Done;
Gateway_Access Gateway_Access_Info;
Gateway Gateway_Info;
Device Device_Info;
Sensor Sensor_Info;

float ADC_Read_Value;
float Toatal_Value,min,max,value;
extern  unsigned int Sample_Flag;
static char once=0;
char sendData=0;
unsigned int Sample_Flag;
unsigned char heartbeat=0;
unsigned char GPRSFailed=0;
unsigned char uc_HTTP_DONE=1;
unsigned char url_build=0;
BYTE uc_Retries=0;
WORD AnalogData;

