//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

//#include "LittleFS.h"
#include "DHT.h"
// Раскомментируйте одну из строк ниже в зависимости от того, какой датчик вы используете!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
/* Установите здесь свои SSID и пароль */
const char* ssid = "george";  // Enter SSID here
const char* password = "1234567891";  //Enter Password here

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64//32 // OLED display height, in pixels

AsyncWebServer   server(80);
// DHT Sensor
uint8_t DHTPin = 4;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
String PARAM_MESSAGE = "status";

bool LED1status = LOW;
const int LED_PIN = LED_BUILTIN;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  display.cp437(true);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  showText("start");

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  delay(100);
  pinMode(DHTPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  //check wi-fi is connected to wi-fi network
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  
  LED1status = HIGH;//откл.
  Serial.println("LED status :" + String(LED1status));
  digitalWrite(LED_PIN, LED1status);
  Serial.println("LED status :" + String(LED1status));

  server.onNotFound(notFound);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(LittleFS, "/index.js", "text/javascript");
  });

  // Respond to toggle event
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    String status;
    if (request->hasParam(PARAM_MESSAGE)) {
      status = request->getParam(PARAM_MESSAGE)->value();
      if (status == "HIGH") {
        LED1status = HIGH;
        toggleLED("HIGH");
      } else {
        LED1status = LOW;
        toggleLED("LOW");
      }
    }
    
    Serial.println("responce status: " +  String(status));
    Serial.println("LED status :" + String(LED1status));
    showText("LED status");
    request->send(200, "text/plain", status);
  });

  server.on("/stt", HTTP_GET, [](AsyncWebServerRequest * request) {
    String stt = "{\"STT\":\"LOW\"}";
    if (LED1status == HIGH) {
      stt = "{\"STT\":\"HIGH\"}";      
    }
    Serial.println("responce : " + stt);
    Serial.println("LED status :" + String(LED1status));
    request->send(200, "text/plain", stt);
  });

  server.begin();
  Serial.println("HTTP server started");

  //---littleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }


}
void loop() {
   
}

void showText(String txt){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(txt);
  display.display();
}
/*
  void handle_OnConnect() {
  Temperature = 21;//dht.readTemperature(); // Gets the values of the temperature
  Humidity = 55;//dht.readHumidity(); // Gets the values of the humidity
  String ret = ""; //SendHTML(Temperature,Humidity);

  File file = LittleFS.open("/index.html", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();

  server.send(200, "text/html", );
  }
*/

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void toggleLED(String status)
{
  if (status == "ON") {
   // LED1status = LOW; 
  }
  else {
    //LED1status = HIGH;
  }
  digitalWrite(LED_PIN, LED1status); 
}
