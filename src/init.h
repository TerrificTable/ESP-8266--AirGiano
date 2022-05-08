
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include "screen.h"
#include "leds.h"
#include "wlan.h"

void setup() {
    Serial.begin(115200);
    delay(100);
    
    Serial.println("\nBooting...");
    DisplayText("S\ntarting...");
}
