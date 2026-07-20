---
name: keymap-view
description: "Trigger: ver el keymap, mostrame el keymap, show keymap, view keymap, layout, capas. Render every crkbd/rev1:pones layer as a full keyboard, thumb row included, with the held thumb marked."
license: Apache-2.0
metadata:
  author: pones
  version: "1.0"
---

# keymap-view

## Activation Contract

Use whenever the user asks to see, view, or show the keymap / layout / capas
for `crkbd/rev1:pones`.

**Working state wins.** If the conversation has an active redesign in progress
(the user is discussing, deciding, or has agreed changes not yet flashed),
render THAT working state, labeled `REDISEÑO EN CURSO (sin flashear)`, applying
every change agreed so far and marking still-open items as `PENDIENTE`. Render
the flashed source only when no redesign is in flight, or when the user
explicitly asks for the current/flashed keymap. Never show the flashed layout as
if it were the design under discussion.

## Hard Rules

- For the flashed layout, read the source of truth every time; never render
  from memory: `keyboards/crkbd/rev1/keymaps/pones/keymap.c` (layers, combos,
  tap dances) and `config.h` (tapping term, combo term, chordal hold). For a
  redesign in flight, the conversation is the source of truth — apply the agreed
  deltas on top of the flashed base.
- Render **all six** layers, each as a **complete keyboard**: 3 key rows + the
  thumb row `ESC TAB SPC │ ENT BSPC DEL`. Never omit the thumb row.
- On every non-base layer, mark the thumb that activates it with `‹ ›`
  (e.g. `‹SPC›`). Base `_QWERTY` marks no thumb.
- Split hands with `│`; empty keys are `·`; align columns.
- Show one token per key (1-tap value for tap-dance symbol keys).
- Render combos **inline** under the `_QWERTY` home row: a `└─┘` bracket over
  each physical key pair with its action label beneath, aligned to the keys.
  Stack a second tier for pairs that share a key (S in A+S/S+D, L in K+L/L+;).
  The keymap must be self-explanatory from the diagram alone — no separate
  combos table. Precede the grid with the global behaviors block.
- Match `assets/format.txt` exactly. Keep the reply in the user's language.

## Execution Steps

1. Read `keymap.c` and `config.h`; extract layers, thumb defines, combos, tap
   dances, and global config.
2. Emit the behaviors block (tapping term, chordal/permissive hold, combo term,
   home row mods GACS, thumb cluster tap/hold).
3. Emit each layer per `assets/format.txt`, marking the held thumb.
4. Emit the combos table and any per-layer notes.

## Output Contract

Six full-keyboard layer diagrams (thumb row on each, held thumb marked), the
global behaviors block, home-row-mod legend, and combos rendered inline on the
base layer.

## References

- `assets/format.txt` — canonical layer + thumb rendering template.
- `keyboards/crkbd/rev1/keymaps/pones/keymap.c` — source of truth.
- `keyboards/crkbd/rev1/keymaps/pones/config.h` — timing/hold config.
