#!/usr/bin/env bash
# Flash Corne halves on macOS via caterina bootloader (avrdude avr109).
# Watches for NEW /dev/cu.usbmodem* ports so pre-existing serial devices
# (e.g. audio interfaces) and pseudo-terminals are never mistaken for the
# bootloader.
#
# BOOTSTRAP MODE (one-time, before QK_BOOT works): enter bootloader via physical RESET.
# Firmware v6.3+ with EE_HANDS: qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-left/right
# If pty race hits, fallback: avrdude + eeprom handedness write (see SKILL.md).
#
# PERMANENT MODE (all future flashes): enter bootloader via _MEDIA-hold (Backspace) + QK_BOOT.
# This script then detects the port and flashes via avrdude (existing flow).
#
# Usage: ./flash-macos.sh [bootstrap] [halves]   (default: permanent mode, 2 halves)
# Examples:
#   ./flash-macos.sh              # permanent mode, 2 halves
#   ./flash-macos.sh bootstrap    # bootstrap mode, 2 halves
#   ./flash-macos.sh bootstrap 1  # bootstrap mode, 1 half

set -euo pipefail
cd "$(dirname "$0")/../../../.."

MODE="${1:-permanent}"
HALVES="${2:-2}"

if [[ "$MODE" == "bootstrap" ]]; then
  BOOTSTRAP=1
  HALVES="${2:-2}"
else
  BOOTSTRAP=0
  HALVES="${1:-2}"
fi

HEX="crkbd_rev1_pones.hex"
POLL=0.25
TIMEOUT_SECS=120

if [[ ! -f "$HEX" ]]; then
  echo "Error: $HEX not found in $(pwd). Run: qmk compile -kb crkbd/rev1 -km pones"
  exit 1
fi

BEFORE=$(mktemp)
AFTER=$(mktemp)
trap 'rm -f "$BEFORE" "$AFTER"' EXIT

for (( n=1; n<=HALVES; n++ )); do
  if [[ $BOOTSTRAP -eq 1 ]]; then
    # Bootstrap: try qmk flash with handedness bootloader selector
    HAND="left"
    if [[ $n -eq 2 ]]; then HAND="right"; fi
    echo "[$n/$HALVES] Bootstrap mode: flashing $HAND half with handedness mark."
    echo "[$n/$HALVES] Press RESET (${TIMEOUT_SECS}s)..."
    if qmk flash -kb crkbd/rev1 -km pones -bl "avrdude-split-$HAND" 2>&1; then
      echo "[$n/$HALVES] Done."
    else
      # Pty race hit: fallback to manual avrdude + eeprom write (see gotchas.md)
      echo "[$n/$HALVES] qmk flash failed (pty race?). Falling back to manual avrdude + eeprom..."
      ls /dev/cu.usbmodem* 2>/dev/null | sort > "$BEFORE"
      echo "[$n/$HALVES] Press RESET again..."
      PORT=""
      DEADLINE=$(( SECONDS + TIMEOUT_SECS ))
      while [[ -z "$PORT" ]] && (( SECONDS < DEADLINE )); do
        sleep "$POLL"
        ls /dev/cu.usbmodem* 2>/dev/null | sort > "$AFTER"
        PORT=$(comm -13 "$BEFORE" "$AFTER" | head -1)
      done
      if [[ -z "$PORT" ]]; then
        echo "Error: no bootloader port appeared in ${TIMEOUT_SECS}s."
        exit 1
      fi
      echo "[$n/$HALVES] Bootloader at $PORT — flashing + eeprom write..."
      avrdude -p atmega32u4 -c avr109 -P "$PORT" -U "flash:w:$HEX:i"
      # Eeprom handedness write: QMK generates .eep files in build dir
      if [[ -f "crkbd_rev1_pones_${HAND}.eep" ]]; then
        avrdude -p atmega32u4 -c avr109 -P "$PORT" -U "eeprom:w:crkbd_rev1_pones_${HAND}.eep:i"
        echo "[$n/$HALVES] Eeprom handedness mark written."
      else
        echo "[$n/$HALVES] Warning: eeprom file not found, handedness mark may not be set."
      fi
      echo "[$n/$HALVES] Done."
    fi
  else
    # Permanent: use existing port-snapshot + avrdude flow
    ls /dev/cu.usbmodem* 2>/dev/null | sort > "$BEFORE"
    echo "[$n/$HALVES] Enter bootloader via _MEDIA-hold (Backspace) + QK_BOOT — waiting ${TIMEOUT_SECS}s..."

    PORT=""
    DEADLINE=$(( SECONDS + TIMEOUT_SECS ))
    while [[ -z "$PORT" ]] && (( SECONDS < DEADLINE )); do
      sleep "$POLL"
      ls /dev/cu.usbmodem* 2>/dev/null | sort > "$AFTER"
      PORT=$(comm -13 "$BEFORE" "$AFTER" | head -1)
    done

    if [[ -z "$PORT" ]]; then
      echo "Error: no bootloader port appeared in ${TIMEOUT_SECS}s."
      exit 1
    fi

    echo "[$n/$HALVES] Bootloader at $PORT — flashing..."
    avrdude -p atmega32u4 -c avr109 -P "$PORT" -U "flash:w:$HEX:i"
    echo "[$n/$HALVES] Done."
  fi
done

echo "All $HALVES halves flashed."
