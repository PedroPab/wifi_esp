#include <Wire.h>
#include <LiquidCrystal_I2C.h>//D4 d3
#include "ESP8266WiFi.h"
#include "aREST.h"
#include "max6675.h"

aREST rest = aREST();

// WiFi parameters
const char* ssid = "CLARO_WIFIB4";
const char* password = "CLAROI58588";

//PORT
#define LISTEN_PORT 80

//Create instance of server
WiFiServer server(LISTEN_PORT);

//variables API

float temperatura;


int thermoDO = D5;
int thermoCS = D6;
int thermoCLK = D7;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

LiquidCrystal_I2C lcd(0x27, 16, 2); //0x27 is the i2c address, while 16 = columns, and 2 = rows.


void setup() {

  Serial.begin(9600);
  Serial.print("hola mundo");
  //iniciando pantalla lcd

  Wire.begin(2, 0); // gpio 2 and gpio 0 which are D4, and D3
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("saludos humanosꞙ");
  //init variable Api
  rest.variable("temperatura", &temperatura);

  //name id
  rest.set_id("1");
  rest.set_name("nodev");

  //connecten to wifi
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 0);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  Serial.println("");
  Serial.print("Wifi connected!!");
  lcd.print("Wifi connected!!");

  //start server

  server.begin();
  Serial.println("server started!");
  lcd.setCursor(2, 0);
  lcd.print("server ꞙ௹👀👀👀👀🔴🔴☺♀♥ " );
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP() );
  //ip
  Serial.println(WiFi.localIP());
  delay(5000);

}

void loop() {
  //Wait 1s
  delay(1000);

  temperatura = thermocouple.readCelsius();//variable que queremos mandar

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
