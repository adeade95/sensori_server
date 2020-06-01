
//impostazione router casa nuova piano superiore
const char* ssid = "TP-LINK_FF52";
const char* password = "23395985";
#include "WiFi.h" // ESP32 WiFi include
#include "ESPAsyncWebServer.h"// pre creare server
//non funziona l'impostazione ip
// set ip address
IPAddress local_IP(192, 168, 0, 21);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

#define inpin 16 // pin 1 come ingresso

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String ttext="3"; //definizione variabile per lo stato dell'ingresso

void ConnectToWiFi()
{
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to "); Serial.println(ssid);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());

    Serial.println("risetto indirizzo ip");
WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);

    Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}


void setup() 
{
  Serial.begin(115200);
 
  ConnectToWiFi();
  /*
  // Create AsyncWebServer object on port 80
  Serial.println("creazione server su porta 80");
  AsyncWebServer server(80);
  Serial.println("creato server su porta 80");*/
  Serial.println("settaggio pin inpin input");
  pinMode(inpin, INPUT);    // sets the digital pin 4 led pin as output
  digitalWrite(inpin, HIGH);// pull up alto
  Serial.println("settato inpin input");

    //ricezione richiesta
   server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("ricevuta richiesta");   
    if(digitalRead(inpin)==LOW){
      Serial.println("stato basso"); 
      ttext="0";}
    if(digitalRead(inpin)==HIGH){
      Serial.println("stato alto"); 
      ttext="1";}
    request->send_P(200, "text/plain", ttext.c_str());
  });

  server.begin(); //inizializzazione server
}
void loop(){}
