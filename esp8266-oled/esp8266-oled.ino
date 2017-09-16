#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   D7 //Connect to D1 on OLED
#define OLED_CLK    D5 //Connect to D0 on OLED 
#define OLED_DC     D1 //Connect to DC on OLED
#define OLED_CS     D8 //Connect to CS on OLED
#define OLED_RESET  D3 //Connect to RES on OLED

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
int counter = 0;

const char* ssid     = "";
const char* password = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.print("Connecting to ");
  display.println(ssid);
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }

  display.clearDisplay();
  display.display();

  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(2);
  display.println(timeClient.getFormattedTime());
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.println("");
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
}
