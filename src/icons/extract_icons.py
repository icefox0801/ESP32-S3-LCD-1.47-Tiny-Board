#!/usr/bin/env python3
"""
Extract weather icons from sprite sheet and convert to BMP preview
- Extracts icons without background replacement (preserves anti-aliased edges)
- Converts generated C files to BMP format for Windows preview
"""

import sys
import re
import os
import struct

# Icon size in the sprite sheet
ICON_SIZE = 176

# === Icon Extraction Functions ===

def read_c_array(filename):
    """Read the C array data from weathers.c"""
    with open(filename, 'r') as f:
        content = f.read()

    # Extract hex values between the braces
    start = content.find('{') + 1
    end = content.rfind('}')
    data_str = content[start:end]

    # Parse hex values
    hex_values = []
    for item in data_str.split(','):
        item = item.strip()
        if item.startswith('0x'):
            hex_values.append(int(item, 16))

    return bytes(hex_values)

def extract_icon(pixel_data, source_width, source_height, icon_x, icon_y, icon_width, icon_height):
    """Extract a rectangular region from the sprite sheet"""
    extracted = []

    for y in range(icon_height):
        for x in range(icon_width):
            src_x = icon_x + x
            src_y = icon_y + y

            if src_x >= source_width or src_y >= source_height:
                extracted.append(0x0000)
            else:
                pixel_idx = (src_y * source_width + src_x) * 2
                if pixel_idx + 1 < len(pixel_data):
                    low = pixel_data[pixel_idx]
                    high = pixel_data[pixel_idx + 1]
                    rgb565 = (high << 8) | low
                    extracted.append(rgb565)
                else:
                    extracted.append(0x0000)

    return extracted

def resize_nearest(pixel_data, src_width, src_height, dst_width, dst_height):
    """Resize using nearest neighbor"""
    resized = []

    for y in range(dst_height):
        for x in range(dst_width):
            src_x = int(x * src_width / dst_width)
            src_y = int(y * src_height / dst_height)

            src_idx = src_y * src_width + src_x
            if src_idx < len(pixel_data):
                resized.append(pixel_data[src_idx])
            else:
                resized.append(0x0000)

    return resized

def generate_c_file(pixel_data, width, height, var_name, filename):
    """Generate a C file with the icon data"""
    with open(filename, 'w') as f:
        f.write('#include "lvgl.h"\n\n')
        f.write('#ifndef LV_ATTRIBUTE_MEM_ALIGN\n')
        f.write('#define LV_ATTRIBUTE_MEM_ALIGN\n')
        f.write('#endif\n\n')

        # Write pixel data
        f.write(f'const LV_ATTRIBUTE_MEM_ALIGN uint8_t {var_name}_map[] = {{\n')

        for i, pixel in enumerate(pixel_data):
            if i % 16 == 0:
                f.write('  ')

            low = pixel & 0xFF
            high = (pixel >> 8) & 0xFF
            f.write(f'0x{low:02x}, 0x{high:02x}, ')

            if (i + 1) % 16 == 0:
                f.write('\n')

        f.write('};\n\n')

        # Write image descriptor
        f.write(f'const lv_image_dsc_t {var_name} = {{\n')
        f.write(f'  .header.cf = LV_COLOR_FORMAT_RGB565,\n')
        f.write(f'  .header.magic = LV_IMAGE_HEADER_MAGIC,\n')
        f.write(f'  .header.w = {width},\n')
        f.write(f'  .header.h = {height},\n')
        f.write(f'  .data_size = {width * height * 2},\n')
        f.write(f'  .data = {var_name}_map,\n')
        f.write(f'}};\n')

# Known icon positions from previous extraction
ICON_POSITIONS = {
    "sunny": (0, 2),
    "clear-night": (0, 4),
    "partlycloudy": (0, 0),
    "cloudy": (0, 1),
    "rainy": (1, 0),
    "pouring": (1, 1),
    "snowy": (4, 0),
    "snowy-rainy": (4, 0),  # Same as snowy
    "fog": (0, 0),  # Same as partlycloudy
    "hail": (0, 0),  # Same as partlycloudy
    "windy": (5, 0),
    "lightning": (3, 9),
    "lightning-rainy": (3, 9),  # Same as lightning
    "exceptional": (0, 0),  # Same as partlycloudy
}

def calculate_position(row, col):
    """Calculate pixel position from grid coordinates"""
    x = 80 + col * 176
    y = 61 + row * 176
    return x, y

# === BMP Conversion Functions ===

def rgb565_to_rgb888(rgb565):
    """Convert RGB565 to RGB888"""
    r = ((rgb565 >> 11) & 0x1F) << 3
    g = ((rgb565 >> 5) & 0x3F) << 2
    b = (rgb565 & 0x1F) << 3

    # Expand to full 8-bit range
    r = r | (r >> 5)
    g = g | (g >> 6)
    b = b | (b >> 5)

    return (r, g, b)

def read_icon_c_file_for_bmp(filename):
    """Read RGB565 pixel data from a C file for BMP conversion"""
    with open(filename, 'r') as f:
        content = f.read()

    # Find the byte data array (stored as uint8_t pairs for RGB565)
    byte_pattern = r'0x[0-9a-fA-F]{2}'
    matches = re.findall(byte_pattern, content)

    if not matches:
        raise ValueError(f"No pixel data found in {filename}")

    # Convert hex strings to integers
    byte_data = [int(match, 16) for match in matches]

    # Combine byte pairs into RGB565 values (little-endian)
    pixel_data = []
    for i in range(0, len(byte_data), 2):
        if i + 1 < len(byte_data):
            low_byte = byte_data[i]
            high_byte = byte_data[i + 1]
            rgb565 = (high_byte << 8) | low_byte
            pixel_data.append(rgb565)

    # Determine image size (assume square image)
    num_pixels = len(pixel_data)
    size = int(num_pixels ** 0.5)

    if size * size != num_pixels:
        raise ValueError(f"Cannot determine square dimensions from {len(pixel_data)} pixels")

    return pixel_data, size, size

def create_bmp_file(pixel_data, width, height, filename):
    """Create a BMP file from RGB565 pixel data"""

    # BMP header
    file_size = 54 + (width * height * 3)  # Header + RGB data

    # BMP file header (14 bytes)
    bmp_file_header = struct.pack('<2sIHHI',
        b'BM',        # Signature
        file_size,    # File size
        0,            # Reserved
        0,            # Reserved
        54            # Offset to pixel data
    )

    # BMP info header (40 bytes)
    bmp_info_header = struct.pack('<IIIHHIIIIII',
        40,           # Header size
        width,        # Width
        height,       # Height
        1,            # Planes
        24,           # Bits per pixel (RGB)
        0,            # Compression (none)
        width * height * 3,  # Image size
        2835,         # X pixels per meter
        2835,         # Y pixels per meter
        0,            # Colors used
        0             # Important colors
    )

    # Convert pixel data to BMP format (BGR, bottom-up)
    bmp_data = bytearray()
    row_padding = (4 - (width * 3) % 4) % 4

    # BMP stores rows bottom-up
    for y in range(height - 1, -1, -1):
        for x in range(width):
            idx = y * width + x
            rgb565 = pixel_data[idx]
            r, g, b = rgb565_to_rgb888(rgb565)
            # BMP uses BGR order
            bmp_data.extend([b, g, r])
        # Add row padding
        bmp_data.extend([0] * row_padding)

    # Write BMP file
    with open(filename, 'wb') as f:
        f.write(bmp_file_header)
        f.write(bmp_info_header)
        f.write(bmp_data)

def convert_icons_to_bmp():
    """Convert all icon C files to BMP for preview"""
    print()
    print("=" * 70)
    print("Converting icons to BMP for preview...")
    print("=" * 70)

    # Find all icon C files
    icon_files = [f for f in os.listdir('.') if f.endswith('_icon_64.c')]
    icon_files.sort()

    success_count = 0

    for c_filename in icon_files:
        bmp_filename = c_filename.replace('.c', '.bmp')

        try:
            pixel_data, width, height = read_icon_c_file_for_bmp(c_filename)
            create_bmp_file(pixel_data, width, height, bmp_filename)
            success_count += 1
        except Exception as e:
            print(f"  BMP conversion error for {c_filename}: {e}")

    print(f"  Converted {success_count}/{len(icon_files)} icons to BMP")
    print()

def main():
    output_size = 64

    print("=" * 70)
    print("Re-extracting Weather Icons (No Background Replacement)")
    print("=" * 70)
    print()

    # Read sprite sheet
    print("Reading sprite sheet (weathers.c)...")
    pixel_data = read_c_array('weathers.c')
    sprite_width = 1920
    sprite_height = 1178
    print(f"  Loaded: {sprite_width}x{sprite_height}")
    print()

    success_count = 0

    for state_name, (row, col) in ICON_POSITIONS.items():
        print(f"Extracting: {state_name} at ({row},{col})")

        try:
            # Calculate position
            icon_x, icon_y = calculate_position(row, col)

            # Extract icon
            extracted = extract_icon(pixel_data, sprite_width, sprite_height,
                                   icon_x, icon_y, ICON_SIZE, ICON_SIZE)

            # NO background replacement - preserve original

            # Resize
            resized = resize_nearest(extracted, ICON_SIZE, ICON_SIZE, output_size, output_size)

            # Generate C files - replace hyphens with underscores
            file_safe_name = state_name.replace('-', '_')
            output_name = f"{file_safe_name}_icon_{output_size}"
            c_file = f"{output_name}.c"
            h_file = f"{output_name}.h"

            generate_c_file(resized, output_size, output_size, output_name, c_file)

            with open(h_file, 'w') as f:
                f.write(f'#ifndef {output_name.upper()}_H\n')
                f.write(f'#define {output_name.upper()}_H\n\n')
                f.write('#include "lvgl.h"\n\n')
                f.write(f'extern const lv_image_dsc_t {output_name};\n\n')
                f.write('#endif\n')

            print(f"  -> Generated {output_name}.c/h")
            success_count += 1

        except Exception as e:
            print(f"  ERROR: {e}")
            import traceback
            traceback.print_exc()

    print()
    print("=" * 70)
    print(f"Complete: {success_count}/{len(ICON_POSITIONS)} icons extracted")
    print("=" * 70)
    print()
    print("Icons now have original sprite sheet background (purple)")
    print("Update weather card background to match: 0x522c8c or 0x5171 (RGB565)")

    # Convert icons to BMP for preview
    convert_icons_to_bmp()
    print("You can now preview the icons in Windows Explorer!")


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nCancelled by user")
        sys.exit(1)
    except Exception as e:
        print(f"\n\nError: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
