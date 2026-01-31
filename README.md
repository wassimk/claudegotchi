# Claude Code Notifier for M5Stack Core2

A cute Claude mascot that lives on your desk and notifies you when Claude Code needs attention.

## Features

- **Idle Mode**: Claude slowly wanders around with gentle breathing animation
- **Attention Mode**: Bounces excitedly, raises arms, chirps to get your attention
- **Matrix Rain**: Animated code rain background
- **Sound Effects**: Cute chirp sounds (toggleable)

## Hardware

- M5Stack Core2

## Setup

Follow the M5Stack Arduino setup guide: https://docs.m5stack.com/en/arduino/m5core2/program

Install **M5Unified** library (includes M5GFX dependency) via Arduino Library Manager.

Then open `claude_tamagotchi/claude_tamagotchi.ino` and upload.

## Usage

| Button | Action |
|--------|--------|
| Left | Toggle sound on/off |
| Center | Trigger attention mode (test) |
| Right | Dismiss / return to idle |

### Serial Commands

Open Serial Monitor (115200 baud):
- `ATTENTION` - Trigger attention mode
- `IDLE` - Return to idle mode

## Future

- WiFi connectivity for remote notifications
- Claude Code hooks integration

## License

MIT
