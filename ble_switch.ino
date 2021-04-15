/*********
  NAVEEN RAJ
  Smart Ble control switch
*********/

// Load libraries
#include "BluetoothSerial.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "your router name";
const char* password = "your router pass";

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int relayPin =  2;

// Handle received and sent messages
String message = "";
char incomingChar;

AsyncWebServer server(80);

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
 
  //WiFi Station mode enabled
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("The device started, now you can connect with wifi!")
  
  //Wait for connection
  while (WIFI.status() != WL_CONNECTED) {
	  delay(500);
	  Serial.println(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  // Bluetooth device name
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
	AsyncElegantOTA.loop();
  // Read received messages (LED control command)
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  }
  // Check received message and control output accordingly
  if (message =="relay_on"){
    digitalWrite(relayPin, HIGH);
  }
  else if (message =="relay_off"){
    digitalWrite(relayPin, LOW);
  }
  delay(20);
}
