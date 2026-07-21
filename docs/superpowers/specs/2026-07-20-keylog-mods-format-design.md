# Keylog: add modifier field to console log

**Date:** 2026-07-20
**Repo scope:** QMK firmware only. The capture daemon and the combo-frequency
parser live in a separate repository and are out of scope here.

## Problem

The firmware keystroke logger emits one line per key event via `uprintf`, but the
current format carries no modifier state:

```
KL:t%lu,k0x%04X,r%u,c%u,%c
```

Reconstructing which key *combos* were actually typed (e.g. `Ctrl+C`,
`Alt+Backspace`, home-row-mod chords) therefore requires guessing tap-vs-hold from
timing heuristics. That heuristic is weakest exactly where it matters most: the
home-row mods, whose whole point is the ambiguous tap/hold decision. Emitting the
real modifier state removes the guesswork and makes chord reconstruction exact.

## Goal

Add the active modifier bitmask to every logged key event, and document the log
line as a stable contract that the external parser repo depends on.

Non-goals (belong to the other repo): the capture daemon, log rotation, and the
combo-frequency parser.

## Deliverables

1. **Firmware change** — one line in
   `keyboards/crkbd/rev1/keymaps/pones/features/macros.c`.
2. **Log format contract** — `docs/keylog-format.md`, the authoritative spec of
   the emitted line consumed by the parser repo.

## Design

### 1. Firmware change

In `process_record_user` (`macros.c:10`), extend the format string with a
modifier field and pass `get_mods() | get_oneshot_mods()`:

```c
uprintf("KL:t%lu,k0x%04X,r%u,c%u,m0x%02X,%c\n",
        timer_read32(), keycode,
        record->event.key.row, record->event.key.col,
        get_mods() | get_oneshot_mods(),
        record->event.pressed ? 'd' : 'u');
```

Why this makes reconstruction exact: home-row mods register their modifier on
hold resolution, so a held mod is **already active in `get_mods()` when the next
key's event fires**. The parser attributes the modifier to that following key —
no timing heuristic needed. `get_oneshot_mods()` is OR-ed in so one-shot mods are
captured too, matching how the OLED already reports mod state
(`get_mods()|get_oneshot_mods()`).

Both press (`d`) and release (`u`) events keep being logged. The release events
are not needed for chord frequency, but they preserve hold-duration information
for any later tap-vs-hold analysis, at no extra cost.

No behavioral change to the keymap: only the debug log line widens.

### 2. Log format contract (`docs/keylog-format.md`)

```
KL:t<ms>,k0x<KEYCODE>,r<row>,c<col>,m0x<MODS>,<d|u>
```

| Field | Type | Meaning |
|---|---|---|
| `t`   | uint32, decimal | Milliseconds since boot (`timer_read32`). Wraps to 0 after ~49.7 days. |
| `k`   | uint16, hex (`0x%04X`) | QMK keycode, **already resolved for the active layer**. Mod-tap / layer-tap keycodes appear in their full form; the parser extracts the tap keycode. |
| `r`   | uint, decimal | Matrix row, 0-based. |
| `c`   | uint, decimal | Matrix column, 0-based. |
| `m`   | uint8, hex (`0x%02X`) | Active modifier bitmask, `get_mods() \| get_oneshot_mods()`. |
| last  | char | `d` = pressed, `u` = released. |

Modifier bitmask layout (QMK `MOD_BIT`):

| Bit | Value | Modifier |
|---|---|---|
| 0 | `0x01` | Left Ctrl |
| 1 | `0x02` | Left Shift |
| 2 | `0x04` | Left Alt |
| 3 | `0x08` | Left GUI |
| 4 | `0x10` | Right Ctrl |
| 5 | `0x20` | Right Shift |
| 6 | `0x40` | Right Alt |
| 7 | `0x80` | Right GUI |

Reconstruction contract for the parser: for a keydown event, the combo is
`decode(m) + base(k)`. A home-row mod held during a chord shows up in the `m`
field of the following key's event, so the chord is exact rather than inferred.

## Verification

Definition of done for this repo:

1. `qmk compile -kb crkbd/rev1 -km pones` succeeds.
2. Firmware fits the flash budget (currently 1,490 bytes free); confirm the size
   report still shows free space after the change.
3. Flash the board (see `flash-corne` skill) and confirm `qmk console` emits the
   new line shape with a non-empty `m` field when a home-row mod is held during a
   chord (e.g. hold `F` for Shift, tap another key → that key's line shows
   `m0x02`).
4. `docs/keylog-format.md` committed and matches the emitted line exactly.

## Risks

- **Flash size.** Wider format string + `get_mods()` call add a few bytes. Budget
  is tight (1,490 free) but the delta is small; verified at compile in step 2. If
  it ever overflowed, dropping the `u` events would recover space.
- **Timestamp wrap.** `t` wraps every ~49.7 days. The parser must treat a
  decreasing `t` as a session/wrap boundary, not reorder events. Documented in the
  contract so the consuming repo handles it.
