#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/RobotoMono4pt.h>

// If using software SPI (the default case):
#define OLED_MOSI   D7 //Connect to D1 on OLED
#define OLED_CLK    D5 //Connect to D0 on OLED 
#define OLED_DC     D1 //Connect to DC on OLED
#define OLED_CS     D8 //Connect to CS on OLED
#define OLED_RESET  D3 //Connect to RES on OLED
#define MODE_SWITCH D0 //Button to change mode

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
int mode = 0;

const char* ssid     = "";
const char* password = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String weather = String();

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setFont(&RobotoMono4pt);
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
  int buttonState = digitalRead(MODE_SWITCH);
  if (buttonState == HIGH) {
    if (mode == 0) {
      get_weather();
      mode = 1;
    } else if (mode == 1) {
      mode = 0;
    }
    delay(200);
  }
  switch (mode) {
    case 0:
      timeClient.update(); 
      display.clearDisplay();
      display.setCursor(0,20);
      display.setFont(NULL);
      display.setTextColor(BLACK, WHITE);
      display.setTextSize(2);
      display.setTextWrap(true);
      display.println(timeClient.getFormattedTime());
      display.setFont(&RobotoMono4pt);
      display.setTextColor(WHITE, BLACK);
      display.setTextSize(1);
      display.println("");
      display.println("IP address: ");
      display.println(WiFi.localIP());
      display.display();
      break;
    case 1:
      break;
  }

}

void get_weather () {
  HTTPClient http;
  http.setUserAgent("ESP8266 (like curl/7.43.0)");
  http.begin("http://wttr.in/London?0TQM");
  int httpCode = http.GET();
  Serial.println(httpCode);
  
  if(httpCode > 0) {
    weather = http.getString();
    weather = parse_weather_string(weather);
    Serial.println(weather);
  }

  http.end();
  display.clearDisplay();
  display.setCursor(0,10);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print(weather);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}

String parse_weather_string(String input_str) {
    input_str.replace("―", "-");
    input_str.replace("°", "");
    input_str.replace("↘", "");
    input_str.replace("↗", "");
    input_str.replace("↖", "");
    input_str.replace("↙", "");
    input_str.replace("’", "'");
    while (true) {
      if (input_str.indexOf(" \n") < 1) {
        break;
      }
      input_str.replace(" \n", "\n");
    }
    /*
    int sos = 0;
    String inputtmp = String();
    for (int i = 0; i < 5; i++) {
      Serial.println(sos);
      Serial.println(inputtmp);
      inputtmp += input_str.substring(sos, sos+15);
      sos = input_str.indexOf("\n", sos+16);
    }
    */
    
    return input_str;
}
