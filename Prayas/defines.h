/****************************************************************************************/
/* Data Acquisition System Project */
/* File Name : defines.h */
/* Purpose	 : This file contains the declarations for global variables */
/* Author	 : Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/****************************************************************************************/

#define TEST
#define FIRMWARE_VERSION			"V1.00"						//Needs to change as per modification
#define BOOTLOADER_VERSION			"V1.00"					//Needs to chnage as per modification
	
// Gateway Access information
#define ACCESS_KEY              	"5c9a57e3df6ctf94ce87b567ae7ea5t5a96de699"
#define SECRET_KEY					"f6758b7tfedb5274fce65ca35cf535dd7d9be8c4"
#define SERVER_URL							"http://staging.altizon.com/api/v1"

// Gateway information
#define GATEWAY_KEY					"9d763f3e7dbf354bea26etdc2ed26tt4a52cabcd"
#define GATEWAY_NAME				"MapynGateway"
#define GATEWAY_DESCRIPTION		    "mapyn gateway from C"


// Device information
#define DEVICE_KEY					"c31at594ee7958b78f614ce81ec78f49c4131234"
#define DEVICE_NAME					"TEST_DEV1"
#define DEVICE_DESCRIPTION		    "Sample device"
#define DEVICE_TYPE						"Test Device"
#define DEVICE_TIMEZONE					"UTC"

// Sensor information
#define SENSOR_KEY					"1a7cb3te16f47f38fc3e157e4da2244dteeaa123"
#define SENSOR_NAME					"Sens1"
#define SENSOR_DESCRIPTION		    "An analog sensor from C"
#define SENSOR_TYPE 				"analog"
#define SENSOR_TIMEZONE				"UTC"
#define SENSOR_METADATA				"{\"type\":\"object\",\"properties\":{\"value\": {\"type\":\"number\"}}}"

#define MAX_RETRIES					5	//maximum reties for Gateway register

#define GATE_HEARTBEAT 0
#define SENS_HEARTBEAT 1
#define SEND_DATA	  3

//RTC initialization frequency setting
#define SYSCLK						(80000000)					//system Clock for I2c
#define Fsck						50000						//Frequency for I2c
#define PBCLK  						(SYSCLK/1)					//Peripharal clock calculation
#define BRG_VAL 					((PBCLK/2/Fsck)-2)			//BaudRate calculation for I2c
#define SEC_OFFSET					19800						//seconds offset in the timestamp as per the GMT difference
//Input Output
#define TRUE						1							// To set the flags
#define FALSE						0							// To clear the flags
#define SET							1							//to enable
#define RESET						0							//to disable

#define CMD							SET							//to distinguished as cmd or string 
#define SREG						(!SET)							//used in GSM section


#define GSM_RX_ENABLE				(IEC0bits.U1RXIE)			//UART1 RX enable pin(GSM)
#define GSM_TX_ENABLE				(IEC0bits.U1TXIE)			//UART1 TX enable pin(GSM)

#define UMODE_MOD					U2MODE
#define USTA_MOD					U2STA
#define UBRG_MOD					U2BRG
#define DataReadyUART_MOD			DataRdyUART2()
#define ReadUART_MOD				ReadUART2()


#define UMODE_GSM					U1MODE
#define USTA_GSM					U1STA
#define UBRG_GSM					U1BRG
#define DataReadyUART_GSM			DataRdyUART1()
#define ReadUART_GSM				ReadUART1()

//RTC releated Structure
typedef struct
{
	unsigned char ss;												// variable for second
	unsigned char mm;												// variable for minute
	unsigned char hh;												// variable for hours
	unsigned char Day;												// variable for day
	unsigned char Date;												// variable for date
	unsigned char Month;											// variable for month
	unsigned char Year;												// variable for year
	
}RTCStruct;

typedef union _RTCDT												// union to store RTC time format  
{
	unsigned char dt[7];												//Array for data 
       RTCStruct date;													//RTCStruct structure object "date"
}RTCDT;

//RTC related
#define RTC_DT_RAM_ADD				0x00					// RTC Ram Address
#define RTC_ADD						0xD0					// RTC Address
#define SIZE_OF_RTCTIME				7						// macro to define size of rtctime 
																	// structure.
#define BASE_10						10						
#define BASE_16						16
#define NIBBLE						4

#define SMS_DELAY_LOW 20000					//around 9-10 sec
#define SMS_DELAY_MID 60000					//around 22-25 sec delay
#define SMS_DELAY_HIGH 220000				//around 91-94 sec

//Default Parrameters for Ethernet Configuration
#define DE_BAUD_RATE_GSM 			"9600"						// Default Input UART BAUD RATE

//structure Defination

//Structure for Analog Channel Calibrated data
typedef struct
{
	WORD UpperRange;
	WORD LowerRange;
	WORD UpperLimit;
	WORD LowerLimit;
	float m_Factor;
}AnalogCali;

//maximum number of channel
#define MAX_CHANNEL	8

typedef enum
{
	GET = 0,
	POST
}METHOD;



//GPRS GET/POST state machine

typedef enum _SM_GPRS_POST
{
	SM_GPRS_POST_HOME,
	SM_GPRS_POST_HOME_RES,
	SM_GPRS_POST_ECHO,
	SM_GPRS_POST_ECHO_RES,
	SM_GPRS_POST_SIGNAL_QUALITY,						
	SM_GPRS_POST_SIGNAL_QUALITY_RES,
	SM_GPRS_POST_ACT,
	SM_GPRS_POST_ACT_RES,
	SM_GPRS_POST_STAT,
	SM_GPRS_POST_STAT_RES,
	SM_GPRS_POST_APN,
	SM_GPRS_POST_APN_RES,
	SM_GPRS_POST_REG_TCPIP,
	SM_GPRS_POST_REG_TCPIP_RES,
	SM_GPRS_POST_ACTIVATE,
	SM_GPRS_POST_ACTIVATE_RES,
	SM_GPRS_POST_HTTP_VER,
	SM_GPRS_POST_HTTP_VER_RES,
	SM_GPRS_POST_URL,
	SM_GPRS_POST_URL_RES,
	SM_GPRS_POST_URL_OK,
	SM_GPRS_POST_GET,
	SM_GPRS_POST_GET_RES,
	SM_GPRS_POST_READ,
	SM_GPRS_POST_READ_RES,
	SM_GPRS_POST_DONE,
	SM_GPRS_POST_DACTIVATE,
	SM_GPRS_POST_DACTIVATE_RES
}SM_GPRS_POST;



#define GATEWAY_REGISTER_URL					0
#define GATEWAY_HEARTBEAT_URL 					1
#define GET_DEVICE_REGISTER_URL					2
#define GET_SENSOR_HEARTBEAT_URL				3
#define GET_DATA_URL							4




typedef enum
{
 	CMND = 0,
 	DIGNO
}COMMAND;





