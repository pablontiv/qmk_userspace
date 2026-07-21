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
  -l QWERTY NUMBER SYMBOL MOVE NAV MOUSE \
  -o docs/keymap.yaml

# Highlight, on each layer, the thumb key that is held to reach it.
python3 - docs/keymap.yaml <<'PY'
import sys, yaml
# layer name -> (thumb index in LAYOUT order, tap glyph of that thumb)
ACTIVATORS = {
    "NAV":    (36, "esc"),
    "SYMBOL": (37, "⇥"),
    "NUMBER": (38, "␣"),
    "MOVE":   (39, "⏎"),
    "MOUSE":  (40, "⌫"),
}
path = sys.argv[1]
data = yaml.safe_load(open(path))
for name, (idx, glyph) in ACTIVATORS.items():
    layer = data["layers"].get(name)
    if layer and idx < len(layer):
        layer[idx] = {"t": glyph, "type": "held"}
with open(path, "w") as out:
    yaml.safe_dump(data, out, allow_unicode=True, sort_keys=False)
PY

keymap -c "$CONFIG" draw docs/keymap.yaml -o docs/keymap.svg

# Post-process: strip the clickable layer-activator links (they don't work in a
# README <img> embed) and add a solid white background so the diagram reads in
# both GitHub light and dark themes.
python3 - docs/keymap.svg <<'PY'
import re, sys
path = sys.argv[1]
svg = open(path).read()
svg = re.sub(r"<a\b[^>]*>", "", svg)   # drop <a ...> wrappers
svg = svg.replace("</a>", "")
svg = re.sub(r"(<svg\b[^>]*>)",
             r'\1\n<rect width="100%" height="100%" fill="#ffffff"/>',
             svg, count=1)
open(path, "w").write(svg)
PY

echo "Done: docs/keymap.svg"
