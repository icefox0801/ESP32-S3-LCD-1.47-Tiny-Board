#!/usr/bin/env python3
"""
Convert SVG weather icons to PNG using Inkscape (with full path)
"""

import subprocess
import sys
import os
from pathlib import Path

# Change to script directory
SCRIPT_DIR = Path(__file__).parent.parent
os.chdir(SCRIPT_DIR)

# Inkscape path (Windows default installation)
INKSCAPE = r"C:\Program Files\Inkscape\bin\inkscape.exe"

# Directories
SVG_DIR = Path("resources/icons")
PNG_DIR = Path("data/icons")
SIZE = 64

def convert_svg_to_png(svg_file, png_file):
    """Convert SVG to PNG using Inkscape"""
    try:
        cmd = [
            INKSCAPE,
            str(svg_file),
            "--export-type=png",
            f"--export-filename={png_file}",
            f"--export-width={SIZE}",
            f"--export-height={SIZE}",
            "--export-background-opacity=0"
        ]

        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

        if result.returncode == 0:
            # Verify file was created and is not empty
            if png_file.exists() and png_file.stat().st_size > 200:
                print(f"✓ {svg_file.name} -> {png_file.name} ({png_file.stat().st_size} bytes)")
                return True
            else:
                print(f"✗ {svg_file.name} - file too small or not created")
                return False
        else:
            print(f"✗ {svg_file.name} - {result.stderr[:100]}")
            return False

    except subprocess.TimeoutExpired:
        print(f"✗ {svg_file.name} - timeout")
        return False
    except Exception as e:
        print(f"✗ {svg_file.name} - {e}")
        return False

def main():
    print("=" * 60)
    print("Weather Icon Converter - SVG to PNG (64x64)")
    print("Using Inkscape")
    print("=" * 60)

    # Check Inkscape
    if not Path(INKSCAPE).exists():
        print(f"\n✗ Inkscape not found at: {INKSCAPE}")
        print("Please install Inkscape or update the INKSCAPE path in this script")
        return 1

    print(f"✓ Found Inkscape: {INKSCAPE}\n")

    # Create output directory
    PNG_DIR.mkdir(parents=True, exist_ok=True)
    print(f"Output directory: {PNG_DIR}\n")

    # Get all SVG files
    svg_files = sorted(SVG_DIR.glob("*.svg"))

    if not svg_files:
        print(f"No SVG files found in {SVG_DIR}")
        return 1

    print(f"Converting {len(svg_files)} SVG files...\n")

    # Convert each file
    success_count = 0
    for i, svg_file in enumerate(svg_files, 1):
        png_file = PNG_DIR / f"{svg_file.stem}.png"
        print(f"[{i}/{len(svg_files)}] ", end="")
        if convert_svg_to_png(svg_file, png_file):
            success_count += 1

    print("\n" + "=" * 60)
    print(f"✓ Successfully converted {success_count}/{len(svg_files)} files")

    if success_count > 0:
        print(f"\nPNG files are ready in: {PNG_DIR}")
        print("\nNext steps:")
        print("1. Upload filesystem: pio run --target uploadfs")
        print("2. Upload code: pio run --target upload")

    return 0 if success_count > 0 else 1

if __name__ == "__main__":
    sys.exit(main())
