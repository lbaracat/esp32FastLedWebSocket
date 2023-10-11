#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "cred.h"

WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

//String webpage = "<html><head></head><body><h1>-=( I'm on! )=-</h1><p>Take a random number:&nbsp;<span id='rand'>-</span></p></body><script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function processCommand(n){document.getElementById('rand').innerHTML=n.data,console.log(n.data)}window.onload=function(n){init()}</script></html>";
String webpage = "<html><head></head><body><h1>-=( I'm on! )=-</h1><p>Take a random number:&nbsp;<span id='rand'>-</span></p><p><button type='button' id='btnSendBack'>Send info</button></p></body>\
<script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function processCommand(n){document.getElementById('rand').innerHTML=n.data,console.log(n.data)}\
function btnSendBack(){Socket.send('Sending back some random stuff')}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script></html>";

// Variables for timer check
unsigned long interval = 0; // in milliseconds
unsigned long previousMillis = 0;
unsigned long now = 0;
// run initTimer() inside setup

char buffer[33];

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ON.");
 
  initWiFi();
  initWebComponents();
  initTimer(1000);
}
 
void loop() {
  webServer.handleClient();
  webSocket.loop();

  now = millis();
  if (now - previousMillis > interval) {
    itoa(random(100), buffer, 10);
    webSocket.broadcastTXT(buffer);
    previousMillis += interval;
  }
}

/*
 * Helper functions below
 */

// initWifi connects to wifi network specified in cred.h
void initWiFi() {
  WiFi.begin(SSID, PASS);
  Serial.println("Establishing connection to WiFi with SSID: " + String(SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.print(" CONNECTED!\n");
  delay(4000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// initWebComponents initializes webserver and websockets endpoints
void initWebComponents() {
  webServer.on("/", []() {
    webServer.send(200, "text/html", webpage);
  });
  webServer.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void initTimer(int i) {
  interval = i;
  previousMillis = millis();
  now = millis();
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      for (int i=0; i<length; i++) {                  // print received data from client
        Serial.print((char)payload[i]);
      }
      Serial.println("");
      break;
  }
}