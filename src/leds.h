
#include "FastLED.h"
#define LED_Count   8
#define LED_Used    5
#define LED_PIN     3
#define LED_TYPE    WS2812B

CRGB leds[LED_Count];



void SetLED(int LED_index, CRGB  c) {
    leds[LED_index] = c;
    FastLED.setBrightness(100);
    FastLED.show();
}
void SetAllLED(CRGB  c) {

    for (int i = 0; i < LED_Used; i++) { leds[i] = c; }
    FastLED.setBrightness(100);
    FastLED.show();
}

void LED(int length, CRGB color, int del) {
    static boolean LEDConnected = false;

    if (!LEDConnected) {
        Serial.println("LED init..");
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

        LEDConnected = true;
    }
}
