#pragma once
///
///
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#endif
class ESP8266WebServer;

#include "CWebServerBasic.h"
#include <DNSServer.h>

class CWifiBasic
{

//BASIC SINGLETON START
public:
    static CWifiBasic &getInstance()
    {
        static CWifiBasic S(CWebServerBasic::getInstance());
        return S;
    }

    CWifiBasic(const CWifiBasic &) = delete;
    void operator=(CWifiBasic const &) = delete;

public:
    CWifiBasic(CWebServerBasic &_webserver);
    ~CWifiBasic();
//BASIC SINGLETON END

public:
    String getIpAdress();
    void setIpAdress(int, int, int, int);

    void setSSID(String *_pSSID);
    void setPassword(String *_psk);

    String* getSSID();
    String* getPassword();

    bool isInAPMode();
    void wifiConnect();
    void handleSettingsUpdate();

    CWebServerBasic &getWebserver();

    void setResetPin(const uint8_t _pin);
    uint8_t getResetPin();

    //INIT WIFI SSID + PASSWORD
    //
    //
    const char *INIT_SSID = "WIFI ESP8266";
    const char *INIT_APPASSWORD = "12345678";
    bool m_isInAPMode;

    void init(uint8_t _resetPin);
    
    bool handleResetButton();

    DNSServer* getDNSServer();
    void run();
    
private:
    String* m_pSSID;
    String* m_pPassword;
    bool m_isStaticIP;

    CWebServerBasic& m_WebServer;

    bool m_isWIFIConnected;
    uint8_t m_WifiResetPin;

    IPAddress *m_pIP;
    IPAddress *m_pGateway;
    IPAddress *m_pSubnetmask;

    int m_resetTimeCounter;
    static const int s_limitResetCounter = 5;


    const byte DNS_PORT = 53;
    DNSServer m_dnsServer;
};