// Example sketch for testing OLED display

// We need to include Wire.h for I2C communication
#include <Wire.h>
#include "OLED.h"

// Declare OLED display
// display(SDA, SCL);
// SDA and SCL are the GPIO pins of ESP8266 that are connected to respective pins of display.
OLED display(4, 5);

void setup() {
  Serial.begin(9600);
  Serial.println("OLED test!");

  // Initialize display
  display.begin();

  // Test message
  display.print("start");
  delay(3*1000);

  // Test long message
  //display.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
  //delay(3*1000);

  // Test display clear
  //display.clear();
  //delay(3*1000);

  // Test message postioning
  //display.print("TOP-LEFT");
  //display.print("4th row", 4);
  //display.print("RIGHT-BOTTOM", 7, 4);
  //delay(3*1000);

  // Test display OFF
  //display.off();
  //display.print("3rd row", 3, 8);
  //delay(3*1000);

  // Test display ON
  //display.on();
  //delay(3*1000);
}

int r = 0, c = 0;

void loop() {

  display.clear();

  display.print("George ...", 4, 4);

  delay(2000);
}
