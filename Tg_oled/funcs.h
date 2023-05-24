#ifndef FUNCS_H
#define FUNCS_H

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <UniversalTelegramBot.h>
#include "defs.h"

void init(Adafruit_SSD1306* display);
void initWiFi(Adafruit_SSD1306* display);
void handleNewMsg(int num, UniversalTelegramBot* bot, Adafruit_SSD1306* display);

String upTime();
String IpAddress2String(const IPAddress& ipAddress);
String utf8rus(String source);

//void onWifiConnect(const WiFiEventStationModeGotIP& event);
//void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);

#endif
