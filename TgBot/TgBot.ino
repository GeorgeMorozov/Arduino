#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <SPI.h>                        // Подключение библиотеки SPI
#include <Wire.h>                       // Подключение библиотеки Wire
#include <Adafruit_GFX.h>               // Подключение библиотеки Adafruit_GFX
#include <Adafruit_SSD1306.h>           // Подключение библиотеки Adafruit_SSD1306
#define OLED_RESET                      16
Adafruit_SSD1306 display(OLED_RESET);

#define WIFI_SSID "george"//"HTC"//"george"//"George"//"Xiaomi 11T"
#define WIFI_PASSWORD  "1234567891"//""//"1234567891"  //"12345678"//"George1980"//"1234567891"

WiFiClientSecure secured_client;
//#define BOT_TOKEN ""//GeorgeHomeBot
#define BOT_TOKEN ""//GRGMrz1980Bot
#define HOME_CHAT_ID "-1001644683170" 
//#define VFR_CHAT_ID "-1001534956210"
//#define GLEBOVA43_CHAT_ID "-1001701386178"
#define BOT_NAME "@"

// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//https://api.telegram.org/
//"CAACAgIAAx0CYgffogACASRjvd585U6oehUaMyelCi7tVj8XQQACMwcAAkRTwUmByLxH2zzKqC0E" - stiker 
const unsigned long BOT_MTBS = 1000 * 3;//
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

void setup() {
    display.cp437(true);             
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // Указываем адрес дисплея
  pinMode(LED_BUILTIN, OUTPUT);
  display.clearDisplay();                    // Очищаем дисплей
  display.setTextSize(1);                    // Устанавливаем размер шрифта
  display.setTextColor(WHITE);               // Цвет фона
  display.setCursor(0,0);                    // Установить позицию x, y для отображения
  //display.println(" OLED 0.96 TESTER ");     // Отправка текста

  Serial.begin(9600);
  configTime(0,0, "pool.ntp.org");
  time_t now = time(nullptr);
  /*
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  */
  Serial.println(now);
  
  secured_client.setTrustAnchors(&cert);
  //Serial.print("\r\n delay 60 sec begin");
  //delay(1000 * 60);
  Serial.print("\r\n delay 60 sec after");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int i = 1;
    while(WiFi.status() != WL_CONNECTED){
     Serial.print("\r\n\ Подключение к WiFi попытка №: " + String(i) );
     delay(1000 * 30); //  60sec
  };

  Serial.print("\r\n WiFi connected, IP:");
  localIP = IpAddress2String( WiFi.localIP());
  display.clearDisplay();   
  Serial.println(localIP);
  display.println("IP: " + localIP);
  
  //bot.sendMessage(HOME_CHAT_ID, "Свет включили!", "Markdown");
  //bot.sendMessage(GLEBOVA43_CHAT_ID, "Свет включили", "Markdown");
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
 // Serial.print("\r\n currentMillis: " + String(currentMillis));
}

void handleNewMsg(int num){
  Serial.print("\r\n enter handleNewMsg num:"  + String(num) );
  for(int i = 0; i < num; i++){
    Serial.print("\r\n chat_id: " + String(bot.messages[i].chat_id));
    String  CURR_CHAT_ID = bot.messages[i].chat_id;
    if (CURR_CHAT_ID == HOME_CHAT_ID /*|| CURR_CHAT_ID == GLEBOVA43_CHAT_ID*/ ){
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
        if (text == "/start"){
         bot.sendMessage(CURR_CHAT_ID, "i'm started", "");
        }       
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
             bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name + "Перезапуск...", ""  );
             needRestart = true;
          }
             else  if (text == "/uptime"){
             bot.sendMessage(CURR_CHAT_ID, "@" +  bot.messages[i].from_name + " Продолжительность работы: " + upTime(), "" );
             display.clearDisplay();   
             Serial.println(text);
             display.println(" Продолжительность работы: " + upTime());
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
                            "{\"command\":\"restart\",\"description\":\"Перезапуск бота\"},"
                            "{\"command\":\"uptime\",\"description\":\"Продолжительность работы в сети\"}" 
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
