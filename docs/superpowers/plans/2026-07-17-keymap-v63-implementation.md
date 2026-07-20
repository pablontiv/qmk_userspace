# Keymap v6.3 Implementation Plan — crkbd/rev1:pones

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement the validated keymap redesign (spec: `docs/keymap-redesign-plan.md`) — 6 layers, hold-free shortcut set, herdr prefix layer, EE_HANDS, Chordal Hold, firmware telemetry hook.

**Architecture:** All changes live in the QMK userspace keymap `keyboards/crkbd/rev1/keymaps/pones/`. A new shared `features/layers.h` exposes the layer enum to keymap.c, macros.c, and tap_dance.c. The herdr layer uses plain keycodes translated by one dispatch function in `process_record_user`. Every task ends with a successful `qmk compile` and a commit.

**Tech Stack:** QMK firmware (master 0.33.8 at `~/qmk_firmware`), ATmega32U4 (Caterina bootloader), avrdude via the `flash-corne` project skill.

## Global Constraints

- Test gate per task: `qmk compile -kb crkbd/rev1 -km pones` ends with `* The firmware size is fine` — record the byte count; the build must stay under the 28,672-byte application limit.
- Conventional commits only. No AI attribution, no Co-Authored-By.
- Tapping term stays 220 ms globally; per-key terms in `get_tapping_term` stay as-is except where a task says otherwise.
- `COMBO_TERM 30` stays unchanged.
- Do NOT touch: `_NUMBER` layer contents, F-key rows, OLED code, KVM macro implementations, `TD_SLB`, `TD_MNU`, `TD_AMP`, `TD_EXQ` tap dances.
- Verification hardware flow: `flash-corne` skill. First flash after this plan = EE_HANDS bootstrap (physical RESET one last time, per Task 9).

---

### Task 1: Shared layer header

**Files:**
- Create: `keyboards/crkbd/rev1/keymaps/pones/features/layers.h`
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c` (enum at lines 26-31)
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/tap_dance.h` (drop `TD_LAYER_SYMBOL` hack)

**Interfaces:**
- Produces: enum `_QWERTY, _NUMBER, _SYMBOL, _NAV, _HERDR, _MEDIA` in `features/layers.h`, included by later tasks. `tap_dance.c` uses `_SYMBOL` directly.

- [ ] **Step 1: Create `features/layers.h`**

```c
#pragma once

// Layer order. _SYMBOL index is used by tap_dance.c; keep before _NAV.
enum layer_names {
  _QWERTY,   // Base QWERTY with home row mods
  _NUMBER,   // F-keys + numpad (unchanged)
  _SYMBOL,   // Symbols right, KVM + copy/paste cluster left
  _NAV,      // Pure cursor navigation
  _HERDR,    // herdr prefix layer (Ctrl+B + key)
  _MEDIA,    // RGB, media, base utility keys, BOOT/DEBUG
};
```

- [ ] **Step 2: Replace the enum in `keymap.c`**

Delete the `enum layer_names { ... };` block (lines 26-31) and add to the includes at the top:

```c
#include "features/layers.h"
```

- [ ] **Step 3: Point tap_dance at the enum**

In `features/tap_dance.h`, delete the line `#define TD_LAYER_SYMBOL 2` and add `#include "layers.h"`. In `features/tap_dance.c`, replace both uses of `TD_LAYER_SYMBOL` with `_SYMBOL`.

- [ ] **Step 4: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine` (same size as baseline — no functional change). Compiling with the enum missing from keymap.c would fail with `_HERDR undeclared` in later tasks; this task makes that impossible.

- [ ] **Step 5: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/
git commit -m "refactor(keymap): extract layer enum to shared features/layers.h"
```

---

### Task 2: Firmware configuration (EE_HANDS, Chordal Hold, console)

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/config.h` (line 3 `#define MASTER_LEFT`)
- Modify: `keyboards/crkbd/rev1/keymaps/pones/rules.mk`
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c`

**Interfaces:**
- Consumes: `HM_D` define (`keymap.c:16`).
- Produces: `chordal_hold_layout[]` and `get_chordal_hold()` in keymap.c.

- [ ] **Step 1: config.h — handedness and tap-hold behavior**

Replace `#define MASTER_LEFT` with:

```c
#define EE_HANDS
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD
```

- [ ] **Step 2: rules.mk — console**

Add:

```make
CONSOLE_ENABLE = yes
```

- [ ] **Step 3: keymap.c — handedness map and D exemption**

Add after the `keymaps[]` array:

```c
// Chordal Hold handedness: opposite-hands rule; thumbs ('*') always allowed.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x6_3(
  'L','L','L','L','L','L',      'R','R','R','R','R','R',
  'L','L','L','L','L','L',      'R','R','R','R','R','R',
  'L','L','L','L','L','L',      'R','R','R','R','R','R',
                '*','*','*',    '*','*','*'
);

// Same-hand exemption for the Ctrl mod-tap on D: Ctrl+W/R/A/F etc. stay usable
// left-handed (decided in the plan; the _SYMBOL cluster covers C/V).
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    if (tap_hold_keycode == HM_D) return true;
    return get_chordal_hold_default(tap_hold_record, other_record);
}
```

- [ ] **Step 4: Compile and record size**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine`. CONSOLE_ENABLE adds ~1-2 KB — record the new byte count in the commit body. If the size check FAILS, stop and report; do not trim features unilaterally.

- [ ] **Step 5: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/
git commit -m "feat(keymap): enable EE_HANDS, chordal+permissive hold, console"
```

---

### Task 3: Combos v6.3

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c:104-118`

- [ ] **Step 1: Replace the whole combo block**

Delete lines 104-118 (both arrays) and write:

```c
// Combo definitions (QMK introspection requires these in keymap.c)
const uint16_t PROGMEM combo_capsword[] = {HM_F, HM_J, COMBO_END};   // F+J
const uint16_t PROGMEM del_word_left[]  = {HM_K, HM_L, COMBO_END};   // K+L
const uint16_t PROGMEM del_word_right[] = {HM_S, HM_D, COMBO_END};   // S+D
const uint16_t PROGMEM del_wordc_left[] = {HM_L, HM_SCLN, COMBO_END};// L+;
const uint16_t PROGMEM del_wordc_right[]= {HM_A, HM_S, COMBO_END};   // A+S

combo_t key_combos[] = {
    COMBO(combo_capsword, CW_TOGG),      // F+J  = Caps Word
    COMBO(del_word_left,  A(KC_BSPC)),   // K+L  = Alt+Backspace (delete word left)
    COMBO(del_word_right, A(KC_DEL)),    // S+D  = Alt+Delete (delete word right)
    COMBO(del_wordc_left, C(KC_BSPC)),   // L+;  = Ctrl+Backspace
    COMBO(del_wordc_right,C(KC_DEL)),    // A+S  = Ctrl+Delete
};
```

Removed on purpose (spec v6.3): J+K=Esc, D+F=Tab, ,+.=Enter — Esc/Tab/Enter live on thumbs.

- [ ] **Step 2: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine`.

- [ ] **Step 3: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/keymap.c
git commit -m "feat(keymap): v6.3 combo set - Alt/Ctrl word deletes, drop redundant combos"
```

---

### Task 4: Tab tap dance 3x level

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/tap_dance.c:164-186` (`td_tab_finished`)

- [ ] **Step 1: Add the 3-tap case**

In `td_tab_finished`, add after the `case 2:` block:

```c
        case 3:
            tap_code16(C(S(KC_TAB)));
            break;
```

Resulting switch: 1 = Tab (tap) / `_SYMBOL` (hold), 2 = Ctrl+Tab, 3 = Ctrl+Shift+Tab. Custom dances resolve at term expiry or interrupt regardless of count — the 2x path pays no extra latency (verified against `process_tap_dance.c:169-195`).

- [ ] **Step 2: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine`.

- [ ] **Step 3: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/features/tap_dance.c
git commit -m "feat(keymap): add 3x tap level Ctrl+Shift+Tab to tab dance"
```

---

### Task 5: Symbol tap dance uniform grammar

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/tap_dance.c` (`td_j_finished`, `td_k_finished`, `td_sc_finished`, `td_l_finished`)
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c:43-46` (alias comments)

Grammar (spec): **1 tap = open · 2 taps = close · hold = pair**. Exotic actions dropped.

- [ ] **Step 1: Rewrite the four finished functions**

Replace each function body completely (reset functions stay untouched):

```c
void td_j_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) { SEND_STRING("{}"); } // hold = pair
            else                { tap_code16(KC_LCBR); } // 1 tap = {
            break;
        case 2:
            tap_code16(KC_RCBR); // 2 taps = }
            break;
    }
}

void td_k_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) { SEND_STRING("()"); }
            else                { tap_code16(KC_LPRN); }
            break;
        case 2:
            tap_code16(KC_RPRN);
            break;
    }
}

void td_sc_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) { SEND_STRING("<>"); }
            else                { tap_code16(KC_LT); }
            break;
        case 2:
            tap_code16(KC_GT);
            break;
    }
}

void td_l_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) { SEND_STRING("[]"); }
            else                { tap_code16(KC_LBRC); }
            break;
        case 2:
            tap_code16(KC_RBRC);
            break;
    }
}
```

- [ ] **Step 2: Update the alias comments in keymap.c (lines 43-46)**

```c
#define TD_JBR TD(TD_J_BRACES)    // { | 2x=} | hold={}
#define TD_KPR TD(TD_K_PARENS)    // ( | 2x=) | hold=()
#define TD_SAN TD(TD_SC_ANGLES)   // < | 2x=> | hold=<>
#define TD_LBK TD(TD_L_BRACKETS)  // [ | 2x=] | hold=[]
```

- [ ] **Step 3: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine` (size should DROP — exotic SEND_STRINGs removed).

- [ ] **Step 4: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/
git commit -m "feat(keymap): uniform symbol tap-dance grammar open/close/pair"
```

---

### Task 6: Purge dead VS Code keycodes and macros

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/custom_keycodes.h`
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/macros.c`
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c:7-11` (defines)

- [ ] **Step 1: custom_keycodes.h — keep only KVM**

```c
#pragma once

#include QMK_KEYBOARD_H

// Custom keycodes
enum custom_keycodes {
  KVM_PREV = SAFE_RANGE, // KVM switch: previous computer (Ctrl, Ctrl, Page Up)
  KVM_NEXT,              // KVM switch: next computer (Ctrl, Ctrl, Page Down)
};
```

Removed: `DV_ARR`, `DV_CMNT`, `DV_UNCMT`, `DV_SELLN`, `DV_REFS` (DV_ARR/DV_REFS were never mapped — dead code).

- [ ] **Step 2: macros.c — remove the dead cases**

In `process_record_user`, delete the `case DV_ARR:`, `case DV_CMNT:`, `case DV_UNCMT:`, `case DV_REFS:`, and `case DV_SELLN:` blocks. Keep `KVM_PREV`/`KVM_NEXT` untouched.

- [ ] **Step 3: keymap.c — remove the VS Code defines (lines 7-11)**

Delete the block `#define DV_TRM ... #define CS_F ...` entirely (all four defines and the comment). The layer rewrite in Task 7 removes their uses; to keep this task compiling on its own, this task and Task 7 may be committed together ONLY if compile fails here — otherwise commit separately.

- [ ] **Step 4: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: FAILS with `DV_TRM undeclared` (still referenced by the old `_SYMBOL` layer). This is the expected red state — proceed to Task 7 and compile there; commit Tasks 6+7 together in Task 7's commit step.

---

### Task 7: Layer rewrite — thumbs, `_SYMBOL`, `_NAV`, `_MEDIA`

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/keymap.c` (layer-tap defines lines 34-40, `keymaps[]` array)

**Interfaces:**
- Consumes: enum from `features/layers.h` (Task 1).
- Produces: `_HERDR` layer keys as plain keycodes for Task 8's dispatcher.

- [ ] **Step 1: Replace the layer-tap defines (lines 34-40)**

```c
// Layer-tap definitions (tap for key, hold to access layer)
#define L_ESC LT(_HERDR, KC_ESCAPE)    // Escape: tap Esc, hold herdr prefix layer
#define L_TAB TD(TD_TAB_CTRLTAB)       // Tab: 1=Tab 2=C+Tab 3=C+S+Tab, hold SYMBOL
#define L_SPC LT(_NUMBER, KC_SPACE)    // Space: tap space, hold numbers
#define L_ENT LT(_NAV, KC_ENTER)       // Enter: tap enter, hold navigation
#define L_BK  LT(_MEDIA, KC_BSPC)      // Backspace: tap bspc, hold media/utility
#define L_DEL KC_DEL                   // Delete: plain (free hold slot)
```

- [ ] **Step 2: Replace `_SYMBOL` in `keymaps[]`**

```c
    [_SYMBOL] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX,KVM_PREV,KVM_NEXT, XXXXXXX,                      KC_GRV,  TD_AMP, KC_HASH,  TD_SLB, KC_CIRC, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, C(KC_C), C(KC_V),C(S(KC_V)),                     KC_AT,  TD_JBR,  TD_KPR,  TD_SAN,  TD_LBK, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD,  TD_MNU,  KC_DLR, KC_PERC,  TD_EXQ, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),
```

- [ ] **Step 3: Replace `_NAV` in `keymaps[]`**

```c
    [_NAV] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_HOME,   KC_UP,  KC_END, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_PGUP, KC_LEFT, KC_DOWN,KC_RIGHT, KC_PGDN, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),
```

- [ ] **Step 4: Add `_HERDR` and `_MEDIA` to `keymaps[]`**

```c
    [_HERDR] = LAYOUT_split_3x6_3(
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),

    [_MEDIA] = LAYOUT_split_3x6_3(
      XXXXXXX, QK_BOOT, DB_TOGG, KC_CAPS, KC_PSCR,  KC_INS,                      XXXXXXX, UG_SPDD, UG_SPDU, UG_NEXT, UG_TOGG, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),
```

Note `_MEDIA` right home row is `KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY` on J/K/L/;/' columns — prev/vol-/vol+/next/play with volume on index/middle per spec. `_HERDR` keys are PLAIN letters; Task 8 translates them.

- [ ] **Step 5: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine` (this resolves Task 6's red state).

- [ ] **Step 6: Commit (covers Tasks 6+7)**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/
git commit -m "feat(keymap): v6.3 layers - herdr/media/nav rewrite, purge VS Code macros"
```

---

### Task 8: herdr prefix dispatcher

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/macros.c`

**Interfaces:**
- Consumes: `_HERDR` from `features/layers.h`; plain keycodes placed by Task 7.

- [ ] **Step 1: Add the dispatcher to macros.c**

Add `#include "layers.h"` to the includes, then above `process_record_user`:

```c
// herdr prefix layer: translate the pressed key into Ctrl+B + herdr binding.
// Returns false when the key was handled (swallow the plain keycode).
static bool process_herdr(uint16_t keycode, keyrecord_t *record) {
    if (get_highest_layer(layer_state) != _HERDR || !record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case KC_Q: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "w");  return false; // workspace picker
        case KC_W: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "p");  return false; // prev tab
        case KC_E: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "k");  return false; // focus up
        case KC_R: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "n");  return false; // next tab
        case KC_T: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "[");  return false; // copy mode
        case KC_A: SEND_STRING(SS_LCTL("b") SS_DELAY(10) SS_TAP(X_TAB)); return false; // cycle pane
        case KC_S: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "h");  return false; // focus left
        case KC_D: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "j");  return false; // focus down
        case KC_F: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "l");  return false; // focus right
        case KC_G: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "g");  return false; // goto
        case KC_Z: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "z");  return false; // zoom
        case KC_X: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "x");  return false; // close pane
        case KC_C: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "c");  return false; // new tab
        case KC_V: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "v");  return false; // split vertical
        case KC_B: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "-");  return false; // split horizontal
    }
    return true;
}
```

- [ ] **Step 2: Call it first inside `process_record_user`**

At the top of `process_record_user`, before the existing switch:

```c
  if (!process_herdr(keycode, record)) {
    return false;
  }
```

- [ ] **Step 3: Compile**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine`.

- [ ] **Step 4: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/features/macros.c
git commit -m "feat(keymap): herdr prefix dispatcher on _HERDR layer"
```

---

### Task 9: Telemetry hook (debug-gated console logger)

**Files:**
- Modify: `keyboards/crkbd/rev1/keymaps/pones/features/macros.c`

- [ ] **Step 1: Add the gated logger**

Add `#include "print.h"` to macros.c includes. At the very top of `process_record_user` (before the herdr call):

```c
  if (debug_enable) {
    uprintf("KL:t%lu,k0x%04X,r%u,c%u,%c\n",
            timer_read32(), keycode,
            record->event.key.row, record->event.key.col,
            record->event.pressed ? 'd' : 'u');
  }
```

Gating on `debug_enable` makes `_MEDIA`'s DB_TOGG the runtime on/off switch — logging is OFF by default at boot.

- [ ] **Step 2: Compile and record final size**

Run: `qmk compile -kb crkbd/rev1 -km pones`
Expected: `* The firmware size is fine`. Record the final byte count — this is the number that goes in the spec's flash line.

- [ ] **Step 3: Commit**

```bash
git add keyboards/crkbd/rev1/keymaps/pones/features/macros.c
git commit -m "feat(keymap): debug-gated key event logger for telemetry"
```

---

### Task 10: EE_HANDS bootstrap flash + on-keyboard verification

**Files:**
- Modify: `.claude/skills/flash-corne/SKILL.md` and `assets/flash-macos.sh` (add handedness + QK_BOOT flow)

- [ ] **Step 1: Update the flash-corne skill**

Add to the skill: (a) bootstrap section — one-time per-half EEPROM handedness write flashed together with the firmware (`qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-left` for the left half, `-bl avrdude-split-right` for the right, or the equivalent avrdude `-U eeprom` write in `flash-macos.sh` if the qmk wrapper hits the pty race); (b) permanent flow — enter bootloader with `_MEDIA`-hold + QK_BOOT on whichever half holds the USB cable, then run the existing avrdude write; physical RESET only as fallback.

- [ ] **Step 2: Bootstrap flash (LAST physical-RESET session)**

Follow the flash-corne skill: flash left half with handedness=left, flash right half with handedness=right (physical RESET each, one last time). Both halves now carry the v6.3 firmware + EEPROM marks.

- [ ] **Step 3: On-keyboard verification checklist**

Verify each feature physically; every line must pass:

- Base typing: normal prose, no stuck mods, no misfired layers.
- Combos: K+L deletes word left (Alt+Bspc), S+D word right, L+; Ctrl+Bspc, A+S Ctrl+Del, F+J CapsWord. J+K / D+F / ,+. produce NOTHING beyond the letters.
- Tab: single=Tab, 2x=Ctrl+Tab (browser tab switch), 3x=Ctrl+Shift+Tab (reverse), hold+key=symbol layer, Shift(home-row)+tap=Shift+Tab (confirms clear_weak_mods does not eat real Shift).
- Symbol TDs: `(` 1 tap, `)` 2 taps, `()` hold — same for `{} [] <>`.
- `_SYMBOL` cluster: Tab-hold + D/F/G = copy/paste/paste-special; Tab-hold + E/R = KVM.
- `_HERDR`: Esc-hold + S/D/F/E moves herdr pane focus; C new tab; V/B splits; T copy mode.
- `_NAV`: Enter-hold + arrows/Home/End; Enter-hold + home-row mod + arrow = Ctrl/Shift+arrow.
- `_MEDIA`: Bspc-hold + J/K = volume; W = DB_TOGG (then `qmk console` shows `KL:` lines; toggle off stops them); Q = QK_BOOT enters bootloader (verify with `ls /dev/cu.usbmodem*`).
- Chordal Hold: roll "df", "jk", "kl" fast in prose — no misfired mods; D-hold + C still gives Ctrl+C.
- EE_HANDS: plug USB into the RIGHT half — layout stays correct; QK_BOOT bootloaders the right half.

- [ ] **Step 4: Commit skill update**

```bash
git add .claude/skills/flash-corne/
git commit -m "feat(tooling): flash-corne EE_HANDS bootstrap and QK_BOOT flow"
```

---

### Task 11: Documentation sync

**Files:**
- Modify: `CLAUDE.md` (project)
- Modify: `docs/keymap-redesign-plan.md` (status + final flash size)

- [ ] **Step 1: Rewrite CLAUDE.md keymap sections**

Update the "Keymap Architecture" section: the 6 real layers (`_QWERTY`, `_NUMBER`, `_SYMBOL`, `_NAV`, `_HERDR`, `_MEDIA`), the thumb table with the Tab TD grammar, the v6.3 combo list, EE_HANDS flashing note pointing at the flash-corne skill. Delete the stale 9-layer description and stale layer-access list.

- [ ] **Step 2: Update the spec status**

In `docs/keymap-redesign-plan.md`: change status line to `Implemented <date> — validation period running`, record the final flash byte count.

- [ ] **Step 3: Commit**

```bash
git add CLAUDE.md docs/keymap-redesign-plan.md
git commit -m "docs: sync CLAUDE.md and spec status with implemented keymap v6.3"
```
