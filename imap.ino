/*
 * With this library an ESP8266 can ping a remote machine and know if it's reachable. 
 * It provides some basic measurements on ping messages (avg response time).
 */

#include <WiFi.h>
#include "ESP32Ping.h"

#define PING_COUNT 1
#define SLEEPDUR 5*60*1000000


const char* ssid     = "Mochi11";
const char* password = "hanif123";

IPAddress localip;
IPAddress localsm;
IPAddress localgw;
IPAddress localbc;

RTC_DATA_ATTR int bootcount = 0;
int subadd = 0;
bool justfinished = true;

void setup() {
    Serial.begin(115200);
    delay(10);

    switch(esp_sleep_get_wakeup_cause()){
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println(String(bootcount) + "# boot to scan");
            break;
        default:
            Serial.println("New boot");
    }

    esp_sleep_enable_timer_wakeup(SLEEPDUR);

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
    localbc = IPAddress(ntohl(htonl(uint32_t(localgw)) | ~htonl(uint32_t(localsm))));

    Serial.println();
    Serial.println("WiFi Connected: " + String(ssid));
    Serial.print("IP: ");
    Serial.println(localip);
    Serial.print("Gateway: ");
    Serial.println(localgw);
    Serial.print("Subnet mask: ");
    Serial.println(localsm);
    Serial.print("Broadcast: ");
    Serial.println(localbc);
    Serial.println("===|SCANNING|===");

    if(WiFi.status() == WL_CONNECTED){
        uint32_t ugw = htonl(uint32_t(localgw));
        uint32_t uip = htonl(uint32_t(localip));
        uint32_t ubc = htonl(uint32_t(localbc));

        for(uint32_t ipstart = ugw; ipstart <= ubc; ipstart++){
            IPAddress target(ntohl(ipstart));
            Serial.print(target);

            unsigned long tstart = millis();
            bool pret = Ping.ping(target, PING_COUNT);
            unsigned long tstop = millis();

            if(pret){
                Serial.print(" succed ");
            }
            else{
                Serial.print(" failed ");
            }

            Serial.print(String(tstop - tstart) + "ms ");
            if(ipstart == ugw){
                Serial.println("(gateway)");
            }
            else if(ipstart == uip){
                Serial.println("(self)");
            }
            else if(ipstart == ubc){
                Serial.println("(broadcast)");
            }
            else if(pret){
                Serial.println("<---");
            }
            else{
                Serial.println("");
            }
        }

        Serial.println("===|SCAN FINISHED|===");
        delay(5000);
        Serial.println("going sleep...");
        esp_deep_sleep_start();
    }
}

void loop() {
    
}