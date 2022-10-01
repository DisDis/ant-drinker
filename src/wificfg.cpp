#include "wificfg.h"

#include <WiFi.h>
#include <config.h>
#include "common.h"

void initAPWiFi()
{
  // Connect to Wi-Fi network with SSID and password
  Serial.println("  WIFI Setting AP (Access Point)");
  // NULL sets an open Access Point
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("    AP IP address: ");
  Serial.println(IP);
}

// Initialize WiFi
bool initWiFi()
{
  Serial.print("  WiFi...");
  preferences.begin(wifiConfigKey, RO_MODE);

  String ssid = preferences.getString(wifiSSIDKey, "");
  if (ssid == "")
  {
    preferences.end();
    Serial.println("Undefined SSID.");
    return false;
  }

  WiFi.mode(WIFI_STA);

  String pass = preferences.getString(wifiPassKey, "");
  WiFi.begin(ssid.c_str(), pass.c_str());
  preferences.end();
  Serial.println("Connecting to WiFi...");

  // Timer variables
  unsigned long previousMillis = 0;
  const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  Serial.print("    ");
  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      Serial.println("Failed to connect.");
      return false;
    }
  }
  Serial.print("    IP:");
  Serial.println(WiFi.localIP());
  Serial.println("  WiFi...connected");
  return true;
}