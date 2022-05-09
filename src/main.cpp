
#include "screen.h"
#include "wlan_leds.h"




void setup() {
    Wire.begin();
    Serial.begin(115200);
    delay(100);
    
    Serial.println("\nBooting...");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    DisplayText("\nStarting...");
}


void loop(){
    WLAN();
    MQTT(); 
    LED(5, CRGB::Green, 30);
}
