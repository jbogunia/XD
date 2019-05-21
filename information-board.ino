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
    if (request->hasParam("password", true) && request->hasParam("title", true) && request->hasParam("body", true)) {
      String adertTitle = request->getParam("title", true)->value();
      String advertBody = request->getParam("body", true)->value();
      String advertPassword = request->getParam("password", true)->value();

      const size_t CAPACITY = JSON_OBJECT_SIZE(100);
      StaticJsonDocument<CAPACITY> doc;

      // create an object
      JsonObject advert = doc.to<JsonObject>();
      advert["title"] = adertTitle;
      advert["body"] = advertBody;
      advert["password"] = advertPassword;
      if (fileAdapter.saveAdvert(advert)) {
        Serial.println("New advert saved!");
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        serializeJson(advert, *response);
        request->send(response);
      } else {
        request->send(500, "application/json", "{\"message\" : \"\Someting wetn wrong :(\"}" );
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });

  server.on("/advert", HTTP_PATCH, [](AsyncWebServerRequest * request) {
    Serial.println("Advert PATCH request");
    if (request->hasParam("id") && request->hasParam("password") && request->hasParam("title") && request->hasParam("body")) {
      int advertId = request->getParam("id")->value().toInt();
      String advertTitle = request->getParam("title")->value();
      String advertBody = request->getParam("body")->value();
      String advertPassword = request->getParam("password")->value();

      if (fileAdapter.editAdvert(advertId, advertTitle, advertBody)) {
        Serial.println("Advert edited!");
        const size_t CAPACITY = JSON_OBJECT_SIZE(100);
        StaticJsonDocument<CAPACITY> doc;

        JsonObject advert = doc.to<JsonObject>();
        advert["id"] = advertId;
        advert["title"] = advertTitle;
        advert["body"] = advertBody;

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        serializeJson(advert, *response);
        request->send(response);
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });

  server.on("/advert", HTTP_DELETE, [](AsyncWebServerRequest * request) {
    Serial.println("Avdert remove request");
    if (request->hasParam("id") && request->hasParam("password")) {
      int advertId = request->getParam("id")->value().toInt();
      String advertPassword = request->getParam("password")->value();
      Serial.print("Removing advert: ");
      Serial.println(advertId);
      if (fileAdapter.removeAdvert(advertId, advertPassword)) {
        request->send(200, "application/json", "{\"message\" : \"\Advert removed!\"}" );
      } else {
        request->send(500, "application/json", "{\"message\" : \"\Something went wrong :(\"}" );
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });
  server.begin();
}

// the loop function runs over and over again forever
void loop() {

}
