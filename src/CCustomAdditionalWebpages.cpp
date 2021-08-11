
#include "CCustomAdditionalWebpages.h"
#include "CWebServerBasic.h"
// #define debug 1


static void handleFetchDataForDashBoardCCustomAddionalWebpages()
{
    Serial.println("fetchData CCustomAdditionalWebpages... Dashboard");
    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/plain", "Dashboard");
}

static void handleRootCCustomAddionalWebpages()
{
    //Configure your rootpage here ....
    CWebServerBasic::getInstance().getESP8266WebServer()->send_P(200, "text/html", "Root");
}

/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CCustomAdditionalWebpages::setupAdditionalWebPageNormalMode()
{
#ifdef debug
    Serial.println("Setting up CCustomAdditionalWebpages setupAdditionalWebPageNormalMode()");
#endif

    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRootCCustomAddionalWebpages);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoardCCustomAddionalWebpages);
}

/**
 * @brief Setup your Custom Webpages here - and create a Method for the routing
 * 
 */
void CCustomAdditionalWebpages::setupAdditionalWebpagesAPMode()
{
    #ifdef debug
    Serial.println("Setting up CCustomAddionalWebpages setupAdditionalWebpagesAPMode()");
#endif
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/", handleRootCCustomAddionalWebpages);
    CWebServerBasic::getInstance().getESP8266WebServer()->on("/dashboard", handleFetchDataForDashBoardCCustomAddionalWebpages);
}