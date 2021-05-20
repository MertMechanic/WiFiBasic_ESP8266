
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

CWifiBasic::CWifiBasic(CWebServerBasic &_webserver) : m_WebServer(_webserver)
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
void CWifiBasic::init(const uint8_t _resetPin)
{
    this->setResetPin(_resetPin);
    this->wifiConnect();
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
    WiFi.softAPdisconnect(true);
    WiFi.disconnect();
    delay(500);
    this->m_isInAPMode = false;

    const char *_ssid = "";
    const char *_pass = "";
    String str;
    if (CFileSystem::getInstance().readFile("/config.json", &str))
    {

        DynamicJsonBuffer JsonBuffer;
        JsonObject &jObject = JsonBuffer.parseObject(str);
        if (jObject.success())
        {
            _ssid = jObject["ssid"];
            _pass = jObject["password"];
            this->m_pSSID = new String(_ssid);
            this->m_pPassword = new String(_pass);

            Serial.print("Try to connect to:");
            Serial.print(_ssid);

            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _pass);
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
        //Wifi is connected to config wifi setup
        this->m_pIP = new IPAddress(WiFi.localIP());
        this->m_pSubnetmask = new IPAddress(WiFi.subnetMask());
        this->m_pGateway = new IPAddress(WiFi.gatewayIP());

        Serial.print("IpAddress is:");
        Serial.println(this->m_pIP->toString());
        Serial.print("SubnetMask is:");
        Serial.println(this->m_pSubnetmask->toString());
        Serial.print("Gateway is:");
        Serial.println(this->m_pGateway->toString());
        
    }
    else
    {
        //IMPORTANT - PASSWORD MUST BE MIN LENGHT BY 8 CHARS!
        const char *initssid = INIT_SSID;
        const char *initpassword = INIT_APPASSWORD;
        Serial.print("name:");
        Serial.println(initssid);
        Serial.print("password:");
        Serial.println(initpassword);
        Serial.println("--------");
        Serial.println("AP Mode - no config file found or created...");
        WiFi.mode(WIFI_AP);

        this->m_isInAPMode = true;

        if (this->m_pIP == nullptr || this->m_pSubnetmask == nullptr || this->m_pGateway == nullptr)
        {
            this->m_isStaticIP = true;
            WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
        }
        else
        {
            this->m_isStaticIP = false;
            WiFi.softAPConfig(*this->m_pIP, *this->m_pSubnetmask, *this->m_pGateway);
        }

        //AP with passwordphrase
        // WiFi.softAP(initssid, initpassword, 1, 0);

        WiFi.softAP(initssid);
        
        // if DNSServer is started with "*" for domain name, it will reply with
        // provided IP to all DNS request
        this->m_dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        this->m_dnsServer.start(DNS_PORT, "*", IPAddress(192,168,1,1));
    }

    WiFi.printDiag(Serial);

    if(this->isInAPMode())
    {
        this->getWebserver().setupWebPageAPMode();
    }
    else
    {
        this->getWebserver().setupWebPageNormalMode();
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
    else
    {
        return false;
    }
    
}