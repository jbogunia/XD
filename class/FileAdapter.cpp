#include <ArduinoJson.h>
#include "FS.h"

class FileAdapter {

private:
    FileAdapter();
    bool saveAdvertsToJson();

public:
    JsonArray loadAdverts();
    bool saveAdvert(JsonObject newAdvert);
    bool removeAdvert(int id, String advertPassword);
    bool editAdvert(int id, String title, String body);
    char getAdverts();
    int getAdvertIndex(int advertId);
    ~FileAdapter();
    static FileAdapter& getInstance();
    StaticJsonDocument<2000> arrayDoc;
    JsonObject rootObject;
    JsonArray advertsArray;

};

FileAdapter::FileAdapter () {
    SPIFFS.begin();
    Serial.println("File adapter initialized ... Ready");
    this->rootObject = this->arrayDoc.to<JsonObject>();
    this->advertsArray = this->rootObject.createNestedArray("adverts");
};

FileAdapter& FileAdapter::getInstance() {
  static FileAdapter instance;
  return instance;
}

FileAdapter::~FileAdapter() {
    this->arrayDoc.clear();
}

JsonArray FileAdapter::loadAdverts() {

  File adverts = SPIFFS.open("/adverts.json", "r");
  if (!adverts) {
    Serial.println("Reading adverts file failure...");
    return this->advertsArray;
  }

  adverts.setTimeout(5000);

  size_t size = adverts.size();
  Serial.print("Adverts file size: ");
  Serial.println(size);
  if(size == 0){
    Serial.println("Adverts file is empty!");
    return this->advertsArray;
  }

  DynamicJsonDocument doc(400);

  adverts.find("\"adverts\":[");
  do {
      DeserializationError err = deserializeJson(doc, adverts);
      if (err) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(err.c_str());
      }
      JsonObject object = doc.as<JsonObject>();
      this->advertsArray.add(object);
  } while (adverts.findUntil(",","]"));
  serializeJsonPretty(this->rootObject, Serial);
  adverts.close();
  return this->advertsArray;
}

bool FileAdapter::saveAdvertsToJson(){
  File advertsFile = SPIFFS.open("/adverts.json", "w");
  if (!advertsFile) {
    Serial.println("Problem with opening adverts.json file");
    return false;
  }

  Serial.println("");
  Serial.println("Saving adverts to json file...");

  serializeJson(this->rootObject, advertsFile);
  serializeJsonPretty(this->rootObject, Serial);
  advertsFile.close();
  return true;
}

bool FileAdapter::saveAdvert(JsonObject newAdvert) {
  int id = 0;
  Serial.print("Adverts count: ");
  Serial.println(this->advertsArray.size());
  if(this->advertsArray.size() > 0) {
    id = (int) this->advertsArray.getElement(this->advertsArray.size()-1)["id"];
    id++;
  }

  newAdvert["id"] = id;
  this->advertsArray.add(newAdvert);
  return this->saveAdvertsToJson();
}

int FileAdapter::getAdvertIndex(int advertId){
  int index=0;
  Serial.print("Finding ");
  Serial.print(advertId);
  Serial.println(" advert");
  for (JsonObject advert : this->advertsArray) {
    JsonVariant variant = advert.getMember("id");
    int id = variant.as<int>();
      if(id == advertId){
        Serial.print("Advert found on ");
        Serial.print(index);
        Serial.println(" position");
        return index;
      }
      index++;
  }
  return -1;
}

bool FileAdapter::removeAdvert(int id, String advertPassword){
  int index = this->getAdvertIndex(id);
  if(index != -1){
    Serial.print("Removing advert on ");
    Serial.print(index);
    Serial.println(" position in array");
    this->advertsArray.remove(index);

    this->saveAdvertsToJson();
    return true;
  } else {
    Serial.println("Given advert not found :(");
    return false;
  }
}

bool FileAdapter::editAdvert(int id, String title, String body){
  int index = this->getAdvertIndex(id);
  if(index != -1){
    Serial.print("Editing advert on ");
    Serial.print(index);
    Serial.println(" position in array");
    JsonVariant variant = this->advertsArray.getElement(index);
    variant.getMember("title").set(title);
    variant.getMember("body").set(body);
    this->saveAdvertsToJson();
    return true;
  } else {
    Serial.println("Given advert not found :(");
    return false;
  }
}
