#include "funcs.h"

void init(Adafruit_SSD1306* display) {
  Serial.begin(9600);
  Serial.println("start");
  pinMode(LED_BUILTIN, OUTPUT);
  display->cp437(true);

  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(0, 0);
  display->println(utf8rus("Старт"));
}
/*--------------------------------------------------------*/

void initWiFi(Adafruit_SSD1306* display) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int  i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    i++;
    Serial.println("Connect2WiFi # " + String(i) );
    display->clearDisplay();
    display->setCursor(0, 0);
    display->println("Connect2WiFi #" + String(i));
    display->display();
    delay(1000 * 5); //  30sec
  };

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  String localIP = IpAddress2String( WiFi.localIP());
  String msg = "WiFi Connected, IP: " + localIP;
  Serial.print(msg);
  display->println(msg);
  display->display();
}
/*--------------------------------------------------------*/ 
void handleNewMsg(int num, UniversalTelegramBot* bot, Adafruit_SSD1306* display) {
  String localIP = IpAddress2String( WiFi.localIP());
  //Serial.print("\r\n enter handleNewMsg num:"  + String(num) );

  for (int i = 0; i < num; i++) {
    Serial.print("\r\n chat_id: " + String(bot->messages[i].chat_id));
    String  CURR_CHAT_ID = bot->messages[i].chat_id;
    if (CURR_CHAT_ID ==  bot->messages[i].chat_id /*HOME_CHAT_ID || CURR_CHAT_ID == GLEBOVA43_CHAT_ID*/ ) {
      String text = bot->messages[i].text;
      int message_id =  bot->messages[i].message_id;
      String from_id = bot->messages[i].from_id;
      String msg = "новое сообщение:";

      int reply_to_message_id =  bot->messages[i].reply_to_message_id;
      if (text.indexOf(BOT_NAME)) {
        text.replace(BOT_NAME, "");
        text.toLowerCase();
      }
      Serial.print("\r\n text: " + text);
      display->println(utf8rus("новое сообщение:"));
      display->println("msg: '" + utf8rus( text ) + "'");
      display->display();

      //Serial.print("\r\n message_id: " + String(message_id) );
      //Serial.print("\r\n from_id: " + String(from_id) );
      //Serial.print("\r\n reply_to_message_id: " + String(reply_to_message_id) );

      //display.clearDisplay();
      //display.setCursor(0,24);

      //delay(2000);

      if (text == "/start") {
        bot->sendMessage(CURR_CHAT_ID, "i'm started", "");
      }
      if (text == "/alive") {
        bot->sendMessage(CURR_CHAT_ID, "i'm Alive", "");
      }

      else if (text == "/on") {
        digitalWrite(LED_BUILTIN, LOW);
        bot->sendMessage(HOME_CHAT_ID, "*light On*" , "Markdown");
        //bot.sendSticker(HOME_CHAT_ID, "CAACAgIAAx0CYgffogACASRjvd585U6oehUaMyelCi7tVj8XQQACMwcAAkRTwUmByLxH2zzKqC0E");
        //bot.sendPhoto(HOME_CHAT_ID, "CAACAgIAAx0CYgffogACASRjvd585U6oehUaMyelCi7tVj8XQQACMwcAAkRTwUmByLxH2zzKqC0E", "This photo was sent using File ID");
      }
      else if (text == "/off") {
        digitalWrite(LED_BUILTIN, HIGH);
        bot->sendMessage(HOME_CHAT_ID, "_light Off_", "Markdown");
      }
      else if (text == "/IP") {
        bot->sendMessage(HOME_CHAT_ID, "my IP: " + localIP + " [inline URL](http://www.example.com/)", "Markdown");
      }
      /*
      else  if (text == "/current") {
        char currTime[20];
        strftime(currTime, 20, "%H:%M:%S - %d-%m-%YY ", localtime(&now));
        bot->sendMessage(HOME_CHAT_ID, String(currTime), "");
      }*/

      if (text == "/restart") {
        bot->sendMessage(CURR_CHAT_ID, "@" +  bot->messages[i].from_name + " Перезапуск...", ""  );
        //needRestart = true;
      }
      else  if (text == "/uptime") {
        bot->sendMessage(CURR_CHAT_ID, "@" +  bot->messages[i].from_name + " Продолжительность работы: " + upTime(), "" );
      }
    }
  }
}

/*--------------------------------------------------------*/
String upTime() {
  uint32_t sec = millis() / 1000ul;      // полное количество секунд
  int timeHours = (sec / 3600ul);        // часы
  int timeMins = (sec % 3600ul) / 60ul;  // минуты
  int timeSecs = (sec % 3600ul) % 60ul;  // секунды

  return (timeHours < 10 ? "0" + String( timeHours ) : String( timeHours ) ) + ":" + (timeMins < 10 ? "0" + String( timeMins ) : String( timeMins ) ) + ":" + ( timeSecs < 10 ? "0" + String(timeSecs) : String(timeSecs));
}
/*--------------------------------------------------------*/
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}

/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
            n = source[i]; i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
          }
        case 0xD1: {
            n = source[i]; i++;
            if (n == 0x91) {
              n = 0xB8;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
          }
      }
    }
    m[0] = n; target = target + String(m);
  }
  return target;
}

/*
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi, trying to connect...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}
*/

/*
void display_message(String message){
   for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    //matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background off, reverse to invert the image
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    delay(wait/2);
  }
}
*/