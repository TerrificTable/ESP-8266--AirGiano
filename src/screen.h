
#include <Arduino.h>
#include <Adafruit_SSD1306.h>



#define SDA             D1
#define SCL             D2

#define OLED_RESET      0
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// IMG -> C/C++ Array     <->     https://lvgl.io/tools/imageconverter
static const uint8_t image_data_array[488] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0x57, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfe, 0xa5, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfa, 0xa2, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfd, 0x45, 0x7f, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xf0, 0xa0, 0xbf, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xe8, 0x40, 0xef, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xa8, 0x00, 0x57, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xe0, 0x00, 0x15, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xa1, 0x00, 0x0b, 0x7f, 0xff, 0xf0, 
  0xff, 0xff, 0x41, 0x40, 0x00, 0xbf, 0xff, 0xf0, 
  0xff, 0xff, 0x81, 0xff, 0x00, 0x5f, 0xff, 0xf0, 
  0xff, 0xff, 0x02, 0xff, 0xe8, 0x17, 0xff, 0xf0, 
  0xff, 0xfd, 0x03, 0xff, 0xfe, 0x01, 0xff, 0xf0, 
  0xff, 0xfe, 0x05, 0xff, 0xfd, 0x05, 0xff, 0xf0, 
  0xff, 0xf5, 0x0f, 0xff, 0xfe, 0xa2, 0xff, 0xf0, 
  0xff, 0xfa, 0x1f, 0xff, 0xfd, 0x01, 0xff, 0xf0, 
  0xff, 0xd4, 0x2f, 0xff, 0xfe, 0x01, 0x7f, 0xf0, 
  0xff, 0xf0, 0x31, 0xff, 0xff, 0x01, 0x7f, 0xf0, 
  0xff, 0xa8, 0x20, 0xaf, 0xfd, 0x80, 0xbf, 0xf0, 
  0xff, 0xf0, 0x40, 0xd5, 0x57, 0xe0, 0xff, 0xf0, 
  0xff, 0xa8, 0x00, 0x02, 0x5d, 0x50, 0x5f, 0xf0, 
  0xff, 0xb0, 0x20, 0x00, 0x02, 0xb0, 0x5f, 0xf0, 
  0xff, 0xa0, 0x00, 0x00, 0x00, 0x08, 0x3f, 0xf0, 
  0xff, 0x50, 0x00, 0x00, 0x01, 0x00, 0x57, 0xf0, 
  0xff, 0xc1, 0x40, 0x00, 0x00, 0x00, 0x1f, 0xf0, 
  0xfe, 0x81, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xf0, 
  0xff, 0x01, 0xa0, 0x00, 0x00, 0x00, 0x17, 0xf0, 
  0xfd, 0x46, 0xa0, 0x00, 0x00, 0x00, 0x2f, 0xf0, 
  0xff, 0x8a, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xf0, 
  0xff, 0x45, 0x00, 0x3a, 0xaa, 0x00, 0x2f, 0xf0, 
  0xff, 0x80, 0x00, 0x55, 0xea, 0x00, 0x5f, 0xf0, 
  0xff, 0x40, 0x00, 0x7b, 0x55, 0x20, 0x2f, 0xf0, 
  0xff, 0xa0, 0x00, 0xbf, 0xf6, 0x00, 0x5f, 0xf0, 
  0xff, 0xc0, 0x01, 0x7f, 0xfd, 0x10, 0xaf, 0xf0, 
  0xff, 0xd0, 0x02, 0xff, 0xfe, 0x00, 0x5f, 0xf0, 
  0xff, 0xaa, 0x01, 0xff, 0xff, 0x00, 0xaf, 0xf0, 
  0xff, 0xd4, 0x02, 0xff, 0xde, 0x00, 0x7f, 0xf0, 
  0xff, 0xb5, 0x03, 0xff, 0x1e, 0x00, 0xaf, 0xf0, 
  0xff, 0xea, 0x05, 0xfe, 0x1a, 0x01, 0x7f, 0xf0, 
  0xff, 0xb6, 0x83, 0xfe, 0x14, 0x08, 0xbf, 0xf0, 
  0xff, 0xf7, 0x01, 0xf4, 0x10, 0x01, 0xff, 0xf0, 
  0xff, 0xf6, 0x41, 0x3c, 0x00, 0x09, 0x7f, 0xf0, 
  0xff, 0xf6, 0x21, 0x14, 0x00, 0x15, 0xff, 0xf0, 
  0xff, 0xf6, 0x60, 0x14, 0x00, 0x0a, 0xff, 0xf0, 
  0xff, 0xff, 0x80, 0x00, 0x00, 0x57, 0xff, 0xf0, 
  0xff, 0xfd, 0x40, 0x00, 0x00, 0xad, 0xff, 0xf0, 
  0xff, 0xff, 0xa0, 0x00, 0x01, 0x57, 0xff, 0xf0, 
  0xff, 0xff, 0x50, 0x00, 0x01, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xe0, 0x00, 0x02, 0xbf, 0xff, 0xf0, 
  0xff, 0xff, 0xd4, 0x00, 0x0b, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfa, 0x00, 0x12, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xf5, 0x00, 0x17, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0x40, 0x2b, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfd, 0xa0, 0x57, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0x54, 0xaf, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xac, 0xd7, 0xff, 0xff, 0xf0, 
};





void oled_init() {

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

}


void DisplayText(String text, int text_size = 2) {

    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(text_size);
    display.setCursor(0, 0);
    display.println(text);
    display.display();

}



void drawButton(int x, int y, int w, int h, String text, int font_length, uint16_t color, int font_size = 1) {
        
    display.drawRoundRect(x, y, w, h, 3, color);

    display.setTextSize(font_size);
    display.setTextColor(color);
    display.setCursor((w / 2) + x - /* String length */     font_length, 
                      (h / 2) + y - /* Font height   */ 4 * font_size);
    display.println(text);

    display.display();

}

void drawButtonPressed(int x, int y, int w, int h, String text, int font_length, uint8_t color, int font_size = 1) {

    display.drawRoundRect(x, y, w, h, 3, color);
    display.fillRoundRect(x, y, w, h, 3, color);

    display.setTextSize(font_size);
    display.setTextColor(!color);
    display.setCursor((w / 2) + x - /* String length */     font_length, 
                      (h / 2) + y - /* Font height   */ 4 * font_size + 1);
    display.println(text);

    display.display();

}




void drawInfo() {

    display.setTextColor(1);
    display.setTextColor(WHITE);

    display.setCursor(80, 5);
    display.println("GitHub");
    
    display.drawLine(70, 17, 128, 17, WHITE);

    display.setCursor(73, 22);
    display.println("Terrific");

    display.setCursor(73, 30);
    display.println("Table");

    display.display();

}



void drawImageAndText(boolean pressed) {
    static boolean old_pressed = pressed;

    
    if (old_pressed != pressed) {
        display.clearDisplay();
        display.display();
    }
    


    // ╔══════════════════╗
    // ║                  ║     GitHub
    // ║                  ║ ══════════════
    // ║     PROFILE      ║   Terrific
    // ║     PICTURE      ║   Table
    // ║                  ║  ╔══════════╗
    // ║                  ║  ║  Follow  ║
    // ╚══════════════════╝  ╚══════════╝


    // IMAGE
    display.drawBitmap(0, 2, image_data_array, 60, 60, 1);
    display.fillRect(0, 0, 60, 2, WHITE);
    display.fillRect(0, 62, 60, 64, WHITE);


    // GITHUB TEXT
    drawInfo();

    // BUTTON
    if (pressed)
        drawButtonPressed(70, 45, 55, 19, "Followed", 23, WHITE, 1);
    else
        drawButton(70, 45, 55, 19, "Follow", 18, WHITE, 1);
    

    display.display();
    old_pressed = pressed;
}


