#!/usr/bin/env bash
# Flash a Corne half (left or right) via caterina bootloader.
# Misma estrategia que QMK: snapshot de /dev/tty*, espera un puerto NUEVO.
#
# Usage: ./flash-corne.sh

set -euo pipefail
cd "$(dirname "$0")"

HEX="crkbd_rev1_pones.hex"
MCU="atmega32u4"
RETRY_TIME=0.5

if [[ ! -f "$HEX" ]]; then
  echo "Error: $HEX not found in $(pwd)"
  exit 1
fi

# ── 1. Esperar que el teclado aparezca ──────────────────────────────────────
echo "Conecta la mitad del Corne por USB..."
QMK_PORT=""
while [[ -z "$QMK_PORT" ]]; do
  QMK_PORT=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -1 || true)
  [[ -z "$QMK_PORT" ]] && sleep "$RETRY_TIME"
done
echo "Detectado: $QMK_PORT"

# ── 2. Trigger 1200 baud → bootloader ───────────────────────────────────────
echo "Enviando reset (1200 baud)..."
python3 -c "
import serial, time
s = serial.Serial('$QMK_PORT', 1200)
time.sleep(0.1)
s.close()
" 2>/dev/null || true

# ── 3. Snapshot + esperar puerto NUEVO (estrategia QMK) ─────────────────────
TMP1=$(mktemp)
TMP2=$(mktemp)
trap 'rm -f "$TMP1" "$TMP2"' EXIT

ls /dev/tty* 2>/dev/null | sort > "$TMP1"

echo "Presiona RESET en el Pro Micro si no entra solo (tienes 30s)..."
BOOT_PORT=""
DEADLINE=$(( SECONDS + 30 ))
while [[ -z "$BOOT_PORT" ]] && (( SECONDS < DEADLINE )); do
  sleep "$RETRY_TIME"
  printf "."
  ls /dev/tty* 2>/dev/null | sort > "$TMP2"
  BOOT_PORT=$(comm -13 "$TMP1" "$TMP2" | grep -o '/dev/tty.*' || true)
  cp "$TMP2" "$TMP1"
done
echo ""

if [[ -z "$BOOT_PORT" ]]; then
  echo "Error: bootloader no detectado en 30s."
  exit 1
fi
echo "Bootloader detectado: $BOOT_PORT"

# Esperar que el puerto sea escribible
printf "Esperando acceso a $BOOT_PORT"
while [[ ! -w "$BOOT_PORT" ]]; do sleep "$RETRY_TIME"; printf "."; done
echo ""

# ── 4. Flashear ─────────────────────────────────────────────────────────────
echo "Flasheando $HEX..."
avrdude -p "$MCU" -c avr109 -P "$BOOT_PORT" -U "flash:w:$HEX:i"

echo ""
echo "Flash completado. Desconecta y reconecta el teclado."
