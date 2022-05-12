#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#define                 CONFIG_h
#include                "config.h"

#ifndef STASSID
#define STASSID         name
#define STAPSK          passwd
#endif

const char *ssid            = STASSID;
const char *password        = STAPSK;



typedef struct {
    boolean         WLANConnected               = false;  
    boolean         WLANConnecting              = false;  
    unsigned long   WLANConnectingTimeOut       = 0;  
    unsigned long   WLANConnectingWaitTimeOut   = 0;  

    boolean         AP_Created                  = false;
} StatesData;
StatesData States;


WiFiClient          espClient;
ESP8266WebServer    server(80);






void WIFI_AccessPoint() {
    if (!States.WLANConnected) return;
    if (States.AP_Created) return;

    // Create WIFI Access Point
    WiFi.softAP("GitHub: TerrificTable", "Password1234!");
    Serial.println("WiFi APIP: " + String(WiFi.softAPIP().toString()));
    States.AP_Created = true;
}


void WEB() {
    if (!States.WLANConnected) return;

    static boolean SERVER_CONNECTED = false;


    if (!SERVER_CONNECTED) {
        // if (MDNS.begin("esp2866")) {
        //     Serial.println("DNS Started: http://esp8266.local");
        // }
 
        Serial.println("WebServer Running on: http://" + String(WiFi.localIP().toString()));
        // DisplayText("WebServer Running on: http://" + String(WiFi.localIP().toString()));
        SERVER_CONNECTED = true;


        server.onNotFound([](){
            server.send(404, "text/html", "<h1>Not Found</h1><hr><h3>404 Not Found</h3>");
            Serial.println("[SERVER -> /* 404]  Error: 404 Not Found");
        });

        server.on("/", [](){
            server.send(200, "text/html", "<h1>ESP-8266</h1><hr><h3>Hallo!</h3><br><h3><a href=\"https://github.com/TerrificTable\">Github</a></h3>");
            Serial.println("[SERVER -> /]  Loaded");
        });

        server.on("/password", [](){
            String result;

            result =  "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>ESP-8266 -> PasswordGenerator</title><style>body {\nfont-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;\n}\n.main {\nfont-size: 20px;\n}\n.gen_v1 {\nfont-size: 25px;\n} .gen_v2 {\nfont-size: 25px;\n}\n#password {\ncolor: rgb(255, 0, 0);\nposition: absolute;\nbottom: 67%;\nleft: 1%;\n}\n#password-label {\nposition: absolute;\nbottom: 70%;\nleft: 1%;\n}\n.footer {\nposition: absolute;\nbottom: 80%;\n}\n#label {\nfont-size: 55px;\nline-height: 20px;\n}\n</style>\n</head><body>\n<div class=\"main\">\n<h1 id=\"label\">Password Generator</h1><hr>\n<h3 id=\"password-label\">Password:</h3>\n<h3 id=\"password\"></h3>\n<footer class=\"footer\">\n<button onclick=\"generatePassword_v1()\" class=\"gen_v1\">Generate (v1)</button>\n<button onclick=\"generatePassword_v2()\" class=\"gen_v2\">Generate (v2)</button>\n</footer></div>\n<script type=\"text/javascript\">\nfunction generatePassword_v2() {\nvar password_label = document.getElementById(\"password\");\nvar password = '';\nvar chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#$/&%$!=?_-+';\nfor (let i = 1; i <= 20; i++) {\nvar char = Math.floor(Math.random() * chars.length + 1);\npassword += chars.charAt(char);\n}\nconsole.log(password);\npassword_label.innerHTML = password;\nreturn password;\n}\nfunction generatePassword_v1() {\nvar password_label = document.getElementById(\"password\");\npassword = Math.random().toString(36).slice(2) + Math.random().toString(36).toUpperCase().slice(2);\nconsole.log(password);\npassword_label.innerHTML = password;\nreturn password;\n}\ngeneratePassword_v2()\n</script>\n</body></html>";

            server.send(200, "text/html", result);
            Serial.println("[SERVER -> /password]  Loaded");
        });


        server.begin();
    }

    server.handleClient();
}


void WLAN_Connect() {
    if (!States.WLANConnected &&  WiFi.status() == WL_CONNECTED) {
        States.WLANConnecting = false;
        States.WLANConnectingWaitTimeOut = 0;
        Serial.print("WiFi Connected: ");
        String IP = WiFi.localIP().toString();

        Serial.println(IP);
    }

    States.WLANConnected = WiFi.status() == WL_CONNECTED || WiFi.status()  == WL_IDLE_STATUS;
    if (States.WLANConnected) return;  


    if (States.WLANConnectingWaitTimeOut > 0 && millis() < States.WLANConnectingWaitTimeOut ) return;
    if (States.WLANConnectingWaitTimeOut > 0 && millis() > States.WLANConnectingWaitTimeOut ) States.WLANConnectingWaitTimeOut = 0;


    if (!States.WLANConnecting) {
        States.WLANConnecting = true;
        States.WLANConnectingTimeOut = millis() + 10000;  // 10 Sekunfen
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
