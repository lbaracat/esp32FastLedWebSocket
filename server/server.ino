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
String webpage="<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Led Strip</title><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' integrity='sha256-MBffSnbbXwHCuZtgPYiwMQbfE7z+GOZ7fBPCNB06Z98=' crossorigin='anonymous'></head>\
<body><div class='container'><div class='row'><div class='col'><h3>Led strip controller</h3></div><div class='col text-end'><span class='align-middle' id='uptime'>Uptime:</span></div></div><div class='row'><div class='col'><div class='input-group mb-3'><button class='btn btn-outline-secondary' type='button' id='btnSendBack'>Send to server</button>\
<input type='text' class='form-control' placeholder='' aria-label='Example text with button addon' aria-describedby='btnSendBack' id='textToSend'></div></div></div></div></body><script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(n){processCommand(n)}}function btnSendBack(){Socket.send(document.getElementById('textToSend').value)}\
function processCommand(n){console.log(n.data);var e=JSON.parse(n.data);document.getElementById('uptime').innerHTML=e.uptime}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),window.onload=function(n){init()}</script><script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.min.js' integrity='sha256-YMa+wAM6QkVyz999odX7lPRxkoYAan8suedu4k2Zur8=' crossorigin='anonymous'></script></html>";
*/

String webpage="<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Led Strip</title><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' integrity='sha256-MBffSnbbXwHCuZtgPYiwMQbfE7z+GOZ7fBPCNB06Z98=' crossorigin='anonymous'></head>\
<body><div class='container'><div class='row'><div class='col'><h3>Led strip controller</h3></div><div class='col text-end mt-1'><span class='align-middle' id='uptime'></span></div><div class='col text-end mt-1'><button type='button' class='badge bg-danger' data-bs-toggle='modal' data-bs-target='#restartModal'>Restart</button>\
<div class='modal fade' id='restartModal' tabindex='-1' aria-labelledby='restartModalLabel' aria-hidden='true'><div class='modal-dialog'><div class='modal-content'><div class='modal-header'><h1 class='modal-title fs-5' id='restartModalLabel'>Confirm restart</h1><button type='button' class='btn-close' data-bs-dismiss='modal' aria-label='Close'></button></div>\
<div class='modal-body'>This will soft restart the controller.<br>Normally, it'll take less then a minute to return.<br>Are you sure?</div><div class='modal-footer'><button type='button' class='btn btn-secondary' data-bs-dismiss='modal'>No, abort</button><button type='button' class='btn btn-danger' data-bs-dismiss='modal' id='btnRestart'>Yes, restart controller</button></div></div></div></div></div></div>\
<div class='row'><div class='col'><div class='input-group mb-3'><button class='btn btn-outline-secondary' type='button' id='btnSendBack'>Send to server</button><input type='text' class='form-control' placeholder='' aria-label='Input field to send commands to server' aria-describedby='btnSendBack' id='textToSend'></div></div></div></div></body>\
<script>var Socket;function init(){(Socket=new WebSocket('ws://'+window.location.hostname+':81/')).onmessage=function(e){processCommand(e)}}function btnSendBack(){Socket.send(document.getElementById('textToSend').value)}function btnRestart(){Socket.send('{\"command\":1}')}function processCommand(e){console.log(e.data);\
var t=JSON.parse(e.data);document.getElementById('uptime').innerHTML=t.uptime}document.getElementById('btnSendBack').addEventListener('click',btnSendBack),document.getElementById('btnRestart').addEventListener('click',btnRestart),window.onload=function(e){init()}</script><script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.min.js' integrity='sha256-YMa+wAM6QkVyz999odX7lPRxkoYAan8suedu4k2Zur8=' crossorigin='anonymous'></script></html>";

// Variables for timer check
unsigned long interval = 0; // in milliseconds
unsigned long previousMillis = 0;
unsigned long now = 0;
// run initTimer() inside setup

StaticJsonDocument<200> docTX;
StaticJsonDocument<200> docRX;
String jsonString = "";

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

BaseType_t xLedTaskReturned;
TaskHandle_t xLedTaskHnd = NULL;

void setup() {
  Serial.begin(500000);
  while(!Serial) {
    ; // wait for serial port
  }

  delay(1000);
  Serial.println("ESP32 ON.");
 
  initWiFi();
  initWebComponents();
  initTimer(1000);

  xLedTaskReturned = xTaskCreate(ledTask, "LED Control", 1000, NULL, 0, &xLedTaskHnd);
  if (xLedTaskReturned == pdPASS) {
    Serial.println("Led task created.");
  }
}
 
void loop() {
  webServer.handleClient();
  webSocket.loop();

  now = millis();
  //Timer loop
  if (now - previousMillis > interval) {
    JsonObject object = docTX.to<JsonObject>();
    object["uptime"] = uptime();
    serializeJson(docTX, jsonString);
    webSocket.broadcastTXT(jsonString);

    if (DEBUG) Serial.println(jsonString);
    jsonString = "";
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
      /*
      for (int i=0; i<length; i++) {                  // print received data from client
        Serial.print((char)payload[i]);
      }
      Serial.println("");                                 // if a client has sent data, then type == WStype_TEXT
      */
      DeserializationError err = deserializeJson(docRX, payload);
      if(err) {
        Serial.println("deserializeJson failed for payload ");
        //Serial.println(payload);
        return;
      }
//      const int command = docRX["command"];
      switch((int) docRX["command"]) {
        case 1:
          rebootCommand();
          break;
        default:
          Serial.println("Command not found");
          break;
      }
      /*
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

void rebootCommand() {
  Serial.println("Closing resources...");
  webSocket.close();
  webServer.close();
  Serial.println("Rebooting...");
  delay(500);
  Serial.end();
  ESP.restart();
}

void initTimer(int i) {
  interval = i;
  Serial.print("Syncing timer... ");
  while (millis() % 1000 != 999) {
    ; // wait here until sync timer with second...
  }
  previousMillis = millis();
  now = millis();
  Serial.println("DONE.");
}

String uptime() {
  unsigned long now = millis();
  //unsigned long millis, seconds, minutes, hours, days;
  unsigned long seconds, minutes, hours, days;
  String output = String("");
  //millis = now % 1000;
  now = now / 1000;
  seconds = now % 60;
  now = now / 60;
  minutes = now % 60;
  now = now / 60;
  hours = now % 24;
  days = now / 24;
  output += String("Uptime: ");
  output += String(days);
  output += String(" days, ");
  if(hours < 10) { output += "0";}
  output += String(hours);
  output += ":";
  if(minutes < 10) { output += "0";}
  output += String(minutes);
  output += ":";
  if(seconds < 10) { output += "0";}
  output += String(seconds);
  //output += String(".");
  //output += String(millis);
  //output += String(" seconds.");
  return output;
}

void ledTask(void *pvParameters) {
  delay(500); // Original code makes 3 secs delay. As my code has 2,5 sec delay in previous routines, that's ok
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);

  FastLED.addLeds<STRIPTYPE, DATAPIN, CLOCKPIN, COLORSCHEME>(leds, NUMLEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  static uint8_t motionSpeed = 1;

  while(1) {
    ChangePalettePeriodically(motionSpeed);

    static uint8_t startIndex = 0;
    startIndex = startIndex + motionSpeed; /* motion speed */

    FillLEDsFromPaletteColors(startIndex);

    FastLED.show();
    FastLED.delay(1000 / FPS);
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;

    for( int i = 0; i < NUMLEDS; ++i) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically(uint8_t& motionSpeed) {
    uint8_t secondHand = (millis() / 1000) % 140;
    static uint8_t lastSecond = 199;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
//        if( secondHand ==  0)   { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; motionSpeed = 2; }
//        if( secondHand == 9)    { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND; motionSpeed = 254; }
//        if( secondHand == 19)   { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; motionSpeed = 2; }
        if( secondHand == 29)   { currentPalette = OceanColors_p;           currentBlending = LINEARBLEND; motionSpeed = 254; }
//        if( secondHand == 39)   { SetupYellowAndGreenPalette();             currentBlending = LINEARBLEND; motionSpeed = 2; }
//        if( secondHand == 49)   { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; motionSpeed = 1; }
        if( secondHand == 59)   { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; motionSpeed = 2; }
        if( secondHand == 69)   { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; motionSpeed = 254; }
        if( secondHand == 79)   { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; motionSpeed = 2; }
//        if( secondHand == 89)   { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; motionSpeed = 254; }
//        if( secondHand == 99)   { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND; motionSpeed = 2; }
//        if( secondHand == 109)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; motionSpeed = 254; }
        if( secondHand == 119)  { currentPalette = LavaColors_p;            currentBlending = LINEARBLEND; motionSpeed = 2; }
        if( secondHand == 129)  { currentPalette = ForestColors_p;          currentBlending = LINEARBLEND; motionSpeed = 254; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0]  = CRGB::White;
    currentPalette[4]  = CRGB::White;
    currentPalette[8]  = CRGB::White;
    currentPalette[12] = CRGB::White;
}

// This function sets up a palette of yellow and green stripes.
void SetupYellowAndGreenPalette() {
    CRGB color1 = CHSV(HUE_YELLOW, 255, 255);
    CRGB color2 = CHSV(HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    currentPalette = CRGBPalette16(color1, color1, black,  black,
                                   color2, color2, black,  black,
                                   color1, color1, black,  black,
                                   color2, color2, black,  black );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  /*
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
  */
  CRGB::Green,
  CRGB::Yellow,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Black,

  CRGB::Green,
  CRGB::Yellow,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Black,

  CRGB::Green,
  CRGB::Yellow,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Black,
  CRGB::Black
};
// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from color1-to-blue, and then Blue.

