#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "class/FileAdapter.cpp"

AsyncWebServer server(80);

IPAddress localIp(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

FileAdapter fileAdapter = FileAdapter::getInstance();

void setup() {

  Serial.begin(115200);
  Serial.println();

  JsonArray adverts = fileAdapter.loadAdverts();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(localIp, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("teest") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
    Serial.println("Index.html GET Request");
  });

  server.on("/advert", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Adverts GET Request");
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(fileAdapter.advertsArray, *response);
    request->send(response);
  });

  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/main.css");
    Serial.println("main.css Request");
  });

  server.on("/resources.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/resources.js");
    Serial.println("resources.js Request");
  });

  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/main.js");
    Serial.println("main.js Request");
  });

  server.on("/advert", HTTP_PUT, [](AsyncWebServerRequest * request) {
    Serial.println("Advert PUT request");
    const size_t CAPACITY = JSON_OBJECT_SIZE(100);
    StaticJsonDocument<CAPACITY> doc;

    // create an object
    JsonObject advert = doc.to<JsonObject>();
    advert["title"] = "New advert Title";
    advert["body"] = "New advert Body";
    if (fileAdapter.saveAdvert(advert)) {
      Serial.println("New advert saved!");
    }

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(fileAdapter.rootObject, *response);
    request->send(response);

  });

  server.on("/advert", HTTP_PATCH, [](AsyncWebServerRequest * request) {
    Serial.println("Advert PATCH request");
    const size_t CAPACITY = JSON_OBJECT_SIZE(100);
    StaticJsonDocument<CAPACITY> doc;

    // create an object
    JsonObject advert = doc.to<JsonObject>();
      advert["title"] = "New advert Title";
      advert["body"] = "New advert Body";
      if (fileAdapter.saveAdvert(advert)) {
        Serial.println("New advert saved!");
      }

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(fileAdapter.rootObject, *response);
    request->send(response);

  });

  server.on("/advert", HTTP_DELETE, [](AsyncWebServerRequest * request) {
    Serial.println("Avdert remove request");
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    const size_t CAPACITY = JSON_OBJECT_SIZE(50);
    StaticJsonDocument<CAPACITY> doc;
    JsonObject responseMessage = doc.to<JsonObject>();
    if (request->hasParam("id") && request->hasParam("password")) {
      Serial.print("Removing advert: ");
      Serial.println(request->getParam("id")->value());
    } else {
      responseMessage["message"] = "Advert ID or password is missing!";
      serializeJson(responseMessage, *response);
      request->send(response);
    }

    if (fileAdapter.removeAdvert(request->getParam("id")->value().toInt())) {
      responseMessage["message"] = "Advert removed!";
    } else {
      responseMessage["message"] = "Something went wrong :/";
    }

    serializeJson(responseMessage, *response);
    request->send(response);
  });
  server.begin();
}

// the loop function runs over and over again forever
void loop() {

}
