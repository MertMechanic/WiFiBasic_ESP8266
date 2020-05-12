
#include "CWebServer.h"
#include "CWifi.h"

#include "ArduinoJson.h"
#include "FS.h"

//
//GLOBAL FUNCTIONS FOR WEBSERVER HTTP REQUESTS
//

void handleAPModeRootPage()
{
    Serial.println("try to send wifisetupwebpage...");
    CWebServer &Server = CWebServer::getInstance();
    Server.getESP8266WebServer()->send_P(200, "text/html", wifisetupwebpage);
}

void handleAPModeSettingsUpdate()
{

    Serial.println("handleSettingsUpdate called...");
    String data = CWebServer::getInstance().getESP8266WebServer()->arg("plain");
    DynamicJsonBuffer jBuffer;
    JsonObject &JsonObject = jBuffer.parseObject(data);

    //DEBUG
    // char buffer[1024];
    // JsonObject.printTo(buffer,1024);
    // Serial.println(buffer);

    // String ssid = JsonObject.get("ssid");
    // String password = JsonObject.get("password");

    File configFile = SPIFFS.open("/config.json", "w");
    JsonObject.printTo(configFile);
    configFile.close();

    CWebServer::getInstance().getESP8266WebServer()->send(200, "application/json", "{\"status\":\"ok\"}");
    Serial.println("send status ok");
    delay(500);

    CWifi &wifi = CWifi::getInstance();
    wifi.wifiConnect();
}

void handleRoot()
{
    //Configure your rootpage here ....
    Serial.println("try to send servus moiiiin...");
    CWebServer &Server = CWebServer::getInstance();
    // Server.getESP8266WebServer()->send_P(200, "text/html", index);
    Server.getESP8266WebServer()->send_P(200, "text/plain", "hallo Servus Moin!");
}

CWebServer::CWebServer()
{
}
CWebServer::~CWebServer()
{
}

ESP8266WebServer *CWebServer::getESP8266WebServer()
{
    return &this->m_Webserver;
}

void CWebServer::setupWebPageAPMode()
{
    this->getESP8266WebServer()->on("/", handleAPModeRootPage);
    this->getESP8266WebServer()->on("/settings", HTTP_POST, handleAPModeSettingsUpdate);

    // replay to all requests with same HTML
    this->getESP8266WebServer()->onNotFound(handleAPModeRootPage);
    this->getESP8266WebServer()->begin();
}

void CWebServer::setupWebPageNormalMode()
{
    this->getESP8266WebServer()->on("/", handleRoot);
    this->getESP8266WebServer()->begin();
}
