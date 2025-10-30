// Own header
#include "wifi_setup.h"

WiFiSetup::WiFiSetup()
{
}

bool WiFiSetup::init()
{
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  return true;
}

bool WiFiSetup::connect()
{
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long start_time = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < connection_timeout)
  {
    delay(500);
  }

  // Configure Beijing timezone (CST - China Standard Time, UTC+8)
  if (WiFi.status() == WL_CONNECTED)
  {
    configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    // Wait for NTP time sync (up to 5 seconds)
    Serial.print("Waiting for NTP time sync...");
    int retry = 0;
    time_t now = 0;
    while (retry < 10)
    {
      time(&now);
      if (now > 1000000000)
      { // Valid timestamp (after year 2001)
        Serial.printf(" synced! Time: %lu\n", (unsigned long)now);
        break;
      }
      Serial.print(".");
      delay(500);
      retry++;
    }
    if (now < 1000000000)
    {
      Serial.println(" timeout! Time may be incorrect.");
    }
  }

  return WiFi.status() == WL_CONNECTED;
}

bool WiFiSetup::isConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

String WiFiSetup::getStatusString()
{
  switch (WiFi.status())
  {
  case WL_CONNECTED:
    return "Connected";
  case WL_NO_SSID_AVAIL:
    return "SSID not found";
  case WL_CONNECT_FAILED:
    return "Connection failed";
  case WL_CONNECTION_LOST:
    return "Connection lost";
  case WL_DISCONNECTED:
    return "Disconnected";
  case WL_IDLE_STATUS:
    return "Idle";
  case WL_SCAN_COMPLETED:
    return "Scan completed";
  default:
    return "Unknown (" + String(WiFi.status()) + ")";
  }
}

String WiFiSetup::getIPAddress()
{
  if (isConnected())
  {
    return WiFi.localIP().toString();
  }
  return "0.0.0.0";
}

void WiFiSetup::handleReconnect()
{
  if (!isConnected() && (millis() - last_retry) > retry_interval)
  {
    last_retry = millis();
    connect();
  }
}

int WiFiSetup::getRSSI()
{
  if (isConnected())
  {
    return WiFi.RSSI();
  }
  return 0;
}

int WiFiSetup::getSignalStrength()
{
  if (!isConnected())
    return 0;

  int rssi = WiFi.RSSI();

  // Convert RSSI to percentage (rough approximation)
  if (rssi >= -50)
    return 100;
  else if (rssi >= -60)
    return 80;
  else if (rssi >= -70)
    return 60;
  else if (rssi >= -80)
    return 40;
  else if (rssi >= -90)
    return 20;
  else
    return 0;
}
