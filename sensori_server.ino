/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  posto librerie
  C:\Users\adewa\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\libraries\WiFi
*/

// Import required libraries
//#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WiFi.h>
#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

// Set your access point network credentials
const char* ssid = "TP-LINK_F552";
const char* password = "23395985";

// set ip address
IPAddress local_IP(192, 168, 0, 21);
IPAddress gateway(192, 168, 01, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
#define inpin 1
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
/*
String readTemp() {
  return String(bme.readTemperature());
  //return String(1.8 * bme.readTemperature() + 32);
}

String readHumi() {
  return String(bme.readHumidity());
}

String readPres() {
  return String(bme.readPressure() / 100.0F);
}
*/

  char ttext='3';
  
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
   pinMode(inpin, INPUT);    // sets the digital pin 4 led pin as output
  
  // Setting the ESP as an access point
  //Serial.print("Setting AP (Access Point)…");
  Serial.print("Adesso mi connetto al router");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  /*WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);*/
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");}

  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
//    WiFi.config(ip);//non usato i n esp32
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  //ricezione richiesta
   server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
    if(digitalRead(inpin)==LOW)
      ttext='0';
    if(digitalRead(inpin)==HIGH)
      ttext='1';      
    request->send_P(200, "text/plain", &ttext);
  });
  /*
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
  
  bool status;
/*
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  */
  // Start server
  server.begin();
}
 
void loop(){
  
}
