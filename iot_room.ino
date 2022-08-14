
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>


#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include "aREST.h"

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "CLARO_WIFIB4";
const char* password = "CLAROI58588";
const char* web = "http://iot-domiburguer.herokuapp.com/api/IOT/roomMan";
String payload ;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (millis() > 10000) {
      Serial.println("");
      Serial.print("Wifi not connected!!");
      delay(2000);
    }
  }
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://iot-domiburguer.herokuapp.com/api/IOT/roomMan/starLight"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload_post = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload_post);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }


}

void loop() {


  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, web)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
           payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());

      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  if (payload == "true") {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(0, LOW);
  } else if (payload == "false") {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(0, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(0, LOW);

  }
  delay(1);
}
