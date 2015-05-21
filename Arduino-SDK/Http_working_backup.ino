      /*
        
        ARDUINO-SDK
        
        HTTP Post Request
        using an Arduino Wiznet Ethernet shield.      
      
       created 7 May 2015
       by Naman Jain
       */
      
      #include <SPI.h>
      #include <Ethernet.h>
      #include <sha256.h>
      #include <EthernetUdp.h>
      //#include <Time.h>
      //#include <Arduino.h>
      
      
      #define SECRET_KEY "e26eeea2df24bctta878557t4t651d8a3t3feefe"
      //#define HOST_URL "datonis.altizon.com"
      #define ACCESS_KEY "fe1d2fc9tbt23ebf84c6c88d88f83e3dd98b2et3"
      #define SENSOR_KEY "51tdda331f1dca11252f6ab63175caa844t4bf54"
      
      // Enter a MAC address for your controller below.
      byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
      
      IPAddress server(54, 152, 59, 3); // numeric IP for Datonis (no DNS)
      //char server[]  ="www.datonis.altizon.com";    // name address for Datonis (using DNS)
      
      // Set the static IP address to use if the DHCP fails to assign
      IPAddress ip(192, 168, 2, 177);
      
      // Initialize the Ethernet client library
      // with the IP address and port of the server
      // that you want to connect to (port 80 is default for HTTP):
      EthernetClient client;
      
      unsigned long epoch_time;
      //unsigned long long mill_sec;
      
      unsigned int localPort = 8888;       // local port to listen for UDP packets
      
      char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server
      
      const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
      
      byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
      
      // A UDP instance to let us send and receive packets over UDP
      EthernetUDP Udp;
      
      /***************************************** System Functions ****************************************/
      
      // First function to be called in the sketch. Executed only once. 
      void setup() {
        // Open serial communications and wait for port to open:
        Serial.begin(57600); //57600 is the baud rate
        while (!Serial) {
          ; // wait for serial port to connect.
        }
      
        // start the Ethernet connection:
        if (Ethernet.begin(mac) == 0) {
          Serial.println("Failed to configure Ethernet using DHCP");
      
          // try to congifure using IP address instead of DHCP:
          Ethernet.begin(mac, ip);
        }
      
        Udp.begin(localPort);
        // give the Ethernet shield a second to initialize:
        delay(1000);
        Serial.print("Connecting...\r\nLocal Ip= ");
       
        Serial.println(Ethernet.localIP());
      
      
        //For Creating a new Sensor
        //String jsonString = "{\"sensor\":{\"name\":\"sensor-naman\", \"description\":\"testapi\", \"type\":\"analog\"}}";
      
        // For Datonis SignIn
        //String jsonString = "{\"email\":\"oee-demo@altizon.com\",\"password\":\"oee12345\"}";
      
        // For Sensor_Heartbeat
        //String jsonString = "{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1431436786000}";
      
        //For Registering a Sensor
        //String jsonString =  "{\"name\":\"Press machine sensor 2\",\"description\":\"Press machine sensor 2\",\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"type\":\"digital\",\"data\":{\"type\":\"object\",\"properties\":{\"value\": {\"type\":\"number\"}}}}"; // For Sensor Register
      
        //For Sensor Event
        // String jsonString =  "{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1431436600000,\"data\":{\"value\": 1}}";
      
      
      
        //mill_sec= (epoch_time*1000);
      
      
        //      String json_heartbeat = "{\"sensor_key\":\"";
        //      json_heartbeat += SENSOR_KEY;
        //      json_heartbeat += "\",\"timestamp\":";
        //      json_heartbeat += epoch_time;
        //      json_heartbeat += "}";
        //Serial.println(json_heartbeat);
      
      
      
      
      
        //------------------------------Avoid this
      
        //            String post_heartbeat="POST /api/v1/sensor/heartbeat.json HTTP/1.1\r\nHost: ";
        //            post_heartbeat += HOST_URL;
        //            post_heartbeat += "\r\nAccept: */*\r\n";
        //            post_heartbeat += "X-Access-Key: ";
        //            post_heartbeat += ACCESS_KEY;
        //            post_heartbeat += "\r\nX-Dtn-Signature: ";
        //            post_heartbeat += hash_buffer;
        //            post_heartbeat += "\r\nContent-Type: application/json";
        //            post_heartbeat += "\r\nContent-Length: 100";
        //            post_heartbeat += "\r\nCache-Control: no-cache\r\n\r\n";
        //            post_heartbeat += "{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1432037400000}";
        // post_heartbeat += json_heartbeat;
        //
        //     // Serial.println(json_heartbeat);
        //
        //    Serial.println(post_heartbeat);
      
      
      
        //For Sensor Heartbeat
        // sprintf(json,"{\"sensor_key\":\"%s\",\"timestamp\":%lu%s}",SENSOR_KEY,epoch_time,"000");
      
        //For Sensor Event
        //sprintf(json,"{\"sensor_key\":\"%s\",\"timestamp\":%lu%s,\"data\":{\"value\": %d}}",SENSOR_KEY,epoch_time,"000",sensor_data);
      
      
      
      }
      
      //This funtion keeps on executing infinitely. 
      void loop() {
        
        //Send 3 events to Datonis
        for (int i = 0; i < 3; i++) {
          connectAndRead(); //connect to the server and read the output
          readResponse();
      
          delay(1000); 
        }
      
        while (true); //Do nothing
      
      }
      
      
      /******************************* User Defined Functions ******************************/
      
      void connectAndRead() {
      
        /******* Create a String for Sensor Heartbeat here *******/
        //      char *sensor_heartbeat;
        //      sensor_heartbeat = send_heartbeat();
        //      Serial.println("Sending a Heartbeat \r\n");
        //      Serial.println(sensor_heartbeat);
      
      
      
        /******* Create a String for Sensor Event  here ********/
      
        char *sensor_event;
        byte event=0;
        byte heartbeat=1;
        sensor_event = send_event(event); //For sending  a heartbeat, send "heartbeat" here. 
        
        Serial.println(sensor_event);
        
      
        // -----------------------//Working Code!! Hurray ;)
      
        
      
        // if you get a connection, report back via serial:
        if (client.connect(server, 80)) {
          Serial.println("\n----Connected----\r\n");
          
          //Serial.println("sending request");
      
          // Make a HTTP request:
      
          //client.println(sensor_heartbeat);      /***  For Sending heartbeat to the server ******/
      
          client.println(sensor_event);           /***  For Sending Event to the server ******/
      
      
          //Create a Sensor --> Working, Verified
          //client.println("POST /api/v1/sensors HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Auth-Token: zRaqmnpOmFsszXDbyT3HpQ\r\nContent-Type: application/json\r\nContent-Length: 80\r\nCache-Control: no-cache\r\n\r\n{\"sensor\":{\"name\":\"sensor-naman\", \"description\":\"testapitst\", \"type\":\"digital\"}}");
      
          //Sign-In to Datonis  ---> Working, Verified
          //client.println("POST /api_sign_in HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nContent-Type: application/json\r\nContent-Length: 56\r\n\r\n{\"email\":\"oee-demo@altizon.com\",\"password\":\"oee12345\"}");
      
          //Send a Sensor Event --> Working, verified
          //client.println("POST /api/v1/sensor/event.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: fe1d2fc9tbt23ebf84c6c88d88f83e3dd98b2et3\r\nX-Dtn-Signature: cf9381514d45fc11054cdde3ba5447cb5cefe473d95f6c954a0292aa3bb831ee\r\nContent-Type: application/json\r\nContent-Length: 103\r\nCache-Control: no-cache\r\n\r\n{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1431502200000,\"data\":{\"value\": 5}}");
      
          //Send a Sensor Heartbeat --> Working, verified
          // client.println("POST /api/v1/sensor/heartbeat.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: fe1d2fc9tbt23ebf84c6c88d88f83e3dd98b2et3\r\nX-Dtn-Signature: 0f8089608eaa9b306aee977267d64770c87579e5670ae6f19376b5dcaae16979\r\nContent-Type: application/json \r\nContent-Length: 83\r\nCache-Control: no-cache\r\n\r\n{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1431671200000}");
      
          //Register a Sensor --> Working, verified
          //client.println("POST /api/v1/sensor/register.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: fe1d2fc9tbt23ebf84c6c88d88f83e3dd98b2et3\r\nX-Dtn-Signature: d45021497adc60bf44b6ae37421fc00706b199113588ce7964c4c667d251f93b\r\nContent-Type: application/json \r\nContent-Length: 212\r\nCache-Control: no-cache\r\n\r\n{\"name\":\"Press machine sensor 2\",\"description\":\"Press machine sensor 2\",\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"type\":\"digital\",\"data\":{\"type\":\"object\",\"properties\":{\"value\": {\"type\":\"number\"}}}}");
      
          //client.println("POST /api/v1/sensor/heartbeat.json?X-Access-Key=fe1d2fc9tbt23ebf84c6c88d88f83e3dd98b2et3&X-Dtn-Signature=84835f0bc6a6931a00208f3ba9f255e1b0372e6c4d5360cb7198d7402ae8ce57 HTTP/1.1\r\nHost: datonis.altizon.com\r\nContent-Type: application/json\r\nContent-Length: 86\r\nCache-Control: no-cache\r\n\r\n{\"sensor_key\":\"51tdda331f1dca11252f6ab63175caa844t4bf54\",\"timestamp\":1431495700000}");
      
      
          //Serial.println("request completed ");
        }
        else {
          // if you didn't get a connection to the server:
          Serial.println("\nXXXX Connection failed XXXX");
        }
      
      }
      
      
      void readResponse() {
      
        // if there are incoming bytes available
        // from the server, read them and print them:
        //while(client.connected())
        while (true) {
          if (client.available()) {
            //Serial.print("reading server");
            char c = client.read();
            Serial.print(c);
          }
          // if the server's disconnected, stop the client:
          if (!client.connected()) {
            Serial.println();
            Serial.println("**** Disconnecting..****");
            client.stop();
            break;
      
            // do nothing forevermore:
            // while (true);
          }
        }
      
      }
      
      
      void printHash(uint8_t* hash, char buf[]) {
        int i;
        int j = 0;
        for (i = 0; i < 32; i++) {
          buf[j++] = "0123456789abcdef"[hash[i] >> 4];
          buf[j++] = "0123456789abcdef"[hash[i] & 0xf];
        }
        Serial.println();
      }
      
      
      
      
      unsigned long get_epoch_time() {
        sendNTPpacket(timeServer); // send an NTP packet to a time server
      
        // wait to see if a reply is available
        delay(1000);
        if ( Udp.parsePacket() ) {
          // We've received a packet, read the data from it
          Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      
          //the timestamp starts at byte 40 of the received packet and is four bytes,
          // or two words, long. First, esxtract the two words:
      
          unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
          unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
          // combine the four bytes (two words) into a long integer
          // this is NTP time (seconds since Jan 1 1900):
          unsigned long secsSince1900 = highWord << 16 | lowWord;
          //Serial.print("Seconds since Jan 1 1900 = " );
          //Serial.println(secsSince1900);
      
          // now convert NTP time into everyday time:
          //Serial.print("Unix time = ");
          // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
          const unsigned long seventyYears = 2208988800UL;
          // subtract seventy years:
          unsigned long epoch = secsSince1900 - seventyYears;
          // print Unix time:
          //Serial.println(epoch);
          return epoch;
        }
      
      }
      
      // send an NTP request to the time server at the given address
      unsigned long sendNTPpacket(char* address)
      {
        // set all bytes in the buffer to 0
        memset(packetBuffer, 0, NTP_PACKET_SIZE);
        // Initialize values needed to form NTP request
        // (see URL above for details on the packets)
        packetBuffer[0] = 0b11100011;   // LI, Version, Mode
        packetBuffer[1] = 0;     // Stratum, or type of clock
        packetBuffer[2] = 6;     // Polling Interval
        packetBuffer[3] = 0xEC;  // Peer Clock Precision
        // 8 bytes of zero for Root Delay & Root Dispersion
        packetBuffer[12]  = 49;
        packetBuffer[13]  = 0x4E;
        packetBuffer[14]  = 49;
        packetBuffer[15]  = 52;
      
        // all NTP fields have been given values, now
        // you can send a packet requesting a timestamp:
        Udp.beginPacket(address, 123); //NTP requests are to port 123
        Udp.write(packetBuffer, NTP_PACKET_SIZE);
        Udp.endPacket();
      }
      
      
      
      
      void get_hash(char buf[], char json_buf[]) {
        uint8_t *hash;
        Sha256.initHmac((const unsigned char*)SECRET_KEY, strlen(SECRET_KEY)); // key, and length of key in bytes
      
        Sha256.print(json_buf);
        //Serial.println(json_buf);
        //hash = Sha256.resultHmac();
      
        printHash(Sha256.resultHmac(), buf);
        buf[64] = '\0';
        return;
      }
      
//      char* send_heartbeat() {
//        char array[320];
//        char hash_buffer[65];
//        char json[128];
//      
//        epoch_time = get_epoch_time();
//        // Serial.println(epoch_time);
//      
//        //For Sensor Heartbeat
//        sprintf(json, "{\"sensor_key\":\"%s\",\"timestamp\":%lu%s}", SENSOR_KEY, epoch_time, "000");
//        get_hash(hash_buffer, json);
//      
//        //For Sensor Heartbeat
//      
//        sprintf(array, "POST /api/v1/sensor/heartbeat.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: %s\r\nX-Dtn-Signature: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nCache-Control: no-cache\r\n\r\n%s", ACCESS_KEY, hash_buffer, strlen(json), json);
//      
//        return array;
//      
//      
//      }
      
      char* send_event(byte n) {
        char array[320];
        char hash_buffer[65];
        char json[128];
        int sensor_data = 10;
    
        epoch_time = get_epoch_time();
        // Serial.println(epoch_time);
      
        if(n==0){
        //For Sensor Event
        Serial.println("Sending an Event \r\n");
        
        sprintf(json, "{\"sensor_key\":\"%s\",\"timestamp\":%lu%s,\"data\":{\"value\": %d}}", SENSOR_KEY, epoch_time, "000", sensor_data);
        get_hash(hash_buffer, json);
        sprintf(array, "POST /api/v1/sensor/event.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: %s\r\nX-Dtn-Signature: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nCache-Control: no-cache\r\n\r\n%s", ACCESS_KEY, hash_buffer, strlen(json), json);
        }
        
        /***** Uncomment this if u want to send a heartbeat *********/
        
//        if(n==1){
//        Serial.println("Sending a Heartbeat \r\n");  
//        //For Sensor Heartbeat
//        sprintf(json, "{\"sensor_key\":\"%s\",\"timestamp\":%lu%s}", SENSOR_KEY, epoch_time, "000");
//        
//        get_hash(hash_buffer, json);
//        
//        sprintf(array, "POST /api/v1/sensor/heartbeat.json HTTP/1.1\r\nHost: datonis.altizon.com\r\nAccept: */*\r\nX-Access-Key: %s\r\nX-Dtn-Signature: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nCache-Control: no-cache\r\n\r\n%s", ACCESS_KEY, hash_buffer, strlen(json), json);
//        }
    
        return array;
      
      
      }
      
      
      
      
      

