

#include "CAddtionalWebpages.h"
#include "CWebServerBasic.h"
#define debug 1


CAddtionalWebpages::CAddtionalWebpages()
{
}
CAddtionalWebpages::~CAddtionalWebpages()
{
}

void handleFetchDataForDashBoard()
{
    Serial.println("fetchData... Dashboard");
    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/plain", "Dashboard");
}

void handleRoot()
{
    //Configure your rootpage here ....
    CWebServerBasic::getInstance().getESP8266WebServer()->send_P(200, "text/html", "Root");
}

void CAddtionalWebpages::setupAdditionalWebPageNormalMode()
{
#ifdef debug
    Serial.println("Setting up setupAdditionalWebPageNormalMode()");
#endif

    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRoot);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dash", handleFetchDataForDashBoard);
}

void CAddtionalWebpages::setupAdditionalWebpagesAPMode()
{
    #ifdef debug
    Serial.println("Setting up setupAdditionalWebpagesAPMode()");
#endif
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRoot);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dash", handleFetchDataForDashBoard);
}