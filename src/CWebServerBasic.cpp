
#include "CWebServerBasic.h"
#include "CWifiBasic.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "ArduinoJson.h"
#include "FS.h"
#include "CFileSystem.h"

//
//GLOBAL FUNCTIONS FOR WEBSERVER HTTP REQUESTS
//



/**
 * @brief Used Standard routes are:
 * In AP Mode:
 *  /
 *  /settings
 * In Normal Mode:
 * /login
 * /update
 */

JsonObject& getJsonObjectFromResponse()
{
    String data = CWebServerBasic::getInstance().getESP8266WebServer()->arg("plain");

    // Parsing
    const size_t bufferSize = JSON_OBJECT_SIZE(48) + 512;
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject &root = jsonBuffer.parseObject(data);

    return root;
}

void handleAPModeRootPage()
{
    Serial.println("try to send wifisetupwebpage...");
    CWebServerBasic &Server = CWebServerBasic::getInstance();
    Server.getESP8266WebServer()->send_P(200, "text/html", wifisetupwebpage);
}

void handleAPModeSettingsUpdate()
{

    Serial.println("handleSettingsUpdate called...");
    JsonObject &JsonObject = getJsonObjectFromResponse();
    String str;
    JsonObject.printTo(str);
    CFileSystem::getInstance().writeFile("/config.json", &str);

    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "application/json", "{\"status\":\"ok\"}");
    #ifdef debug 
    Serial.println("send status ok");
    #endif
    delay(200);
    
    //Restart ESP after getting settings updated!
    ESP.restart();
}


void handleRootPage()
{
    //Configure your rootpage here ....
    Serial.println("handleRootPage() from CWebServerBasic");
    CWebServerBasic &Server = CWebServerBasic::getInstance();
    Server.getESP8266WebServer()->send_P(200, "text/plain", "handleRootPage() from CWebServerBasic");
}



void setupOTA()
{
    // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleLogin()
{
      CWebServerBasic::getInstance().getESP8266WebServer()->sendHeader("Connection", "close");
      CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/html", loginIndex);
}

void handleUpdate()
{
      CWebServerBasic::getInstance().getESP8266WebServer()->sendHeader("Connection", "close");
      CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/html", updateIndex);
}


void handledofirmwareupdate()
{
    CWebServerBasic::getInstance().getESP8266WebServer()->sendHeader("Connection", "close");
    CWebServerBasic::getInstance().getESP8266WebServer()->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
} 

void handledofirmwareupdateCheckErrors()
{
  HTTPUpload &upload = CWebServerBasic::getInstance().getESP8266WebServer()->upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    Serial.printf("Update: %s\n", upload.filename.c_str());
    if (!Update.begin(0xFFFFFFFF))
    { //start with max available size
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    /* flashing firmware to ESP*/
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (Update.end(true))
    { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    }
    else
    {
      Update.printError(Serial);
    }
  }
} 



CWebServerBasic::CWebServerBasic()
{
}
CWebServerBasic::~CWebServerBasic()
{
}

ESP8266WebServer *CWebServerBasic::getESP8266WebServer()
{
    return &this->m_Webserver;
}

/**
 * @brief Setup Webserver Webpages - call setupAdditionalAPModeWebPages() it in your subclase to override the routes!
 * 
 */
void CWebServerBasic::setupWebPageAPMode()
{
  
  Serial.println("setupWebPageAPMode()");
  this->getESP8266WebServer()->on("/"         , HTTP_GET,   handleAPModeRootPage);
  this->getESP8266WebServer()->on("/settings" , HTTP_POST,  handleAPModeSettingsUpdate);

  // replay to all requests with same HTML
  this->getESP8266WebServer()->onNotFound(handleAPModeRootPage);

}

/**
 * @brief Setup Webserver Webpages - call setupAdditionalWebPageNormalMode() it in your subclase to override the routes!
 * 
 */
void CWebServerBasic::setupWebPageNormalMode()
{
  Serial.println("setupWebPageNormalMode()");
  // this->getESP8266WebServer()->on("/"       , HTTP_GET, handleRootPage);
  this->getESP8266WebServer()->on("/login"  , HTTP_GET, handleLogin);
  this->getESP8266WebServer()->on("/update" , HTTP_GET, handleUpdate);
  this->getESP8266WebServer()->on(
      "/dofirmwareupdate", HTTP_POST, handledofirmwareupdateCheckErrors, handledofirmwareupdate);
}

