# Claude Code Notifier for M5Stack Core2

A cute Claude mascot that lives on your desk and notifies you when Claude Code needs attention. The character wanders around peacefully when idle, then gets excited and chirps when it's time to interact with Claude Code.

## Features

- **Idle Mode**: Claude slowly wanders around the screen with gentle breathing animation
- **Attention Mode**: Claude bounces excitedly, raises arms, and chirps to get your attention
- **Matrix Rain**: Animated code rain background effect
- **Sound Effects**: Cute chirp sounds (toggleable)
- **Touch Controls**: Three bottom buttons for interaction
- **Serial Commands**: Can be triggered via serial for testing (future: WiFi/hooks integration)

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

### Touch Buttons

| Button | Label | Action |
|--------|-------|--------|
| Left | SOUND | Toggle sound on/off |
| Center | ALERT | Manually trigger attention mode (for testing) |
| Right | DISMISS | Return to idle mode |

### Serial Commands (for testing)

Open Serial Monitor (115200 baud) and send:
- `ATTENTION` - Trigger attention mode
- `IDLE` - Return to idle mode

### States

- **IDLE**: Claude wanders slowly, gentle breathing animation, peaceful
- **ATTENTION!**: Claude bounces excitedly, arms up, chirps every 3 seconds, flashing "!" indicator

## Project Structure

```
claude-tamagotchi/
├── README.md
├── LICENSE
└── claude_tamagotchi/
    └── claude_tamagotchi.ino
```

## Future Enhancements (Planned)

- [ ] WiFi connectivity for remote notifications
- [ ] Claude Code hooks integration (macOS → device communication)
- [ ] Multiple notification types (user input needed, error, task complete)
- [ ] Persistent settings storage
- [ ] Battery level display
- [ ] Different chirp sounds for different notification types

## How It Will Work with Claude Code

The plan is to use Claude Code hooks to send notifications to this device:

1. Claude Code hook detects when user input is needed
2. Hook sends HTTP request or serial command to M5Stack
3. M5Stack enters attention mode and chirps
4. User notices, interacts with Claude Code
5. Hook sends idle signal when done

## License

MIT License - See LICENSE file for details
