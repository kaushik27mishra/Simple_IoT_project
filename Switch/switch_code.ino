/*
  Upload switch data to ThingSpeak
  Hardware: ESP8266 based boards
*/

#include "ThingSpeak.h"
#include "secrets.h"         //file containing wifi and channel details
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;

const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;             //variable storing 1(On) or 0(Off)

void setup() {
  Serial.begin(115200);  // Initialize serial
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
// Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
 if(digitalRead(16)==1){
    number=1;           //Uploads value 1 when switch is On
  }
  if(digitalRead(16)==0){
    number=0;           //Uploads value 0 when switch is Off
  }
  
   // Wait 20 seconds to update the channel again
}
