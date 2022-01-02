/*
 * With this library an ESP8266 can ping a remote machine and know if it's reachable. 
 * It provides some basic measurements on ping messages (avg response time).
 */

#include <WiFi.h>
#include "ESP32Ping.h"

#define PING_COUNT 4

const char* ssid     = "Mochi11";
const char* password = "hanif123";

IPAddress localip;
IPAddress localsm;
IPAddress localgw;

int subadd = 0;

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.print("Connecting to WiFi");
  
    WiFi.begin(ssid, password);
  
    while(WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    localip = WiFi.localIP();
    localsm = WiFi.subnetMask();
    localgw = WiFi.gatewayIP();

    Serial.println();
    Serial.println("WiFi Connected: " + String(ssid));
    Serial.print("IP: ");
    Serial.println(localip);
    Serial.print("Gateway: ");
    Serial.println(localgw);
    Serial.print("Subnet mask: ");
    Serial.println(localsm);
    Serial.println("===|SCANNING|===");
}

void loop() {
    if(WiFi.status() == WL_CONNECTED && subadd <= 256){
        IPAddress target(192, 168, 1, subadd);
        Serial.print(String("192.168.1.") + subadd + String(" "));

        unsigned long start = millis();
        bool pret = Ping.ping(target, PING_COUNT);
        unsigned long stop = millis();

        if(pret){
            Serial.print("succed ");
        }
        else{
            Serial.print("failed ");
        }

        Serial.println(String(stop - start) + "ms");

        subadd++;
    }
    else if(subadd > 256){
        Serial.println("===|SCAN FINISHED|==");
    }
    else{
        Serial.println("No Connection...");
    }
    delay(1000);
}