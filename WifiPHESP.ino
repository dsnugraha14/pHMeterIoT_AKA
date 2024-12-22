#include <WiFi.h>
#include <HTTPClient.h>
#include "SoftwareSerial.h"

const char *ssid = "Dida";
const char *password = "012345678";
const char *thingSpeakAPIKey = "8H7Z5JOF4ZAX60R9";
const char *thingSpeakChannelID = "2479122";

SoftwareSerial pH(34, 35); //(Rx, Tx)

void setup() {
  Serial.begin(9600);
  pH.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  String ph_act = "";
  
  // Wait until a complete string is received
  while (pH.available() > 0) {
    char c = pH.read();
    if (c == '\n') { // assuming '\n' is the end of the pH reading
      sendDataToThingSpeak(ph_act.toFloat());
      ph_act = ""; // Reset the string for next reading
    } else {
      ph_act += c;
    }
  }
  delay(1000);
}

void sendDataToThingSpeak(float ph_act) {
  WiFiClient client;

  // Create URL to send data to ThingSpeak
  String url = "/update?api_key=";
  url += thingSpeakAPIKey;
  url += "&field1=";
  url += String(ph_act);

  // Connect to ThingSpeak server
  if (client.connect("api.thingspeak.com", 80)) {
    Serial.println("Connected to ThingSpeak");

    // Send HTTP GET request
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();

    // Wait until data is sent
    delay(5000);

    // Close the connection
    client.stop();
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}
