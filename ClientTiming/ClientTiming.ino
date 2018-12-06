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
bool ledState = LOW;
#define LED_PIN D0
#define INTERRUPT_PIN D8

bool boolDataToSend = LOW;
char dataToSendBuff[5];

WebSocketsClient webSocket;
int status = WL_IDLE_STATUS;
#define USE_SERIAL Serial
long millisDebounce = 0;

void btnInterrupt(){
  if(millis() - millisDebounce > 100 && ledState){
    digitalWrite(LED_PIN, LOW);
    ledState = LOW;
    uint32_t millisTime = millis() - millisForTiming;
    if(millisTime < 0 || millisTime > 0xFFFFFF){
      millisTime = 0xFFFFFF;
    }
    char dataToSend[5];
    dataToSend[0] = 84; //T for timing
    dataToSend[1] = millisTime & 0x7F;//Map first 8 (lsb) bits of the timing to first char
    dataToSend[2] = (millisTime & 0x3F80) >> 7;
    dataToSend[3] = (millisTime & 0x1FC000) >> 14;
    dataToSend[4] = NULL;
    Serial.println(millisTime);
    //Serial.println(dataToSend);
    int i;
    for(i = 0; i < 5; i++){
      dataToSendBuff[i] = dataToSend[i];
    }
    boolDataToSend = HIGH;
    //webSocket.sendTXT(dataToSend);
  }
  millisDebounce = millis();
  
}

void handleInput(uint8_t * payload){
  Serial.println(*payload);
  switch(*payload){
    case 76://O
      digitalWrite(LED_PIN, HIGH);
      ledState = HIGH;
      millisForTiming = millis();
      //Serial.println("LEDON");
      break;
      /*
  case 79://L
      digitalWrite(LED_PIN, LOW);
      Serial.println("LEDOFF");
      break;
      */
  default:
      break;
  }
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
            //Serial.println((char)*payload);
            handleInput(payload);

      // send message to server
      // webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }

}

void setup() {
    pinMode(INTERRUPT_PIN, INPUT);
    attachInterrupt(INTERRUPT_PIN, btnInterrupt, RISING);
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
