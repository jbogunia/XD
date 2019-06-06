#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "class/FileAdapter.cpp"
#include "class/Validation.cpp"

#define TYPE_GET 0
#define TYPE_PUT 1
#define TYPE_PATH 2
#define TYPE_DELETE 3

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
  Serial.println(WiFi.softAP("Information-Board") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //Tutaj obsługiwane są requuesty, poniższy zwraca plik index.html z pamięci urządzenia
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
    Serial.println("Index.html GET Request");
  });

  //Ten endpoint odpowiada za zwrócenie listy wszystkich ogłoszeń w formacie JSON
  //(wraca z pamięci podręcznej, ale wszystkie ogłoszenia zapisywane/modyfikowane są równiez w pliku adverts.json - dostępne są po restartie urządzenia)
  server.on("/advert", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Adverts GET Request");
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(fileAdapter.advertsArray, *response);
    request->send(response);
  });

  //Zwraca kaskadowy arkusz stylów z pamięci urządzenia
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/main.css");
    Serial.println("main.css Request");
  });

  //Zwraca js idk co to xD (pytać Karola)
  server.on("/resources.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/resources.js");
    Serial.println("resources.js Request");
  });

  //Zwraca js, zapewne logika frontendu
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/main.js");
    Serial.println("main.js Request");
  });


  //Endpoint umożliwiający dodanie nowego ogłoszenia
  //Wymagane pola: title, body, password (trzeba by zrobić ich validacje, aktualnie tylko sprawdza, czy są w requescie)
  server.on("/advert", HTTP_PUT, [](AsyncWebServerRequest * request) {
    Serial.println("Advert PUT request");
    if (request->hasParam("password", true) && request->hasParam("title", true) && request->hasParam("body", true)) {
      String adertTitle = request->getParam("title", true)->value();
      String advertBody = request->getParam("body", true)->value();
      String advertPassword = request->getParam("password", true)->value();

//      ErrorResponse validationResponse = validate(request, 1);
//      if (validationResponse.getCode() != 200) {
//        request->send(validationResponse.getCode(), "application/json", validationResponse.getJsonMessage() );
//      }

      const size_t CAPACITY = JSON_OBJECT_SIZE(100);
      StaticJsonDocument<CAPACITY> doc;

      JsonObject advert = doc.to<JsonObject>();
      advert["title"] = adertTitle;
      advert["body"] = advertBody;
      advert["password"] = advertPassword;

      ErrorResponse errorResponse = fileAdapter.saveAdvert(advert);
      if (errorResponse.getCode() == 200) {
        Serial.println("New advert saved!");
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        serializeJson(advert, *response);
        request->send(response);

      } else {
        request->send(errorResponse.getCode(), "application/json", errorResponse.getJsonMessage() );
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });

  //Endpoint służący do edycji ogłoszenia
  //Wymagane pola: id,title, body, password (rówież rzydała by się validacja)
  server.on("/advert", HTTP_PATCH, [](AsyncWebServerRequest * request) {
    Serial.println("Advert PATCH request");
    if (request->hasParam("id") && request->hasParam("password") && request->hasParam("title") && request->hasParam("body")) {
      int advertId = request->getParam("id")->value().toInt();
      String advertTitle = request->getParam("title")->value();
      String advertBody = request->getParam("body")->value();
      String advertPassword = request->getParam("password")->value();

      ErrorResponse errorResponse = fileAdapter.editAdvert(advertId, advertTitle, advertBody, advertPassword);

      if (errorResponse.getCode() == 200) {
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
      } else {
        request->send(errorResponse.getCode(), "application/json", errorResponse.getJsonMessage());
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });

  //Endpoint służący do usuwania ogłoszenia
  //Wymagane pola: id, password
  server.on("/advert", HTTP_DELETE, [](AsyncWebServerRequest * request) {
    Serial.println("Avdert remove request");
    if (request->hasParam("id") && request->hasParam("password")) {
      int advertId = request->getParam("id")->value().toInt();
      String advertPassword = request->getParam("password")->value();
      Serial.print("Removing advert: ");
      Serial.println(advertId);

      ErrorResponse errorResponse = fileAdapter.removeAdvert(advertId, advertPassword);
      if (errorResponse.getCode() == 200) {
        request->send(200, "application/json", errorResponse.getJsonMessage());
      } else {
        request->send(errorResponse.getCode(), "application/json", errorResponse.getJsonMessage());
      }
    } else {
      request->send(400, "application/json", "{\"message\" : \"\You need to specyify all request params!\"}" );
    }
  });
  server.begin();
}

//ErrorResponse validate(AsyncWebServerRequest * request, int type) {
//  switch (1) {
//    case 1:
//      if (!Validation::validateTitle(request->getParam("title")->value().c_str())) {
//        ErrorResponse response(400, "Invalid title");
//        return response;
//      }
//      if (!Validation::validateBody(request->getParam("body")->value().c_str())) {
//        ErrorResponse response(400, "Invalid body");
//        return response;
//      }
//
//      if (!Validation::validatePassword(request->getParam("password")->value().c_str())) {
//        ErrorResponse response(400, "Invalid body");
//        return response;
//      }
//      break;
//  }
//  ErrorResponse response(200, "OK");
//  return response;
//}

// the loop function runs over and over again forever
void loop() {

}
