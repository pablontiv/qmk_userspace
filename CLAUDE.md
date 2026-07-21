# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a QMK userspace repository for custom keyboard keymaps. It contains configurations for multiple keyboards including the Corne (crkbd) and YMDK YMD40 Air40, each with personalized keymaps and features.

## Build Commands

### Basic compilation

```bash
# Compile a specific keymap
qmk compile -kb your_keyboard -km your_keymap

# Using make syntax
make your_keyboard:your_keymap

# Compile all configured userspace targets
qmk userspace-compile
```

### Configuration commands

```bash
# Add a keymap to build targets
qmk userspace-add -kb <keyboard> -km <keymap>

# Remove a keymap from build targets  
qmk userspace-remove -kb <keyboard> -km <keymap>

# List current build targets
qmk userspace-list

# Set global userspace path (run from userspace directory)
qmk config user.overlay_dir="$(realpath .)"
```

### Current Build Targets

Based on `qmk.json`, the currently configured build target is:

- `crkbd/rev1:pones`

## Repository Structure

### Keyboard Configurations

- `keyboards/crkbd/rev1/keymaps/` - Corne keyboard keymaps
  - `pones/` - Main active keymap (v6.3)
  - `pones-old/` - Previous version
  - `vial/` - Vial-compatible keymap with visual configuration support
- `keyboards/ymdk/ymd40/air40/keymaps/` - YMD40 Air40 keymaps
  - `pones/` - Current keymap
  - `pones_old/` - Previous version

### Key Files

- `qmk.json` - Userspace configuration and build targets
- `Makefile` - Forwards make commands to QMK firmware with proper userspace path
- `*.hex` - Compiled firmware files ready for flashing

## Keymap Architecture

The main keymap (`keyboards/crkbd/rev1/keymaps/pones/`) uses six layers optimized for programming workflows. See the keymap diagram and layer summary in [`README.md`](README.md).

### Layers (v6.3)

- `_QWERTY` - Base layer with home row modifiers
- `_NUMBER` - Numbers and F-keys (hold Space)
- `_SYMBOL` - Symbols and brackets, tap-dance pairs (hold Tab)
- `_MOVE` - herdr/KVM window management, Ctrl+Tab, media & volume (hold Enter)
- `_NAV` - Cursor navigation and word-jump (hold Esc)
- `_MOUSE` - Mouse movement/clicks/wheel, boot/debug/caps/insert (hold Backspace)

### Home Row Modifiers

Left hand: GUI(A), Alt(S), Ctrl(D), Shift(F)  
Right hand: Shift(J), Ctrl(K), Alt(L), GUI(;)

### Thumb Cluster

| Thumb | Tap | Hold |
|---|---|---|
| Esc (L1) | Escape | `_NAV` |
| Tab (L2) | Tab | `_SYMBOL` |
| Space (L3) | Space | `_NUMBER` |
| Enter (R1) | Enter | `_MOVE` |
| Backspace (R2) | Backspace | `_MOUSE` |
| Delete (R3) | Delete | — |

### Combos (v6.3)

Fast-access shortcuts without additional holds:

- **F+J** = CapsWord
- **K+L** = Alt+Backspace (word delete)
- **S+D** = Alt+Delete (word delete forward)
- **L+;** = Ctrl+Backspace (word delete backward, Ctrl variant)
- **A+S** = Ctrl+Delete (word delete forward, Ctrl variant)

### Custom Features

- OLED display support with layer and modifier status
- Custom macros for herdr window-manager integration
- Chordal Hold + Permissive Hold (220 ms term)
- Configurable tapping term (220ms)
- Console logging support (debug toggle via `_MOUSE`)

## Flashing

Use the `flash-corne` project skill (`.claude/skills/flash-corne/SKILL.md`). The keymap uses **EE_HANDS** for hand detection — each half stores its role in EEPROM on first flash, allowing either half to be plugged and programmed without physical RESET.

## Development Notes

- Firmware size: 27,182 / 28,672 bytes (1,490 bytes free)
- CONSOLE_ENABLE for firmware-side keystroke logging (toggled via `_MOUSE`)
- Chordal Hold prevents unintended modifiers on fast typing
- OLED display includes custom font for layer/modifier status
- Master hand role stored in EEPROM; either half can initialize via EE_HANDS
