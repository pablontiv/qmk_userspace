#!/usr/bin/env bash
# Flash a Corne half (left or right) via caterina bootloader.
# 1. Espera que aparezca un puerto /dev/ttyACM*  (teclado enchufado corriendo QMK)
# 2. Envía el trigger 1200 baud → Pro Micro entra en bootloader
# 3. Espera que el puerto reaparezca (re-enumeración del bootloader)
# 4. Flashea con avrdude
#
# Usage: ./flash-corne.sh

set -euo pipefail
cd "$(dirname "$0")"

HEX="crkbd_rev1_pones.hex"
MCU="atmega32u4"

if [[ ! -f "$HEX" ]]; then
  echo "Error: $HEX not found in $(pwd)"
  exit 1
fi

# ── 1. Esperar que el teclado aparezca ──────────────────────────────────────
echo "Conecta la mitad del Corne por USB..."
QMK_PORT=""
while [[ -z "$QMK_PORT" ]]; do
  QMK_PORT=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -1 || true)
  [[ -z "$QMK_PORT" ]] && sleep 0.5
done
echo "Detectado: $QMK_PORT"

# ── 2. Trigger 1200 baud → bootloader ───────────────────────────────────────
echo "Enviando reset (1200 baud)..."
stty -F "$QMK_PORT" 1200 cs8 -cstopb -parenb 2>/dev/null || true

# Esperar que el puerto desaparezca (la mitad que tarda en reiniciar)
DEADLINE=$(( SECONDS + 5 ))
while [[ -e "$QMK_PORT" ]] && (( SECONDS < DEADLINE )); do
  sleep 0.1
done

# ── 3. Esperar que reaparezca el puerto del bootloader ───────────────────────
echo "Esperando bootloader..."
BOOT_PORT=""
DEADLINE=$(( SECONDS + 10 ))
while [[ -z "$BOOT_PORT" ]] && (( SECONDS < DEADLINE )); do
  BOOT_PORT=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -1 || true)
  [[ -z "$BOOT_PORT" ]] && sleep 0.2
done

if [[ -z "$BOOT_PORT" ]]; then
  echo "Error: bootloader no detectado. Intenta presionar reset dos veces rápido."
  exit 1
fi
echo "Bootloader en: $BOOT_PORT"

# ── 4. Flashear ─────────────────────────────────────────────────────────────
echo "Flasheando $HEX..."
avrdude -p "$MCU" -c avr109 -P "$BOOT_PORT" -U "flash:w:$HEX:i"

echo ""
echo "Flash completado. Desconecta y reconecta el teclado."
