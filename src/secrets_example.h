#ifndef SECRETS_H
#define SECRETS_H

// WiFi Configuration
// Copy this file to secrets.h and update with your actual WiFi credentials
// secrets.h is ignored by git for security

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Home Assistant Configuration
#define HA_SERVER_IP "192.168.1.100"
#define HA_SERVER_PORT 8123
#define HA_BEARER_TOKEN "YOUR_HOME_ASSISTANT_BEARER_TOKEN_HERE"
#define HA_WEATHER_ENTITY "weather.forecast_home"

#endif // SECRETS_H
