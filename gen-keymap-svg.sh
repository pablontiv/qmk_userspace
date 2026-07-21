#!/usr/bin/env bash
# Generate docs/keymap.svg from the current keymap.c
# Usage: ./gen-keymap-svg.sh
# Requires: pipx install keymap-drawer

set -euo pipefail
cd "$(dirname "$0")"

python3 parse_keymap.py

# keymap-drawer replaces dict config fields (e.g. qmk_keycode_map) instead of
# merging, so deep-merge our overrides onto the full default config first —
# that extends the default keycode map rather than wiping it.
CONFIG="$(mktemp -t kd_config.XXXXXX)"
trap 'rm -f "$CONFIG"' EXIT
python3 - keymap_drawer.config.yaml "$CONFIG" <<'PY'
import subprocess, sys, yaml
default = yaml.safe_load(subprocess.check_output(["keymap", "dump-config"]))
override = yaml.safe_load(open(sys.argv[1]))

def deep_merge(base, extra):
    for key, value in extra.items():
        if isinstance(value, dict) and isinstance(base.get(key), dict):
            deep_merge(base[key], value)
        else:
            base[key] = value
    return base

with open(sys.argv[2], "w") as out:
    yaml.safe_dump(deep_merge(default, override), out, allow_unicode=True, sort_keys=False)
PY

keymap -c "$CONFIG" parse \
  -q crkbd_rev1_pones_keymap.json \
  -l QWERTY NUMBER SYMBOL MOVE NAV MEDIA MOUSE \
  -o docs/keymap.yaml

keymap -c "$CONFIG" draw docs/keymap.yaml -o docs/keymap.svg

echo "Done: docs/keymap.svg"
