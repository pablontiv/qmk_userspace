# Keylog line format

The firmware logs one line per key event over the QMK console (`uprintf` in
`keyboards/crkbd/rev1/keymaps/pones/features/macros.c`), gated on `debug_enable`.

## Line shape

```
KL:t<ms>,k0x<KEYCODE>,r<row>,c<col>,m0x<MODS>,<d|u>
```

| Field | Type | Meaning |
|---|---|---|
| `t`   | uint32, decimal | Milliseconds since boot (`timer_read32`). Wraps to 0 after ~49.7 days. |
| `k`   | uint16, hex (`0x%04X`) | QMK keycode, already resolved for the active layer. Mod-tap / layer-tap keycodes appear in full form; extract the tap keycode. |
| `r`   | uint, decimal | Matrix row, 0-based. |
| `c`   | uint, decimal | Matrix column, 0-based. |
| `m`   | uint8, hex (`0x%02X`) | Active modifier bitmask, `get_mods() \| get_oneshot_mods()`. |
| last  | char | `d` = pressed, `u` = released. |

## Modifier bitmask (QMK `MOD_BIT`)

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

## Reconstructing combos

For a keydown (`d`) event, the combo is `decode(m) + base(k)`. Home-row mods
register on hold resolution, so a held mod appears in the `m` field of the
**following** key's event — the chord is exact, not inferred from timing.

## Notes for consumers

- `t` wraps every ~49.7 days: a decreasing `t` marks a session/wrap boundary.
  Do not reorder events across it.
- Both `d` and `u` events are emitted. Chord frequency only needs `d`; `u`
  carries hold-duration info for optional tap-vs-hold analysis.
