#!/usr/bin/env python3
"""Parse QMK keymap.c and generate a keymap.json for keymap-drawer."""

import re
import json
import sys

KEYMAP_C = "keyboards/crkbd/rev1/keymaps/pones/keymap.c"
OUTPUT = "crkbd_rev1_pones_keymap.json"
KEYBOARD = "crkbd/rev1"
KEYMAP_NAME = "pones"
LAYOUT = "LAYOUT_split_3x6_3"

def extract_defines(src):
    """Extract #define macros (single-line, no function-like with body)."""
    defines = {}
    for m in re.finditer(r'#define\s+(\w+)\s+(.*?)(?=\n)', src):
        name, val = m.group(1).strip(), m.group(2).strip()
        if val:
            defines[name] = val
    return defines

def extract_layer_enum(src):
    """Extract enum layer_names values -> numeric indices."""
    m = re.search(r'enum layer_names\s*\{([^}]+)\}', src)
    if not m:
        return {}
    body = m.group(1)
    names = [tok.strip().lstrip('_') for tok in re.findall(r'\b(\w+)\b', body)]
    # Remove comments (words like 'Base', 'QWERTY', etc. after //)
    raw_names = re.findall(r'\b(_\w+)\b', body)
    return {name: i for i, name in enumerate(raw_names)}

def extract_tap_dance_enum(src):
    """Extract tap dance enum names -> indices."""
    m = re.search(r'enum tap_dances\s*\{([^}]+)\}', src)
    if not m:
        return {}
    body = m.group(1)
    raw_names = re.findall(r'\b(\w+)\b', body)
    # Filter out comment words, only keep UPPER_CASE names
    names = [n for n in raw_names if n.isupper() or '_' in n]
    return {name: i for i, name in enumerate(names)}

def extract_layout_calls(src):
    """Find each keymap layer's LAYOUT_split_3x6_3(...) call and return its inner content.

    Only matches layout calls that are layer definitions (`[_NAME] = LAYOUT(...)`),
    skipping other uses of the macro such as the `chordal_hold_layout` handedness map.
    """
    results = []
    pattern = r'\[_\w+\]\s*=\s*LAYOUT_split_3x6_3\s*\('
    for match in re.finditer(pattern, src):
        start = match.end()
        depth = 1
        i = start
        while i < len(src) and depth > 0:
            if src[i] == '(':
                depth += 1
            elif src[i] == ')':
                depth -= 1
            i += 1
        results.append(src[start:i-1])
    return results

def strip_comments(src):
    """Remove C-style // and /* */ comments."""
    src = re.sub(r'/\*.*?\*/', '', src, flags=re.DOTALL)
    src = re.sub(r'//[^\n]*', '', src)
    return src

def tokenize_keys(layout_body):
    """Split layout body into individual key tokens, respecting nested parens."""
    tokens = []
    depth = 0
    current = []
    for ch in layout_body:
        if ch == '(' :
            depth += 1
            current.append(ch)
        elif ch == ')':
            depth -= 1
            current.append(ch)
        elif ch == ',' and depth == 0:
            tok = ''.join(current).strip()
            if tok:
                tokens.append(tok)
            current = []
        else:
            current.append(ch)
    tok = ''.join(current).strip()
    if tok:
        tokens.append(tok)
    return tokens

def expand_token(tok, defines, layer_enum, td_enum):
    """Expand a single key token through defines once."""
    tok = tok.strip()
    # If it's a known #define (non-function-like), expand it
    if tok in defines:
        expanded = defines[tok]
        # Replace layer enum names in expanded value
        for name, idx in layer_enum.items():
            expanded = re.sub(r'\b' + re.escape(name) + r'\b', str(idx), expanded)
        return expanded
    return tok

def main():
    with open(KEYMAP_C) as f:
        src = f.read()

    src_clean = strip_comments(src)

    defines = extract_defines(src_clean)
    layer_enum = extract_layer_enum(src_clean)
    td_enum = extract_tap_dance_enum(src_clean)

    # Build layer name map for display (strip leading _)
    layer_names = {idx: name.lstrip('_') for name, idx in layer_enum.items()}

    # Extract LAYOUT calls (one per layer, in order)
    layout_calls = extract_layout_calls(src_clean)

    # Determine layer order from keymaps[] definition
    layer_order = re.findall(r'\[(_\w+)\]\s*=\s*LAYOUT_split_3x6_3', src_clean)

    if len(layer_order) != len(layout_calls):
        print(f"Warning: found {len(layer_order)} layer names but {len(layout_calls)} layouts",
              file=sys.stderr)

    layers = []
    for call in layout_calls:
        tokens = tokenize_keys(call)
        expanded = [expand_token(t, defines, layer_enum, td_enum) for t in tokens]
        layers.append(expanded)

    # Build output JSON
    keymap_json = {
        "keyboard": KEYBOARD,
        "keymap": KEYMAP_NAME,
        "layout": LAYOUT,
        "layers": layers,
    }

    with open(OUTPUT, 'w') as f:
        json.dump(keymap_json, f, indent=2)

    print(f"Wrote {OUTPUT} with {len(layers)} layers:", file=sys.stderr)
    for i, name in enumerate(layer_order):
        print(f"  Layer {i}: {name} ({len(layers[i])} keys)", file=sys.stderr)

if __name__ == '__main__':
    main()
