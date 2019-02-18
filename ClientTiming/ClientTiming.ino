/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
long millisForTiming = 0;
#include <Hash.h>
#include <Ticker.h>
bool ledState = LOW;
#define LED_PIN D0
#define INTERRUPT_PIN D8

bool boolDataToSend = LOW;
char dataToSendBuff[5];

WebSocketsClient webSocket;
int status = WL_IDLE_STATUS;
#define USE_SERIAL Serial
long millisDebounce = 0;

uint8_t dataReceived = 0;

Ticker counterWDT;

void handleInput(uint8_t* payload, size_t lenght){
  //Data enters as: Main, sub, left btn, middle btn, right btn
  analogWrite(D0, payload[0]);
  if(lenght == 5){
    dataReceived = 1;
  }
}

void WDTInterrupt(){
  if(!dataReceived){
    analogWrite(D0, 0);
  }
  dataReceived = 0;
}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            //USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);
        
          // send message to server when Connected
        webSocket.sendTXT("Conn_succesful");
            }
            break;
        case WStype_TEXT:
            //USE_SERIAL.printf("[WSc] get text: %s\n", payload);
            //webSocket.sendTXT("Conn_succesful");
            

      // send message to server
      // webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            //Data enters as: Main, sub, left btn, middle btn, right btn
            hexdump(payload, lenght);
            handleInput(payload, lenght);
            

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }

}

void setup() {
    pinMode(INTERRUPT_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }


    connectWifi();
    printWifiStatus();

    counterWDT.attach(1.1, WDTInterrupt);
    
      /*
    //WifiMulti.ApListClean();
    WiFiMulti.addAP("ESPap", "thereisnospoon");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
    */
    
    webSocket.begin(WiFi.gatewayIP().toString(), 80);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);

}



void loop() {
    webSocket.loop();
    if(boolDataToSend){
      webSocket.sendTXT(dataToSendBuff);
      boolDataToSend = LOW;
    }
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
      
       // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) { 
      Serial.print("Attempting to connect to SSID: ");
      Serial.println("ESPap");
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
      status = WiFi.begin("ESPap", "thereisnospoon");
    
      // wait 10 seconds for connection:
      delay(10000);
    } 
    Serial.println("Connected to wifi");
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
