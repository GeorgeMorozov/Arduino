#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "Xiaomi 11T"
#define WIFI_PASSWORD "12345678"//"George1980"//"1234567891"//George1980
WiFiClientSecure secured_client;
#define BOT_TOKEN "5870790444:AAFm2znUWphDseKkliHksLd84xV_4nqKUfA"
#define HOME_CHAT_ID "-1001644683170" 
#define VFR_CHAT_ID "-1001534956210"
//https://api.telegram.org/bot5870790444:AAFm2znUWphDseKkliHksLd84xV_4nqKUfA/getUpdates
const unsigned long BOT_MTBS = 300;
const unsigned long BOT_MTBS_HOUR = 1000 * 10; //3600; // 1 hour timeout for alive
X509List cert(TELEGRAM_CERTIFICATE_ROOT); 
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lastTime;
unsigned long bot_lastTimeHour;
String localIP;


// constants won't change. Used here to set a pin number:
const int ledPin = LED_BUILTIN;  // the number of the LED pin

// Variables will change:
int ledState = LOW;  // ledState used to set the LED
// the setup function runs once when you press reset or power the board

int i = 0;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  configTime(0,0, "pool.ntp.org");
  secured_client.setTrustAnchors(&cert);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED){
     Serial.print(".");
     delay(300);  
  };

  Serial.print("\r\n WiFi connected, IP:");
  Serial.println(WiFi.localIP());
  localIP = IpAddress2String( WiFi.localIP());
  bot.sendMessage(HOME_CHAT_ID, "Свет дали!");
  int num = bot.getUpdates(bot.last_message_received + 1);
    Serial.print("\r\n new msg cnt setup: " + String(num));
}

// the loop function runs over and over again forever
void loop() {
  /*
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  */

  unsigned long currentMillis = millis();
   
  if(currentMillis - bot_lastTime > BOT_MTBS){
    int num = bot.getUpdates(bot.last_message_received + 1);
    Serial.print("\r\n new msg cnt: " + String(num));
    while(num){
      handleNewMsg(num);
      num = bot.getUpdates(bot.last_message_received + 1);
    } 
    bot_lastTime = currentMillis;

    /*
    if (ledState == LOW) {
      ledState = HIGH;
      Serial.print("\r\n ledState HIGH");
    } else {
      ledState = LOW;
      Serial.print("\r\n ledState LOW");
    }     
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    */    
  }
/*
   if(currentMillis - bot_lastTimeHour > BOT_MTBS_HOUR){
    bot.sendMessage(HOME_CHAT_ID, "i'm Alive " + String(i), "");
    i++;
    bot_lastTimeHour = currentMillis;
  }
  */
  Serial.print("\r\n currentMillis: " + String(currentMillis));
}

void handleNewMsg(int num){
  Serial.print("\r\n enter handleNewMsg num:"  + String(num) );
  for(int i = 0; i < num; i++){
    Serial.print("\r\n chat_id: " + String(bot.messages[i].chat_id));
    if (bot.messages[i].chat_id == HOME_CHAT_ID){
       String text = bot.messages[i].text;
       Serial.print("\r\n " + text);
       if (text == "/alive"){
         bot.sendMessage(HOME_CHAT_ID, "i'm Alive", "");
        }
        else if (text == "/on"){
          //digitalWrite(LED_BUILTIN, LOW);
          bot.sendMessage(HOME_CHAT_ID, "light On", "");
        }
        else if (text == "/off"){
          //digitalWrite(LED_BUILTIN, HIGH);
          bot.sendMessage(HOME_CHAT_ID, "light Off", "");
        } 
        //
        else if (text == "/IP"){
          bot.sendMessage(HOME_CHAT_ID,"my IP: "+ localIP, "");
        }     
      }
  }
}

String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}
