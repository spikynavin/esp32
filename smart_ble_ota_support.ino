/**************************************
* Smart Ble switch with OTA support
* 29/04/2021 11:45AM
*
* to turn wifi and Bluetooth off:
* WiFi.mode(WIFI_OFF);
* btStop();
***************************************/

#include <WiFi.h>
#include <AsyncTcp.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enable! run menuconfig to enable it
#endif

int pin = 4; //control module interept I/0 pin
const char received_char;
const char turn_on = '1';
const char turn_off = '0';
const char ota_on = 'u';

const char* ssid = "network_ssid_name";
const char* password = "network_pass";

BluetoothSerial SerialBT;
AsyncWebServer server(80);

//static ip address for OTA Server
IPAddress local_IP(192.168.0.200);
IPAddress gateway(192.168.0.1);
IPAddress subnetmask(255, 255, 0, 0);

void setup(){
	Serial.begin(115200);
	SerialBT.begin("Smart_ble");
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void wifi_ota_setup(){
	//config static ip address
	if (!WiFi.config(local_IP, gateway, subnetmask)){
		Serial.println("STA Failed to config");
	}
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	//Wait for connection
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
	}
	AsyncElegantOTA.begin(&server);
	server.begin();
}

void loop(){
	AsyncElegantOTA.loop();
	if (received_char = (char) SerialBT.read()){
		if (Serial.available){
			SerialBT.write(Serial.read);
		}
		if (received_char == turn_on){
			digitalWrite(pin, LOW);
		}
		if (received_char == turn_off){
			digitalWrite(pin, HIGH);
		}
		if (received_char == ota_on){
			//OTA Function call for update code
			btStop();//bluetooth turn off;
			wifi_ota_setup();
		}
	}
	delay(20);
}
