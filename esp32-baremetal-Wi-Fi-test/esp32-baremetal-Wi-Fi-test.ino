#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "wi-fi-config.h"

const char* ssid     = MY_SSID;
const char* password = MY_PASSWORD;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop(){

  // wait for WiFi connection
  if(WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      
      http.begin("https://umayadia-apisample.azurewebsites.net/api/persons/Shakespeare"); // sample url

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if(httpCode == HTTP_CODE_OK) {
            StaticJsonDocument<200> doc;
            
            DeserializationError error = deserializeJson(doc, http.getString());
          
            // パースが成功したか確認。できなきゃ終了
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
            }else{
              const char* jsonName = doc["data"]["name"];
              Serial.printf("jsonName = %s\n",jsonName);
              const int jsonAge = doc["data"]["age"];
              Serial.printf("jsonAge = %d\n",jsonAge);
            }
          }
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
  }
  else
  {
    Serial.println("WiFi no connected.");
  }

  delay(5000);  
}
