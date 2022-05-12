#include <Adafruit_SSD1306.h>


// OLED pins
#define SDA             D1
#define SCL             D2

#define OLED_RESET      0       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH    128     // OLED display width, in pixels
#define SCREEN_HEIGHT   64      // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define CURSER_POS  0



void DisplayText(String text) {
    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(CURSER_POS, 0);
    display.println(text);
    display.display();
}
