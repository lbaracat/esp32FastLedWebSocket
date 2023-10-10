#include <WiFi.h>
#include "cred.h"

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ON.");
 
  WiFi.begin(SSID, PASS);
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print(" CONNECTED!\n");
  delay(4000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  
}
