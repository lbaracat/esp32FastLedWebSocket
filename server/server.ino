#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "cred.h"
#include "ledstrip.h"

#define DEBUG false

WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/*
String webpage = "<!DOCTYPE html><html lang='en'><head></head><body><h1>-=( I'm on! )=-</h1><p>Take my first random number:&nbsp;<span id='rand1'></span></p><p>Take my second random number:&nbsp;<span id='rand2'></span></p>\
<p><button type='button' id='btnSendBack'>Send info</button></p></body><script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function btnSendBack(){Socket.send(JSON.stringify({num_leds:'193',led_model:'APA102',color_mode:'BGR',clock_pin:'13',data_pin:'14'}))}\
function processCommand(n){var e=JSON.parse(n.data);document.getElementById('rand1').innerHTML=e.rand1,document.getElementById('rand2').innerHTML=e.rand2,console.log(e.rand1),console.log(e.rand2)}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script></html>";
*/

String webpage="<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Led Strip</title><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' integrity='sha256-MBffSnbbXwHCuZtgPYiwMQbfE7z+GOZ7fBPCNB06Z98=' crossorigin='anonymous'></head>\
<body><div class='container'><div class='col'><div class='row'><h3>Led strip controller</h3></div><div class='row'><div class='input-group mb-3'><button class='btn btn-outline-secondary' type='button' id='btnSendBack'>Send to server</button><input type='text' class='form-control' placeholder='' aria-label='Example text with button addon' aria-describedby='btnSendBack' id='textToSend'></div>\
</div></div></div></body><script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function btnSendBack(){Socket.send(document.getElementById('textToSend').value)}function processCommand(n){console.log(n.data)}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script>\
<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.min.js' integrity='sha256-YMa+wAM6QkVyz999odX7lPRxkoYAan8suedu4k2Zur8=' crossorigin='anonymous'></script></html>";

// Variables for timer check
unsigned long interval = 0; // in milliseconds
unsigned long previousMillis = 0;
unsigned long now = 0;
// run initTimer() inside setup

StaticJsonDocument<200> docTX;
StaticJsonDocument<200> docRX;
String jsonString = "";

BaseType_t xReturned;
TaskHandle_t xInitLedStripHnd = NULL;

void setup() {
  Serial.begin(500000);
  while(!Serial) {
    ; // wait for serial port
  }

  Serial.println("ESP32 ON.");
 
  initWiFi();
  initWebComponents();
  initTimer(1000);

  xReturned = xTaskCreate(initLedStrip, "LED Control", 1000, NULL, 0, &xInitLedStripHnd);
  if (xReturned == pdPASS) {
    Serial.println("Led task created.");
  }
}
 
void loop() {
  webServer.handleClient();
  webSocket.loop();

  now = millis();
  //Timer loop
  if (now - previousMillis > interval) {
    jsonString = String(now);
/*
    JsonObject object = docTX.to<JsonObject>();
    object["rand1"] = random(100);
    object["rand2"] = random(100);
    serializeJson(docTX, jsonString);
*/
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
    delay(100);
    Serial.print(".");
  }

  Serial.println(" CONNECTED!");
  delay(500);
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

void initLedStrip(void *pvParameters) {
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  FastLED.addLeds<STRIPTYPE, DATAPIN, CLOCKPIN, COLORSCHEME>(leds, NUMLEDS);

  for (int blink = 0; blink < 3; blink++) {
    for(int i = 0; i < NUMLEDS; i++){
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(50);
    for(int i = 0; i < NUMLEDS; i++){
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(50);
  }
  delay(10000);

  for(int i = 0; i < NUMLEDS; i++){
    leds[i] = CRGB::White;
  }
  FastLED.show();

  while(1) {

  }
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
    case WStype_TEXT:
      for (int i=0; i<length; i++) {                  // print received data from client
        Serial.print((char)payload[i]);
      }
      Serial.println("");                                 // if a client has sent data, then type == WStype_TEXT
      /*
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
      */

      break;
  }
}
