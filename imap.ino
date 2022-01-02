/*
 * With this library an ESP8266 can ping a remote machine and know if it's reachable. 
 * It provides some basic measurements on ping messages (avg response time).
 */

#include <WiFi.h>
#include <time.h>
#include <FS.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "ESP32Ping.h"

#define PING_COUNT 1
#define SLEEPDUR 5*60*1000000
#define GMT_OFFSET 7*60*60
#define SPIFFS_FORMAT_IF_FAILED true
#define HEURISTIC true


const char* ssid     = "Mochi11";
const char* password = "hanif123";

const char* ntpserver = "pool.ntp.org";

const char* servername = "https://maker.ifttt.com/trigger/new_wifi_device/with/key/biYKB5vkmXW1-pSDyQUGspzOrDTAIFFhRQ-thbm7Z5L";


IPAddress localip;
IPAddress localsm;
IPAddress localgw;
IPAddress localbc;

RTC_DATA_ATTR int bootcount = 2;
int subadd = 0;

void printTimeStamp(){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time from ntp server: " + String(ntpserver));
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void ipadd(fs::FS &cstream, const char* path, uint32_t ip){
    File file = cstream.open(path, FILE_APPEND);
    
    if(!file){
        Serial.println("Error reading file");
        return;  
    }
    
    file.println(ip);
    file.close();
}

void ipread(fs::FS &cstream, const char* path){
    File file = cstream.open(path, FILE_READ);

    if(!file){
        Serial.println("Error reading file");
        return;   
    }

    while(file.available()){
        String addr = file.readStringUntil('\n');
        addr.trim();
        uint32_t ipaddr = strtoul(addr.c_str(), NULL, 10);
        IPAddress cip(ntohl(ipaddr));
        Serial.println(cip);
    }

    file.close();
}

bool ipexist(fs::FS &cstream, const char* path, IPAddress current){
    File file = cstream.open(path, FILE_READ);

    if(!file){
        Serial.println("Error reading file");
        return false;    
    }

    while(file.available()){
        String addr = file.readStringUntil('\n');
        addr.trim();
        uint32_t ipaddr = strtoul(addr.c_str(), NULL, 10);
        IPAddress cip(ntohl(ipaddr));
        if(current == cip){
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void callback(char *topic, byte *payload, unsigned int length){
    Serial.println("Info sent");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    if(!SPIFFS.begin(SPIFFS_FORMAT_IF_FAILED)){
        Serial.println("Error mounting SPIFFS! Restarting...");
        ESP.restart();
    }

    switch(esp_sleep_get_wakeup_cause()){
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println(String(bootcount++) + "# Boot");
            break;
        default:
            Serial.println("New Boot");
            SPIFFS.format();
    }

    esp_sleep_enable_timer_wakeup(SLEEPDUR);

    Serial.println();
    Serial.print("Connecting to WiFi");
  
    WiFi.begin(ssid, password);
  
    while(WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    configTime(GMT_OFFSET, 0, ntpserver);

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
    printTimeStamp();
    Serial.println("");

    if(WiFi.status() == WL_CONNECTED){
        WiFiClientSecure client;
        HTTPClient http;

        uint32_t ugw = htonl(uint32_t(localgw));
        uint32_t uip = htonl(uint32_t(localip));
        uint32_t ubc = htonl(uint32_t(localbc));

        String info = "";

        int devcount = 0;
        int testcount = 0;
        int newdev = 0;

        for(uint32_t ipdx = ugw; ipdx <= ubc; ipdx++){
            if(HEURISTIC && testcount >= 20){
                break;
            }

            IPAddress target(ntohl(ipdx));
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
            if(ipdx == ugw){
                Serial.println("(gateway)");
            }
            else if(ipdx == uip){
                Serial.println("(self)");
            }
            else if(ipdx == ubc){
                Serial.println("(broadcast)");
            }
            else if(pret){
                Serial.print("<---");

                if(!ipexist(SPIFFS, "/user.txt", target)){
                    Serial.print(" [NEW]");
                    ipadd(SPIFFS, "/user.txt", ipdx);
                    newdev++;
                    info += String(target);
                    info += ";";
                }

                Serial.println("");

                devcount++;
            }
            else{
                Serial.println("");
            }

            testcount++;
        }

        Serial.println("===|SCAN FINISHED|===");
        printTimeStamp();
        Serial.println("Number of connected devices this scan: " + String(devcount));
        Serial.println("Number of new devices: " + String(newdev));

        ipread(SPIFFS, "/user.txt");

        client.setInsecure();
        client.connect(servername, 443);

        http.begin(client, servername);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String httpRequestData = "value1=" + info + "&value2=" + String(devcount) + "&value3=" + String(newdev);

        int httpResponseCode = http.POST(httpRequestData);
        Serial.println("HTTP Response code for webhook: " + String(httpResponseCode));

        http.end();

        delay(5000);

        Serial.println("going sleep...");
        esp_deep_sleep_start();
    }
}

void loop() {
    
}
