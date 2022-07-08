#include <Arduino.h>

#include "ESP8266WiFi.h"
#include "aREST.h"

aREST rest = aREST();

// WiFi parameters
const char* ssid = "Marcelo agachate";
const char* password = "CLAROI58588";

//PORT
#define LISTEN_PORT 80

//Create instance of server
WiFiServer server(LISTEN_PORT);

//variables API

float miVariable;

void setup() {
  Serial.begin(9600);

  //init variable Api
  rest.variable("miVariable", &miVariable);

  //name id
  rest.set_id("1");
  rest.set_name("nodev");

  //connecten to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Wifi connected!!");

  //start server

  server.begin();
  Serial.println("server started!");

  //ip
  Serial.println(WiFi.localIP());

}

void loop() {
  //Wait 1s
  delay(1000);

  miVariable = random(100);//variable que queremos mandar

  //REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);


}
