
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
WiFiClient  client;

// Counting channel details
unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
unsigned long counterChannelNumber2 = SECRET_CH_ID_COUNTER2;
const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;
const char * myCounterWriteAPIKey = SECRET_WRITE_APIKEY_COUNTER;
unsigned int counterFieldNumber = 1; 
float temp;

  boolean lastButton = LOW;
  boolean currentButton = LOW;
  boolean ledOn = false;

void setup() {
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(0,OUTPUT);
  pinMode(A0,INPUT);
}
  
  boolean debounce(boolean last) //function for debounce
{
  boolean current = ThingSpeak.readIntField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);
  if (last != current)
  {
    delay(5);
    current = ThingSpeak.readIntField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);
  }
  return current;
}

void loop() {

 int statusCode = 0;
  

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }


  // variable shows temprature
  temp=analogRead(A0)*3.3*100/1024;
  
  // To upload the temp variable on ThingSpeak 
  ThingSpeak.writeField(counterChannelNumber2, 1, temp, myCounterWriteAPIKey);  

  // To retrieve the data from ThingSpeak in digital format
  int count = ThingSpeak.readIntField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);  

   // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200)
  {
    Serial.println("Counter: " + String(count));
  }
  else
  {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
   
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH)
  {
    ledOn = !ledOn;
  }
  
  lastButton = currentButton;
  
  digitalWrite(count, ledOn);
}
