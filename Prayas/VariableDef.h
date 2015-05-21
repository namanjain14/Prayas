/****************************************************************************************/
/*  Data Acquisition System Project */
/* File Name 	: VariableDef.h */
/* Purpose		: This file contains the declarations for global variables */
/* Author		: Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/****************************************************************************************/
#include "time.h"
#include "ff.h"
#include "Aliotgateway.h"
//#include "defines.h"

extern BYTE res;
extern BYTE RunOne;
extern UINT Readcnt1,Readcnt2;													//used in read file
extern DWORD ui_SMSCount;
extern unsigned char uc_URLSent;
extern unsigned char uc_Flag_SDInit;
extern unsigned char uc_GPRS_Receive;
extern unsigned char uc_URLBuild;
extern unsigned char uc_GPRSFailed;
extern unsigned char uc_RetriesGPRS;
extern unsigned char uc_RTR;		// counter to check gprs failed condition
extern unsigned char uc_GSMRestart;
extern unsigned char uc_Gsm_Cammand;		//for the data tobe saved if cmd_set ==1 
extern DWORD GSM_Timer;


extern BYTE TESTING[100];						//to convert data into ascii from bin. in error log function 
extern unsigned char uc_MainRetries ;
extern long long int lli_Closest_UBRG_Value_GSM;
extern long long int lli_Baud_Rate_GSM;
extern long long int lli_Baud_Actual_GSM;
extern long long int lli_Baud_Error;
extern long long int lli_Baud_Error_Percent;
extern unsigned int ui_Mode_GSM;

extern RTCDT t_currdt,t_currdtDec;					// structure variable to hold
extern time_t t_day;
extern BYTE uc_DateString[18];
extern BYTE uc_TimeString[18];

//HttpPost Array
extern BYTE HTTP_POST_RX_FIFO[1500];
extern BYTE RemoteURL[200];
extern BYTE ReadADC;
//GSM Array
extern BYTE GPRS_APN[26];
extern BYTE GSM_UART_TX_FIFO[250];
extern BYTE GSM_UART_RX_FIFO[10];
extern BYTE *OUART_RXHeadPtr;											//pointers of the GSM UART reciever buffer
extern BYTE *OUART_RXTailPtr;
extern BYTE *OUART_TXHeadPtr;											//pointers of the GSM UART transmit buffer
extern BYTE *OUART_TXTailPtr;

extern unsigned char uc_GPRSTried;

//Functions used in this Program
extern void Initialization(void);
extern void InitConnection(void);
extern void SetGSMUARTParameter(void);									//sets the parameter for GSM UART
extern void GSM_UARTInit(void);											//initialize the GSM UART
extern void PowerONGSM(unsigned char mode);											//Power ONs the GSM module
extern void EmergancyOFF_GSM(void);
extern void SendGSMComm(char* ATcommand,unsigned char Type);			//Copy sms sring to array
					
extern void Bin2Ascii(WORD *DATA , BYTE *Ascii_DATA);					//Convert Binary Data into Ascii
extern void Ascii2Bin(BYTE *DATA, WORD *Bin_DATA);					//convert Ascii data into Binary
extern void ConvertBCDDateToDec(RTCDT *dt);							// function to onvert BCD time format to  decimal
extern void ConvertDECDateToBCD(RTCDT *dt);							// function to convert decimal format to BCD time format.
extern void GetTime(void);													//Get and Stored current Time
extern void Build_Remote_URL(BYTE URL);									//Build remote URL
//extern BOOL SendURL(METHOD method);								//Stored to be read variables detail

extern void GetTimeStamp(void);											//gets timestamp

/**************ERROR LOG*******************/
extern char Err_String[1500];
extern BYTE ERR_LOG_FILE[];

/**************FILE NMAE LOG*******************/


extern char* Get_Hmac(char* message) ;
extern  char  message[300];

extern BYTE RTCTime[16];
extern Gateway_Access Gateway_Access_Info;
extern Gateway Gateway_Info;
extern Device Device_Info;
extern Sensor Sensor_Info;
extern time_t  t_day;
extern BYTE Attendance_Data[700];
extern BYTE RemoteURL[200];
extern BYTE uc_GPRS_Done;
extern BYTE uc_GSMRestart;
extern FIL hFile;
extern FILINFO finfo;
extern FATFS fatfs[_DRIVES];
static BYTE LocalBuff[70];													//for conversion

extern BYTE DATA[1024];
extern Gateway_Access Gateway_Access_Info;
extern Gateway Gateway_Info;
extern Device Device_Info;
extern Sensor Sensor_Info;
extern WORD AnalogData;
extern BYTE uc_Retries;
extern float ADC_Read_Value;
extern float Toatal_Value,min,max,value;

// static SM_GPRS_POST smGPRSPOST;
//extern BYTE VBuffer[50];
//extern static BYTE len;
//extern static BYTE TemporaryBuff[100];
//extern static BYTE uc_GprsDactFlg = 0;
//extern BYTE BufferSize;
//extern BYTE uc_GPRS_Done;