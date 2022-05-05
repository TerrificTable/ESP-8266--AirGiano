#include <ESP8266WiFi.h>

#include <time.h>
#include <chrono>
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

#define     CONFIG_h
#include    "config.h"

// OLED pins
#define SDA D1
#define SCL D2

#define OLED_RESET 0     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#ifndef STASSID
#define STASSID     name        // WLAN Name
#define STAPSK      passwd      // WLAN Password
#endif

const char *ssid = STASSID;
const char *password = STAPSK;


typedef struct {
    boolean         WLANConnected               = false;  
    boolean         WLANConnecting              = false;  
    unsigned long   WLANConnectingTimeOut       = 0;  
    unsigned long   WLANConnectingWaitTimeOut   = 0; 


    boolean         updateTime                  = false;        
    unsigned long   timeout                     = 0;      


    boolean         LEDConnected                = false;
} StatesData;

StatesData States;


#include "FastLED.h"
#define LED_Count   8
#define LED_Used    5
#define LED_PIN     3
#define LED_TYPE    WS2812B

#define CURSER_Pos  0

CRGB        leds[LED_Count];
WiFiClient  espClient;


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
    auto dayInSeconds = 86400;
    auto germamyPlusTime = 7200;
    auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch());
    auto timeWithoutDays = currentTime % dayInSeconds;
    auto currentTimeInSeconds = timeWithoutDays.count() + germamyPlusTime;
    
    int hour = currentTimeInSeconds / 60 / 60u;
    int minute = (currentTimeInSeconds / 60) - (hour * 60u);
    int second = currentTimeInSeconds - (hour * 60 * 60) - (minute * 60u);
    
    String hours = String(hour);
    String minutes = String(minute);
    String seconds = String(second);


//    hs = righT("00" + h,2) 
    if (hour >= 24) hour = 0;

    if (hour < 10)  hours = "0" + String(hour);
    if (minute < 10) minutes = "0" + String(minute);
    if (second < 10) seconds = "0" + String(second);

    return hours + ":" + minutes + ":" + seconds;
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
        Serial.println("\nLED init..");
        DisplayText("\nLED init..");
        
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
        currTime.replace("\n", "  ");
        Serial.println(currTime);


        States.timeout = millis() + 1000;
        States.updateTime = false;
    }
}

void WLAN() {
    if (! States.WLANConnected &&  WiFi.status() == WL_CONNECTED) {
        States.WLANConnecting = false;
        States.WLANConnectingWaitTimeOut = 0;
        Serial.print("WiFi Connected: ");
        String IP = WiFi.localIP().toString();

        Serial.println(IP);
        DisplayText("WiFi: \n" + IP);
    }

    States.WLANConnected = WiFi.status() == WL_CONNECTED || WiFi.status()  == WL_IDLE_STATUS;
    
    if (States.WLANConnected) return;  
    
    
    if (States.WLANConnectingWaitTimeOut > 0 && millis() < States.WLANConnectingWaitTimeOut ) return;
    if (States.WLANConnectingWaitTimeOut > 0 && millis() > States.WLANConnectingWaitTimeOut ) States.WLANConnectingWaitTimeOut = 0;


    if (!States.WLANConnecting) {
        States.WLANConnecting = true;
        States.WLANConnectingTimeOut = millis() + 10000;  // 10 Sekunfen
        Serial.println("WiFi connecting....");
        DisplayText("WiFi connecting....");
        
        WiFi.persistent(false); 
        WiFi.mode( WIFI_OFF );
        delay( 1 );
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        return;
    }

    if (States.WLANConnecting &&  millis() > States.WLANConnectingTimeOut) {
        States.WLANConnecting  = false;
        Serial.println("WiFi connecting failed....");
        DisplayText("WiFi Failed");
        States.WLANConnectingWaitTimeOut = millis() + 10000;
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);Serial.println("\nStarting...");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    DisplayText("\nStarting...");
}

void loop(){
    WLAN();

    LED(5, CRGB::Cyan, 30);
    mainFunc();
}
