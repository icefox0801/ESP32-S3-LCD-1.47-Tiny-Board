# SVG to PNG Conversion Guide

## Current Setup (✅ WORKING)

The project uses **Inkscape** to convert SVG weather icons to 64x64 PNG format for LVGL display.

## Quick Start

Simply run the conversion script from anywhere:

```bash
python resources/convert_with_inkscape.py
```

The script automatically:
- Changes to the project root directory
- Converts all 64 SVG files from `resources/icons/`
- Outputs PNG files to `data/icons/`
- Uses Inkscape for high-quality rendering

## Requirements

**Inkscape** must be installed:
1. Download: https://inkscape.org/release/
2. Install to default location: `C:\Program Files\Inkscape\`
3. The script automatically finds it

## Output

All PNG files are saved to:
```
data/icons/
├── day_1_1.png ... day_4_8.png    (32 day icons)
└── night_1_1.png ... night_4_8.png (32 night icons)
```

Each PNG is 64x64 pixels, ~900-2000 bytes with transparency.

## Deployment

After conversion:

```bash
# 1. Build firmware
pio run

# 2. Upload filesystem (PNG files to SPIFFS)
pio run --target uploadfs

# 3. Upload firmware
pio run --target upload
```

## Technical Details

- **PNG Decoder**: LVGL's internal LODEPNG (`LV_USE_LODEPNG = 1`)
- **Filesystem**: SPIFFS with custom LVGL driver (letter 'S:')
- **Icon Loading**: PNG files loaded dynamically via `lv_image_set_src(img, "S:/icons/day_1_1.png")`
- **Mapping**: WeatherAPI.com condition codes → PNG filenames

## Alternative Methods

If Inkscape is unavailable:

### ImageMagick
```bash
magick input.svg -background none -resize 64x64 output.png
```

### Online Converters
- https://cloudconvert.com/svg-to-png
- https://convertio.co/svg-png/

Note: Manual methods require moving PNGs to `data/icons/` directory.
