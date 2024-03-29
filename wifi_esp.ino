#include <Wire.h>
#include <LiquidCrystal_I2C.h>//D4 d3
#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include "aREST.h"
#include "max6675.h"

aREST rest = aREST();
WiFiClient wifiClient;

// WiFi parameters
const char* ssid = "Domiburguer";
const char* password = "Ventanalimpia";

//PORT
#define LISTEN_PORT 80

//Create instance of server
WiFiServer server(LISTEN_PORT);

//variables API

float temperatura;
float temperatura_2;

int thermoDO = D5;
int thermoCS = D6;
int thermoCLK = D7;

int temp1 = 40;
int temp2 = 60;

int zum = D1;//zumbador

String payload ;// varible de la hora
String hora;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

LiquidCrystal_I2C lcd(0x27, 16, 2); //0x27 is the i2c address, while 16 = columns, and 2 = rows.


void setup() {
  pinMode(zum, OUTPUT);//zumbador

  Serial.begin(9600);
  Serial.print("hola mundo");
  //iniciando pantalla lcd

  Wire.begin(2, 0); // gpio 2 and gpio 0 which are D4, and D3
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("saludos humanos");
  //init variable Api
  rest.variable("temperatura", &temperatura);
  rest.variable("hora", &hora);
  
  //name id
  rest.set_id("1");
  rest.set_name("nodevTermocupla");
  
  //connecten to wifi
  WiFi.begin(ssid, password);

  lcd.setCursor(0, 0);
  delay(500);
  lcd.clear();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  HTTPClient http;
  http.begin(wifiClient, "http://worldtimeapi.org/api//timezone/America/Bogota/");  //Specify request destination
  int httpCode = http.GET();
  payload = http.getString();   //Get the request response payload
  Serial.println(payload);
  //Serial.println(http);
  

  // Disconnect
  http.end();
  Serial.println("");
  Serial.print("Wifi connected!!");
  lcd.clear();
  lcd.print("Wifi connected!!");

  //start server

  server.begin();
  Serial.println("server started!");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("server started!" );


  //ip
  Serial.println(WiFi.localIP());
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP() );
  delay(500);


}

void loop() {
  //Wait 1s
  //pedir la hora
  HTTPClient http;
  http.begin(wifiClient, "http://worldtimeapi.org/api//timezone/America/Bogota/");  //Specify request destination
  int httpCode = http.GET();
  payload = http.getString();   //Get the request response payload
  Serial.println(payload);
  hora = payload.substring(73, 82);
  
  delay(100);

  temperatura = thermocouple.readCelsius();//variable que queremos mandar

  lcd.clear();
  lcd.setCursor(10, 1);
  lcd.print(payload.substring(73, 79));
  lcd.setCursor(0, 0);
  lcd.print("C*= ");
  lcd.print(temperatura);
  lcd.print("  ");

  if (int(temperatura) > int(temperatura_2)) {
    lcd.print("+");
  } else if (temperatura < temperatura_2) {
    lcd.print("-");
  } else {
    lcd.print("=");
  }
  
  
  

  condiciones();
  


  temperatura_2 = thermocouple.readCelsius();


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

void condiciones() {

  lcd.setCursor(0, 1);
  switch  (int(temperatura)) {
    case 69:
      lcd.print("jejeje, 69 XD");
      break;
    case 26:
      lcd.print("y tu que miras?");
      break;

  }
  if (temperatura > 165) {
    lcd.setCursor(0, 1);
    lcd.print("moderado :)       ");
    alerta(0);
  }
  if (temperatura > 174) {
    lcd.setCursor(0, 1);
    lcd.print("se quema !! :("   );
    alerta(1);
  }

}

void alerta(int tipo) {
  switch (tipo) {
    case 0:
      digitalWrite(zum, HIGH);
      delay(temp2);
      digitalWrite(zum, LOW);
      //delay(775);

      break;
    case 1:
      digitalWrite(zum, HIGH);

      break;
    default:
      digitalWrite(zum, LOW);
      break;

  }

}
void hora7(){
  
}
