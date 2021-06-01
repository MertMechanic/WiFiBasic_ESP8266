#pragma once


#include "ESP8266WebServer.h"
#include "WebServerDataBasic.h"
#include "ArduinoJson.h"

class CWebServerBasic 
{
//Stingleton START
public:
    static CWebServerBasic &getInstance()
    {
        static CWebServerBasic S;
        return S;
    }

    CWebServerBasic(const CWebServerBasic &) = delete;
    void operator=(CWebServerBasic const &) = delete;

protected:
    CWebServerBasic();
    ~CWebServerBasic();
//Stingleton END
public:
    ESP8266WebServer *getESP8266WebServer();

    void setupWebPageAPMode();
    void setupWebPageNormalMode();

    virtual void setupAdditionalAPModeWebPages();
    virtual void setupAdditionalWebPageNormalMode();


private:
    ESP8266WebServer m_Webserver;
    int static const m_WebserverPort = 80;
};