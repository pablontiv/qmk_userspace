#!/usr/bin/env bash
# Flash Corne halves on macOS via caterina bootloader (avrdude avr109).
# Watches for NEW /dev/cu.usbmodem* ports so pre-existing serial devices
# (e.g. audio interfaces) and pseudo-terminals are never mistaken for the
# bootloader. The keyboard firmware is pure HID: entering the bootloader
# always requires the physical RESET button.
#
# Usage: ./flash-macos.sh [halves]   (default 2)

set -euo pipefail
cd "$(dirname "$0")/../../../.."

HEX="crkbd_rev1_pones.hex"
HALVES="${1:-2}"
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
  ls /dev/cu.usbmodem* 2>/dev/null | sort > "$BEFORE"
  echo "[$n/$HALVES] Press RESET on the half to flash (${TIMEOUT_SECS}s)..."

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
done

echo "All $HALVES halves flashed."
