/****************************************************************************************/
/* Data Acquisition System Project */
/* File Name 	: i2c_master.c*/
/* Purpose		: This file contains the declarations for global variables */
/* Author		: Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/****************************************************************************************/
#include <plib.h>
#include <i2c_master.h>
#include "GenericTypeDefs.h"
#include "VariableDef.h"

//-------------- FUNCTION: I2C WAIT ---------------------------------
// Function: 	i2c_wait
// Purpose: 	I2C Wait routine 
// Variables Passed: count value
// Variables Returned: None
//-------------------------------------------------------------------
void i2c_wait(unsigned int cnt)
{
	while(--cnt)
	{
		Nop();
		Nop();
	}
}

//-------------- END FUNCTION: I2C WAIT -----------------------------


//-------------- END FUNCTION: EEPROM READ ----------------------
/*-------------------------------------------------------------------------------------*/
// Function Name: WriteRTC(BYTE RTC_Address, BYTE RTC_RAM_ADD, BYTE data)
// Purpose: To read Current Time From RTC. 
// Variables Passed: Slave address, address of lacation and data to be written
// Variables Returned: True - If successful
//					   False - If unsuccessful
/*--------------------------------------------------------------------------------------*/
unsigned char WriteRTC(unsigned char RTC_Address, unsigned char  RTC_RAM_ADD, 
							unsigned char data)
{
	unsigned char SlaveAddress;
	unsigned char RTC_RAM_Address;
	char i2cData[5];
	int  DataSz;

	OpenI2C2( I2C_EN | I2C_SLW_DIS | I2C_7BIT_ADD, BRG_VAL );

	SlaveAddress 	= RTC_Address;						//0b1010000 Serial EEPROM address
    RTC_RAM_Address = RTC_RAM_ADD;

	// Send Data to RTC to program one location
	i2cData[0] = SlaveAddress; 		//EEPROM Device Address and WR Command
	i2cData[1] = RTC_RAM_Address;				//RAM Address of RTC
	i2cData[2] = data;							// DATA on that address
	DataSz = 3;

	StartI2C2();								//Send the Start Bit
	IdleI2C2();									//Wait to complete

	unsigned char Index = 0;
	while( DataSz )
	{
		MasterWriteI2C2( i2cData[Index++] );
		IdleI2C2();								//Wait to complete

		DataSz--;

		//ACKSTAT is 0 when slave acknowledge. if 1 then slave has not acknowledge the data.
		if( I2C2STATbits.ACKSTAT )
			break;
	}

	StopI2C2();									//Send the Stop condition
	IdleI2C2();									//Wait to complete

	// wait for eeprom to complete write process. poll the ack status
	while(1)
	{
		i2c_wait(10);

		StartI2C2();							//Send the Start Bit
		IdleI2C2();								//Wait to complete

		MasterWriteI2C2( i2cData[0] );
		IdleI2C2();								//Wait to complete

		if( I2C2STATbits.ACKSTAT == 0 )			//eeprom has acknowledged
		{
			StopI2C2();							//Send the Stop condition
			IdleI2C2();							//Wait to complete
			break;
		}

		StopI2C2();								//Send the Stop condition
		IdleI2C2();								//Wait to complete
	}
	return 0;
}

/*-------------------------------------------------------------------------------------*/
// Function Name: WriteRTC(BYTE RTC_Address, BYTE RTC_RAM_ADD, BYTE data)
// Purpose: To read Current Time From RTC. 
// Variables Passed: Slave address, address of lacation and pointer to data, length of data
// Variables Returned: Data
/*--------------------------------------------------------------------------------------*/
unsigned char ReadRTC(unsigned char  RTC_Address, unsigned char RTC_RAM_ADD,
									unsigned char *data, unsigned char length)
{
	unsigned char SlaveAddress;
	unsigned char RTC_RAM_Address;
	char i2cData[10];
	int  DataSz;
    
    
	//Enable channel
	OpenI2C2( I2C_EN | I2C_SLW_DIS | I2C_7BIT_ADD, BRG_VAL );

	SlaveAddress 	= RTC_Address;	//0b1010000 Serial EEPROM address
	RTC_RAM_Address = RTC_RAM_ADD;
    
	// Send Data to eeprom to program one location

	i2cData[0] = SlaveAddress;		//EEPROM Device Address and WR Command
	i2cData[1] = RTC_RAM_Address;
	DataSz = 2;

	StartI2C2();								//Send the Start Bit
	IdleI2C2();									//Wait to complete

	//send the address to read from the serial eeprom
	int Index = 0;
	while( DataSz )
	{
		MasterWriteI2C2( i2cData[Index++] );
		IdleI2C2();								//Wait to complete

		DataSz--;

		//ACKSTAT is 0 when slave acknowledge. if 1 then slave has not acknowledge the data.
		if( I2C2STATbits.ACKSTAT )
			break;
	}

	//now send a start sequence again
	RestartI2C2();								//Send the Restart condition

	//wait for this bit to go back to zero
	IdleI2C2();									//Wait to complete

	MasterWriteI2C2(SlaveAddress | 1 ); //transmit read command
	IdleI2C2();									//Wait to complete

	unsigned char i2cbyte;
	*data = MasterReadI2C2();
//			#if defined(TEST)
//			{
//					memset(Err_String,0x00,sizeof(Err_String));							//testing
//					memcpypgm2ram(Err_String,"RTC DTA VAL",11);							//testing
//					ERR_LOG (Err_String);
//					ERR_LOG (data);
//			}
//			#endif
	StopI2C2();									//Send the Stop condition
	IdleI2C2();									//Wait to complete
    return (i2cbyte);	

}

/*-------------------------------------------------------------------------------------*/
// Function Name: GetCurrentTime
// Purpose: To read Current Time From RTC. 
// Variables Passed: None
// Variables Returned: None
/*--------------------------------------------------------------------------------------*/

void GetCurrentTime (void)	
{
	unsigned char i;
    //INTCONbits.GIE = DO_OFF;
	for(i = 0; i < SIZE_OF_RTCTIME; i++)
    {
     	t_currdt.dt[i] = 0;
     	//LDByteReadI2C(RTC_ADD, (RTC_DT_RAM_ADD + i), &(t_currdt.dt[i]), 1);	  
		ReadRTC(RTC_ADD, (RTC_DT_RAM_ADD + i), &(t_currdt.dt[i]), 1);	  
		if (i == 0)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 1)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 2)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 3)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x07);	  
		}
		else if (i == 4)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 5)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x1F);	  
		}
		else if (i == 6)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0xFF);	  
		}
    }
    //INTCONbits.GIE = DO_ON; 
}	


/*--------------------------------------------------------------------------------------*/
// Function Name: SetCurrentTime
// Purpose: to set current time of RTC. 
// Variables Passed: None
// Variables Returned: None
/*--------------------------------------------------------------------------------------*/

void SetCurrentTime (void)
{
	unsigned char  i;
	
    for(i = 0; i < 7; i++)
   	{
		if (i == 0)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 1)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 2)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 3)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x07);	  
		}
		else if (i == 4)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 5)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x1F);	  
		}
		else if (i == 6)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0xFF);	  
		}
    	WriteRTC(RTC_ADD, (RTC_DT_RAM_ADD + i), t_currdt.dt[i]);
        t_currdt.dt[i] = 0;
		ReadRTC(RTC_ADD,(RTC_DT_RAM_ADD + i), &(t_currdt.dt[i]), 1);
		if (i == 0)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 1)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x7F);	  
		}
		else if (i == 2)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 3)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x07);	  
		}
		else if (i == 4)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x3F);	  
		}
		else if (i == 5)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0x1F);	  
		}
		else if (i == 6)
		{
			t_currdt.dt[i] = (t_currdt.dt[i] & 0xFF);	  
		}
    }
 
}

/*-------------------------------------------------------------------------------------*/
// Function Name: void CheckValidTime (void)
// Purpose: To check valid data of time buffers. 
// Variables Passed: None
// Variables Returned: None
/*--------------------------------------------------------------------------------------*/

unsigned char CheckValidTime(void)	
{
	Nop();
	
//    if(t_currdt.date.ss < 0 || t_currdt.date.ss > 59 || t_currdt.date.mm < 0 
//        || t_currdt.date.mm > 59 || t_currdt.date.hh < 0 || t_currdt.date.hh > 24 
//        || t_currdt.date.Day < 0 || t_currdt.date.Day > 7 || t_currdt.date.Date <= 0 
//        || t_currdt.date.Date > 31 || t_currdt.date.Month <= 0 || t_currdt.date.Month > 12 
 //       || t_currdt.date.Year < 0 || t_currdt.date.Year > 99)
//    {
//		return 1;
 //   }
//	if(t_prevdt.date.ss < 0 || t_prevdt.date.ss > 59 || t_prevdt.date.mm < 0 
  //      || t_prevdt.date.mm > 59 || t_prevdt.date.hh < 0 || t_prevdt.date.hh > 24 
    //    || t_prevdt.date.Day < 0 || t_prevdt.date.Day > 7 || t_prevdt.date.Date <= 0 
//        || t_prevdt.date.Date > 31 || t_prevdt.date.Month <= 0 || t_prevdt.date.Month > 12
    //    || t_prevdt.date.Year < 0 || t_prevdt.date.Year > 99)
   // {
//		return 1;
//    }

	return 0;
}

	
//-------------- END FUNCTION: RTC RELETED ----------------------
