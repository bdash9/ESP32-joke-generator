/*
  ESP32 HTTPClient Jokes API Example

  https://wokwi.com/projects/342032431249883731

  Copyright (C) 2022, Uri Shaked
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

const char* ssid = "AP_name";
const char* password = "password";

/*
#define BTN_PIN 5
#define TFT_DC 2
#define TFT_CS 15
*/
//Adafruit_ST77XX tft = Adafruit_ST77XX(TFT_CS, TFT_DC);
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);


const String url = "https://v2.jokeapi.dev/joke/Programming";

String getJoke() {
  HTTPClient http;
  http.useHTTP10(true);
  http.begin(url);
  http.GET();
  String result = http.getString();

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, result);

  // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "<error>";
  }

  String type = doc["type"].as<String>();
  String joke = doc["joke"].as<String>();
  String setup = doc["setup"].as<String>();
  String delivery = doc["delivery"].as<String>();
  http.end();
  return type.equals("single") ? joke : setup + "  " + delivery;
}

void nextJoke() {
  tft.setTextColor(ST77XX_WHITE);
  tft.println("\nLoading joke...");  
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);

  String joke = getJoke();
  tft.setTextColor(ST77XX_GREEN);
  tft.println(joke);
}

void setup() {
   // turn on backlight
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  WiFi.begin(ssid, password, 6);
  tft.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    tft.print(".");
  }

  tft.print("\nOK! IP=");
  tft.println(WiFi.localIP());

  nextJoke();
}

void loop() {
  //if (digitalRead(BTN_PIN) == LOW) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    nextJoke();
  //}

  delay(10000);
}