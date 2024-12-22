#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

const char *ssid = "fyszna";
const char *password = "17072004";
const char *thingSpeakAPIKey = "26W5VAZNELS1ACQ6";
const char *thingSpeakChannelID = "2410865";

const int potPin = A0;
float ph;
float value = 0;

void setup() {
  Serial.begin(115200);

  // Inisialisasi LCD
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("IoT pH Meter");
  LCD.setCursor(0, 1);
  LCD.print("WiFi");
  LCD.clear();

  // Inisialisasi koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(potPin, INPUT);
  delay(1000);
}

void loop() {
  value = analogRead(potPin);
  Serial.print("Raw Value: ");
  Serial.print(value);
  Serial.print(" | ");

  float voltage = value * (5 / 4095.0);
  ph = 5 * voltage;

  Serial.print("pH Value: ");
  Serial.println(ph);

  // Menampilkan pH di LCD
  LCD.setCursor(0, 0);
  LCD.print("pH= ");
  LCD.setCursor(0, 1);
  LCD.print(ph);

  // Kirim data ke ThingSpeak
  sendDataToThingSpeak(ph);

  delay(5000);  // Tunggu 5 detik sebelum membaca ulang nilai pH
}

void sendDataToThingSpeak(float pHValue) {
  WiFiClient client;

  // Buat URL untuk mengirim data ke ThingSpeak
  String url = "/update?api_key=";
  url += thingSpeakAPIKey;
  url += "&field1=";
  url += String(pHValue);

  // Hubungkan ke server ThingSpeak
  if (client.connect("api.thingspeak.com", 80)) {
    Serial.println("Connected to ThingSpeak");

    // Kirim permintaan HTTP GET
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();

    // Tunggu sampai data terkirim
    delay(2000);

    // Tutup koneksi
    client.stop();
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}