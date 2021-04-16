#include <Arduino.h>
#include "CWifi.h"



CWifi& wifi = CWifi::getInstance();
#define PIN_WIFI_RESET D0

void setup() {
      Serial.begin(115200);
      
      wifi.init(PIN_WIFI_RESET);
     
      
}

void loop() {
  // put your main code here, to run repeatedly:
    CWifi::getInstance().run();
}
