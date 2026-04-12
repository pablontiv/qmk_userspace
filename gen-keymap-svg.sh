#!/usr/bin/env bash
# Generate docs/keymap.svg from the current keymap.c
# Usage: ./gen-keymap-svg.sh

set -euo pipefail
cd "$(dirname "$0")"

python3 parse_keymap.py

keymap parse \
  -q crkbd_rev1_pones_keymap.json \
  -l QWERTY NUMBER SYMBOL NAV \
  -o docs/keymap.yaml

keymap draw docs/keymap.yaml -o docs/keymap.svg

echo "Done: docs/keymap.svg"
