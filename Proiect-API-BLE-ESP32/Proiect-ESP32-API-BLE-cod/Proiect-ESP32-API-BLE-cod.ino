#include <Arduino.h>
#include <BLEDevice.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define bleServerName "ByteBuilders"

bool deviceConnected = false;

const String teamID = "A38";

const char* apiURL = "http://proiectia.bogdanflorea.ro/api/better-call-saul/characters";
String dealDetailsApiURL = "http://proiectia.bogdanflorea.ro/api/breaking-bad/character?char_id=";

#define SERVICE_UUID "b833ed74-93e1-467e-9cfb-207d0bdca93e"
#define CHARACTERISTIC_UUID "ca73b3ba-39f6-4ab3-91ae-186dc9577d99"


BLECharacteristic characteristic(
  CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
);

BLEDescriptor *characteristicDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2902));

// Setup callbacks onConnect and onDisconnect ]
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};


void BLESendData(BLECharacteristic* characteristic, const String& data) {
  const size_t maxDataSize = 512;
  size_t dataSize = data.length();
  size_t offset = 0;

  //we break the data into manageable chunks so that we are able to send over BLE

  while (offset < dataSize) {
    size_t len = min(maxDataSize, dataSize - offset);
    String value = data.substring(offset, offset + len);
    Serial.println(value);
    characteristic->setValue(value.c_str());
    characteristic->notify();
    offset += len;
    delay(20);
  }
}
class CharacteristicsCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      // Get characteristic value sent from the app, according to the specs
      std::string data = characteristic->getValue();
      Serial.println(data.c_str()); // <-- This is the message sent from the app, according to the specs

      /*
        Proccess the data
      */
      DynamicJsonDocument jsonDoc(15000);

      DeserializationError error = deserializeJson(jsonDoc,data);

      if(error)
      {
        Serial.println("JSON deserialization failed bro :(");
        Serial.println(error.c_str());
      }else
      {
        String action = jsonDoc["action"].as<String>();
        Serial.println(action);

          if(action == "getNetworks")
          {
            int n = WiFi.scanNetworks();
            if(n == 0)
            {
              Serial.println("ERROR : NO NETWORKS FOUND");
              return;
            }

              for (int i = 0; i < n; i++) {
                DynamicJsonDocument networkDoc(256);
                JsonObject network = networkDoc.to<JsonObject>();
                network["ssid"] = WiFi.SSID(i);
                network["strength"] = WiFi.RSSI(i);
                network["encryption"] = WiFi.encryptionType(i);
                network["teamId"] = teamID;
                String response;
                serializeJson(network, response);
                BLESendData(characteristic, response);
              }
              Serial.println("SCAN COMPLETE");
          }
              else if (action == "connect") 
            {
              String ssid = jsonDoc["ssid"].as<String>();
              String password = jsonDoc["password"].as<String>();
              Serial.print("COMMAND RECEIVED : CONNECTING TO NETWORK :");
              Serial.println(ssid);

              WiFi.begin(ssid.c_str(), password.c_str());

              int maxRetries = 10;
              while (WiFi.status() != WL_CONNECTED && maxRetries > 0) {
                delay(1000);
                Serial.print(".");
                maxRetries--;
              }
              Serial.println();

              bool connected = (WiFi.status() == WL_CONNECTED);
              Serial.print("CONNECTED: ");
              Serial.println(connected);

              DynamicJsonDocument responseDoc(256);
              responseDoc["ssid"] = ssid;
              responseDoc["connected"] = connected;
              responseDoc["teamId"] = teamID;


              String response;
              serializeJson(responseDoc, response);
              BLESendData(characteristic, response);
          }
          else if(action == "getData")
          {
                if (WiFi.status() == WL_CONNECTED) {
                HTTPClient http;
                http.begin(apiURL);
                int httpCode = http.GET();

                Serial.print("HTTP RESPONSE CODE: ");
                Serial.println(httpCode);

                int redirectCount = 0;
                while (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND || httpCode == HTTP_CODE_SEE_OTHER) {
                  if (redirectCount > 5) {
                    Serial.println("ERROR : TOO MANY REDIRECTS");
                    break;
                  }
                  String newLocation = http.header("Location");
                  Serial.print("REDIRECTING TO: ");
                  Serial.println(newLocation);
                  http.end();

                  if (newLocation.length() > 0) {
                    http.begin(newLocation);
                    httpCode = http.GET();
                    Serial.print("HTTP RESPONSE CODE : ");
                    Serial.println(httpCode);
                    redirectCount++;
                  } else {
                    break;
                  }
                }

                if (httpCode > 0) {
                  String payload = http.getString();
                  Serial.println("HTTP RESPONSE PAYLOAD: ");
                  Serial.println(payload);

                  if (payload.length() == 0) {
                    Serial.println("ERROR:EMPTY PAYLOAD");
                    return;
                  }

                  DynamicJsonDocument apiResponseDoc(4096);
                  DeserializationError error = deserializeJson(apiResponseDoc, payload);

                  if (error) {
                    Serial.print("ERROR : JSON DESERIALIZATION FAILED ");
                    Serial.println(error.c_str());
                    return;
                  }

                  JsonArray apiResponseArray = apiResponseDoc.as<JsonArray>();

                  for (JsonObject apiRecord : apiResponseArray) {
                    DynamicJsonDocument recordDoc(1024); // Adjusted memory size
                    recordDoc["char_id"] = apiRecord["char_id"];
                    recordDoc["name"] = apiRecord["name"];
                    recordDoc["img"] = apiRecord["img"];
                    recordDoc["teamId"] = teamID;

                    String response;
                    serializeJson(recordDoc, response);
                    Serial.println("RESPONSE " + response);
                    BLESendData(characteristic, response);
                  }
                } else {
                  Serial.print("ERROR : INVALID HTTP REQUEST ");
                  Serial.println(httpCode);
                }
                http.end();
              } else {
                Serial.println("ERROR : WIFI NOT CONNECTED");
              }
          }else if (action == "getDetails") {

          if (WiFi.status() == WL_CONNECTED) {
            String id = jsonDoc["char_id"].as<String>();
            String detailsUrl = dealDetailsApiURL + id;
            Serial.println(detailsUrl);

            HTTPClient http;
            http.begin(detailsUrl);
            int httpCode = http.GET();

            Serial.print("HTTP Response code: ");
            Serial.println(httpCode);

            int redirectCount = 0;
            while (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND || httpCode == HTTP_CODE_SEE_OTHER) {
              if (redirectCount > 5) {
                Serial.println("Too many redirects");
                break;
              }
              String newLocation = http.header("Location");
              Serial.print("Redirecting to: ");
              Serial.println(newLocation);
              http.end(); 

              if (newLocation.length() > 0) {
                http.begin(newLocation);
                httpCode = http.GET();
                Serial.print("HTTP Response code after redirect: ");
                Serial.println(httpCode);
                redirectCount++;
              } else {
                break;
              }
            }

            if (httpCode > 0) {
              String payload = http.getString();
              Serial.println("HTTP Response payload: ");
              Serial.println(payload);

              if (payload.length() == 0) {
                Serial.println("Received empty payload");
                return;
              }

              DynamicJsonDocument apiResponseDoc(4096);
              DeserializationError error = deserializeJson(apiResponseDoc, payload);

              if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                return;
              }
              

              JsonObject apiRecord = apiResponseDoc.as<JsonObject>();
              DynamicJsonDocument responseDoc(4096);
              responseDoc["char_id"] = apiRecord["char_id"];
              responseDoc["name"] = apiRecord["name"]; // schimbat title cu name
              responseDoc["birthday"] = apiRecord["birthday"];
              //occupation
              responseDoc["occupation"] = apiRecord["occupation"];
              responseDoc["img"] = apiRecord["img"]; //schimbat thumb cu image
              responseDoc["status"] = apiRecord["status"];
              responseDoc["nickname"] = apiRecord["nickname"];
              //appearance
              responseDoc["appearance"] = apiRecord["appearance"];
              responseDoc["portrayed"] = apiRecord["portrayed"];

            
              String response;
              serializeJson(responseDoc, response);
              Serial.println("Response payload: ");
              Serial.println(response);
              BLESendData(characteristic, response);
            } else {
              Serial.print("Error on HTTP request: ");
              Serial.println(httpCode);
            }
            http.end();
          } else {
            Serial.println("WiFi not connected");
          }
        }
          else
          {
            Serial.println("ERROR : UNKNOWN COMMAND");
          }      
      }   

    }
};



void setup() {
  // Start serial communication 
  Serial.begin(115200);
//BLE INIT

  // BEGIN DON'T CHANGE
  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  // Set server callbacks
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bleService = pServer->createService(SERVICE_UUID);

  // Create BLE characteristics and descriptors
  bleService->addCharacteristic(&characteristic);  
  characteristic.addDescriptor(characteristicDescriptor);
  // Set chacrateristic callbacks
  characteristic.setCallbacks(new CharacteristicsCallbacks());
  
  // Start the service
  bleService->start();

  // Start advertising
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  // END DON'T CHANGE

//WIFI INIT
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop() {
  // possible code to be executed for the actions which run periodically 
  // (possibly conditioned by an interrupt flag)

  //if connected to wi-fi,get list of characters from
  //http://proiectia.bogdanflorea.ro/api/better-call-saul/characters

}