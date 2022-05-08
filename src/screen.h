#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

// OLED pins
#define SDA             D1
#define SCL             D2

#define OLED_RESET      0       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH    128     // OLED display width, in pixels
#define SCREEN_HEIGHT   64      // OLED display height, in pixels
#define CURSER_POS_X    0       // X Offsert        1 Character = 10
#define CURSER_POS_Y    0       // Y Offset         1 Line      = 16

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void DisplayText(String text) {
    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(CURSER_POS_X, CURSER_POS_Y);
    display.println(text);
    display.display();
}

void SCREEN() {
    static boolean OLEDConnected = false;

    if (!OLEDConnected) {
        Serial.println("OLED init..");
    
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        
        DisplayText("Hallo!");

        OLEDConnected = true;
    }


    static unsigned long TimerLEDAnimation = 0;

    if (millis() > TimerLEDAnimation ) {
        TimerLEDAnimation = millis() + 10;
    }
}
