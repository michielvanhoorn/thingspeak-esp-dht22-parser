// Thingspeak DHT humudity / temperature data parser
// Using NODEMCU 1.0 / ESP-12E Board
// by Michiel van Hoorn - 2016 - http://stuffingpi.azurewebsites.net/


// MIT License
// Copyright (c) 2016 Michiel van Hoorn
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// DHT Setup
#include "DHT.h"
DHT dht(D2, DHT22);
float humidity;
float temperature;

// Wifi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Network configuration - use your own data
const char* ssid = "Your WIFI SSID here";
const char* password = "Your WIFI key here";

// Thingspeak API configuration  - use your own data
const char* thingspeakServer = "api.thingspeak.com";
String apikey = "Your API key here";


WiFiClient client;


void setup(void)
{
  dht.begin();

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.println("Serial done....");
 
  WiFi.begin(ssid, password);


  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop(void)
{

getSensorData(); // Get sensor data
sendDataToThingspeak(); //Post sensor data

delay(20000);  // wait 20 seconds

}

void sendDataToThingspeak(){
  if (client.connect(thingspeakServer, 80)){
String getJSON = "";
getJSON += "GET http://" + (String) thingspeakServer;
getJSON += "/update.json?api_key=" + (String) apikey;
getJSON += "&field1=" + (String) humidity;
getJSON += "&field2=" + (String) temperature + "\n";
getJSON += "Connection: close\n\n";
client.print(getJSON);


// display response from Thingspeak
while (client.available())
  {
     Serial.write(client.read());
  }
  Serial.println("Data sent to Thingspeak");
  }
  else
  {
  Serial.println("Connection to Thingspeak failed");
  }
  client.stop();
}

void getSensorData(void){

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

   if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
   
  }
