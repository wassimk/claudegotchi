# Claude Tamagotchi for M5Stack Core2

A cute Claude mascot tamagotchi that runs on the M5Stack Core2 IoT device.

## Features

- Pixel-art Claude character with idle breathing animation
- Matrix-style code rain background effect
- Status display showing current state (CODING, REASONING, TESTING, etc.)
- Touch screen interaction via bottom buttons
- Status bar with WiFi and battery indicators

## Hardware Requirements

- M5Stack Core2 (or Core2 AWS)

## Software Requirements

- Arduino IDE 2.x (recommended) or 1.8.x
- M5Unified library (with M5GFX dependency)

## Setup Instructions

### 1. Install Arduino IDE

Download and install Arduino IDE from: https://www.arduino.cc/en/software

### 2. Install USB Drivers

M5Stack Core2 uses either CP210X or CH9102 USB chip. Install the appropriate driver:
- **CP210X driver**: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- **CH9102 driver**: https://www.wch.cn/downloads/CH9102SER_ZIP.html

If unsure which chip your device has, install both drivers.

### 3. Add M5Stack Board Manager URL

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Boards Manager URLs", add:
   ```
   https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
   ```
   (If you have other URLs, separate them with commas)
4. Click OK

### 4. Install M5Stack Board

1. Go to **Tools → Board → Boards Manager**
2. Search for "M5Stack"
3. Install "M5Stack by M5Stack official" (latest version)

### 5. Install M5Unified Library

1. Go to **Tools → Manage Libraries** (or Sketch → Include Library → Manage Libraries)
2. Search for "M5Unified"
3. Install "M5Unified by M5Stack"
4. When prompted, click "Install All" to include the M5GFX dependency

### 6. Configure Board Settings

1. Connect your M5Stack Core2 via USB-C
2. Go to **Tools → Board → M5Stack** and select **"M5Stack-Core2"**
3. Go to **Tools → Port** and select the appropriate COM port (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
4. Recommended settings:
   - Upload Speed: 921600
   - CPU Frequency: 240MHz
   - Flash Frequency: 80MHz
   - Flash Mode: QIO
   - Partition Scheme: Default 4MB with spiffs

### 7. Upload the Sketch

1. Open `claude_tamagotchi/claude_tamagotchi.ino`
2. Click the Upload button (→) or press Ctrl+U
3. Wait for the upload to complete (you should see "Hard resetting via RTS pin..." when done)

## Usage

- **Bottom Left Touch Area**: Changes state to CODING
- **Bottom Center Touch Area**: Changes state to REASONING
- **Bottom Right Touch Area**: Changes state to TESTING

The Claude character will animate with a breathing/bouncing motion, and the matrix code rain effect will continuously animate in the background.

## Project Structure

```
claude-tamagotchi/
├── README.md
├── LICENSE
└── claude_tamagotchi/
    └── claude_tamagotchi.ino
```

## Future Enhancements (Planned)

- [ ] More expressive animations (happy, sad, excited)
- [ ] Stats system (hunger, happiness, energy)
- [ ] Sound effects via built-in speaker
- [ ] Real Claude Code integration via WiFi/Serial
- [ ] Persistent state storage
- [ ] More interactive mini-games

## License

MIT License - See LICENSE file for details
