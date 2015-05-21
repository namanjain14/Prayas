
#define THIS_IS_STACK_APPLICATION

// Include all headers for any enabled TCPIP Stack functions
#include <p32xxxx.h>
#include <plib.h>
#include <stdlib.h>

#include "TCPIP Stack/TCPIP.h"
#include "declearation.h"
#include "i2c_master.h"
#include "diskio.h"
#include "ff.h"
#include "Aliotgateway.h"
#include "TCPIP Stack/Tick.h"
#include "VariableDef.h"
#include "GenericTypeDefs.h"

// C30 and C32 Exception Handlers
// If your code gets here, you either tried to read or write
// a NULL pointer, or your application overflowed the stack
// by having too many local variables or parameters declared.

#if defined(__C32__)
	void _general_exception_handler(unsigned cause, unsigned status)
	{
		Nop();
			#if defined(TEST)
			{
				memcpypgm2ram(Err_String,"C32 GENRAl EXECPTION",19);
				ERR_LOG (Err_String);
			}
			#endif
		Nop();
	}
#endif

int main(void)
{
	static DWORD per_sec = 0;
	InitializeBoard();													// Initialize application specific hardware
	PowerONGSM(TRUE);
	Initialization();		

	//................File System Initialisation...................//
	
	while(disk_status(MMC_DRIVE));     //Wait untill SD card is inserted											
	if(!(disk_status(MMC_DRIVE) ))//& STA_NODISK))					//if SD card is inserted	
	{
		// disk inserted so initialise it
		if (disk_initialize(MMC_DRIVE) == 0) 							//if SD card initialize
		{
			if (f_mount(MMC_DRIVE, &fatfs[MMC_DRIVE]) == FR_OK) 
			{
				uc_Flag_SDInit = TRUE;	  //succefully initialise sd card							//if file system initialize properly
			}
			else
			{
				Reset();
			}
		}
		else
		{
			Reset();
		}
	}
	InitConnection();														//functional initialization gateway device sensors,etc	
	while(1)
	{
		ClrWdt();
		if(ReadADC==TRUE)
		{
			Sample_Flag++;
			ReadADC=FALSE;
			ADC_Read_Value=AnalogData*3.3/1024;
			Toatal_Value=Toatal_Value+ADC_Read_Value;
			if(ADC_Read_Value<min)
			{
				min=ADC_Read_Value;
			}
			else if(ADC_Read_Value>max)
			{
				max=ADC_Read_Value;
			}
			
		}
		if((Sample_Flag>=600) && (uc_HTTP_DONE==TRUE))
		{
			heartbeat++;
			value=Toatal_Value/Sample_Flag;
			if(uc_GPRS_Done==FALSE)
			{
				GPRSFailed++;
			}
			uc_GPRS_Done=FALSE;
		
			Sample_Flag=0;
			min=0;
			max=0;
			Toatal_Value=0;
			sendData=TRUE;
			POST_DATA=SEND_DATA;
			uc_HTTP_DONE=FALSE;	
		}
		if(sendData==TRUE)
		{
		
			switch(POST_DATA)
			{
				case GATE_HEARTBEAT:
				{
					if(url_build==FALSE)
					{
						Transmit_Gateway_Heartbeat();
						url_build=TRUE;
						uc_GPRS_Done=FALSE;
					}	
					SendURL((METHOD) POST);	
					if(uc_GPRS_Done==TRUE)
					{
						POST_DATA=SENS_HEARTBEAT;
						url_build=FALSE;
					}
					break;
				}
				
				case SENS_HEARTBEAT:
				{
					if(url_build==FALSE)
					{
						Transmit_Sensor_Heartbeat();
						url_build=TRUE;
						uc_GPRS_Done=FALSE;
					}	
					SendURL((METHOD) POST);	
					if(uc_GPRS_Done==TRUE)
					{
						url_build=FALSE;
						sendData=FALSE;
						uc_HTTP_DONE=TRUE;
						POST_DATA=10;
					}
					break;
				}
				case SEND_DATA:
				{
					if(url_build==FALSE)
					{
						Transmit_Analog_Data(value);
						url_build=TRUE;
						uc_GPRS_Done=FALSE;
						
						//	GetTimeStamp();
						//time=t_day;
					}	
					SendURL((METHOD)POST);	
					if(uc_GPRS_Done==TRUE)
					{
						if(heartbeat>=5)
						{
							heartbeat=0;
							POST_DATA=GATE_HEARTBEAT;
							url_build=FALSE;
						}
						else
						{
							POST_DATA=10;
							url_build=FALSE;
							sendData=FALSE;
							uc_HTTP_DONE=TRUE;
							//	GetTimeStamp();
							//time1=t_day;
							asm("NOP");
						}
					
					}
					break;
				}
				default:
				break;
			}
		}
		if(TickGet() - per_sec >= TICK_SECOND/2ul)								// Blink LED0 (right most one) every half second.
		{
			per_sec = TickGet();
			LED4_IO ^= 1;													//internal heartbit LED
			HRTBIT_LED_IO ^=1; 											//pannel heartbit LED
	
		}
		if((uc_GSMRestart==TRUE) || ((uc_GPRSFailed == TRUE) && (uc_GPRSTried == TRUE)))
		{
			PowerONGSM(FALSE);
			DelayMs(200);
			uc_GSMRestart=3;	
			PowerONGSM(TRUE);
			uc_GPRSFailed=FALSE;
			uc_GPRSTried=FALSE;
		}
	}
}



