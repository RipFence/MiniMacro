#include <Arduino.h>
#include "configHelper.h"
#include "storageHelper.h"
#include "buttonHelper.h"
#include "web/index.h"
#include "config.h"
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <nvs_flash.h>
#include <ESPmDNS.h>

AsyncWebServer server(80);
bool needsReboot = false;

// check if this string is an IP address
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

// Checks if the request is to me. If not, redirect it to me
boolean isNotForMe(AsyncWebServerRequest *request) {
  if (!isIp(request->host())) { // server.hostHeader()
    Serial.println("Request redirected to captive portal");
    request->redirect(String("http://") + toStringIp(WiFi.localIP()), 302);
    return true;
  }
  return false;
}

// Callback for /
void onRoot(AsyncWebServerRequest *request)
{
  if (isNotForMe(request)) { 
    return;
  }
  // ls(LittleFS.open("/"), 0);
  // request->send(LittleFS, "/index.html", "text/html");
  request->send(200, "text/html", index_html);
}

void onSaveMacros(AsyncWebServerRequest *request)
{
  // browser has submitted form so collect the field values and process/store
}
void onSaveMacrosBody(AsyncWebServerRequest *request, JsonVariant &json)
{
  if (!json.is<JsonObject>())
  {
    request->send(400, "text/plain", "Invalid JSON format");
    return;
  }

  JsonObject jsonObj = json.as<JsonObject>();

  // Process each button's macros
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (jsonObj[buttonNames[i]].is<JsonObject>())
    {
      JsonObject buttonObj = jsonObj[buttonNames[i]].as<JsonObject>();

      // Handle single click macro
      if (buttonObj["single"].is<const char*>())
      {
        const char *singleMacro = buttonObj["single"].as<const char *>();
        if (singleMacro)
        {
          strncpy(singleMacroBuffer[i], singleMacro, MACRO_LENGTH - 1);
          singleMacroBuffer[i][MACRO_LENGTH - 1] = '\0'; // Ensure null termination
        }
      }

      // Handle double click macro
      if (buttonObj["double"].is<const char*>())
      {
        const char *doubleMacro = buttonObj["double"].as<const char *>();
        if (doubleMacro)
        {
          strncpy(doubleMacroBuffer[i], doubleMacro, MACRO_LENGTH - 1);
          doubleMacroBuffer[i][MACRO_LENGTH - 1] = '\0'; // Ensure null termination
        }
      }
    }
  }

  // Flag for saving the config
  // Save the updated macros to storage
  saveMacros(singleMacroBuffer, SINGLE_MACRO_FILE);
  saveMacros(doubleMacroBuffer, DOUBLE_MACRO_FILE);
  request->send(200, "text/plain", "Macros saved successfully.  Restarting MiniMacro...");
  needsReboot = true;
}

void onGetMacros(AsyncWebServerRequest *request)
{
  // web server --> browser client --> web server request to "/json"
  String jsonStr;
  JsonDocument doc;
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    doc[buttonNames[i]]["single"] = singleMacroBuffer[i];
    doc[buttonNames[i]]["double"] = doubleMacroBuffer[i];
  }
  serializeJson(doc, jsonStr); // equiv to JSON.stringify() in JS
  request->send(200, "text/html", jsonStr);
}

void onFactoryReset(AsyncWebServerRequest *request)
{
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init();  // initialize the NVS partition.
  LittleFS.format(); // Format Flash
  request->send(200, "text/plain", "Reset to Factory Defaults.  Restarting MiniMacro...");
  needsReboot = true;
}
void onNotFound(AsyncWebServerRequest *request)
{
  // Handle Unknown Request
  request->send(404, "text/html", "<html><body><h1>404 Not Found</h1></body></html>");
}
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
}
void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
}

void configSetup()
{
  if (!MDNS.begin("minimacro"))
  {
    while (1)
    {
      delay(1000);
    }
  }

  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

  server.on("/factory-reset", HTTP_GET, [](AsyncWebServerRequest *request)
            { onFactoryReset(request); });

  // attach filesystem root at URL /fs
  server.serveStatic("/fs", LittleFS, "/");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { onRoot(request); });

  server.on("/get-macros", HTTP_GET, [](AsyncWebServerRequest *request)
            { onGetMacros(request); });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/save-macros", onSaveMacrosBody);
  server.addHandler(handler);

  // catch all unhandled requests
  server.onNotFound(onNotFound);
  server.onFileUpload(onUpload);

  server.begin();
}

void configLoop()
{
  if (needsReboot)
  {
    needsReboot = false;
    delay(500);
    // Restart the ESP to apply the changes
    ESP.restart();
  }
}

void configStop()
{
  server.end();
  MDNS.end();
  LittleFS.end();
  nvs_flash_deinit();
}
