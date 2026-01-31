# Claudegotchi

A cute Claude mascot that lives on your desk and notifies you when Claude Code needs attention.

## Features

- **Idle Mode**: Claude slowly wanders around with gentle breathing animation and occasional blinking
- **Attention Mode**: Bounces excitedly with wiggling arms and > < eyes
- **Matrix Rain**: Animated code rain background
- **Sound Effects**: Cute chirp sounds (toggleable via bottom-left touch area)
- **Clean UI**: Minimal interface with just a small sound indicator dot

## Hardware

- M5Stack Core2

## Setup

Follow the M5Stack Arduino setup guide: https://docs.m5stack.com/en/arduino/m5core2/program

Install **M5Unified** library (includes M5GFX dependency) via Arduino Library Manager.

Then open `claudegotchi/claudegotchi.ino` and upload.

## Usage

| Touch Area | Action |
|------------|--------|
| Bottom Left | Toggle sound on/off |
| Bottom Center | Trigger attention mode (test) |
| Bottom Right | Dismiss / return to idle |

### Serial Commands

Open Serial Monitor (115200 baud):
- `ATTENTION` - Trigger attention mode
- `IDLE` - Return to idle mode

## License

MIT
