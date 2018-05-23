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
#include <SPI.h>        // Pour communiquer avec le shield Ethernet
#include <Ethernet.h>   // Pour la partie Ethernet, evidemment !
#include <IPStack.h>    // Permet de gérer la couche IP
#include <Countdown.h>  // Timer utilisé par le protocole MQTT
#include <MQTTClient.h> // Permet de gérer le protocole MQTT
//88:63:df:9d:91:b3
const int led = 14; // pin de la LED
  //Setup MQTT
  EthernetClient c; // remplacez par un YunClient si vous utilisez Yun
  IPStack ipstack(c);
  MQTT::Client<IPStack, Countdown, 50, 1> client = MQTT::Client<IPStack, Countdown, 50, 1>(ipstack);

  byte mac[] = { 0x88, 0x63, 0xdf, 0x9d, 0x91, 0xb3 };  // remplacer par l'adresse MAC de votre shield ethernet
  const char* topic = "LEDArduino"; // le topic utilisé pour communiquer
void setup()
{

 Serial.begin(9600);
 Ethernet.begin(mac);
 connect();
 
  /*
  Serial.begin(115200);
  Serial.println();

  //Setup Wifi
  WiFi.begin("iPhone de Julien", "julien1234");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());  

 pinMode(led, OUTPUT);
 digitalWrite(led, HIGH); // on initialise la LED en l'éteignant
 
 Serial.begin(9600);
 Ethernet.begin(mac);

 connect();
  */
}

void loop() 
{
  if (!client.isConnected())
   connect();

  delay(500);
  client.yield(1000); // permet la reception des messages
}

void messageArrived(MQTT::MessageData& md)
{
 MQTT::Message &message = md.message;
   
 char* msg = (char*) message.payload; // on recupere le message
 msg[message.payloadlen] = 0; // indique la fin du char*
 
 if (strcmp(msg, "ON") == 0) // on a reçu le message "ON"
   digitalWrite(led, LOW);   // on allume la LED
 else if (strcmp(msg, "OFF") == 0) // on a reçu le message "OFF"
   digitalWrite(led, HIGH);        // on éteint la LED
}

void connect()
{
 char hostname[] = "172.20.10.14"; // IP où se trouve le broker MQTT
 int port = 1896; // port utilisé par le broker

 int rc = ipstack.connect(hostname, port);
 if (rc != 1)
 {
   Serial.print("rc from TCP connect is ");
   Serial.println(rc);
 }

 Serial.println("MQTT connecting");
 MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
 data.MQTTVersion = 3;
 data.clientID.cstring = (char*)"arduino-id";
 rc = client.connect(data);
 if (rc != 0)
 {
   Serial.print("rc from MQTT connect is ");
   Serial.println(rc);
 }
 Serial.println("MQTT connected");
 
 rc = client.subscribe(topic, MQTT::QOS0, messageArrived);    // le client souscrit au topic
 if (rc != 0)
 {
   Serial.print("rc from MQTT subscribe is ");
   Serial.println(rc);
 }
 Serial.println("MQTT subscribed");
}

