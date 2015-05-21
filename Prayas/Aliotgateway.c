
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <Compiler.h>
#include "GenericTypeDefs.h"
#include "time.h"
#include "Aliotgateway.h"
#include "hmac.h"
#include "defines.h"
#include "VariableDef.h"

/***************************************************************************************
 * Function			:void Get_Gateway_Register_Json()  
 
 * Overview			:Build Json encoded gateway register string

 *e.g				:{"name":mapyn_gateway","description":"mapyn gateway from c",
					"gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52c9dtb","timestamp":
					"1407306752","type":"GATEWAY_REGISTER"}
 *					 
 *
 * Note				:None
 **************************************************************************************/
void Get_Gateway_Register_Json() 
{

	memset(DATA, 0x00,sizeof(DATA));
	GetTimeStamp();
	startJSON(DATA);
	putJSONStringAndComma(DATA, "name", Gateway_Info.Name);
	putJSONStringAndComma(DATA, "description", Gateway_Info.Description);
	putJSONStringAndComma(DATA, "gateway_key", Gateway_Info.Key);
	strcpy(&DATA[strlen(DATA)],"\"timestamp\":");
	sprintf(&DATA[strlen(DATA)],"%d",t_day);
	strcpy(&DATA[strlen(DATA)],"000");
	strcpy(&DATA[strlen(DATA)],",");
	putJSONString(DATA, "type", "GATEWAY_REGISTER");
	endJSON(DATA);
	//return DATA;
}



/***************************************************************************************
 * Function			:Get_Sensor_Register_Json() 
 
 * Overview			:Build Json encoded Sensor register string

 *e.g				:[{"name":"AS1","description":"An analog sensor from C",
						"sensor_key":"1a7cb3te16f47f38fc3e157e4da2244dteeae8df",
						"type":"digital","timezone":"UTC","data":{"type":"object",
						"properties":{"value": {"type":"number"}}}}]}
 *					 
 *
 * Note				:None
 **************************************************************************************/

void Get_Sensor_Register_Json()
 {
	char temp[1024];
	//Now add the sensor JSON in a JSON array format
	strcpy(&DATA[strlen(DATA)],"[");
	int count = 0;
	while (count < Device_Info.Num_Sensors) 
	{
	
		startJSON(DATA);
		putJSONStringAndComma(DATA, "name",Device_Info.Sensors[count].Name);
		if (Device_Info.Sensors[count].Description)
			putJSONStringAndComma(DATA, "description", Device_Info.Sensors[count].Description);
		putJSONStringAndComma(DATA, "sensor_key",Device_Info.Sensors[count].Key);
		putJSONStringAndComma(DATA, "type", Device_Info.Sensors[count].Type);
		if (Device_Info.Sensors[count].Timezone)
			putJSONStringAndComma(DATA, "timezone", Device_Info.Sensors[count].Timezone);
		else
			putJSONStringAndComma(DATA, "timezone", "UTC");
	
		strcat(DATA, "\"data\":");
		strcat(DATA,"{\"type\":\"object\",\"properties\":{\"value\": {\"type\":\"number\"}}}");
		endJSON(DATA);
		count++;
		if (count < Device_Info.Num_Sensors) {
			strcat(DATA, ",");
		}
	}
	strcat(DATA, "]");
}

/***************************************************************************************
 * Function			:void Get_Device_Register_Json()  
 
 * Overview			:Build Json encoded Device register string

 * e.g				:{"name":"DEV1","gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52c9dtb",
					"description":"Sample device","device_key":
					"c31at594ee7958b78f614ce81ec78f49c4137abc","timestamp":1407489757407,
					"type":"Test Device","timezone":"UTC","sensors":[{"name":"AS1","description":
					"An analog sensor from C","sensor_key":"1a7cb3te16f47f38fc3e157e4da2244dteeae8df",
					"type":"digital","timezone":"UTC","data":{"type":"object","properties":{"value":
					 {"type":"number"}}}}]}
 *					 
 *
 * Note				:None
 **************************************************************************************/
void Get_Device_Register_Json() 
{
	memset(DATA, 0x00, 1024*sizeof(char));
	GetTimeStamp();
	startJSON(DATA);

	//First add the device JSON
	putJSONStringAndComma(DATA, "name", Device_Info.Name);
	putJSONStringAndComma(DATA, "gateway_key", Gateway_Info.Key);

	if (Device_Info.Description)
	putJSONStringAndComma(DATA, "description", Device_Info.Description);
	putJSONStringAndComma(DATA, "device_key", Device_Info.Key);
	strcpy(&DATA[strlen(DATA)],"\"timestamp\":");
	sprintf(&DATA[strlen(DATA)],"%d",t_day);
	strcpy(&DATA[strlen(DATA)],"000");
	strcpy(&DATA[strlen(DATA)],",");
	putJSONStringAndComma(DATA, "type", Device_Info.Type);
	putJSONStringAndComma(DATA, "timezone", Device_Info.Timezone);
	strcat(DATA, "\"sensors\":");
	Get_Sensor_Register_Json();
	
	endJSON(DATA);

}

/***************************************************************************************
 * Function			:char* get_analog_data_json(float value) 
 
 * Overview			:Build Json encoded analog value string read from sensors

 *e.g				:{"gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52cabcd",
					"sensor_key":"1a7cb3te16f47f38fc3e157e4da2244dteeaa123","type":
					"DATA","timestamp":1407834013000,"data":{"value": 1.70}}

 *					 
 *
 * Note				:None
 **************************************************************************************/

char* Get_Analog_Data_Json(float value) 
{
	//char* json = malloc(1024 * sizeof(char));
	memset(DATA, 0 ,sizeof(DATA));
	GetTimeStamp();
	startJSON(DATA);

	//First add the device JSON
	putJSONStringAndComma(DATA, "gateway_key", Gateway_Info.Key);
	putJSONStringAndComma(DATA, "sensor_key", Device_Info.Sensors[0].Key);
	putJSONStringAndComma(DATA, "type", "DATA");
	strcpy(&DATA[strlen(DATA)],"\"timestamp\":");
	sprintf(&DATA[strlen(DATA)],"%d",t_day);
	strcpy(&DATA[strlen(DATA)],"000");
	strcpy(&DATA[strlen(DATA)],",");

	char datastr[50];
	memset(datastr, 0 , 50);
	sprintf(datastr, "\"data\":{\"value\": %0.2f}", value);
	strcat(DATA, datastr);
		
	endJSON(DATA);
}

/***************************************************************************************
 * Function			:void Get_Sensor_Heartbeat_Json() 
 
 * Overview			:Build Json encoded sensor heartbeat string

 *e.g 				:{"gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52cabcd","sensor_key"
					:"1a7cb3te16f47f38fc3e157e4da2244dteeaa123","type":"SENSOR_HEARTBEAT",
					"timestamp":1407834240000}
 *					 
 *
 * Note				:None
 **************************************************************************************/
void Get_Sensor_Heartbeat_Json()
{
	memset(DATA, 0 ,sizeof(DATA));

	GetTimeStamp();
	startJSON(DATA);

	//First add the device JSON
	putJSONStringAndComma(DATA, "gateway_key", Gateway_Info.Key);
	putJSONStringAndComma(DATA, "sensor_key", Device_Info.Sensors[0].Key);
	putJSONStringAndComma(DATA, "type", "SENSOR_HEARTBEAT");
	strcpy(&DATA[strlen(DATA)],"\"timestamp\":");
	sprintf(&DATA[strlen(DATA)],"%d",t_day);
	strcpy(&DATA[strlen(DATA)],"000");
	strcpy(&DATA[strlen(DATA)],"}");
	endJSON(DATA);

}

/***************************************************************************************
 * Function			:void Get_Gateway_Heartbeat_Json()  
 
 * Overview			:Build Json encoded gateway heartbeat string

 *e.g               :{"gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52cabcd","type":
					"GATEWAY_HEARTBEAT","timestamp":1407834240000}

 *					 
 *
 * Note				:None
 **************************************************************************************/
void Get_Gateway_Heartbeat_Json() 
{
	memset(DATA, 0 ,sizeof(DATA));

	GetTimeStamp();

	startJSON(DATA);
	putJSONStringAndComma(DATA, "gateway_key", Gateway_Info.Key);
	putJSONStringAndComma(DATA, "type", "GATEWAY_HEARTBEAT");
	strcpy(&DATA[strlen(DATA)],"\"timestamp\":");
	sprintf(&DATA[strlen(DATA)],"%d",t_day);
	strcpy(&DATA[strlen(DATA)],"000");
	strcpy(&DATA[strlen(DATA)],"}");
	endJSON(DATA);
	//free(DATA);


}

///***************************************************************************************
// * Function			:char* Create_Gateway_Access 
// 
// * Overview			:Copy data Acess Key,secret key,URL into structur
//
// * Note				:None
// **************************************************************************************/
//char Create_Gateway_Access(char* Access_Key, char* Secret_Key,
//		char* URL)
//{
//
//
//	if(*Access_Key!=0x00 && *Secret_Key!=0x00 && *URL!=0x00)
//	{
//
//		strcpy(Gateway_Access_Info.Access_Key, Access_Key);
//		strcpy(Gateway_Access_Info.Secret_Key, Secret_Key);
//
//		strcpy(Gateway_Access_Info.URL, URL);
//		return TRUE;
//	}
//	else 
//	return FALSE;
//}
//
//
///***************************************************************************************
// * Function			:Create_Gateway() 
// 
// * Overview			:Copy data related to creating gateway into structure
//
// * Note				:None
// **************************************************************************************/
//char Create_Gateway(char* Key, char* Name, char* Description) 
//{
//
//	strcpy(Gateway_Info.Key, Key);
//	strcpy(Gateway_Info.Name, Name);
//	strcpy(Gateway_Info.Description, Description);
//	if(*Key!=0x00 && *Name!=0x00 && *Description!=0x00)
//	{
//		return TRUE;
//	}
//	else
//		return FALSE;
//}
//
//
///***************************************************************************************
// * Function			:Create_Device() 
// 
// * Overview			:Copy data related to Device into structure
//
// * Note				:None
// **************************************************************************************/
//void Create_Device(char* Key, char* Name, char* Description,char* Type, char* Timezone) 
//{
//
//	strcpy(Device_Info.Key, Key);
//	strcpy(Device_Info.Name, Name);
//	strcpy(Device_Info.Description, Description);
//	strcpy(Device_Info.Type, Type);
//	Device_Info.Num_Sensors = 0;
//	strcpy(Device_Info.Timezone, Timezone);
//
//	
//}
///***************************************************************************************
// * Function			:Create_Analog_Sensor 
// 
// * Overview			:Copy data related to Sensor into structure
//
// * Note				:None
// **************************************************************************************/
//void Create_Analog_Sensor(char* Key, char* Name, char* Description,char* Timezone) 
//{
//	//struct sensor sensor = create_sensor(key, name, description, timezone);
//	strcpy(Device_Info.Sensors[0].Key, Key);
//	strcpy(Device_Info.Sensors[0].Name, Name);
//	strcpy(Device_Info.Sensors[0].Description, Description);
//	strcpy(Device_Info.Sensors[0].Metadata,"{\"type\":\"object\",\"properties\":{\"value\": {\"type\":\"number\"}}}");
//	strcpy(Device_Info.Sensors[0].Timezone, Timezone);
//	strcpy(Device_Info.Sensors[0].Type, "analog");
//	Device_Info.Num_Sensors = Device_Info.Num_Sensors + 1;
//}
//
//
/***************************************************************************************
 * Function			:Register_Gateway()
 
 * Overview			:Send HTTP request to the server for registring Gateway

 * Note				:None
 **************************************************************************************/
void Register_Gateway() 
{

	Get_Gateway_Register_Json();
	uc_Retries=0;
	Build_Remote_URL((BYTE) GATEWAY_REGISTER_URL);			//build the command for parameter
	while(1)
	{
		SendURL((METHOD) POST);				//if it is true 
			
		if(uc_GPRS_Done==TRUE)
		{
			uc_GPRS_Done=FALSE;
			uc_Retries=0;
			asm("NOP");
			break;
		}
		else if(uc_GPRSFailed == TRUE)
		{
			// handel error condition
			uc_GPRS_Done=FALSE;
			uc_GPRSFailed=FALSE;
			uc_Retries++;
			if(uc_Retries>=MAX_RETRIES)
			{
				Reset();
			}
			
		}
		if(uc_GSMRestart==TRUE)
		{
			PowerONGSM(FALSE);
			DelayMs(300);
			uc_GSMRestart=3;	
			PowerONGSM(TRUE);
		}
	}
}

/***************************************************************************************
 * Function			:Register_Device()
 
 * Overview			:Send HTTP request to the server for registring Device

 * Note				:None
 **************************************************************************************/
void Register_Device() 
{
	if (Device_Info.Key && Device_Info.Name && Device_Info.Sensors && Device_Info.Type) 
	{

	    Get_Device_Register_Json();
		Build_Remote_URL((BYTE) GET_DEVICE_REGISTER_URL);	
	
		while(1)
		{
			SendURL((METHOD) POST);				//if it is true 
				
			if(uc_GPRS_Done==TRUE)
			{
				uc_GPRS_Done=FALSE;
				asm("NOP");
				break;
			}
			else if(uc_GPRSFailed == TRUE)
			{
				// handel error condition
				uc_GPRS_Done=FALSE;
				uc_GPRSFailed=FALSE;
				uc_Retries++;
				if(uc_Retries>=MAX_RETRIES)
				{
					Reset();
				}
			}
			if(uc_GSMRestart==TRUE)
			{
				PowerONGSM(FALSE);
				DelayMs(300);
				uc_GSMRestart=3;	
				PowerONGSM(TRUE);
			}
		}
	}

}
//

/***************************************************************************************
 * Function			:Transmit_Analog_Data()
 
 * Overview			:Send data from sensors to the server through HTTP POST request 

 * Note				:None
 **************************************************************************************/
void Transmit_Analog_Data(float value) 
{
	Get_Analog_Data_Json(value);
	Build_Remote_URL((BYTE) GET_DATA_URL);
}

/***************************************************************************************
 * Function			:Transmit_Sensor_Heartbeat()
 
 * Overview			:Transmit sensor heartbeat

 * Note				:None
 **************************************************************************************/
void Transmit_Sensor_Heartbeat()
 {
	 Get_Sensor_Heartbeat_Json();
	Build_Remote_URL((BYTE) GET_SENSOR_HEARTBEAT_URL);

}

/***************************************************************************************
 * Function			:Transmit_Gateway_Heartbeat() 
 
 * Overview			:Transmit sensor heartbeat

 * Note				:None
 **************************************************************************************/
void Transmit_Gateway_Heartbeat() 
{
	 Get_Gateway_Heartbeat_Json(Gateway_Info.Key);
	Build_Remote_URL((BYTE) GATEWAY_HEARTBEAT_URL);

}


/***************************************************************************************
 * Function			:Jason Related
 
 * Overview			:Encode the string into Jason format 
					e.g:{"gateway_key":"9d763f3e7dbf354bea26etdc2ed26tt4a52cabcd","sensor_key":"1a7cb3te16f47f38fc3e157e4da2244dteeaa123","type":"DATA","timestamp":1407843637000,"data":{"value": 5.70}}


 * Note				:None
 **************************************************************************************/

void startJSON(char *buffer) 
{
	strcat(buffer, "{");
}

void startbracket(char *buffer) 
{
	strcat(buffer, "[");
}

void endJSON(char *buffer) 
{
	strcat(buffer, "}");
}

void closebracket(char *buffer)
 {
	strcat(buffer, "]");
}


void putJSONString(char *buffer, char *key, char *value)
 {
	strcat(buffer, "\"");
	strcat(buffer, key);
	strcat(buffer, "\":\"");
	strcat(buffer, value);
	strcat(buffer, "\"");
}


void putJSONStringAndComma(char *buffer, char *key, char *value)
 {
	putJSONString(buffer, key, value);
	strcat(buffer, ",");
}


void putJSONDouble(char *buffer, char *key, double value) 
{

	char doublestr[1024];
	memset(doublestr, 1024, '\0');
	sprintf(doublestr, "\"%s\":%0.0f", key, value);
	strcat(buffer, doublestr);
}

void putJSONDoubleAndComma(char *buffer, char *key, double value)
 {
	putJSONDouble(buffer, key, value);
	strcat(buffer, ",");
}



/***************************************************************************************
 * Function			:Get_Hmac() 
 
 * Overview			:Create HMAC SHA256 key from sceret key & data key
					e.g d73e130022fce2ab49111dac7e93551dbda075000841aada266e353c23518337
 * Note				:None
 **************************************************************************************/

char* Get_Hmac(char* message) 
{

     hmac_sha256 hmac;
     int i;
     char* hmacdigest = malloc(65 * sizeof(char));
     hmac_sha256_initialize(&hmac, Gateway_Access_Info.Secret_Key, strlen(Gateway_Access_Info.Secret_Key));
     hmac_sha256_update(&hmac, message, strlen(message));
     hmac_sha256_finalize(&hmac, NULL, 0);

     char *ptr = hmacdigest;
     for (i = 0; i < 32; ++i) {
         sprintf(ptr, "%02lx", (unsigned long ) hmac.digest[i]);
         ptr += 2;
     }
     hmacdigest[64] = '\0';


	char a[64],j=0;

	for(j=0;j<=63;j++)
	{
		a[j]=*(hmacdigest+j);
	}
	asm("NOP");
     return hmacdigest;
	free(hmacdigest);

}
/***************************************************************************************
 * Function			:InitConnection() 
 
 * Overview			:Create Connection to server and Register gateway & device
					
 * Note				:None
 **************************************************************************************/
void InitConnection()
{
	unsigned int i=0;

//*******************Copy Gateway acess information into structure***************//

	if((strlen(ACCESS_KEY)!=0) && (strlen(SECRET_KEY)!=0) && (strlen(SERVER_URL)!=0))
	{
		for(i=0;i<=strlen(ACCESS_KEY);i++)
		{
			Gateway_Access_Info.Access_Key[i]=ACCESS_KEY[i];
		}
		
		for(i=0;i<=strlen(SECRET_KEY);i++)
		{
			Gateway_Access_Info.Secret_Key[i]=SECRET_KEY[i];
		}
		for(i=0;i<=strlen(SERVER_URL);i++)
		{
			Gateway_Access_Info.URL[i]=SERVER_URL[i];
		}
	}
	else
	{
		while(1);	//error in Gateway Acess info
	}


//*****************Copy gateway information into structure**********************.*//

	if((strlen(GATEWAY_KEY)!=0) && (strlen(GATEWAY_NAME)!=0) && (strlen(GATEWAY_DESCRIPTION)!=0))
	{
		for(i=0;i<=strlen(ACCESS_KEY);i++)
		{
			Gateway_Info.Key[i]=GATEWAY_KEY[i];
		}	
		for(i=0;i<=strlen(GATEWAY_NAME);i++)
		{
			Gateway_Info.Name[i]=GATEWAY_NAME[i];
		}
		for(i=0;i<=strlen(GATEWAY_DESCRIPTION);i++)
		{
			Gateway_Info.Description[i]=GATEWAY_DESCRIPTION[i];
		}

	}
	else
	{
		while(1);		//error in gateway information
	}	

	Register_Gateway();      // register gateway


//*****************Copy Device information into structure***********************//

	if((strlen(DEVICE_KEY)!=0) && (strlen(DEVICE_NAME)!=0) && (strlen(DEVICE_DESCRIPTION)!=0) && (strlen(DEVICE_TIMEZONE)!=0))
	{
		for(i=0;i<=strlen(DEVICE_KEY);i++)
		{
			Device_Info.Key[i]=DEVICE_KEY[i];
		}	
		for(i=0;i<=strlen(DEVICE_NAME);i++)
		{
			Device_Info.Name[i]=DEVICE_NAME[i];
		}
		for(i=0;i<=strlen(DEVICE_DESCRIPTION);i++)
		{
			Device_Info.Description[i]=DEVICE_DESCRIPTION[i];
		}
		for(i=0;i<=strlen(DEVICE_TYPE);i++)
		{
			Device_Info.Type[i]=DEVICE_TYPE[i];
		}
		for(i=0;i<=strlen(DEVICE_TIMEZONE);i++)
		{
			Device_Info.Timezone[i]=DEVICE_TIMEZONE[i];
		}
		Device_Info.Num_Sensors = 0;
	}
	else
	{
		while(1);		//error in Device information
	}	

//*****************Copy Sensor information into structure***********************//


	if((strlen(SENSOR_KEY)!=0) && (strlen(SENSOR_NAME)!=0) && (strlen(SENSOR_DESCRIPTION)!=0) && (strlen(SENSOR_TIMEZONE)!=0) && (strlen(SENSOR_METADATA)!=0))
	{
		for(i=0;i<=strlen(SENSOR_KEY);i++)
		{
			Device_Info.Sensors[0].Key[i]=SENSOR_KEY[i];
		}	
		for(i=0;i<=strlen(SENSOR_NAME);i++)
		{
			Device_Info.Sensors[0].Name[i]=SENSOR_NAME[i];
		}
		for(i=0;i<=strlen(SENSOR_DESCRIPTION);i++)
		{
			Device_Info.Sensors[0].Description[i]=SENSOR_DESCRIPTION[i];
		}
		for(i=0;i<=strlen(SENSOR_TYPE);i++)
		{
			Device_Info.Sensors[0].Type[i]=SENSOR_TYPE[i];
		}
		for(i=0;i<=strlen(SENSOR_TIMEZONE);i++)
		{
			Device_Info.Sensors[0].Timezone[i]=SENSOR_TIMEZONE[i];
		}
		for(i=0;i<=strlen(SENSOR_METADATA);i++)
		{
			Device_Info.Sensors[0].Metadata[i]=SENSOR_METADATA[i];
		}
		Device_Info.Num_Sensors = Device_Info.Num_Sensors + 1;
	}
	else
	{
		while(1);		//error in Device information
	}	
	 
	Register_Device();
}


