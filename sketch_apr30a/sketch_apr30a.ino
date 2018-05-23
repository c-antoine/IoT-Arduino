/*
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>

#include <dummy.h>
#include <SPI.h>        // Pour communiquer avec le shield Ethernet
//#include <Ethernet.h>   // Pour la partie Ethernet, evidemment !
#include <IPStack.h>    // Permet de gérer la couche IP
#include <Countdown.h>  // Timer utilisé par le protocole MQTT
#include <MQTTClient.h> // Permet de gérer le protocole MQTT
*/


#include <CircularLED.h>
CircularLED circularLED1(2,9);
CircularLED circularLED1;
unsigned int LED[24];
int k=0;
void setup() 
{

  Serial.begin(9600);
//  circularLED1.init();
}
void loop() 
{
  for (int i =0;i<24;i++)
  {
    LED[i]=0;
  }
    LED[k]= 0xff;
    k++;
    if (k==24)
    k=0;
    
  circularLED1.CircularLEDWrite(LED);
//  circularLED2.CircularLEDWrite(LED);
  delay(40);  
}
