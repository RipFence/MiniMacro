#include <Arduino.h>
#include "configHelper.h"
#include "storageHelper.h"
#include "buttonHelper.h"
#include "web/index.h"
#include "config.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

AsyncWebServer server(80);
bool needsConfigSave = false;

// Callback for /
void onRoot(AsyncWebServerRequest *request)
{
  Serial.println("in onRoot()");
  // ls(LittleFS.open("/"), 0);
  // request->send(LittleFS, "/index.html", "text/html");
  request->send_P(200, "text/html", index_html);
}

void onSaveMacros(AsyncWebServerRequest *request)
{
  // browser has submitted form so collect the field values and process/store
  Serial.println("in onSaveMacros");
}

void onSaveMacrosBody(AsyncWebServerRequest *request, JsonVariant &json)
{
  Serial.println("in onSaveMacrosBody");

  if (!json.is<JsonObject>())
  {
    request->send(400, "text/plain", "Invalid JSON format");
    return;
  }

  JsonObject jsonObj = json.as<JsonObject>();

  // Process each button's macros
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (jsonObj.containsKey(buttonNames[i]))
    {
      JsonObject buttonObj = jsonObj[buttonNames[i]];

      // Handle single click macro
      if (buttonObj.containsKey("single"))
      {
        const char *singleMacro = buttonObj["single"].as<const char *>();
        if (singleMacro)
        {
          strncpy(singleMacroBuffer[i], singleMacro, MACRO_LENGTH - 1);
          singleMacroBuffer[i][MACRO_LENGTH - 1] = '\0'; // Ensure null termination
        }
      }

      // Handle double click macro
      if (buttonObj.containsKey("double"))
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
  needsConfigSave = true;
  request->send(200, "text/plain", "Macros saved successfully");
}

void onGetMacros(AsyncWebServerRequest *request)
{
  // web server --> browser client --> web server request to "/json"
  Serial.println("in GetMacros()");
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

void onNotFound(AsyncWebServerRequest *request)
{
  Serial.println("in onNotFound()");
  // Handle Unknown Request
  request->send(404, "text/html", "<html><body><h1>404 Not Found</h1></body></html>");
}
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  Serial.println("in onBody()");
}
void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  Serial.println("in onUpload()");
}

void configSetup()
{
  Serial.println(("Configuring WebServer..."));

  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

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
  Serial.print("Connect to: http://");
  Serial.println(WiFi.localIP());
}

void configLoop()
{
  if (needsConfigSave)
  {
    Serial.println("Saving config...");
    needsConfigSave = false;
    // Save the updated macros to storage
    saveMacros(singleMacroBuffer, SINGLE_MACRO_FILE);
    saveMacros(doubleMacroBuffer, DOUBLE_MACRO_FILE);
    delay(1000);
    // Restart the ESP to apply the changes
    ESP.restart();
  }
}