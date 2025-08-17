#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

class WiFiSetup
{
private:
  String ssid;
  String password;
  unsigned long connection_timeout = 20000; // 20 seconds
  unsigned long retry_interval = 30000;     // 30 seconds between retries
  unsigned long last_retry = 0;

public:
  WiFiSetup(const String &wifi_ssid, const String &wifi_password);

  // Initialize WiFi connection
  bool init();

  // Connect to WiFi
  bool connect();

  // Check WiFi status
  bool isConnected();

  // Get connection status string
  String getStatusString();

  // Get IP address
  String getIPAddress();

  // Auto-reconnect if disconnected
  void handleReconnect();

  // Get signal strength
  int getRSSI();

  // Get signal strength as percentage
  int getSignalStrength();
};

#endif // WIFI_SETUP_H
