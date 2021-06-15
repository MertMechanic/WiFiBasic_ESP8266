

#include "CCustomAddionalWebpages.h"
#include "CWebServerBasic.h"
#define debug 1


void handleFetchDataForDashBoardCCustomAddionalWebpages()
{
    Serial.println("fetchData CCustomAddionalWebpages... Dashboard");
    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/plain", "Dashboard");
}

void handleRootCCustomAddionalWebpages()
{
    //Configure your rootpage here ....
    CWebServerBasic::getInstance().getESP8266WebServer()->send_P(200, "text/html", "Root");
}

/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CCustomAddionalWebpages::setupAdditionalWebPageNormalMode()
{
#ifdef debug
    Serial.println("Setting up CCustomAddionalWebpages setupAdditionalWebPageNormalMode()");
#endif

    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRootCCustomAddionalWebpages);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoardCCustomAddionalWebpages);
}

/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CCustomAddionalWebpages::setupAdditionalWebpagesAPMode()
{
    #ifdef debug
    Serial.println("Setting up CCustomAddionalWebpages setupAdditionalWebpagesAPMode()");
#endif
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRootCCustomAddionalWebpages);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoardCCustomAddionalWebpages);
}