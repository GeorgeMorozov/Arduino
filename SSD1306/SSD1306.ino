//#include <ESP8266WiFi.h>
#include <SPI.h>                        // Подключение библиотеки SPI
#include <Wire.h>                       // Подключение библиотеки Wire
#include <Adafruit_GFX.h>               // Подключение библиотеки Adafruit_GFX
#include <Adafruit_SSD1306.h>           // Подключение библиотеки Adafruit_SSD1306
#include <Fonts/FreeSerif9pt7b.h>
#define OLED_RESET                      16
Adafruit_SSD1306 display(OLED_RESET);

#define WIFI_SSID "george"//"HTC"//"george"//"George"//"Xiaomi 11T"
#define WIFI_PASSWORD  "1234567891"//"Z5N35W123"//"1234567891"  //"12345678"//"George1980"//"1234567891"
//#define WIFI_SSID "george"//"George"//"Xiaomi 11T"
//#define WIFI_PASSWORD  "1234567891"  //"12345678"//"George1980"//"1234567891"
//WiFiClientSecure secured_client;

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
double count=0;
String localIP;

// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {   
  display.cp437(true);             
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("setup begin");
  display.display();
  Serial.print("\r\n WiFi connected, IP: P");

  /*
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int i = 1;
    while(WiFi.status() != WL_CONNECTED){
     String msg = " \r\n\ Подключение к WiFi попытка №: " + String(i);
     Serial.print(msg);
      display.clearDisplay();
      display.println(msg);
      display.display();
     delay(1000 * 30); //  60sec
  };
  localIP = IpAddress2String( WiFi.localIP());
  Serial.print("\r\n WiFi connected, IP: " + localIP); 
  display.clearDisplay();   
  //display.println("IP: " + localIP);
  */
}


  void loop() {
    /*
    Serial.println("loop");
   display.clearDisplay(); 
  display.println(utf8rus("повтор"));
  display.display();
   delay(5000);
   */


} 

/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i,k;
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
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
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
String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}
*/