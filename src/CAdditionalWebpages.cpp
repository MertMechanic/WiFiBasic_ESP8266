

#include "CAdditionalWebpages.h"
#include "CWebServerBasic.h"
#define debug 1


CAdditionalWebpages::CAdditionalWebpages()
{
}
CAdditionalWebpages::~CAdditionalWebpages()
{
}

void handleFetchDataForDashBoard()
{
    Serial.println("fetchData... Dashboard");
    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/plain", "This is the Dashboard Page <br> Configured by Class setupAdditionalWebPageNormalMode");
}

void handleRoot()
{
    //Configure your rootpage here ....
    CWebServerBasic::getInstance().getESP8266WebServer()->send_P(200, "text/html", "This is the root Page <br> Configured by Class setupAdditionalWebPageNormalMode");
}




/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CAdditionalWebpages::setupAdditionalWebPageNormalMode()
{
#ifdef debug
    Serial.println("Setting up setupAdditionalWebPageNormalMode()");
#endif

    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRoot);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoard);
}

/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CAdditionalWebpages::setupAdditionalWebpagesAPMode()
{
    #ifdef debug
    Serial.println("Setting up setupAdditionalWebpagesAPMode()");
#endif
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRoot);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoard);
}