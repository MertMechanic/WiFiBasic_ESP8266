
#include "CWifiBasic.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>

#include "CWebServerBasic.h"
#include "ArduinoJson.h"
#include "FS.h"

#include "CFileSystem.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "CAdditionalWebpages.h"

#define debug 1 

CWifiBasic::CWifiBasic(CWebServerBasic &_webserver) : 
m_WebServer(_webserver)
{
}
CWifiBasic::~CWifiBasic()
{
}

CWebServerBasic &CWifiBasic::getWebserver()
{
    return CWebServerBasic::getInstance();
}

/**
 * @brief Set the SSID by String
 * 
 * @param _pSSID 
 */
void CWifiBasic::setSSID(String *_pSSID)
{
    this->m_pSSID = _pSSID;
}
/**
 * @brief Set the Wifi passord bby stirng 
 * 
 * @param _psk 
 */
void CWifiBasic::setPassword(String *_psk)
{
    this->m_pPassword = _psk;
}

/**
 * @brief return ssid as string
 * 
 * @return String* 
 */
String *CWifiBasic::getSSID()
{
    return this->m_pSSID;
}


/**
 * @brief return password as string
 * 
 * @return String* 
 */
String *CWifiBasic::getPassword()
{
    return this->m_pPassword;
}

/**
 * @brief Init Method - call it once in Setup! 
 * Todo: implement a flag for beeing one-call-savety!
 * 
 * @param _resetPin Reset pin will reset the settings
 */
void CWifiBasic::init(const uint8_t _resetPin, CAdditionalWebpages* _pAdditional)
{
    this->setResetPin(_resetPin);
    this->wifiConnect();
    this->m_pAdditionalWebpages = _pAdditional;
    this->configureWebServerPages();
}

void CWifiBasic::configureWebServerPages()
{
    if (this->isInAPMode())
    {
        CWebServerBasic::getInstance().setupWebPageAPMode();
        if (this->m_pAdditionalWebpages != nullptr)
        {
            this->m_pAdditionalWebpages->setupAdditionalWebpagesAPMode();
        }
    }
    else
    {
        CWebServerBasic::getInstance().setupWebPageNormalMode();
        if (this->m_pAdditionalWebpages != nullptr)
        {
            this->m_pAdditionalWebpages->setupAdditionalWebPageNormalMode();
        }
    }

    CWebServerBasic::getInstance().getESP8266WebServer()->begin();
}

/**
 * @brief return status of wifi in Access Point mode or not
 * 
 * @return true 
 * @return false 
 */
bool CWifiBasic::isInAPMode()
{
    return this->m_isInAPMode;
}

/**
 * @brief setup the reset in for deleting the wifi config file on file system
 * 
 * @param _pin 
 */
void CWifiBasic::setResetPin(const uint8_t _pin)
{
    pinMode(_pin, INPUT); // WIFI RESET PIN
    this->m_WifiResetPin = _pin;
}

/**
 * @brief return reset pin
 * 
 * @return uint8_t 
 */
uint8_t CWifiBasic::getResetPin()
{
    return this->m_WifiResetPin;
}

/**
 * @brief Connect to wifi by settings which are configured 
 * if no conifguration found it will start as AP mode and wait for inputs 
 * 
 */
void CWifiBasic::wifiConnect()
{

    this->m_pIP         = nullptr;
    this->m_pSubnetmask = nullptr;
    this->m_pGateway    = nullptr;
    // WiFi.softAPdisconnect(true);
    // WiFi.disconnect();
    // delay(200);
    this->m_isInAPMode = false;

    const char *pSSIDChar        = "";
    const char *pPasswordChar    = "";
    const char *pIPAddressChar   = "";
    const char *pSubnetmaskChar  = "";
    const char *pGatewayChar     = "";
    String str;

    /**
    * @brief Try reading Config File
    * 
    */
    if (CFileSystem::getInstance().readFile("/config.json", &str))
    {

        DynamicJsonBuffer JsonBuffer;
        JsonObject &jObject = JsonBuffer.parseObject(str);
        if (jObject.success())
        {
            pSSIDChar           = jObject["ssid"      ];
            pPasswordChar       = jObject["password"  ];

            this->m_pSSID       = new String(pSSIDChar);
            this->m_pPassword   = new String(pPasswordChar);

#ifdef debug
            Serial.print("SSID:");
            Serial.println(*this->m_pSSID);

            Serial.print("Password:");
            Serial.println(*this->m_pPassword);
#endif 

            //*** Getting IP Address.....
            if (jObject["ipaddress"].success())
            {
                pIPAddressChar   = jObject["ipaddress"];
                String IpAddrStr = String(pIPAddressChar);
                this->m_pIP      = new IPAddress();
                if (!this->m_pIP->fromString(IpAddrStr))
                {
                    //Something failed....
                    Serial.println("Info: deleting WIfiConfig and Restart as AP ");
                    this->deleteWifiConfigAndStartAP();
                }
            }
            else
            {
                //Do this if not IP is not Setted....
                this->m_pIP = new IPAddress(this->getAPModeIP());
            }
#ifdef debug
                Serial.print("setted :");
                Serial.println(this->m_pIP->toString());
#endif

            //*** Getting SubnetMask.....
            if (jObject["subnetmask"].success())
            {
                pSubnetmaskChar   = jObject["subnetmask"];
                String SubnetmaskStr = String(pSubnetmaskChar);
                this->m_pSubnetmask      = new IPAddress();

                if(!this->m_pSubnetmask->fromString(SubnetmaskStr))
                {
                    //Something failed....
                    Serial.println("Info: deleting WIfiConfig and Restart as AP ");
                    this->deleteWifiConfigAndStartAP();
                }
            }
            else
            {
                //Do this if not IP is not Setted....
                this->m_pSubnetmask = new IPAddress(this->getAPModeSubnetMask());
            }

#ifdef debug
                Serial.print("setted :");
                Serial.println(this->m_pSubnetmask->toString());
#endif
            //*** Getting Gateway.....
            if (jObject["gateway"].success())
            {
                pGatewayChar        = jObject["gateway"];
                String GatewayStr   = String(pGatewayChar);
                this->m_pGateway    = new IPAddress();

                if(!this->m_pGateway->fromString(GatewayStr))
                {
                    //Something failed....
                    Serial.println("Info: deleting WIfiConfig and Restart as AP ");
                    this->deleteWifiConfigAndStartAP();
                }
            }
            else
            {
                //Do this if not IP is not Setted....
                this->m_pGateway = new IPAddress(this->getAPModeGateway());
            }

#ifdef debug
            Serial.print("setted :");
            Serial.println(this->m_pGateway->toString());
#endif
            Serial.println();
            Serial.print("Try to connect to: ");
            Serial.println(pSSIDChar);
            Serial.print("WifiINFO: Configured using ip: ");
            Serial.println(m_pIP->toString());

            Serial.print("WifiINFO: Configured using mask: ");
            Serial.println(m_pSubnetmask->toString());

            Serial.print("WifiINFO: Configured using gateway: ");
            Serial.println(m_pGateway->toString());

            WiFi.mode(WIFI_STA);
            WiFi.begin(pSSIDChar, pPasswordChar);
            /**
             * @brief The Wifi.config call will destroy the the functionalitiy of the NTP (Network Time Protocol)!
             *          --> Fix set the DNS == Gateway -> Fixed! 
             */
            WiFi.config(*this->m_pIP, *this->m_pGateway, *this->m_pSubnetmask, *this->m_pGateway);
            this->m_isInAPMode = false;

            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.println(".");
                if (CWifiBasic::handleResetButton())
                {
                    wifiConnect();
                    break;
                }
            }
        }   
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Status:Conncted");

        Serial.print("WifiINFO: using ip: ");
        Serial.println(WiFi.localIP().toString());

        Serial.print("WifiINFO: using mask: ");
        Serial.println(WiFi.subnetMask().toString());

        Serial.print("WifiINFO: using gateway: ");
        Serial.println(WiFi.gatewayIP().toString());
    }
    
}

DNSServer* CWifiBasic::getDNSServer()
{
    return &this->m_dnsServer;
}

//Main Method of WIFI - 
//handle client
//is dns when in AP mode
//handle reset button for resetting WIFI settings....
void CWifiBasic::run()
{
    if (this->m_isInAPMode == true)
    {
        CWifiBasic::getInstance().getDNSServer()->processNextRequest();
    }
    
    CWebServerBasic::getInstance().getESP8266WebServer()->handleClient(); //Method for http request handleing
    CWifiBasic::getInstance().handleResetButton();                        //Method for observing WIFI reset button 
    ArduinoOTA.handle();
}

//Listen to the Reset Button ... Interrupt everything and count 5 seconds 
//if 5 seconds is finished - the wifi settings are resetete by deleteing the config file... 
bool CWifiBasic::handleResetButton()
{
    
    unsigned int count = 5;
    bool buttonPressed = false;
    //Count 5 times is button pressed - if yes - reset settings 
    for (size_t i = 0; i < count; i++)
    {
        if (digitalRead(this->getResetPin()) == HIGH)
        {
            buttonPressed = true;
            Serial.print("Reset WLAN Config in ");
            Serial.print(5 - i);
            Serial.println(" Seconds.");
        }
        else
        {
            buttonPressed = false;
            break;
        }
        delay(1000);
    }

    if (buttonPressed)
    {

        //Delete Wifi Settings and create AP
        this->deleteWifiConfigAndStartAP();
    }
    else
    {
        return false;
    }
    
}


//**This is actually not good to return a full instance instead of a ptr
IPAddress CWifiBasic::getAPModeIP()
{
#ifdef debug
    Serial.println("getAPModeIP() called");
#endif
    return IPAddress(AP_MODE_IPADDRESS[0],AP_MODE_IPADDRESS[1],AP_MODE_IPADDRESS[2],AP_MODE_IPADDRESS[3]);
}
IPAddress CWifiBasic::getAPModeSubnetMask()
{
#ifdef debug
    Serial.println("getAPModeSubnetMask() called");
#endif
    return IPAddress(AP_MODE_SUBNETMASK[0],AP_MODE_SUBNETMASK[1],AP_MODE_SUBNETMASK[2],AP_MODE_SUBNETMASK[3]);
}
IPAddress CWifiBasic::getAPModeGateway()
{
#ifdef debug
    Serial.println("getAPModeGateway() called");
#endif
    return IPAddress(AP_MODE_GATEWAY[0],AP_MODE_GATEWAY[1],AP_MODE_GATEWAY[2],AP_MODE_GATEWAY[3]);
}

/**
 * @brief Delete WifiSettings & ConfigFile - start in AP Mode 
 * 
 */
void CWifiBasic::deleteWifiConfigAndStartAP()
{
    Serial.println("Deleting WIFI SETUP");
    if (this->m_isInAPMode == false && this->m_pIP != nullptr && this->m_pSubnetmask != nullptr && this->m_pGateway != nullptr)
    {
        delete this->m_pIP;
        delete this->m_pSubnetmask;
        delete this->m_pGateway;
    }

    this->m_isInAPMode = true;

    String str;
    if (CFileSystem::getInstance().readFile("/config.json", &str))
    {
        CFileSystem::getInstance().deleteFile("/config.json");
        Serial.println("Remove Config file...");
        this->wifiConnect();
    }
}

// IPAddress* CWifiBasic::getIPAddressByString(String *_pIpAsStr)
// {


// #ifdef debug
//     Serial.print("IpReceived getIPAddressByString()");
//     Serial.println(*_pIpAsStr);
// #endif 
//     int size = _pIpAsStr->length();

//     const char *pCstr = _pIpAsStr->c_str();

// #ifdef debug
//     Serial.print("pCstr getIPAddressByString()");
//     Serial.println(*pCstr);
// #endif 

//   uint8_t ip[4];
//   sscanf(pCstr, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);

// #ifdef debug
//   Serial.print("IP[0]:");
//   Serial.print(ip[0]);
//   Serial.print("IP[1]:");
//   Serial.print(ip[1]);
//   Serial.print("IP[2]:");
//   Serial.print(ip[2]);
//   Serial.print("IP[3]:");
//   Serial.print(ip[3]);
// #endif
//   IPAddress* pAddr = new IPAddress(ip[0],ip[1],ip[2],ip[3]);
//   return pAddr;
// }