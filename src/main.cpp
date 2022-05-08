#include "init.h"

void loop() {
    // INTERNET STUFF
    WLAN();
    MQTT();
    WEB();

    // LEDS
    LED();
    // SCREEN
    SCREEN();
}
