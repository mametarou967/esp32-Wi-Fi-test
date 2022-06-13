#include <freertos/FreeRTOS.h>    /* FreeRTOSを用いるためのヘッダファイル */
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "wi-fi-config.h"

#define WIFI_TIMEOUT_MS 20000 // 20 second WiFi connection timeout
#define WIFI_RECOVER_TIME_MS 30000 // Wait 30 seconds after a failed connection attempt


const char* ssid     = MY_SSID;
const char* password = MY_PASSWORD;


void keepWiFiAlive(void * parameter){
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
            exeWebApi();
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WIFI] Connecting");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        unsigned long startAttemptTime = millis();

        // Keep looping while we're not connected and haven't reached the timeout
        while (WiFi.status() != WL_CONNECTED && 
                millis() - startAttemptTime < WIFI_TIMEOUT_MS){}

        // When we couldn't make a WiFi connection (or the timeout expired)
      // sleep for a while and then retry.
        if(WiFi.status() != WL_CONNECTED){
          Serial.println("[WIFI] FAILED");
          vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
          continue;
        }else{
          Serial.println("[WIFI] Connected: ");
          Serial.println( WiFi.localIP());
        }
    }
}

void exeWebApi()
{
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
}


void setup()
{
    Serial.begin(115200);

    xTaskCreateUniversal(
      keepWiFiAlive,
      "keepWiFiAlive",  // Task name
      5000,             // Stack size (bytes)
      NULL,             // Parameter
      2,                // Task priority
      NULL,             // Task handle
      0
    );
}

void loop()
{
  
  Serial.println("[main] hello");
  vTaskDelay(3000);
}
