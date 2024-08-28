#include <WiFi.h>  
#include <HTTPClient.h>
#include <ArduinoJson.h> 

const char* ssid = "RobertGalaxyS21";      
const char* password = "Knasboll95";   
const char* serverName = "https://api.open-meteo.com/v1/forecast?latitude=55.67&longitude=12.56&current_weather=true";

const float thresholdTemp = 25.0;


void setup() {
  delay(2000); 
  Serial.begin(115200); 
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);  
    int httpCode = http.GET();  

    if (httpCode > 0) { 
      String payload = http.getString();
      //Serial.println(payload);  

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float temperature = doc["current_weather"]["temperature"];

        Serial.print("Temperature: ");
        Serial.println(temperature);

        if (temperature > thresholdTemp) {
          Serial.println("It's higher than 25 degrees celsius");
        } else {
          Serial.println("It's lower than 25 degrees celsius");
        }
      } else {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); 
  } else {
    Serial.println("WiFi not connected");
  }

  delay(60000);  
}
