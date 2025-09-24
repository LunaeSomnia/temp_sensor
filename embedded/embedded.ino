#include "./WiFiCredentials.h"
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <WiFi.h>

#define ONE_WIRE_BUS 5
#define MAXIMUM_WIFI_RETRIES 25
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void connectToWifi(bool reconnecting) {
  int retries = 0;
  if (reconnecting) {
    Serial.println("WiFi connection failed.");
    Serial.print("Reconnecting ");
  } else {
    Serial.print("Connecting to WiFi ");
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED && retries < MAXIMUM_WIFI_RETRIES) {
    delay(1000);
    Serial.print(".");
    retries++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(" Failed\n");
  } else {
    Serial.print(" Connected\n");
  }
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void setup() {
  // Configure time
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Initialize sensors
  Serial.begin(115200);
  sensors.begin();
  Serial.printf("Found %d temperature devices.\n", sensors.getDeviceCount());

  // Connect to Wifi
  connectToWifi(false);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi(true);
    return;
  }

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  HTTPClient http;
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "application/json");

  String jsonData = "{ \"timestamp\": " + String(getTime()) +
                    ", \"data\": " + String(temperature) + " }";
  int httpResponseCode = http.POST(jsonData);
  Serial.printf("%d (%s) : ", httpResponseCode,
                http.errorToString(httpResponseCode).c_str());
  http.end();

  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000);
}
