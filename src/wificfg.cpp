#include "wificfg.h"

#include <WiFi.h>
#include <config.h>
#include "common.h"

void initAPWiFi()
{
  // Connect to Wi-Fi network with SSID and password
  LOG.println("  WIFI Setting AP (Access Point)");
  // NULL sets an open Access Point
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);

  IPAddress IP = WiFi.softAPIP();
  LOG.print("    AP IP address: ");
  LOG.println(IP);
}

// Initialize WiFi
bool initWiFi()
{
  LOG.print("  WiFi...");
  preferences.begin(wifiConfigKey, RO_MODE);

  String ssid = preferences.getString(wifiSSIDKey, "");
  if (ssid == "")
  {
    preferences.end();
    LOG.println("Undefined SSID.");
    return false;
  }

  WiFi.mode(WIFI_STA);

  String pass = preferences.getString(wifiPassKey, "");
  WiFi.setHostname(HOSTNAME_WIFI);
  WiFi.begin(ssid.c_str(), pass.c_str());
  preferences.end();
  LOG.println("Connecting to WiFi...");

  // Timer variables
  unsigned long previousMillis = 0;
  const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  LOG.print("    ");
  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      LOG.println("Failed to connect.");
      return false;
    }
  }
  LOG.print("    IP:");
  LOG.println(WiFi.localIP());
  LOG.println("  WiFi...connected");
  return true;
}