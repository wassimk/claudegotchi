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
- M5Core2 library

## Setup Instructions

### 1. Install Arduino IDE

Download and install Arduino IDE from: https://www.arduino.cc/en/software

### 2. Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Boards Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click OK

### 3. Install ESP32 Board

1. Go to **Tools → Board → Boards Manager**
2. Search for "esp32"
3. Install "esp32 by Espressif Systems" (version 2.x recommended)

### 4. Install M5Core2 Library

1. Go to **Tools → Manage Libraries**
2. Search for "M5Core2"
3. Install "M5Core2 by M5Stack"

### 5. Configure Board Settings

1. Connect your M5Stack Core2 via USB-C
2. Go to **Tools → Board** and select "M5Stack-Core2"
3. Go to **Tools → Port** and select the appropriate COM port
4. Recommended settings:
   - Upload Speed: 921600
   - CPU Frequency: 240MHz
   - Flash Frequency: 80MHz
   - Flash Mode: QIO
   - Partition Scheme: Default 4MB with spiffs

### 6. Upload the Sketch

1. Open `claude_tamagotchi/claude_tamagotchi.ino`
2. Click the Upload button (→) or press Ctrl+U
3. Wait for the upload to complete

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
