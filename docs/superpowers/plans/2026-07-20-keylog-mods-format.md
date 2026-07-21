# Keylog Modifier Field Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add the active modifier bitmask to every firmware keystroke-log line and document that line as a stable contract for the external parser repo.

**Architecture:** One-line change to the `uprintf` in `process_record_user`, adding an `m0x%02X` field fed by `get_mods() | get_oneshot_mods()`. Then a standalone contract doc (`docs/keylog-format.md`) that mirrors the emitted line exactly.

**Tech Stack:** QMK firmware (C, AVR — crkbd/rev1:pones), `qmk` CLI for compile/flash/console.

## Global Constraints

- Repo scope: **QMK firmware only**. Capture daemon and parser live in a separate repo — do NOT add them here.
- Flash budget: currently 1,490 bytes free; the change must still fit.
- No behavioral change to the keymap — only the debug log line widens.
- Both press (`d`) and release (`u`) events keep being logged.
- Conventional commits, work directly on `main`, no AI attribution in commit messages.
- Modifier bitmask is `get_mods() | get_oneshot_mods()`, formatted `m0x%02X`.

---

### Task 1: Add modifier field to the firmware log line

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/macros.c:10-13`

**Interfaces:**
- Consumes: QMK core `get_mods()`, `get_oneshot_mods()` (both `uint8_t`), already used by `oled_task_user` in the old keymap — no new include needed (`print.h` and `QMK_KEYBOARD_H` already included at top of `macros.c`).
- Produces: the emitted console line
  `KL:t<ms>,k0x<KEYCODE>,r<row>,c<col>,m0x<MODS>,<d|u>` — consumed by Task 2's doc and the external parser.

- [ ] **Step 1: Capture the current (baseline) log format**

Confirm the current line has NO modifier field. Read `macros.c:10-13`; it must currently be:

```c
uprintf("KL:t%lu,k0x%04X,r%u,c%u,%c\n",
        timer_read32(), keycode,
        record->event.key.row, record->event.key.col,
        record->event.pressed ? 'd' : 'u');
```

- [ ] **Step 2: Apply the change**

Replace those four lines with:

```c
uprintf("KL:t%lu,k0x%04X,r%u,c%u,m0x%02X,%c\n",
        timer_read32(), keycode,
        record->event.key.row, record->event.key.col,
        get_mods() | get_oneshot_mods(),
        record->event.pressed ? 'd' : 'u');
```

- [ ] **Step 3: Compile and verify it builds**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: build succeeds, ends with `[OK]` and prints the firmware size report.

- [ ] **Step 4: Verify flash budget still fits**

From the size report in Step 3, confirm free space is still positive (was 1,490 bytes free before the change). The `get_mods()` call plus the wider format string add only a few bytes.
Expected: a non-negative "free" figure. If it overflowed, STOP and report — the fallback (dropping `u` events) is a design decision, not a silent edit.

- [ ] **Step 5: Flash the board**

Use the `flash-corne` skill (EE_HANDS — either half). 
Expected: flash completes, board re-enumerates.

- [ ] **Step 6: Observe the new line shape in the console**

Run: `qmk console`
Then, on the keyboard: hold `F` (Left-hand home-row Shift) and, while holding, tap another key (e.g. `J`).
Expected: the line for the key tapped while `F` was held shows a non-zero modifier field — Left Shift is `0x02`, so `...,m0x02,d`. A bare keypress with no mods shows `...,m0x00,d`.

- [ ] **Step 7: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/features/macros.c
git commit -m "feat(keylog): log active modifier bitmask on each key event"
```

---

### Task 2: Document the log format contract

**Files:**
- Create: `docs/keylog-format.md`

**Interfaces:**
- Consumes: the exact line emitted by Task 1.
- Produces: the authoritative format contract the external parser repo reads.

- [ ] **Step 1: Write the contract doc**

Create `docs/keylog-format.md` with this content:

````markdown
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
````

- [ ] **Step 2: Verify the doc matches the firmware exactly**

Cross-check the field order and format specifiers in `docs/keylog-format.md`
against the `uprintf` format string in `macros.c` (from Task 1). Every field and
its `%`-specifier must match.
Expected: identical field order `t, k, r, c, m, d/u` and matching widths.

- [ ] **Step 3: Commit**

```bash
git add docs/keylog-format.md
git commit -m "docs(keylog): document log line format contract"
```

---

## Self-Review Notes

- **Spec coverage:** Firmware change → Task 1. Log format contract doc → Task 2. Verification (compile, size, flash, console observation) → Task 1 Steps 3-6. All spec deliverables covered.
- **Placeholders:** none — full code and exact commands in every step.
- **Type consistency:** `m0x%02X` / `get_mods() | get_oneshot_mods()` identical in spec, Task 1, and Task 2 doc. Bitmask table identical across spec and doc.
