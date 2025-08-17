// Own header
#include "wifi_setup.h"

WiFiSetup::WiFiSetup(const String &wifi_ssid, const String &wifi_password)
    : ssid(wifi_ssid), password(wifi_password)
{
}

bool WiFiSetup::init()
{
  Serial.println("[WiFi] Initializing WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.println("[WiFi] WiFi initialized");
  return true;
}

bool WiFiSetup::connect()
{
  Serial.println("[WiFi] Connecting to WiFi network: " + ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long start_time = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < connection_timeout)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("[WiFi] WiFi connected successfully!");
    Serial.println("[WiFi] IP address: " + getIPAddress());
    Serial.println("[WiFi] Signal strength: " + String(getRSSI()) + " dBm (" + String(getSignalStrength()) + "%)");
    return true;
  }
  else
  {
    Serial.println();
    Serial.println("[WiFi] WiFi connection failed!");
    Serial.println("[WiFi] Status: " + getStatusString());
    return false;
  }
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
    Serial.println("[WiFi] Connection lost, attempting to reconnect...");
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
