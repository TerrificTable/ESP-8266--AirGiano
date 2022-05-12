#include <Wire.h>
#include <SPI.h>

#include "screen.h"
#include "wlan.h"
#include "sdc30.h"



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
    SDC30();


    static unsigned long    Timer       = 0;
    static boolean          pressed     = false;

    if (millis() > Timer ) {
        Timer = millis() + 10000;

        srand(time(0));
        if ((rand() % 100) > 50) pressed = true;
        else pressed = false;

        // Draw Image and GitHub Text and Follow Button
        drawImageAndText(pressed);
    }
}
