#include <time.h>
#include <string>
#include <iostream>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

// OLED pins
#define SDA D1
#define SCL D2

#define OLED_RESET 0     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


typedef struct {
    boolean         LEDConnected                = false;  
    boolean         updateTime                  = false;
    unsigned long   timeout                     = 0;            
} StatesData;

StatesData States;


#include "FastLED.h"
#define LED_Count   8
#define LED_Used    5
#define LED_PIN     3
#define LED_TYPE    WS2812B

#define CURSER_Pos  0

CRGB leds[LED_Count];


String convert_stdstring_string(std::string str) {
    char stage_1[str.length() + 1];
    strcpy(stage_1, str.c_str());
    String result = "";

    for (unsigned int i = 0; i < str.length(); i++) {
        result += stage_1[i];
    }

    return result;
}
std::string convert_string_stdstring(String str) {
    char stage_1[str.length() + 1];
    strcpy(stage_1, str.c_str());
    std::string result = "";

    for (unsigned int i = 0; i < str.length(); i++) {
        result += stage_1[i];
    }

    return result;
}
String currentDate() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d.%m.%Y", &tstruct);

    return convert_stdstring_string(buf);
}
String currentTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return convert_stdstring_string(buf);
}
void SetAllLED(CRGB  c) {

    for (int i = 0; i < LED_Used; i++) { leds[i] = c; }
    FastLED.setBrightness(100);
    FastLED.show();
}
void SetLED(int LED_index, CRGB  c) {
    leds[LED_index] = c;
    FastLED.setBrightness(100);
    FastLED.show();
}
void DisplayText(String text) {
    display.clearDisplay();
    display.display();
    
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(CURSER_Pos, 0);
    display.println(text);
    display.display();
}

void LED(int length, CRGB color, int del) {

    if (!States.LEDConnected) {
        Serial.println("LED init..");
        DisplayText("LED init...");
        
        FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, LED_Count);  
        SetAllLED(CRGB::Black);
        FastLED.setBrightness(100);              
        FastLED.clear();  // clear all pixel data
        FastLED.show();

        for (int a = 0; a < length; a++) {
            for (int i = 0; i < LED_Used; i++) { 
                leds[i] = color; 
                FastLED.show();
                delay(del);
                leds[i] = CRGB::Black; 
                FastLED.show();  
            }
        }

        FastLED.setBrightness(30);              

        States.LEDConnected = true;
    }
}

void mainFunc() {

    if (States.timeout > 0 && millis() < States.timeout) return;
    if (States.timeout > 0 && millis() > States.timeout) States.timeout  = 0;

    if (!States.updateTime) States.updateTime = true;

    if (States.updateTime && millis() > States.timeout) {
        String currTime = currentTime() + "\nHallo\n\n" + currentDate();


        DisplayText(currTime);
        Serial.println(currTime);

        // display.clearDisplay();
        // display.display();

        // display.setTextColor(WHITE);
        // display.setTextSize(2);
        // display.setCursor(CURSER_Pos, 0);
        // display.println(currTime);
        // display.display();


        States.timeout = millis() + 1000;
        States.updateTime = false;
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);Serial.println("\nBooting...");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    DisplayText("\nStarting...");
}

void loop(){
    LED(5, CRGB::Cyan, 30);
    mainFunc();
}
