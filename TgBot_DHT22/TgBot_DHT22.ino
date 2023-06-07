/*
D0 = 16;
D1 = 5;
D2   = 4;
D3   = 0;
D4   = 2;
D5   = 14;
D6   = 12;
D7   = 13;
D8   = 15;
*/

#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
//#include "DHT.h"

#define TELEGRAM_DEBUG 1
#define _debug 1

#define WIFI_SSID "george"
#define WIFI_PASSWORD "1234567891"
WiFiClientSecure secured_client;
#define BOT_TOKEN ""//
#define HOME_CHAT_ID "-1001644683170" 
#define BOT_NAME "@" //"@"

//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321




const unsigned long BOT_MTBS = 500 * 2 * 30;//
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
bool needRestart = false; 
// датчик DHT
uint8_t DHTPin = 5; 
               
// инициализация датчика DHT.
//DHT dht(DHTPin, DHTTYPE);                

float Temperature = 0.0;
float Humidity = 0.0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);

  //pinMode(DHTPin, INPUT);
  //dht.begin();

  
  Serial.begin(9600);
  //configTime(0,0, "pool.ntp.org");
  //time_t now = time(nullptr);
  /*
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  */
  //Serial.println(now);
 
  //secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  //Serial.print("\r\n delay 60 sec begin");
  //delay(1000 * 60);
  //Serial.print("\r\n delay 60 sec after");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 secured_client.setInsecure();
  //secured_client.setTrustAnchors(&cert);
  
  int i = 0;
    while(WiFi.status() != WL_CONNECTED){
      i++;
     Serial.print("\r\n\ Подключение к WiFi попытка №: " + String(i) );
     delay(1000 * 10); //  10sec
  };

  Serial.print("\r\n WiFi connected, IP:");
  Serial.println(WiFi.localIP());
  localIP = IpAddress2String( WiFi.localIP());
  bot.sendMessage(HOME_CHAT_ID, "Свет включили!", "Markdown");
  Serial.println("Свет включили!");
  //bot_setup();
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
    int num = bot.getUpdates(bot.last_message_received);
    Serial.print("\r\n new msg cnt: " + String(num) + "last_message_received +1 " + String(bot.last_message_received));
    while(num){
      handleNewMsg(num);
      num = bot.getUpdates(bot.last_message_received + 1);
    }
    if (needRestart == true){
           ESP.restart();
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
  //Serial.print("\r\n currentMillis: " + String(currentMillis));
}

void handleNewMsg(int num){
  Serial.print("\r\n enter handleNewMsg num:"  + String(num) );
  for(int i = 0; i < num; i++){
    Serial.print("\r\n chat_id: " + String(bot.messages[i].chat_id));
    String  CURR_CHAT_ID = bot.messages[i].chat_id;
    if (CURR_CHAT_ID == HOME_CHAT_ID || 1==1 ){
       String text = bot.messages[i].text;
       int message_id =  bot.messages[i].message_id;
       String from_id = bot.messages[i].from_id;
       int reply_to_message_id =  bot.messages[i].reply_to_message_id;
       if (text.indexOf(BOT_NAME)){
          text.replace(BOT_NAME, "");
          text.toLowerCase();
        }
       Serial.print("\r\n text: " + text);
       Serial.print("\r\n message_id: " + String(message_id) );
       Serial.print("\r\n from_id: " + String(from_id) );
       Serial.print("\r\n reply_to_message_id: " + String(reply_to_message_id) );
       
        
        /*      
        if (text == "/alive"){
         bot.sendMessage(CURR_CHAT_ID, "i'm Alive", "");
        }
        */
        /*
        else if (text == "/on"){
          digitalWrite(LED_BUILTIN, LOW);
          bot.sendMessage(HOME_CHAT_ID, "*light On*" , "Markdown");
          //bot.sendSticker(HOME_CHAT_ID, "CAACAgIAAx0CYgffogACASRjvd585U6oehUaMyelCi7tVj8XQQACMwcAAkRTwUmByLxH2zzKqC0E");
          //bot.sendPhoto(HOME_CHAT_ID, "CAACAgIAAx0CYgffogACASRjvd585U6oehUaMyelCi7tVj8XQQACMwcAAkRTwUmByLxH2zzKqC0E", "This photo was sent using File ID");
        }
        else if (text == "/off"){
          digitalWrite(LED_BUILTIN, HIGH);
          bot.sendMessage(HOME_CHAT_ID, "_light Off_", "Markdown");
        }       
        else if (text == "/IP"){
          bot.sendMessage(HOME_CHAT_ID,"my IP: "+ localIP + " [inline URL](http://www.example.com/)", "Markdown");
        }
        else  if (text == "/send_test_action"){
          bot.sendChatAction(HOME_CHAT_ID, "upload_photo");
        }
        */
          if (text == "/restart"){
             bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name + "\r\nПерезапуск...", ""  );
             needRestart = true;
          }
          else  if (text == "/uptime"){
             bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name +"\r\nПродолжительность работы: " + upTime(), "" );
          }
          else if (text == "/temp"){
           // Temperature = dht.readTemperature(); // получить значение температуры
          //  Humidity = dht.readHumidity();       // получить значение влажности
           //String msg =  "Температура: " + String(Temperature) + "\r\n Влажность: " + String(Humidity);
          // bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name +"\r\n"+ msg, "");
          } 
          else  if (text == "/help"){
            String msg ="/restart - Перезапуск бота\r\n /uptime - Продолжительность работы в сети\r\n /temp - Температура и валажность";
             bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name +"\r\n"+ msg, "" );
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

void bot_setup()
{

const String commands = F("["
                            "{\"command\":\"help\",\"description\":\"Список команд\"},"
                            "{\"command\":\"restart\",\"description\":\"Перезапуск бота\"},"
                            "{\"command\":\"uptime\",\"description\":\"Продолжительность работы в сети\"}" 
                           // "{\"command\":\"temp\",\"description\":\"Температура и валажность\"}" 
                            "]");
  bot.setMyCommands(commands);
}

String upTime(){
  uint32_t sec = millis() / 1000ul;      // полное количество секунд
  int timeHours = (sec / 3600ul);        // часы
  int timeMins = (sec % 3600ul) / 60ul;  // минуты
  int timeSecs = (sec % 3600ul) % 60ul;  // секунды

  return (timeHours < 10 ? "0"+ String( timeHours ) : String( timeHours ) ) + ":"+ (timeMins < 10 ? "0"+ String( timeMins ) : String( timeMins ) ) + ":" + ( timeSecs < 10 ? "0"+ String(timeSecs): String(timeSecs));
  }
