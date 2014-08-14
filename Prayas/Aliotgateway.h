
#ifndef ALIOTGATEWAY_H_
#define ALIOTGATEWAY_H_

#define SENSOR_SIZE	512



typedef struct  {
	char Access_Key[64];
	char Secret_Key[64];
	char URL[256];
	char Message[1024];
}Gateway_Access;

//The model objects in the system

typedef struct  {
	char Key[64];
	char Name[64];
	char Description[256];
}Gateway;

typedef struct  {
	char Key[64];
	char Name[64];
	char Type[64];
	char Description[256];
	char Metadata[256];
	char Timezone[64];
}Sensor ;

//Represents a device. A device has name, description and a list of
//sensors registered with it.
typedef struct  {
	char Key[64];
	char Name[64];
	char Type[64];
	char Description[256];
	char Timezone[64];
	Sensor Sensors[1];
	int Num_Sensors;
}Device;


//char Create_Gateway_Access(char* Access_Key, char* Secret_Key,char* URL);					
//char Create_Gateway(char* Key, char* Name, char* Description);
void Get_Gateway_Register_Json();
//void Create_Device(char* Key, char* Name, char* Description,char* Type, char* Timezone);
//void Create_Analog_Sensor(char* Key, char* Name, char* Description,char* Timezone);
void Register_Gateway();
void Register_Device();
void Transmit_Analog_Data(float value);
void Transmit_Sensor_Heartbeat();
void Transmit_Gateway_Heartbeat();


void startJSON(char *buffer);
void endJSON(char *buffer);
void startbracket(char *buffer);
void closebracket(char *buffer);

void putJSONString(char *buffer, char *key, char *value);

void putJSONStringAndComma(char *buffer, char *key, char *value);


void putJSONDouble(char *buffer, char *key, double value);

void putJSONDoubleAndComma(char *buffer, char *key, double value);
#endif /* ALIOTGATEWAY_H_ */
