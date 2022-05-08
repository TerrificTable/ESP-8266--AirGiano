#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>

#include "screen.h"
#include "leds.h"


#define                 CONFIG_h
#include                "config.h"

typedef struct {
    boolean         WLANConnected               = false;  
    boolean         WLANConnecting              = false;  
    unsigned long   WLANConnectingTimeOut       = 0;  
    unsigned long   WLANConnectingWaitTimeOut   = 0;  

    
    boolean         MQTTConnected               = false;  
    boolean         MQTTConnecting              = false;  
    unsigned long   MQTTConnectingTimeOut       = 0;  
    unsigned long   MQTTConnectingWaitTimeOut   = 0;
    

    boolean         WEBConnected                = false;
} StatesData;

StatesData States;

#ifndef STASSID
#define STASSID         name
#define STAPSK          passwd
#endif

#ifndef MQTT_IP
#define MQTT_IP         mqtt_server
#define MQTT_USERNAME   mqtt_username
#define MQTT_PASSWORD   mqtt_password
#endif

const char *ssid            = STASSID;
const char *password        = STAPSK;
const char *MQTT_Server     = MQTT_IP;
int MQTT_Port               = 1883;
const char *MQTT_UID        = MQTT_USERNAME;
const char *MQTT_PWD        = MQTT_PASSWORD;


WiFiClient      espClient;
PubSubClient    MQTTClient(espClient);
ESP8266WebServer WebServer(80);





void WebStartPage() {
    String Page;
    Serial.println("Webserver WebStartPage..");
    DisplayText("Webserver\nStart Page..");

    Page =  "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
                "<title>ESP-8266 -> AirGiano</title>\n";
 
    Page += "</head>"
            "<body>"
                "<a href=\"/config\">HALLO</a>";
    
    Page += "</body>\n"
            "<html>\n";


    WebServer.send(200, "text/html", Page);

}

void WEB() {
    if (! States.WLANConnected) return;

    if (!States.WEBConnected) {
        Serial.println("Webserver Started..");
        DisplayText("Webserver\nStarted");
        States.WEBConnected = true;      
     
        WebServer.on("/", []() {
            WebStartPage();
        });

   
        WebServer.onNotFound([](){
            WebServer.send(404, "text/plain", "<h1>404 Not Found</h1><hr>");  
        });

        WebServer.begin();
    };

    WebServer.handleClient();
}


void MQTTCallBack(char *Topic, byte *payload, unsigned int length) {

    String rcv = "";

    for (unsigned int i = 0; i < length; i++)
    {
       rcv += (char)payload[i]; // convert *byte to string
    }


    // =============
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str()); 
        return;  
    }

    String message          = doc["message"];   
    String color            = doc["color"];   

    Serial.println(rcv);


    if (message == ".leds") {
        States.LEDConnected = false;
        int del = 0;
        int len = 0;

        del = doc["delay"];
        len = doc["length"];

        // =============        


        if ((len != 0) && (del != 0)) {
            if (color == "red") {
                LED(len, CRGB::Red, del);
            }
            else if (color == "green") {
                LED(len, CRGB::Green, del);
            }
            else if (color == "blue") {
                LED(len, CRGB::Blue, del);
            } else {
                LED(len, CRGB::Yellow, del);
            }
        } else {
            if (color == "red") {
                LED(3, CRGB::Red, 30);
            }
            else if (color == "green") {
                LED(3, CRGB::Green, 30);
            }
            else if (color == "blue") {
                LED(3, CRGB::Blue, 30);
            } else {
                LED(3, CRGB::Yellow, 30);
            }
        }
    } else {
        DisplayText(message);
    }
}



void WLAN() {
    if (! States.WLANConnected &&  WiFi.status() == WL_CONNECTED) {
        States.WLANConnecting = false;
        States.WLANConnectingWaitTimeOut = 0;
        Serial.print("WiFi Connected: ");
        String IP = WiFi.localIP().toString();

        Serial.println(IP);
        DisplayText("WiFi: \n" + IP);
    }

    States.WLANConnected = WiFi.status() == WL_CONNECTED || WiFi.status()  == WL_IDLE_STATUS;
    
    if (States.WLANConnected) return;  
    
    
    if (States.WLANConnectingWaitTimeOut > 0 && millis() < States.WLANConnectingWaitTimeOut ) return;
    if (States.WLANConnectingWaitTimeOut > 0 && millis() > States.WLANConnectingWaitTimeOut ) States.WLANConnectingWaitTimeOut = 0;


    if (!States.WLANConnecting) {
        States.WLANConnecting = true;
        States.WLANConnectingTimeOut = millis() + 10000;  // 10 Sekunfen
        Serial.println("WiFi connecting....");
        DisplayText("WiFi connecting....");
        
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
        DisplayText("WiFi Failed");
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