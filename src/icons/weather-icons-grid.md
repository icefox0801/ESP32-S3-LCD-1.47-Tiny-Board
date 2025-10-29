# Day Weather Icons Grid Layout (4×8)

The icons are positioned at:
- **Top row (y=166px)**: Row 1, columns 1-8
- **Second row (y=214px)**: Row 2, columns 1-8
- **Third row (y=262px)**: Row 3, columns 1-8
- **Bottom row (y=310px)**: Row 4, columns 1-8

Starting x-position: 196px, with 48px spacing between icons.

## Grid Reference:

| Position | Weather Code(s) | Description | File Name |
|----------|----------------|-------------|-----------|
| **Row 1** |
| 1-1 | 113 | Sunny | `day_1_1.svg` |
| 1-2 | 116 | Partly cloudy | `day_1_2.svg` |
| 1-3 | 119, 122 | Cloudy | `day_1_3.svg` |
| 1-4 | 143 | Mist | `day_1_4.svg` |
| 1-5 | 176, 179, 293, 299, 353 | Patchy rain or snow possible | `day_1_5.svg` |
| 1-6 | 182, 362 | Patchy sleet possible | `day_1_6.svg` |
| 1-7 | 185 | Patchy freezing drizzle possible | `day_1_7.svg` |
| 1-8 | 200, 386 | Thundery outbreaks possible | `day_1_8.svg` |
| **Row 2** |
| 2-1 | 227 | Blowing snow | `day_2_1.svg` |
| 2-2 | 230 | Blizzard | `day_2_2.svg` |
| 2-3 | 248 | Fog | `day_2_3.svg` |
| 2-4 | 260 | Freezing fog | `day_2_4.svg` |
| 2-5 | 263, 266 | Patchy light drizzle | `day_2_5.svg` |
| 2-6 | 281 | Freezing drizzle | `day_2_6.svg` |
| 2-7 | 284 | Heavy freezing drizzle | `day_2_7.svg` |
| 2-8 | 323, 329, 368 | Patchy light snow | `day_2_8.svg` |
| **Row 3** |
| 3-1 | 296, 302 | Light rain | `day_3_1.svg` |
| 3-2 | 305, 356 | Heavy rain at times | `day_3_2.svg` |
| 3-3 | 308 | Heavy rain | `day_3_3.svg` |
| 3-4 | 311, 314 | Light freezing rain | `day_3_4.svg` |
| 3-5 | 317, 320 | Light sleet | `day_3_5.svg` |
| 3-6 | 359 | Torrential rain shower | `day_3_6.svg` |
| 3-7 | 326, 332 | Light snow | `day_3_7.svg` |
| 3-8 | 335, 371 | Patchy heavy snow | `day_3_8.svg` |
| **Row 4** |
| 4-1 | 338 | Heavy snow | `day_4_1.svg` |
| 4-2 | 350 | Ice pellets | `day_4_2.svg` |
| 4-3 | 365 | Moderate or heavy sleet showers | `day_4_3.svg` |
| 4-4 | 374 | Light showers of ice pellets | `day_4_4.svg` |
| 4-5 | 377 | Moderate or heavy showers of ice pellets | `day_4_5.svg` |
| 4-6 | 389 | Moderate or heavy rain with thunder | `day_4_6.svg` |
| 4-7 | 392 | Patchy light snow with thunder | `day_4_7.svg` |
| 4-8 | 395 | Moderate or heavy snow with thunder | `day_4_8.svg` |

## Weather Categories:
- **Clear/Sunny**: 1-1
- **Cloudy conditions**: 1-2, 1-3
- **Mist/Fog**: 1-4, 2-3, 2-4
- **Light precipitation**: 1-5, 1-7, 2-5, 3-1
- **Heavy precipitation**: 3-2, 3-3, 3-6
- **Snow conditions**: 2-1, 2-2, 2-8, 3-7, 3-8, 4-1
- **Sleet/Ice**: 1-6, 2-6, 2-7, 3-4, 3-5, 4-2, 4-3, 4-4, 4-5
- **Thunderstorms**: 1-8, 4-6, 4-7, 4-8

## Night Icons
Corresponding night icons follow the same grid layout with `night_` prefix:
- `night_1_1.svg` to `night_4_8.svg`
- Same weather conditions but with moon/night variants
- Icons may show moon behind clouds for transitional weather

## Icon Specifications
- **Size**: 48×48 pixels
- **Format**: Unicode Emoji
- **Colors**: Native emoji colors
- **Usage**: Weather condition display in ESP32-S3 LCD interface

This system uses WeatherAPI.com condition codes with direct emoji mapping for reliable weather display on embedded systems.

## Weather Data Provider

The system uses **WeatherAPI.com** as the weather data provider:
- Accurate weather data with comprehensive condition codes
- Reliable free tier with 1 million calls per month
- Direct emoji mapping for consistent display

Current configuration uses WeatherAPI.com with API key: `6a67b0cfbd074410b41124242252910`
