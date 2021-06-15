#pragma once
///
///
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#endif


#include "CWebServerBasic.h"
#include <DNSServer.h>


class CAdditionalWebpages;

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

protected:
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

    void deleteWifiConfigAndStartAP();

    CWebServerBasic &getWebserver();

    void setResetPin(const uint8_t _pin);
    uint8_t getResetPin();

    //INIT WIFI SSID + PASSWORD
    //
    //
    const char *INIT_SSID        = "WIFI ESP8266";
    const char *INIT_AP_PASSWORD =     "12345678";

    const uint8_t AP_MODE_IPADDRESS   [4]  = { 192, 168, 0   ,1};
    const uint8_t AP_MODE_SUBNETMASK  [4]  = { 255, 255, 255 ,0};
    const uint8_t AP_MODE_GATEWAY     [4]  = { 192, 168, 0   ,1};

    bool m_isInAPMode;

    //void init(uint8_t _resetPin);
    void init(uint8_t _resetPin,  CAdditionalWebpages* _pAdditional = nullptr);
    
    
    bool handleResetButton();

    DNSServer* getDNSServer();
    void run();
private:
    String* m_pSSID;
    String* m_pPassword;
    bool m_isStaticIP;

    IPAddress getAPModeIP();
    IPAddress getAPModeSubnetMask();
    IPAddress getAPModeGateway();
    
    void configureWebServerPages();

    CWebServerBasic&     m_WebServer;
    CAdditionalWebpages* m_pAdditionalWebpages;
    // IPAddress* getIPAddressByString(String *_pIpAsStr);

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