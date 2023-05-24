  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64//32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

    Serial.begin(9600);
  Serial.println("start");


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  // put your setup code here, to run once:
    display.clearDisplay();
    display.println("setup");
    display.display();

}

void loop() {
  // put your main code here, to run repeatedly:
display.clearDisplay();
    display.println("loop");
    display.display();
}
