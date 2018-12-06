/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

int millisStore = 0;
bool ledOn = LOW;

Ticker potentiometer;

int clients = 0;
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

/*
 * MESSAGE CODES:
 * LEDON: L
 * Officially connected: O
 * Number following: T
 * 
 */

WebSocketsServer webSocket = WebSocketsServer(80);

#define USE_SERIAL Serial

void newLedHandler(){
  webSocket.sendTXT(random(clients), "L");
}

void handleInput(uint8_t * payload, uint8_t number){
  uint32_t millisInp = 0;
  switch(*payload){
    case 84:
      millisInp = *(payload +1);
      millisInp += *(payload +2) << 7;
      millisInp += *(payload +3) << 14;
      newLedHandler(); //Let the next led light up!
      USE_SERIAL.printf("[%u] %u\n", number, millisInp);
      break;

    default:
      break;
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            clients--;
            break;
        case WStype_CONNECTED:
            {
                Serial.println(num);
                clients++;
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                if(num == 0){
                  webSocket.sendTXT(num, "L");
                }
            }
            break;
        case WStype_TEXT:
            //USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            handleInput(payload, num);
            // send message to client
            //webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}

void sendPotval(){
    char potVal[] = { 0, 0, 0, 0 };
    itoa(analogRead(A0), potVal, 16);
    webSocket.broadcastTXT(potVal);
}

void setup() {
    // USE_SERIAL.begin(921600);
    potentiometer.attach(0.02, sendPotval);
    
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

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    
    if(MDNS.begin("esp8266")) {
        USE_SERIAL.println("MDNS responder started");
    }
    //Serial.println(WiFi.localIP);
    MDNS.addService("ws", "tcp", 80);
}

void loop() {
    webSocket.loop();

    
    //webSocket.sendTXT(num, "message here");
    /*
    if(millis() - millisStore > 1000){
      if(ledOn){
        webSocket.broadcastTXT("O");
        ledOn = LOW;
      }
      else{
        webSocket.broadcastTXT("L");
        ledOn = HIGH;
      }
      millisStore = millis();
    }
    */
}
