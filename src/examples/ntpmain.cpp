
#include <Arduino.h>
#include "CWifiBasic.h"

#include "CAdditionalWebpages.h"
#include "CCustomAdditionalWebpages.h"


#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "Wallway";
const char *password = "W4!!g455e";

// Define NTP Client to get time
  
WiFiUDP* pNtpUDP;
NTPClient* pTimeClient;


CWifiBasic& wifi = CWifiBasic::getInstance();
#define PIN_WIFI_RESET D8

// #define normal

// void setup() {
//   // Initialize Serial Monitor
//   Serial.begin(115200);

//   pNtpUDP = new WiFiUDP;
//   pTimeClient = new NTPClient(*pNtpUDP, "europe.pool.ntp.org");
// #ifdef normal
//   // Connect to Wi-Fi
//   Serial.print("Connecting to ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
// #endif
// #ifndef normal
//       CCustomAdditionalWebpages pages;
//       CAdditionalWebpages* p = (CAdditionalWebpages*)&pages;     
//       wifi.init(PIN_WIFI_RESET, p);
// #endif

// // Initialize a NTPClient to get time
//   pTimeClient->begin();
//   pTimeClient->setTimeOffset(7200);

  
// }

// void loop() {

//   // while(!pTimeClient->update())
//   // {
//   //   Serial.println("Failed");
//   // }
//   pTimeClient->update();
// // wifi.run();

//   int currentHour = pTimeClient->getHours();
//   int currentMinute = pTimeClient->getMinutes();
//   int currentSecond = pTimeClient->getSeconds();
  
//   Serial.print(currentHour);  
//   Serial.print(":");  
//   Serial.print(currentMinute);  
//   Serial.print(":");  
//   Serial.print(currentSecond);  

//   Serial.println("");

//   delay(1000);
// }