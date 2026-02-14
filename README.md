# Claudegotchi

> [!NOTE]
> **Work in progress** — This is a fun side project under active development. Claude currently looks like a blocky coral rectangle with stick arms, and that's part of the charm. Expect rough edges.

A cute Claude mascot that lives on your desk and notifies you when [Claude Code](https://docs.anthropic.com/en/docs/claude-code) needs attention.

Built for and tested on **M5Stack Core2** only.

## Features

- **Idle Mode**: Claude slowly wanders around with gentle breathing animation and occasional blinking
- **Attention Mode**: Bounces excitedly with wiggling arms and > < eyes
- **Matrix Rain**: Animated code rain background
- **Sound Effects**: Cute chirp sounds (toggleable via bottom-left touch area)
- **Clean UI**: Minimal interface with just a small sound indicator dot

## Hardware

- [M5Stack Core2](https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit)

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

Connect via Serial Monitor at 115200 baud:

- `ATTENTION` - Trigger attention mode
- `IDLE` - Return to idle mode

## Roadmap

- **Make Claude look cuter** — the current character is... charmingly primitive
- **Claude Code hooks integration** — hook into [Claude Code notifications](https://docs.anthropic.com/en/docs/claude-code/hooks) so the device chirps and lights up whenever Claude needs your input, no serial monitor required
- **Better sound design** — more expressive chirps and alerts

## Inspiration

Inspired by [@taku_sid's Stack-chan demo](https://x.com/taku_sid/status/2014540202803658973) — a much cuter version of this idea using a [Stack-chan](https://github.com/stack-chan/stack-chan) robot. That project isn't public yet, so this is my scrappy take on it in the meantime.
