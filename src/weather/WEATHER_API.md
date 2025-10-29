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
- **Conditions**: Unicode emoji weather icons
- **Details**: Humidity, pressure, wind speed/direction
- **Data Refresh**: Automatic every 10 minutes

### Weather Icon System
The system uses Unicode emoji icons mapped to WeatherAPI.com condition codes:
- ☀️ Clear/Sunny
- ⛅ Partly Cloudy  
- ☁️ Cloudy
- 🌧️ Rain
- ⛈️ Thunderstorm
- ❄️ Snow
- 🌫️ Fog/Mist

## Code Architecture

### Main Components
- `WeatherAPI` class: Handles API calls and data parsing
- `WeatherIcons` class: Maps condition codes to emoji symbols
- `WeatherData` struct: Stores current weather information

### Key Methods
- `fetchWeatherData()`: Main update method
- `fetchCurrentAndTodayWeatherAPI()`: Optimized API call
- `getCurrentWeather()`: Get current weather data
- `getTemperatureString()`: Formatted temperature display

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
The system outputs debug information via Serial:
```
WeatherAPI.com: Current temp: 15.2°C
WeatherAPI.com: Today's range: 8°C to 18°C  
WeatherAPI.com: Condition code: 1003
```

## Future Enhancements

### Possible Improvements
- Extended forecast (3-5 days)
- Weather alerts integration
- Historical weather data
- Multiple location support
- Hourly forecast display

### Performance Optimizations
- Cached data storage in preferences
- Reduced API call frequency based on weather stability
- Compression of weather data structures
- Background data fetching