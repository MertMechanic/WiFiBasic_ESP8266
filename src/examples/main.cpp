// #include <Arduino.h>
// #include "CWifiBasic.h"

// #include "CAdditionalWebpages.h"
// #include "CCustomAdditionalWebpages.h"


//   #include <NTPClient.h>
//   #include <WiFiUdp.h>
// //Preset routes are:

// //AP Mode IP = 192.168.0.1

// // in AP Mode:
// //  "/" for setup WIFI config

// // in Normal Mode:
// // "/"        -> Example Root Page
// // "/login"   -> a Basic login form 
// // "/update"  -> OTA Form for firmware update



// CWifiBasic& wifi = CWifiBasic::getInstance();

// #define PIN_WIFI_RESET D8

// #define ENABLE_NTP 1


// #ifdef ENABLE_NTP

//   WiFiUDP   pNtpUDP;  
//   NTPClient pTimeClient(pNtpUDP, "europe.pool.ntp.org");
// #endif

// void setup() {
//       Serial.begin(115200);
      
//       //Init with Reset Pin for reset the WIFI CONFIG 
//       //NOTE - CARE that the pin is set to GND if u dont want to reset !
//       // if pin is not GND the WIFI config will reset
//       // Please let me know if u got a solution for that problem !

// // #ifdef ENABLE_NTP
// //       pNtpUDP = new WiFiUDP();
// //       pTimeClient = new NTPClient(*pNtpUDP, "europe.pool.ntp.org");             // Start NTP
// // #endif

//       CCustomAdditionalWebpages pages;
//       CAdditionalWebpages* p = (CAdditionalWebpages*)&pages;      
//       // wifi.init(PIN_WIFI_RESET, &additional);
//       wifi.init(PIN_WIFI_RESET, p);

// #ifdef ENABLE_NTP
//       pTimeClient.begin();             // Start NTP
//       pTimeClient.setTimeOffset(7200); // Change Time of NTP to Summertime
// #endif
    
      
// }

// void loop() {

//     #ifdef ENABLE_NTP
//     pTimeClient.update();

//     int hours   = pTimeClient.getHours();
//     int minutes = pTimeClient.getMinutes();
//     int seconds = pTimeClient.getSeconds();
    
//     Serial.print(hours);
//     Serial.print(":");
//     Serial.print(minutes);
//     Serial.print(":");
//     Serial.println(seconds);

//     delay(1000);
//     #endif

//   // put your main code here, to run repeatedly:
//     CWifiBasic::getInstance().run();

// }




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

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  pNtpUDP = new WiFiUDP;
  pTimeClient = new NTPClient(*pNtpUDP, "europe.pool.ntp.org");
#ifdef normal
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
#endif
#ifndef normal
      CCustomAdditionalWebpages pages;
      CAdditionalWebpages* p = (CAdditionalWebpages*)&pages;     
      wifi.init(PIN_WIFI_RESET, p);
#endif

// Initialize a NTPClient to get time
  pTimeClient->begin();
  pTimeClient->setTimeOffset(7200);

  
}

void loop() {

  // while(!pTimeClient->update())
  // {
  //   Serial.println("Failed");
  // }
  pTimeClient->update();
// wifi.run();

  int currentHour = pTimeClient->getHours();
  int currentMinute = pTimeClient->getMinutes();
  int currentSecond = pTimeClient->getSeconds();
  
  Serial.print(currentHour);  
  Serial.print(":");  
  Serial.print(currentMinute);  
  Serial.print(":");  
  Serial.print(currentSecond);  

  Serial.println("");

  delay(1000);
}