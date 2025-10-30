# Weather API Configuration

## Overview

This project uses **WeatherAPI.com** as the weather data provider for the ESP32-S3 LCD weather display. The system has been optimized for simplicity and reliability with direct API integration.

## API Configuration

### WeatherAPI.com Setup

1. **Get API Key**: Sign up at [WeatherAPI.com](https://www.weatherapi.com/)
2. **Configure secrets.h**:
   ```cpp
   #define WEATHER_API_KEY "your_api_key_here"
   #define WEATHER_LOCATION "Beijing"  // City name or coordinates
   #define WEATHER_UNITS "metric"      // metric, imperial
   ```

### Current Configuration
- **Provider**: WeatherAPI.com
- **Location**: Beijing
- **Units**: Metric (°C)
- **Update Interval**: 10 minutes
- **API Calls**: Optimized single call for current weather + today's forecast

## API Endpoints Used

### WeatherAPI.com
- **Current + Forecast**: `http://api.weatherapi.com/v1/forecast.json`
- **Parameters**:
  - `key`: Your API key
  - `q`: Location (city name, coordinates, postcode)
  - `days=1`: Today's forecast only
  - `aqi=yes`: Air quality data
  - `alerts=no`: No weather alerts

## Weather Display Features

### Current Implementation
- **Temperature**: Current, min/max from forecast
- **Conditions**: 64x64 PNG weather icons (day/night variants)
- **Details**: Humidity, air quality (PM2.5)
- **Data Refresh**: Automatic every 10 minutes (checks every 5 minutes)

### Weather Icon System
The system uses 64 high-quality PNG weather icons (64x64 pixels) with day/night variants:
- Loaded dynamically via LVGL SPIFFS filesystem
- Mapped to WeatherAPI.com condition codes (1000-1282)
- Day/night detection based on time (6 AM - 6 PM = day)
- Files: `day_1_1.png` to `day_4_8.png` (32 day icons)
- Files: `night_1_1.png` to `night_4_8.png` (32 night icons)

## Code Architecture

### Main Components
- `WeatherAPI` class: Handles API calls and data parsing
- `WeatherIcons` class: Loads PNG icons and maps condition codes
- `WeatherData` struct: Stores essential weather information (simplified)
- `WeatherUI` class: Manages weather display with helper methods

### Key Methods (WeatherAPI)
- `fetchWeatherData()`: Main update method with timestamp capture
- `fetchCurrentAndTodayWeatherAPI()`: Single optimized API call
- `getCurrentWeather()`: Get current weather data
- `getTemperatureString()`: Formatted temperature display
- `getHumidityString()`: Formatted humidity display
- `getAirQualityString()`: Formatted PM2.5 display
- `getLastUpdateTime()`: Get actual fetch timestamp
- `needsUpdate()`: Check if data needs refresh

### Key Methods (WeatherUI)
- `updateWeatherDisplay()`: Main display update orchestrator
- `updateTemperatureDisplay()`: Update temperature values
- `updateHumidityDisplay()`: Update humidity value
- `updateAirQualityDisplay()`: Update PM2.5 AQI value
- `updateTimestampDisplay()`: Update last refresh time with forced LVGL refresh
- `formatTimestamp()`: Format time_t as "HH:MM Mon DD"
- `isDaytime()`: Check if current time is 6 AM - 6 PM

## API Limits & Usage

### WeatherAPI.com Free Tier
- **Requests**: 1,000,000 calls/month
- **Rate Limit**: No specific limit mentioned
- **Features**: Current weather, forecast, historical data
- **Update Frequency**: Recommended 10-15 minutes for this project

### Optimization Features
- Single API call for current + forecast data
- Field filtering to reduce response size
- Error handling with fallback values
- Automatic retry on network failures

## Troubleshooting

### Common Issues
1. **No Data Display**: Check API key validity and network connection
2. **Wrong Location**: Verify location string format (city name works best)
3. **API Errors**: Check serial monitor for HTTP response codes
4. **Icon Display**: Ensure LVGL emoji font support is enabled

### Debug Information
The system uses conditional debug logging controlled by `DEBUG_ENABLED` in `config.h`:

**Production Mode (DEBUG_ENABLED=0):**
```
[INFO] Weather update successful
[ERROR] Failed to fetch weather data
```

**Debug Mode (DEBUG_ENABLED=1):**
```
Temp: 15.2°C, Range: 8.0-18.0°C, Condition: 1003
Weather fetched at: 1698660000
Fetch time: 1698660000
```

Debug macros available:
- `DEBUG_LOG(msg)` - Basic debug message
- `DEBUG_LOGF(fmt, ...)` - Formatted debug message
- `LOG_INFO(msg)` - Always-on info message
- `LOG_ERROR(msg)` - Always-on error message

## Recent Improvements

### Code Refactoring (October 2025)
✅ **Debug System**: Added conditional logging with DEBUG_ENABLED flag
✅ **Helper Methods**: Extracted 6 helper methods from large updateWeatherDisplay()
✅ **Timestamp Fix**: Store actual fetch time instead of current time for display
✅ **Code Cleanup**: Removed unused features and methods (~350 lines removed)

### Removed Features (Simplified)
❌ Wind speed, direction, and bearing
❌ Atmospheric pressure measurements
❌ Cloud coverage and UV index
❌ Forecast arrays and unused helper methods
❌ Empty stub functions (startAutoUpdate, stopAutoUpdate, hideWeatherScreen)

### Current Focus
- Essential weather data only: temperature, humidity, air quality
- Single optimized API call per update
- Clean, maintainable codebase with helper methods
- Conditional debug logging for production/development modes

## Future Enhancements

### Possible Improvements
- Extended forecast (3-5 days) display
- Weather alerts integration
- Multiple location support
- Touch interface for settings
- Custom themes and layouts

### Performance Optimizations
- Cached data storage in preferences
- OTA firmware updates
- Multi-language support
