#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <SPI.h>
#include <Wire.h>
#include <stdio.h>
#include <iostream>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

#include "json/json.h"

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

void MQTTCallBack(char *Topic, byte *payload, unsigned int length) {
    String rcv = "";

    for (unsigned int i = 0; i < length; i++)
    {
        rcv += (char)payload[i]; // convert *byte to string
    }

    std::string msg = convert_string_stdstring(rcv);
    rcv.replace("\n", "\\n");

    Serial.println("MQTT:" + String(Topic) + " : " + rcv);

    // TODO: Parse Json
    // =============
    std::stringstream ss(msg); //simulating an response stream
    const unsigned int BUFFERSIZE = 256;

    //temporary buffer
    char buffer[BUFFERSIZE];
    memset(buffer, 0, BUFFERSIZE * sizeof(char));

    //returnValue.first holds the variables name
    //returnValue.second holds the variables value
    std::pair<std::string, std::string> returnValue;




    // --------------- CUT HERE FOR EXCEPTION DECODER ---------------
    // Soft WDT reset

    // >>>stack>>>

    // ctx: cont
    // sp: 3ffffad0 end: 3fffffc0 offset: 01a0
    // 3ffffc70:  00000000 3fff1d4c 3ffef1f8 4021f607
    // 3ffffc80:  3ffffcd0 00000100 3ffffdd0 402176e9
    // 3ffffc90:  3ffffe00 3ffef1f8 3ffffeb4 fffffff9  
    // 3ffffca0:  3ffffeb4 3ffef1f8 3ffffdd0 4021773c
    // 3ffffcb0:  3ffe8900 3ffffea4 3ffffdd0 fffffff9
    // 3ffffcc0:  3ffffeb4 3ffef1f8 3ffffdd0 40201818  
    // 3ffffcd0:  6f6c0000 37300072 48206e5c 6f6c6c61
    // 3ffffce0:  6e5c6e5c 352e3520 3230322e 00000032
    // 3ffffcf0:  00000000 00000000 00000000 00000000
    // 3ffffd00:  00000000 00000000 00000000 00000000  
    // 3ffffd10:  00000000 00000000 00000000 00000000
    // 3ffffd20:  00000000 00000000 00000000 00000000
    // 3ffffd30:  00000000 00000000 00000000 00000000
    // 3ffffd40:  00000000 00000000 00000000 00000000
    // 3ffffd50:  00000000 00000000 00000000 00000000
    // 3ffffd60:  00000000 00000000 00000000 00000000
    // 3ffffd70:  00000000 00000000 00000000 00000000  
    // 3ffffd80:  00000000 00000000 00000000 00000000
    // 3ffffd90:  00000000 00000000 00000000 00000000
    // 3ffffda0:  00000000 00000000 00000000 00000000
    // 3ffffdb0:  00000000 00000000 00000000 00000000  
    // 3ffffdc0:  00000000 00000000 00000000 00000000
    // 3ffffdd0:  40209f14 00000000 40209f28 40209ed0
    // 3ffffde0:  3fff1eb0 3fff1ee4 3fff1efb 3fff1eb0  
    // 3ffffdf0:  3fff1eb0 3fff1efb 3fff032c 00000018
    // 3ffffe00:  3fff1eb0 40209f3c 00000006 00000000
    // 3ffffe10:  00001002 00000000 00000004 00000000
    // 3ffffe20:  00000000 00000000 00000000 00000000  
    // 3ffffe30:  00000000 00000000 00000000 00000000
    // 3ffffe40:  00000000 00000000 00000000 00000000
    // 3ffffe50:  00000000 00000000 00000000 00000000  
    // 3ffffe60:  00000000 00000000 00000008 3ffffe28
    // 3ffffe70:  3fff032c 00000000 3fff0000 3ffffddc
    // 3ffffe80:  3fff0008 3ffeffe0 3ffeffe8 3fff1c00
    // 3ffffe90:  001d001f 80006e5c 3fff1d58 4020229c  
    // 3ffffea0:  00000000 3fff1b64 004b004f 0a002265
    // 3ffffeb0:  3fff1c58 3fff1d58 0000005a 3fff2080
    // 3ffffec0:  007d005c 3fffdad0 3ffef40c 00000030  
    // 3ffffed0:  3fffff00 00000001 00000002 0000005c
    // 3ffffee0:  000010c3 0000000d 3ffef194 40208d80
    // 3ffffef0:  000010c3 3ffef194 3ffef194 40202948
    // 3fffff00:  00000001 3fffdad0 3ffef40c 00000030  
    // 3fffff10:  00000000 3fff1c4c 3fffff50 40205c25
    // 3fffff20:  3fffdad0 3ffef194 3ffef194 3ffef40c
    // 3fffff30:  3fffdad0 3ffef194 3ffef178 40201d52
    // 3fffff40:  4024dc6f 0023002f 80208d88 40208d70
    // 3fffff50:  3fff0fa4 4020470c 8064a8c0 3fff1d00
    // 3fffff60:  007a1200 18abdb3e 3ffef100 40201b78
    // 3fffff70:  00000000 4564a8c0 3ffef1f8 3ffef40c  
    // 3fffff80:  3fffdad0 00000000 3ffef3cc 40201ee8
    // 3fffff90:  3fffdad0 00000000 3ffef3cc 401003f5
    // 3fffffa0:  feefeffe feefeffe 3ffef3cc 40206738  
    // 3fffffb0:  feefeffe feefeffe 3ffe8694 40101195
    // <<<stack<<<

    // --------------- CUT HERE FOR EXCEPTION DECODER ---------------

    //  ets Jan  8 2013,rst cause:2, boot mode:(3,7)

    // load 0x4010f000, len 3584, room 16
    // tail 0
    // chksum 0xb0
    // csum 0xb0
    // v2843a5ac
    // ~ld




    while(ss.peek() != '{')         
    {
        ss.ignore();
    }

    //get response values until the closing bracket appears
    while(ss.peek() != '}')
    {
        //read until a opening variable quote sign appears
        ss.get(buffer, BUFFERSIZE, '\"'); 
        //and ignore it (go to next position in stream)
        ss.ignore();

        //read variable token excluding the closing variable quote sign
        ss.get(buffer, BUFFERSIZE, '\"');
        //and ignore it (go to next position in stream)
        ss.ignore();
        //store the variable name
        returnValue.first = buffer;

        //read until opening value quote appears(skips the : sign)
        ss.get(buffer, BUFFERSIZE, '\"');
        //and ignore it (go to next position in stream)
        ss.ignore();

        //read value token excluding the closing value quote sign
        ss.get(buffer, BUFFERSIZE, '\"');
        //and ignore it (go to next position in stream)
        ss.ignore();
        //store the variable name
        returnValue.second = buffer;

        
        // Serial.println(convert_stdstring_string(returnValue.first) + " = " + convert_stdstring_string(returnValue.second));
        if (returnValue.first == "message") {
            if (returnValue.second == ".leds") {
                String color = "";
                int del = 0;
                int len = 0;
                
                
                if (returnValue.first == "color") 
                    color = convert_stdstring_string(returnValue.second);
                if (returnValue.first == "delay") 
                    del = atoi(returnValue.second.c_str());
                if (returnValue.first == "length") 
                    len = atoi(returnValue.second.c_str());

                // =============        


                if (color != "") {
                    if ((len != 0) && (del != 0)) {
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
                }
            } else {
                display.clearDisplay();
                display.display();

                display.setTextColor(WHITE);
                display.setTextSize(2);
                display.setCursor(10, 0);
                display.println(convert_stdstring_string(returnValue.second)); // json["message"].GetString()
                display.display();
            }
        }
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
        
        String Tmp = "AirGiano/json";
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
