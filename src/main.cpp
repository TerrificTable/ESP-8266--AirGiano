#include <Wire.h>
#include <SPI.h>

#include "screen.h"
#include "wlan.h"



void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\nStarting...");

    oled_init();

}


void loop() { 
    WLAN_Connect();
    WIFI_AccessPoint();
    WEB();


    static unsigned long    Timer       = 0;
    static boolean          pressed     = false;

    if (millis() > Timer ) {
        Timer = millis() + 10000;

        // Draw Image and GitHub Text and Follow Button

        srand(time(0));
        if ((rand() % 100) > 50)
            pressed = true;
        else
            pressed = false;

        drawImageAndText(pressed);
    }
}
