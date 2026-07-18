# Keymap Redesign Plan — crkbd/rev1:pones (v6.3 IMPLEMENTED)

**Status:** Implemented 2026-07-18 — pending physical flash (EE_HANDS bootstrap) and 2–4 week validation.
**Last updated:** 2026-07-18
**Evidence base:**
- Usage log: `keycombo-log.csv` (Google Drive), 57,543 combo events, 2026-03-23 → 2026-07-16
- Ergonomics research: [`docs/research/ergonomics.md`](research/ergonomics.md)
- QMK feature research: [`docs/research/qmk-features.md`](research/qmk-features.md)
- herdr v0.7.1 default keybindings (verified in source: `ogulcancelik/herdr`, `src/config/model.rs`)
- Dual adversarial validation vs. ergonomics research and QMK master 0.33.8 source (sections below)

## Key data points

| Combo | Count | Share | Consequence |
|---|---|---|---|
| Ctrl+Tab | 22,922 | ~40% | One-key access (existing Tab TD); 34% of presses are bursts (<3 s, runs up to 14) |
| Alt+Backspace | 8,319 | ~14% | Word-delete combos emit Alt, not Ctrl (7:1 in log) |
| Ctrl+Shift+Tab | 3,307 | ~6% | Tab 3x on the Tab TD |
| Ctrl+C / Ctrl+V | ~6,000 | ~11% | copy/paste cluster on `_SYMBOL` home row (D/F/G) |
| Shift+/ (`?`) | 4,358 | ~8% | Zero-overlap rhythm issue — measure only (see diagnosis correction) |
| Shift+Tab | 1,330 | ~2% | Shift + tap on the Tab TD |
| Ctrl+Left/Right | 1,488 | ~3% | `_NAV` home-row mods + arrows |

Leader Key evaluated and **rejected** for the herdr prefix: burst usage demands a
momentary hold layer; Leader pays prefix + timeout per repetition by design.

## Thumb cluster (final)

| Thumb | Tap | Hold |
|---|---|---|
| Esc (L1) | Escape | `_HERDR` |
| Tab (L2) | Tab · 2x = Ctrl+Tab · 3x = Ctrl+Shift+Tab (Shift+tap = Shift+Tab bonus) | `_SYMBOL` |
| Space (L3) | Space | `_NUMBER` |
| Enter (R1) | Enter | `_NAV` |
| Backspace (R2) | Backspace | `_MEDIA` |
| Delete (R3) | Delete | — free slot |

The Tab tap dance gains one level: 3x = Ctrl+Shift+Tab (custom tap dances resolve at
term expiry or interrupt regardless of count, so the 2x path pays nothing extra — see
the correction note in the fast-access section). Shift+tap = Shift+Tab remains as a
bonus via real held Shift composing with the TD output. Verify at implementation:
`clear_weak_mods()` in `td_tab_finished` must not clear physically held Shift.

## Layers (final)

### `_QWERTY` — base

```
 ·   Q   W   E   R   T  │  Y   U   I   O   P   ·
 ·   A●  S●  D●  F●  G  │  H   J●  K●  L●  ;●  ·      ● home row mods
 ·   Z   X   C   V   B  │  N   M   ,   .   '   ·      (GUI Alt Ctrl Shift │ Shift Ctrl Alt GUI)
     ESC  TAB  SPC      │  ENT  BSPC  DEL
```

Combos (v6.3 — J+K=Esc, D+F=Tab, ,+.=Enter removed; Esc/Tab/Enter live on their
thumbs): F+J=CapsWord · K+L=**Alt+Backspace** · S+D=**Alt+Delete** ·
L+;=**Ctrl+Backspace** · A+S=**Ctrl+Delete** (swapped combo names fixed).

**Fast-access allocation — six required shortcuts, ALL hold-free (decided 2026-07-17):**

| Shortcut | Log freq | Gesture | Mechanism |
|---|---|---|---|
| Ctrl+Tab | 22,922 | Tab 2x | existing tap dance |
| Ctrl+Shift+Tab | 3,307 | Tab 3x | new level on the same TD |
| Alt+Backspace | 8,319 | K+L | combo (30 ms window) |
| Alt+Delete | 257 | S+D | combo |
| Ctrl+Backspace | 1,160 | L+; | new combo |
| Ctrl+Delete | ~100 | A+S | new combo |

Symmetry: inner pair = Alt (7:1 more frequent), outer pair = Ctrl. Shift+tap on Tab
remains as a bonus route for Shift+Tab.

Correction on the earlier 3x rejection: custom tap dances resolve at term expiry or
interrupt regardless of count, so today's 2x already waits the 140 ms term — adding a
3x level costs the 2x path nothing. The prior claim that 3x would tax 2x was wrong.

Rejected: hold-based routes (home-row mod + thumb) per the no-hold requirement;
double-tap TDs on base letters E/R (fires on every fast "ee"/"rr" — 3,958/3,646
dictionary words plus Spanish *error/borrar/correr*) and on Backspace (two quick
backspaces would word-delete instead of deleting 2 chars).

### `_HERDR` — hold Esc (dedicated herdr layer)

```
 · wspace prevtab foc↑  nxttab copy  │  ·    ·    ·    ·    ·   ·
 · cycle  foc←    foc↓  foc→   goto  │  ·    ·    ·    ·    ·   ·
 · zoom   close  newtab splitV splitH│  ·    ·    ·    ·    ·   ·
  (ESC)                              │
```

Every key sends `Ctrl+B` + the herdr default binding, with `SS_DELAY(10)` between
prefix and key. E/S/D/F focus cluster mirrors the `_NAV` arrow shape (E↔D SFB accepted
— structurally identical to I↔K in `_NAV` arrows). Letter mirrors kept: Zoom, X=close,
C=new tab, V=split-v, G=goto, W→Q=workspace picker. copy = copy mode (`prefix+[`) on
T. Right half transparent. Dropped (rare, still typeable manually): settings, detach,
notification, sidebar, resize, scrollback, `prefix+shift+1..9`.

### `_NUMBER` — hold Space (unchanged)

```
 ·  F1  F2  F3  F4  ·  │  ·   7   8   9   *   ·
 ·  F5  F6  F7  F8  ·  │  =   4   5   6   +   ·
 ·  F9 F10 F11 F12  ·  │  ·   1   2   3   /   ·
        ·   ·  (SPC)   │  -   .   0
```

### `_SYMBOL` — hold Tab

```
 ·    ·    ·   KVMp  KVMn   ·  │  `   &|   #   /\   ^   ·
 ·    ·    ·   C+C   C+V  CS+V │  @   {}   ()   <>   []  ·
 ·    ·    ·    ·     ·     ·  │  ~   -_   $    %   !?   ·
```

- Left home row (v6.3): copy/paste cluster only — C+C, C+V, C+Shift+V on D/F/G
  (their pones-old positions). Ctrl+Y/Z/X dropped by user decision (Ctrl+Z 172 uses;
  route remains home-row Ctrl + letter, covered by the D chordal exemption). Plain
  keycodes, no tap-hold race, left hand only while the right hand stays on the mouse.
- KVM_PREV/KVM_NEXT on their current physical keys (E/R).
- Symbol tap dances (right half), uniform grammar: **1 tap = open · 2 taps = close ·
  hold = pair** — for `()`, `{}`, `[]`, `<>`. Dropped: `("")`, `()=>{}`, `[0]`, `<=`
  (counting 3–5 taps was the measured "forgetting" error class).
- VS Code shortcuts purged (<0.6% combined use).

### `_NAV` — hold Enter

```
 ·  ·   ·    ·    ·    ·   │  ·   HOME  ↑   END   ·   ·
 ·  ·  GUI  ALT  CTRL SHFT │ PGUP  ←    ↓    →   PGDN ·
 ·  ·   ·    ·    ·    ·   │  ·    ·    ·    ·    ·   ·
        ·    ·    ·        │ (ENT)  ·   ·
```

Pure cursor navigation. Explicit home-row mods enable Ctrl+←/→ (1,488 log events) and
Shift+Arrow chords. KVM, tab-switchers, INS, CAPS all relocated; `DV_SELLN` removed.

### `_MEDIA` — hold Backspace (new)

```
 · BOOT DBTG CAPS PSCR INS │  ·    ·    ·    ·    ·   ·
 ·  ·    ·    ·    ·    ·  │  · prev vol-  vol+  next play
 ·  ·    ·    ·    ·    ·  │  ·   ·    ·    ·     ·    ·
```

- Left row 1: QK_BOOT (Q corner — hardest to hit accidentally), DB_TOGG, CAPS, PSCR,
  INS — base keys centralized here.
- Right: media controls on home row with volume on index/middle (J/K) per the Tier-1 frequency→strong-fingers finding.
- DB_TOGG requires `CONSOLE_ENABLE = yes` (ties into the firmware-telemetry plan).

## Firmware configuration

- **EE_HANDS** replaces `MASTER_LEFT` (config.h:3) — **DECIDED 2026-07-17**. One-time
  handedness write per half; update the `flash-corne` skill to include the EEPROM
  handedness write in its avrdude flow. Accepted edges: an EEPROM clear wipes the mark
  (re-flash it); the OLED primary follows the USB cable. Result: either half can be
  plugged and QK_BOOT'd — physical RESET no longer needed. (Verified: keycodes process
  on the master half, so QK_BOOT only ever bootloaders the plugged half.)
- **Chordal Hold + Permissive Hold**, tapping term 220 ms (validated range 200–250).
  Prerequisites: define `chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS]` with Corne
  handedness ('L'/'R', '*' for thumbs); per-chord exemption via `get_chordal_hold()`
  for the D mod-tap so remaining same-hand Ctrl chords (Ctrl+W 1,137, Ctrl+R/A/F)
  keep working — the `_SYMBOL` cluster covers the big ones.
- `CONSOLE_ENABLE = yes` in rules.mk (DB_TOGG + firmware telemetry sessions).
  Telemetry hook spec: a `process_record_user` logger printing key events via
  `uprintf` MUST gate on `debug_enable` (`if (debug_enable) uprintf(...)`) so
  DB_TOGG actually switches logging on/off at runtime. Host-side capture on macOS
  was spiked and is impossible while keeping Karabiner (hidapi/IOHIDManager/hidutil
  all blocked); the firmware HID console is the only viable channel — raw HID opens
  fine, Karabiner does not touch it. Reference for keycode interception:
  `keyboards/crkbd/lib/keylogger.c` (note: that file is the OLED keylogger, not a
  console logger — our hook is its own small function).
- `SS_DELAY(10)` in herdr prefix macros.
- **Flow Tap deferred** — evaluate with data after v1.
- Upstream: `~/qmk_firmware` at master 0.33.8, 0 commits behind (verified 2026-07-16).
- **Flash: 27,182 / 28,672 bytes (1,490 bytes free)** — measured at compile time with CONSOLE_ENABLE enabled.

## Cleanup

- Remove: `DV_ARR`, `DV_REFS` (defined, never mapped — dead code), `DV_CMNT`,
  `DV_UNCMT`, `DV_TRM`, `DV_SELLN`, `CS_F`, `CS_V`, `CS_P`, `C(KC_B)` from layers,
  `custom_keycodes.h`, and `macros.c`.
- Fix swapped combo names (`del_word_l`/`del_word_r` — actions were always correct).
- Update project `CLAUDE.md`: it describes 9 layers that no longer exist; document
  the 6 layers above, the Tab TD grammar, and the flashing changes (EE_HANDS).

## Telemetry requirements (firmware-first — host capture on macOS is dead)

A spike proved host-side capture on macOS impossible while keeping Karabiner
(hidapi/IOHIDManager/hidutil all blocked; Karabiner is the only layer that sees the
physical Corne and does not export). **The firmware HID console is the instrument**:
`CONSOLE_ENABLE` + the `process_record_user` hook gated on `debug_enable` (spec in
Firmware configuration), toggled from `_MEDIA` DB_TOGG. The historic Linux CSV
remains the baseline corpus. The firmware logger must capture:

1. **All key events** including unmodified keys (Esc, Enter, Backspace, Tab, quote) —
   correction signatures (mistype → Esc → Backspace → retry) are invisible in the
   historic modifier-only log.
2. **Millisecond timestamps** (`timer_read32()`; historic log: 1 s resolution).
   Tap-hold races live in 100–220 ms windows; inter-event gaps separate timing
   misfires from forgetting.
3. Tap-hold/TD resolutions where observable — the firmware sees what the host never
   can: which way each dual-role key settled.

**Diagnosis correction (verified in QMK core `process_tap_dance.c:169-195`):** an
interrupting keypress finishes the active tap dance FIRST and is then re-looked-up on
the updated layer — hold-Tab + `'` with any overlap already yields `?` today, even
under the 140 ms term. The tap dance is NOT the culprit. The failing mode is
zero-overlap sequences (Tab fully released before `'`), which no QMK option can fix —
it can only be measured.

**Already queryable in the existing log** via modifier-only proxies: `Shift+/`
preceded within 5 s by Ctrl+C or Alt+Backspace → 191 candidate corrections = 4.4% of
all 4,358 `?` presses (noisy lower bound; ~1 in 20 `?` presses costs a correction
cycle).

## Error-pattern sweep findings (full-log analysis, 2026-07-16)

Generalized correction-pattern query (`X → Alt/Ctrl+Backspace → Y` within 5 s, plus
same-combo retries) across all 146 logged combos, workflow noise excluded
(copy-paste chains, vim Ctrl+C→`:` flows):

| Error class | Evidence | Cause | Resolution |
|---|---|---|---|
| Tab-switch wrong direction | 19 pair corrections (CS-Tab↔C-Tab) | Existing 2x-tap TD underused; user stacked 2 home-row mods + Tab manually | One-key Tab TD grammar (incl. Shift variants) |
| Closing symbol by tap-count | `(`→`)` corrections + paren retries | `)` was the 5th TD action, `]` 4th, `}` 3rd | Uniform TD grammar: 2 taps = close |
| `?` at speed | 16 same-combo retries + 4.4% marker rate | Zero-overlap sequences — NOT the tap dance | Measure only (ms telemetry) |
| Home-row Shift rolls | `Shift+space` ×114 (never intentional) | Shift held rolling capital→space | Chordal Hold |

## Adversarial validation vs. ergonomics research (2026-07-16)

**Confirmed (Tier 1):** Chordal Hold + Permissive Hold, 220 ms term, home row mods,
`_MEDIA` home-row media with volume on index/middle, low-frequency commands on weak
corners.

**Challenges, adjudicated:**
- *Ctrl+Tab top-row vs frequency→home-row*: the primary Ctrl+Tab route is the Tab TD
  (one key); resolved in v6 by consolidating everything on the Tab key.
- *Alt+Backspace vs "prioritize Ctrl+Z"*: validator conflated word-delete with undo;
  the 7:1 corpus overrides the Tier-2 generalization. Decision stands.

**SFB audit of the herdr cluster:** E↔D (focus up→down), W↔S, R↔F pairs identified.
E↔D is structurally identical to I↔K in the existing `_NAV` arrows — accepted cost of
any inverted-T cluster. Telemetry (which sees letters after Ctrl+B) must measure real
sequence frequency.

**Open measurements:** hand-alternation % and pinky-load % benchmarks (carpalx or
equivalent) before/after implementation.

## Adversarial validation vs. QMK source (2026-07-16, master 0.33.8)

**Confirmed with citations:** Chordal Hold does not interfere with combos on mod-tap
members (`action_tapping.c:941`); `'*'` exempts thumbs (`action_tapping.c:945-950`);
hold-for-close/pair TD grammar reliable at speed; herdr SEND_STRING macros do not
leak mods.

**Feature sweep (previously unevaluated):** Space Cadet, Key Overrides, Auto Shift,
Retro Tapping, Tri Layer, Repeat Key — evaluated, irrelevant to this plan's problems.
Layer Lock: optional nicety (+~200 B), not adopted. Leader Key: validator dissent
recorded; rejection stands on burst data.

## Open items

1. Implementation checks: `clear_weak_mods()` vs real held Shift on the Tab TD;
   actual flash size with CONSOLE_ENABLE.
2. Telemetry-decided: `?` overlap/sequential split, herdr-cluster SFB sequence
   frequency, hand-alternation and pinky-load benchmarks, 2–4 week validation
   protocol (`docs/research/ergonomics.md`).
3. Free slots kept empty until the log justifies a tenant: Delete-hold, `_SYMBOL`
   left rows 1/3 remainder, `_HERDR` right half, `_NAV` row 3.

## Execution sequence (on explicit "go")

1. Finish the macOS logger port (spec above); collect a baseline.
2. Implement keymap + config + prerequisites; compile and verify flash size.
3. Flash via `flash-corne` skill (updated for EE_HANDS handedness writes); measure
   2–4 weeks against baseline.
4. Update project `CLAUDE.md`.
