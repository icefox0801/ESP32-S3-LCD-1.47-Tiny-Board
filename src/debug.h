#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "config.h"

// Debug logging macros - controlled by DEBUG_ENABLED in config.h
#if DEBUG_ENABLED
#define DEBUG_LOG(msg) Serial.println(msg)
#define DEBUG_LOGF(format, ...) Serial.printf(format, ##__VA_ARGS__)
#define DEBUG_PRINT(msg) Serial.print(msg)
#else
#define DEBUG_LOG(msg)
#define DEBUG_LOGF(format, ...)
#define DEBUG_PRINT(msg)
#endif

// Always-on logging for critical messages
#define LOG_INFO(msg) Serial.println(msg)
#define LOG_INFOF(format, ...) Serial.printf(format, ##__VA_ARGS__)
#define LOG_ERROR(msg) Serial.println("[ERROR] " msg)
#define LOG_ERRORF(format, ...) Serial.printf("[ERROR] " format, ##__VA_ARGS__)

#endif // DEBUG_H
