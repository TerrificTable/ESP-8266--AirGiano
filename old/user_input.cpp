// #include <Fonts/FreeSans9pt7b.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <Wire.h>
#include <SPI.h>

// OLED pins
#define SDA D1
#define SCL D2

#define OLED_RESET 0     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String convert(char chIn[]) {
    String result = "";

    for (unsigned int i = 0; i < strlen(chIn); i++) {
        result += chIn[i];
    }

    return result;
}

void MQTTCallBack()
{
    char in[500];

    Serial.print("Message: ");
    std::cin.getline(in, 500);

    String str = convert(in);

    Serial.println("MQTT:LOCAL : " + str);

    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10, 0);
    display.println(str);
    display.display();
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10, 0);
    display.println("HALLO!");
    display.display();

    MQTTCallBack();
}

void loop()
{
    // MQTTCallBack();
}
