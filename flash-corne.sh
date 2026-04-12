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
python3 -c "
import serial, time
s = serial.Serial('$QMK_PORT', 1200)
time.sleep(0.1)
s.close()
" 2>/dev/null || true

# Esperar que el puerto desaparezca (señal de que el reset fue aceptado)
DEADLINE=$(( SECONDS + 3 ))
while [[ -e "$QMK_PORT" ]] && (( SECONDS < DEADLINE )); do
  sleep 0.1
done

if [[ -e "$QMK_PORT" ]]; then
  echo "Trigger automático no funcionó — presiona RESET una vez en el Pro Micro..."
  # Esperar que el puerto desaparezca (confirma que el reset ocurrió)
  DEADLINE=$(( SECONDS + 30 ))
  while [[ -e "$QMK_PORT" ]] && (( SECONDS < DEADLINE )); do
    sleep 0.1
  done
  if [[ -e "$QMK_PORT" ]]; then
    echo "Error: no se detectó reset en 30s."
    exit 1
  fi
fi

# ── 3. Esperar que reaparezca el puerto del bootloader ───────────────────────
echo "Esperando bootloader..."
BOOT_PORT=""
DEADLINE=$(( SECONDS + 10 ))
while [[ -z "$BOOT_PORT" ]] && (( SECONDS < DEADLINE )); do
  BOOT_PORT=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -1 || true)
  [[ -z "$BOOT_PORT" ]] && sleep 0.2
done

if [[ -z "$BOOT_PORT" ]]; then
  echo "Error: bootloader no detectado."
  exit 1
fi
echo "Bootloader en: $BOOT_PORT"

# Dar tiempo al bootloader para inicializar completamente
sleep 1

# ── 4. Flashear ─────────────────────────────────────────────────────────────
echo "Flasheando $HEX..."
avrdude -p "$MCU" -c avr109 -P "$BOOT_PORT" -U "flash:w:$HEX:i"

echo ""
echo "Flash completado. Desconecta y reconecta el teclado."
