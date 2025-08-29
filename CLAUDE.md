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
  - `pones/` - Main active keymap with home row mods, layers for symbols/numbers/navigation
  - `pones-old/` - Previous version keymap
  - `vial/` - Vial-compatible keymap with visual configuration support
- `keyboards/ymdk/ymd40/air40/keymaps/` - YMD40 Air40 keymaps
  - `pones/` - Current keymap
  - `pones_old/` - Previous version

### Key Files

- `qmk.json` - Userspace configuration and build targets
- `Makefile` - Forwards make commands to QMK firmware with proper userspace path
- `*.hex` - Compiled firmware files ready for flashing

## Keymap Architecture

The main keymap (`keyboards/crkbd/rev1/keymaps/pones/`) uses a sophisticated layer system:

### Layers

- `_QWERTY` - Base QWERTY layout with home row modifiers
- `_NUMBER` - Number pad and arithmetic operators  
- `_SYMBOL` - Symbol keys and brackets
- `_SYMBOL2` - Additional symbols (@, #, $, %, etc.)
- `_NAV` - Navigation keys (arrows, page up/down, word movement)
- `_DEV` - Development shortcuts (VS Code terminal, debugging, navigation)
- `_FUNC` - Function keys (F1-F12)
- `_MEDIA` - Media controls and mouse keys
- `_UTIL` - RGB lighting controls

### Home Row Modifiers

The keymap implements home row mods for efficient modifier access:

- Left hand: GUI(A), Alt(S), Ctrl(D), Shift(F)
- Right hand: Shift(J), Ctrl(K), Alt(L), GUI(;)

### Layer Access

Layers are accessed via layer-tap keys on thumbs and specific positions:

- Space → Numbers layer
- Tab → Symbols layer  
- Escape → Symbols2 layer
- Enter → Development layer
- Backspace → Function keys layer
- Delete → Media layer
- Z → Navigation layer
- Quote → Utilities layer

### Custom Features

- OLED display support with layer and modifier status
- RGB Matrix with selective animations enabled
- Custom macros for development workflows (VS Code shortcuts)
- Mouse key integration
- Configurable tapping term (220ms)

## Development Notes

- The keymap is heavily customized for programming workflows with VS Code shortcuts
- Uses split keyboard features extensively (OLED, RGB, etc.)
- Master hand is configured as left side
- Custom font included for OLED display
- RGB animations are selectively enabled to save firmware space
- Never compile
