/*
  #include <ESP8266WiFi.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Fonts/FreeSerif9pt7b.h>
*/

#include "funcs.h"
//#include <Max72xxPanel.h>

time_t now;
const int ledPin = LED_BUILTIN;  // the number of the LED pin
const unsigned long BOT_MTBS = 1000 * 5;//

int i = 0;
unsigned long bot_lastTime;
bool needRestart = false;
String localIP;
char buff[20];

int pinCS = D8; 
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays   = 1;
char time_value[20];

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 70; // In milliseconds

int spacer = 1;
int width  = 5 + spacer; // The font width is 5 pixels

int m;

WiFiClientSecure secured_client;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void setup() {
  
  //Register event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect); 
  init(&display);
  initWiFi(&display);
 
  //secured_client.setTrustAnchors(&cert);
  secured_client.setInsecure();
  /*
    display.clearDisplay();
    display.println(utf8rus("Conn to pool.ntp.org"));
    display.display();
    delay(2000);
    configTime(TIME_ZONE * 3600,0, "pool.ntp.org");
    now = time(nullptr);
    while (now < 24 * 3600){
    Serial.print(".");
    display.print(".");
    display.display();
    delay(100);
    now = time(nullptr);
    }

    strftime(buff, 20, "%H:%M:%S - %d-%m-%YY", localtime(&now));
    //display.clearDisplay();
    display.println(buff);
    display.display();
    delay(2000);
  */
}

void loop() {
  display.clearDisplay();
  unsigned long currentMillis = millis();
  if (currentMillis - bot_lastTime > BOT_MTBS) {
    i++;
    now += (BOT_MTBS / 1000);
    char currTime[20];
    //strftime(currTime, 20, "%H:%M:%S - %d-%m-%Y ", localtime(&now));
    strftime(currTime, 20, "%H:%M:%S", localtime(&now));

    int num = bot.getUpdates(bot.last_message_received);
    //String msg = "новых сообщений: " + String(num);
    //Serial.print("\r\n " +msg);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(utf8rus("loop#" + String(i) + " upTm" + upTime() ));
    display.println(utf8rus("сейчас: ") + String( currTime));
    display.display();

    while (num) {
      handleNewMsg(num, &bot, &display);
      num = bot.getUpdates(bot.last_message_received + 1);
    }
    if (needRestart == true) {
      ESP.restart();
    }
    bot_lastTime = currentMillis;
  }
}


void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //display.println("Connected to Wi-Fi sucessfully.");
  //display.display();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi, trying to connect...");
  //display.println("Disconnected from Wi-Fi, trying to connect...");
  //display.display();
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

