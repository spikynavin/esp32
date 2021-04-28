#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int received;// received value will be stored in this variable
char receivedChar;// received value will be stored as CHAR in this variable

const char turnON ='1';
const char turnOFF ='0';
const int relay = 4;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Smart_iot"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!"); 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); //initial state
}

void loop() {
    receivedChar =(char)SerialBT.read();
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    if(receivedChar == turnON)
    {
     //SerialBT.println("RELAY ON:");// write on BT app
     digitalWrite(relay, LOW);// turn the LED ON
    }
    if(receivedChar == turnOFF)
    {
     //SerialBT.println("RELAY OFF:");// write on BT app
      digitalWrite(relay, HIGH);// turn the LED off 
    }    
  }
  delay(20);
}
