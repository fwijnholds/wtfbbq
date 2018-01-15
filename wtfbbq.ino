#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>




//#define USE_SERIAL Serial

#define ONE_WIRE_BUS 2 // Data wire is plugged into port 2 on the Arduino

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 

const char* ssid     = "<your sid>";
const char* password = "<your key>";


void setup(void) {
Serial.begin(115200); //Open Serial Port
  delay(10);

  // Start up the library
  sensors.begin();
  
  // We start by connecting to a WiFi network

  Serial.println();
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


void loop(void) {
  // put your main code here, to run repeatedly:

sensors.requestTemperatures(); 
float temp = sensors.getTempCByIndex(0);
Serial.println(temp);
  
HTTPClient http;

http.begin("http://<yourserver>:8088/services/collector");
http.addHeader("Authorization", "Splunk <your token>");
String temperature = String(temp,2);
String payload = "{ \"host\" : \"SensorFilip\", \"sourcetype\" : \"_json\", \"index\" : \"main\", \"event\" : {\"temp\" : \"" + temperature + "\"}}"; 
http.POST(payload);
http.writeToStream(&Serial);
http.end();

    delay(1000);
}


