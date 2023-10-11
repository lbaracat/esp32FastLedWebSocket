#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "cred.h"

#define DEBUG false

WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/*
String webpage = "<!DOCTYPE html><html lang='en'><head></head><body><h1>-=( I'm on! )=-</h1><p>Take a random number:&nbsp;<span id='rand'>-</span></p><p><button type='button' id='btnSendBack'>Send info</button></p></body>\
<script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function processCommand(n){document.getElementById('rand').innerHTML=n.data,console.log(n.data)}\
function btnSendBack(){Socket.send('Sending back some random stuff')}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script></html>";
*/

String webpage = "<!DOCTYPE html><html lang='en'><head></head><body><h1>-=( I'm on! )=-</h1><p>Take my first random number:&nbsp;<span id='rand1'>-</span></p><p>Take my second random number:&nbsp;<span id='rand2'>-</span></p>\
<p><button type='button' id='btnSendBack'>Send info</button></p></body><script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function processCommand(n){var e=JSON.parse(n.data);\
document.getElementById('rand1').innerHTML=e.rand1,document.getElementById('rand2').innerHTML=e.rand2,console.log(n.data),console.log(e.rand1),console.log(e.rand2)}function btnSendBack(){Socket.send(JSON.stringify({num_leds:'193',led_model:'APA102',color_mode:'BGR',clock_pin:'13',data_pin:'14'}))}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script></html>";

// Variables for timer check
unsigned long interval = 0; // in milliseconds
unsigned long previousMillis = 0;
unsigned long now = 0;
// run initTimer() inside setup

StaticJsonDocument<200> docTX;
StaticJsonDocument<200> docRX;
String jsonString = "";

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
  //Timer loop
  if (now - previousMillis > interval) {
    JsonObject object = docTX.to<JsonObject>();
    object["rand1"] = random(100);
    object["rand2"] = random(100);
    serializeJson(docTX, jsonString);
    webSocket.broadcastTXT(jsonString);

    if (DEBUG) Serial.println(jsonString);
    previousMillis += interval;
    jsonString = "";
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
      DeserializationError err = deserializeJson(docRX, payload);
      if(err) {
        Serial.print("deserializeJson failed for payload ");
        //Serial.println(payload);
        return;
      }
      const char* ls_num_leds = docRX["num_leds"];
      const char* ls_led_model = docRX["led_model"];
      const char* ls_color_mode = docRX["color_mode"];
      const char* ls_clock_pin = docRX["clock_pin"];
      const char* ls_data_pin = docRX["data_pin"];

      Serial.println("led strip info:");
      Serial.println("  Num leds: " + String(ls_num_leds));
      Serial.println(" Led model: " + String(ls_led_model));
      Serial.println("Color mode: " + String(ls_color_mode));
      Serial.println(" Clock pin: " + String(ls_clock_pin));
      Serial.println("  Data pin: " + String(ls_data_pin));

      break;
  }
}
