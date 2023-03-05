#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"


#define light1 27
#define light2 26
#define light3 25

int j,i;
 bool ledstate[][3]=
 {{false,false,false}, 
  {false, false, true},
  {false, true, false},
  {true, false, false},
  {true, true, false},
  {true, false, true},
  {true, true,true}};

const int numofstates = sizeof (ledstate) / sizeof (ledstate[0]);
void setup() {
  // put your setup code here, to run once:
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);

  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WiFi_SSID, WiFi_PASS);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
  
    http.begin(endpoint);
    
    for(i=0;i<numofstates;i++)
    {
        http.addHeader("X-API-Key",key);
        http.addHeader("Content-type", "application/json");

        StaticJsonDocument<76>doc;
         String httpRequestData;

        digitalWrite(light1, ledstate[i][0]);
        digitalWrite(light2, ledstate[i][1]);
        digitalWrite(light3, ledstate[i][2]);
        
         
        doc["light1"] = ledstate[i][0];
        doc["light2"] = ledstate[i][1];
        doc["light3"] = ledstate[i][2];

        serializeJson(doc, httpRequestData);
	      int httpResponseCode= http.PUT(httpRequestData);
        String http_response;
        if(httpResponseCode>0)
    {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response from server");
      http_response=http.getString();
      Serial.println(http_response);

    }
    else{
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
     delay(2000);
    
      }
      http.end();
    }   
  }