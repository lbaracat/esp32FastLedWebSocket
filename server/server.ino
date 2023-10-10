#include <WiFi.h>
#include <WebServer.h>
#include "cred.h"

WebServer server(80);

String webpage = "<html><head></head><body><h1>-> I'm on! <-</h1></body></html>";

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ON.");
 
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

  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });
  server.begin();
}
 
void loop() {
  server.handleClient();
}
