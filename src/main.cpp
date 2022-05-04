#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <SPI.h>
#include <Wire.h>
#include <stdio.h>
#include <iostream>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

// OLED pins
#define SDA D1
#define SCL D2

#define OLED_RESET 0     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#ifndef STASSID
#define STASSID "AMIGO Event 2021"
#define STAPSK "HrFC4G&nwm"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
char MQTT_Server[61] = "192.168.100.75";
int MQTT_Port = 1883;
char MQTT_UID[61] = "mqtt";
char MQTT_PWD[61] = "terrific";

WiFiClient espClient;
PubSubClient MQTTClient(espClient);

typedef struct {
    boolean         WLANConnected               = false;  
    boolean         WLANConnecting              = false;  
    unsigned long   WLANConnectingTimeOut       = 0;  
    unsigned long   WLANConnectingWaitTimeOut   = 0;  

    boolean         MQTTConnected               = false;  
    boolean         MQTTConnecting              = false;  
    unsigned long   MQTTConnectingTimeOut       = 0;  
    unsigned long   MQTTConnectingWaitTimeOut   = 0;

    boolean         LEDConnected                = false;  


} StatesData;

StatesData States;


#include "FastLED.h"
#define LED_Count   8
#define LED_Used    5
#define LED_PIN     3
#define LED_TYPE    WS2812B

CRGB leds[LED_Count];




void SetAllLED(CRGB  c) {

    for (int i = 0; i < LED_Used; i++) { leds[i] = c; }
    FastLED.setBrightness(100);
    FastLED.show();
}

void SetLED(int LED_index, CRGB  c) {
    leds[LED_index] = c;
    FastLED.setBrightness(100);
    FastLED.show();
}

void LED(int length, CRGB color, int del) {

    if (!States.LEDConnected) {
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

        States.LEDConnected = true;
    }
}

String convert_stdstring_string(std::string str) {
    char stage_1[str.length() + 1];
    strcpy(stage_1, str.c_str());
    String result = "";

    for (unsigned int i = 0; i < str.length(); i++) {
        result += stage_1[i];
    }

    return result;
}

std::string convert_string_stdstring(String str) {
    char stage_1[str.length() + 1];
    strcpy(stage_1, str.c_str());
    std::string result = "";

    for (unsigned int i = 0; i < str.length(); i++) {
        result += stage_1[i];
    }

    return result;
}


void MQTTCallBack(char *Topic, byte *payload, unsigned int length) {
    String rcv = "";

    for (unsigned int i = 0; i < length; i++)
    {
        rcv += (char)payload[i]; // convert *byte to string
    }

    std::string msg = convert_string_stdstring(rcv);
    rcv.replace("\n", "\\n");

    Serial.println("MQTT:" + String(Topic) + " : " + rcv);

    if (msg.find(".leds") != std::string::npos) {
        States.LEDConnected = false;
        std::string sdelay;
        std::string slen;


        std::string dmsg = msg;
        std::string lmsg = msg;
        if (msg.find("-d") != std::string::npos) {
            sdelay = dmsg.substr(msg.find("-d") + 2, msg.find("_ED_") - msg.find("-d") + 2 - 4);
        }
        if (msg.find("-l") != std::string::npos) {
            slen = lmsg.substr(msg.find("-l") + 2, msg.find("_EL_") - msg.find("-l") + 2 - 4);
        }

        int del = std::atoi(sdelay.c_str());
        int len = std::atoi(slen.c_str());

        if (len != 0 && del != 0) {
            if (msg.find("red") != std::string::npos) {
                LED(len, CRGB::Red, del);
            }
            else if (msg.find("green") != std::string::npos) {
                LED(len, CRGB::Green, del);
            }
            else if (msg.find("blue") != std::string::npos) {
                LED(len, CRGB::Blue, del);
            } else {
                LED(len, CRGB::Yellow, del);
            }
        } else {
            if (msg.find("red") != std::string::npos) {
                LED(3, CRGB::Red, 30);
            }
            else if (msg.find("green") != std::string::npos) {
                LED(3, CRGB::Green, 30);
            }
            else if (msg.find("blue") != std::string::npos) {
                LED(3, CRGB::Blue, 30);
            } else {
                LED(3, CRGB::Yellow, 30);
            }
        }
        
    } else {
        display.clearDisplay();
        display.display();

        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(10, 0);
        display.println(convert_stdstring_string(msg));
        display.display();
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);Serial.println("\nBooting...");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10, 0);
    display.println("HALLO!");
    display.display();
}

void WLAN() {
    if (! States.WLANConnected &&  WiFi.status() == WL_CONNECTED) {
        States.WLANConnecting = false;
        States.WLANConnectingWaitTimeOut = 0;
        Serial.print("WiFi Connected : ");
        Serial.println(WiFi.localIP());
    }

    States.WLANConnected = WiFi.status() == WL_CONNECTED || WiFi.status()  == WL_IDLE_STATUS;
    
    if (States.WLANConnected) return;  
    
    /*
    typedef enum {
        WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
        WL_IDLE_STATUS      = 0,
        WL_NO_SSID_AVAIL    = 1,
        WL_SCAN_COMPLETED   = 2,
        WL_CONNECTED        = 3,
        WL_CONNECT_FAILED   = 4,
        WL_CONNECTION_LOST  = 5,
        WL_DISCONNECTED     = 6
    } wl_status_t;
    */
    //Serial.println(WiFi.status());
    
    if (States.WLANConnectingWaitTimeOut > 0 && millis() < States.WLANConnectingWaitTimeOut ) return;
    if (States.WLANConnectingWaitTimeOut > 0 && millis() > States.WLANConnectingWaitTimeOut ) States.WLANConnectingWaitTimeOut = 0;


    if (!States.WLANConnecting) {
        States.WLANConnecting = true;
        States.WLANConnectingTimeOut = millis() + 10000;  // 10 Sekunfen
        States.MQTTConnected = false;
        Serial.println("WiFi connecting....");
        
        WiFi.persistent(false); 
        WiFi.mode( WIFI_OFF );
        delay( 1 );
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        return;
    }

    if (States.WLANConnecting &&  millis() > States.WLANConnectingTimeOut) {
        States.WLANConnecting  = false;
        Serial.println("WiFi connecting failed....");
        States.WLANConnectingWaitTimeOut = millis() + 10000;
    }
}

void MQTT() {
    if (! States.WLANConnected) return;

    if (States.MQTTConnecting && !States.MQTTConnected && MQTTClient.connected()){
        States.MQTTConnecting = false;
        States.MQTTConnectingTimeOut = 0;
        States.MQTTConnectingWaitTimeOut = 0;

        MQTTClient.setCallback(MQTTCallBack);
        Serial.println("MQTTClient Connected");
        
        String Tmp = "AirGiano/recv";
        Serial.println("MQTTClient Subscribe: " + Tmp);
    
        MQTTClient.subscribe(Tmp.c_str());
    }

    States.MQTTConnected = MQTTClient.connected();
   
    if (States.MQTTConnected ) {
        MQTTClient.loop();
        return;
    }

    if (States.MQTTConnecting && States.MQTTConnectingTimeOut       > 0 && millis() < States.MQTTConnectingTimeOut      ) return;
    if (!States.MQTTConnected && States.MQTTConnectingWaitTimeOut   > 0 && millis() < States.MQTTConnectingWaitTimeOut  ) return;

    if (!States.MQTTConnecting) { 
        States.MQTTConnecting = true;
        States.MQTTConnectingTimeOut = millis() + 10000;  // 10 Sekunden
     
        String Tmp =  String(WiFi.macAddress()); 
        Tmp.replace(":","");
        Tmp =  "AIRGIANO_" + Tmp;
        Tmp.toLowerCase();

        Serial.println("MQTTClient Connecting " + Tmp + " to " + MQTT_Server +":" + String(MQTT_Port));
        MQTTClient.setServer(MQTT_Server, MQTT_Port);
        MQTTClient.connect(Tmp.c_str(), MQTT_UID, MQTT_PWD);
        return;
    }

   
    if (States.MQTTConnecting &&  millis() > States.MQTTConnectingTimeOut) {
        States.MQTTConnecting  = false;
        Serial.println("MQTTClient connecting failed....");
        States.MQTTConnectingTimeOut = 0;
        States.MQTTConnectingWaitTimeOut = millis() + 10000;
    }
    
    if (! States.MQTTConnecting &&  millis() > States.MQTTConnectingWaitTimeOut) {
        States.MQTTConnectingWaitTimeOut = 0;
    }

    return ;    // Waiting for MQTT connection
}

void loop(){
    WLAN();
    MQTT(); 
    LED(5, CRGB::Green, 30);
}
