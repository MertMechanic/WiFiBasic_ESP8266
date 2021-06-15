#include <Arduino.h>
#include "CWifiBasic.h"

#include "CAdditionalWebpages.h"
#include "CCustomAdditionalWebpages.h"
//Preset routes are:

//AP Mode IP = 192.168.0.1

// in AP Mode:
//  "/" for setup WIFI config

// in Normal Mode:
// "/"        -> Example Root Page
// "/login"   -> a Basic login form 
// "/update"  -> OTA Form for firmware update



CWifiBasic& wifi = CWifiBasic::getInstance();

#define PIN_WIFI_RESET D8

void setup() {
      Serial.begin(115200);
      
      //Init with Reset Pin for reset the WIFI CONFIG 
      //NOTE - CARE that the pin is set to GND if u dont want to reset !
      // if pin is not GND the WIFI config will reset
      // Please let me know if u got a solution for that problem !
      
      CCustomAdditionalWebpages pages;
      CAdditionalWebpages* p = (CAdditionalWebpages*)&pages;      
      // wifi.init(PIN_WIFI_RESET, &additional);
      wifi.init(PIN_WIFI_RESET, p);
      
     
      
}

void loop() {
  // put your main code here, to run repeatedly:
    CWifiBasic::getInstance().run();
}
