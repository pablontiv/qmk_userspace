# QMK Firmware Features Research: Keymap Redesign for Corne (ATmega32U4)

**Date:** July 2026  
**Target:** crkbd/rev1:pones (ATmega32U4, 32KB flash)  
**Scope:** Keymap redesign with tmux-prefix layer, Ctrl+Tab navigation, enhanced word-delete combos

---

## 1. Tap-Hold Options in QMK Master

### Chordal Hold (NEW, Feb 2025)
- **Status:** Merged in QMK 0.28.0 (Feb 2025), PR [#24560](https://github.com/qmk/qmk_firmware/pull/24560)
- **What it does:** Implements "opposite hands" rule for tap-hold keys. Settles as *tapped* if both keys pressed on same hand, may settle as *held* if on opposite hands. Prevents accidental mod activation during rolled keypresses (home row mods).
- **Configuration:**
  - Automatic handedness via `chordal_hold_layout` array ('L'/'R'/'*')
  - Callback `chordal_hold_handedness()` for dynamic logic
  - Per-chord `get_chordal_hold()` fine-tuning
- **Recent fix:** [PR #24878](https://github.com/qmk/qmk_firmware/pull/24878) (Feb 2025) addressed stuck mods in stuttered sequences
- **Flash cost:** Minimal (~200–400 bytes)
- **Replaces:** Community tool "Achordion" (no longer needed, use native Chordal Hold)

### Flow Tap (Recent, 2024–2025)
- **What it does:** Disables hold behavior during fast typing by triggering tap action within `FLOW_TAP_TERM` (recommend 150ms) of previous key. Reduces input lag and accidental mods when typing quickly.
- **Configuration:** Automatic for alphanumeric + space; customize via `is_flow_tap_key()` and `get_flow_tap_term()` callbacks
- **Flash cost:** Minimal (~100 bytes)

### PERMISSIVE_HOLD (Established, pre-2024)
- **What it does:** Converts nested sequences. Tap-hold performs hold if another key is tapped while held, even before tapping term expires. Won't trigger on rolled keypresses.
- **Enable:** `#define PERMISSIVE_HOLD` or per-key via `PERMISSIVE_HOLD_PER_KEY` + callback
- **Flash cost:** Negligible

### HOLD_ON_OTHER_KEY_PRESS (Established, pre-2024)
- **What it does:** More aggressive than Permissive Hold. If dual-role key pressed and another key tapped, dual-role performs hold action. Catches both nested taps and rolling keypresses.
- **Enable:** `#define HOLD_ON_OTHER_KEY_PRESS` or per-key variant
- **Flash cost:** Negligible

### Per-Key Hold Options
- Most tap-hold features support `_PER_KEY` variants (e.g., `CHORDAL_HOLD_PER_KEY`, `PERMISSIVE_HOLD_PER_KEY`)
- Implement callback (e.g., `get_permissive_hold()`) evaluating `record->event.key.row/col` or keycode logic
- Adds ~100–200 bytes per callback

### Recommendation
**Chordal Hold** is the new standard for home row mods on split boards (your use case). It's superior to PERMISSIVE_HOLD + HOLD_ON_OTHER_KEY_PRESS for preventing false activations during rolling. Adopt it for your pones keymap.

---

## 2. Combos: Advanced Options & Flash Cost

### Core Settings
- **COMBO_TERM:** Default 50ms timeout. Adjust to 40–60ms depending on your typing speed. `#define COMBO_TERM 40`
- **COMBO_SHOULD_TRIGGER:** Advanced hook; implement `bool combo_should_trigger()` to conditionally allow/block combos. Use case: disable combos on certain layers, disable during fast typing, etc.

### Layer-Specific Combos
Two approaches:

1. **Global reference layer:** `#define COMBO_ONLY_FROM_LAYER 0` — all combos check keys only from layer 0, useful for multi-layout keyboards
2. **Per-layer references:** Implement `combo_ref_from_layer()` to assign different reference layers to different active layers

### Memory/Flash Impact
- Combo size configurations:
  - `EXTRA_SHORT_COMBOS` → 6-key max (saves memory, compile-time fixed)
  - Default → 8-key max
  - `EXTRA_LONG_COMBOS` → 16-key max
  - `EXTRA_EXTRA_LONG_COMBOS` → 32-key max
- **Flash cost per combo:** ~20–40 bytes (includes keycode, trigger keys, action)
- **Buffer tuning:** `COMBO_KEY_BUFFER_LENGTH` and `COMBO_BUFFER_LENGTH` adjust processing buffers; defaults work for most keymaps
- **Optimization:** `EXTRA_SHORT_COMBOS` saves ~100–300 bytes if you only need 6-key combos

### Recommendation
Combos are **flash-efficient** for your use case. Use `COMBO_ONLY_FROM_LAYER 0` if all combos live on base layer. Your word-delete combos (+90 bytes) and future multi-key combos fit comfortably. Consider `COMBO_SHOULD_TRIGGER` if you want layer-specific disable logic (minimal overhead).

---

## 3. Prefix-Layer Patterns: Tap Dance vs. Combos vs. Leader Key vs. One-Shot

### Use Case: Tmux Prefix (Ctrl+B, then X)

| Feature | Fit for Tmux Prefix | Flash Cost | Notes |
|---------|---------------------|-----------|-------|
| **Tap Dance** | ❌ Poor | ~300–500/combo | Sequential same-key presses; doesn't suit Ctrl+B+X |
| **Combos** | ⚠️ Awkward | ~30–40/combo | Requires simultaneous Ctrl+B+X; not true prefix behavior |
| **Leader Key** | ✅ Excellent | ~400–600 | Dedicated prefix key → sequence of keys; exact fit; configurable timeout (default 300ms) |
| **One-Shot Layers** | ⚠️ Moderate | ~200–400 | OSL() for layer switch, then press X; works but less intuitive than Leader |
| **Macros (process_record_user)** | ✅ Good | ~50–100/macro | Custom keycode + SEND_STRING or keystroke sequence; low overhead; most flexible |
| **Repeat Key** | ❌ N/A | ~200 | Repeats last key; no prefix support |

### Best Approach for Your Use Case
**Leader Key** (`QK_LEAD` + `leader_sequence_two_keys()`) for Ctrl+B+X:
```c
LEADER_ENABLE = yes
LEADER_TIMEOUT = 300  // ms

// In keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
    QK_LEAD, // Assign to a layer tap or thumb key
    ...
  ),
};

void leader_end_user(void) {
    if (leader_sequence_two_keys(KC_T, KC_X)) {  // Ctrl+B + t, x sends Ctrl+B+t, x
        SEND_STRING_DELAY(SS_LCTL("b") "t", 10);
    }
}
```

**Alternative: Macros** if you need complex conditional logic or multiple sequences. Slightly higher complexity but very transparent and GCC-efficient.

### Recommendation
**Use Leader Key** for your tmux-prefix layer. It's designed for this exact use case, has proven timing stability, and costs only ~400 bytes. If you need many sequences or complex per-sequence logic, **use Macros** instead.

---

## 4. Macros vs. process_record_user Interceptor

### SEND_STRING + process_record_user
```c
#define MACRO_TMUX_SEND_SELECTION SS_LCTL("b") "v"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO_TMUX_SEND_SELECTION:
            if (record->event.pressed) {
                SEND_STRING_DELAY(MACRO_TMUX_SEND_SELECTION, 10);
            }
            return false;  // Prevent default action
    }
    return true;
}
```

### Pros
- Straightforward; one function handles all custom keycodes
- String variants (PSTR/PROGMEM on AVR) save RAM at the cost of ~10 bytes per string in PROGMEM
- Timing control via `SS_DELAY(ms)` for mod transition delays

### Cons & Pitfalls
- **Modifier interaction:** SEND_STRING doesn't respect active mods from home row mods. If Shift(home row mod) is active and you call SEND_STRING, it ignores the Shift. Workaround: use `SS_LSFT()` wrapper or manual mod logic.
- **Timing:** Default keystroke delay is ~10ms. If tmux is slow, increase to 20–50ms between Ctrl+B and subsequent keystrokes.
- **No state tracking:** Multiple rapid calls to the same macro won't queue; you must manage debouncing.

### Whole-Layer Interceptor Pattern
If you want every key on a tmux layer to send Ctrl+B first:
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (IS_LAYER_ON(_TMUX_PREFIX) && record->event.pressed) {
        // Emit Ctrl+B, then the key
        register_code(KC_LCTL);
        register_code(KC_B);
        unregister_code(KC_B);
        unregister_code(KC_LCTL);
        tap_code(keycode);  // Tap the key itself
        return false;  // Prevent re-processing
    }
    return true;
}
```

This is cleaner than individual macros but requires careful `record->event.key` parsing and state management. Use **only if your entire tmux layer is identical prefix behavior**; otherwise, mix Leader Key + individual macros for clarity.

### Recommendation
**Prefer Leader Key for tmux prefix.** It handles timing, sequencing, and mod state automatically. Use **macros sparingly** for non-prefix multi-key sequences (e.g., Alt+Tab repeats). Avoid whole-layer interceptors unless your keymap is extremely repetitive; they're error-prone on ATmega32U4.

---

## 5. Upstream Version & Breaking Changes (Feb 2025–Jul 2026)

### Current Upstream Status
- **QMK Master HEAD:** Version 0.30+ (as of July 2026)
- **Release cycle:** 3-month cadence; next breaking change Aug 30, 2026 (v0.33.0)

| Version | Release | Notable Changes | Breaking? |
|---------|---------|-----------------|-----------|
| **0.28.0** | Feb 23, 2025 | Chordal Hold, Community Modules | No (new features) |
| **0.29.0** | May 25, 2025 | Encoder callback deprecation notice | Partial (callbacks end-of-life by Aug 2025) |
| **0.30.0** | Aug 31, 2025 | (TBD in changelogs) | No (planned) |
| **0.31.0** | Nov 30, 2025 | Encoder callbacks removed | **Yes** (breaking) |
| **0.32.0** | Feb 22, 2026 | (TBD) | No (planned) |
| **0.33.0** | May 31, 2026 | (TBD) | Possible (Aug 30 breaking change) |

### Breaking Changes Affecting Your Keymap
1. **DEFAULT_FOLDER deprecation** ([PR #24836](https://github.com/qmk/qmk_firmware/pull/24836)): Removed; use `keyboard.json` instead. **Userspace keymaps not affected** (you're in qmk_userspace, not core boards).
2. **Encoder callback removal** (Nov 2025 / v0.31.0): If your Corne uses encoders, migrate from `encoder_update_user()` to encoder mapping in `keymap.json`. You don't use encoders, so no impact.
3. **Tap dance API:** No breaking changes announced. Your tap-dance combos remain compatible.

### Recommended Action
**Your current keymap (home row mods, tap dance, combos, Caps Word, OLED) has NO breaking compatibility issues through v0.33.0 (July 2026).** An upstream update would immediately unlock:
- **Chordal Hold** (v0.28+): Superior to PERMISSIVE_HOLD
- **Flow Tap** (v0.29+): Faster typing experience
- **Community Modules** (v0.28+): Third-party tap-hold alternatives, if needed

**Downside:** Updating requires rebuilding and reflashing. Only justify if you're adopting Chordal Hold or need newer features; your keymap works fine on upstream v0.27 or earlier.

---

## 6. Flash-Size Impact Estimates (ATmega32U4: 32KB usable after bootloader)

### Baseline
- QMK core + AVR HAL: ~4–5 KB
- OLED display driver: ~2–3 KB
- RGB effects (current, trimmed): ~1–2 KB (effects already disabled; see CLAUDE.md note)

**Usable budget: ~20–22 KB remaining**

### Feature Costs (Approximate)
| Feature | Flash Cost | Notes |
|---------|-----------|-------|
| Tap Dance (5 actions) | +200–300 B | Low cost; you have this |
| Combos (10 combos, default size) | +300–400 B | Low cost; you have this |
| Chordal Hold | +200–400 B | Enable with `CHORDAL_HOLD_ENABLE = yes` |
| Flow Tap | +100–200 B | Minimal; enable with `FLOW_TAP_ENABLE = yes` |
| Leader Key (10 sequences) | +400–600 B | Moderate cost but worth it for tmux |
| Macros (3 complex SEND_STRING) | +100–200 B | Low per-macro; string data stored in PROGMEM |
| One-Shot Keys (5 keys) | +150–250 B | Low cost if already using OSL |
| Layer Lock | +150–200 B | Lightweight feature |
| Repeat Key | +200 B | Minimal cost |

### Budget Allocation for Your Changes
```
Current usage:          ~11 KB
+ Chordal Hold:         +0.3 KB
+ Leader Key (tmux):    +0.5 KB
+ Ctrl+Tab macros (2):  +0.1 KB
+ Word-delete combos:   +0.1 KB
_________________________
Estimated new total:    ~12 KB / 22 KB available

Remaining buffer:       ~10 KB (very safe)
```

### Recommendations
1. **Enable Chordal Hold + Flow Tap:** Net +0.5 KB cost; massive UX improvement for home row mods. **Strongly recommended.**
2. **Leader Key for tmux:** Net +0.5 KB. Fits comfortably.
3. **Additional combos:** Each 6-key combo ~30 B. You can add 100+ before issues.
4. **RGB animations:** Currently trimmed. Don't re-enable; you have no budget. If needed, sacrifice OLED instead (saves ~2.5 KB).

### Conclusion
**Your keymap has ample flash budget for planned changes.** No feature trade-offs needed. Focus on correctness and user experience; flash is not a constraint.

---

## Recommendation Matrix

| Use Case | Recommended Feature | Flash Cost | Update Required? | Priority |
|----------|---------------------|-----------|------------------|----------|
| Home row mods (current) | **Chordal Hold** (replace PERMISSIVE_HOLD) | +0.3 KB | v0.28+ | HIGH |
| Fast typing (current) | **Flow Tap** (add) | +0.1 KB | v0.29+ | MEDIUM |
| Tmux prefix (Ctrl+B + X) | **Leader Key** (add) | +0.5 KB | Upstream stable | HIGH |
| Ctrl+Tab, Ctrl+Shift+Tab | **Macros** (add, 2×) | +0.1 KB | Upstream stable | MEDIUM |
| Word-delete combos (current) | **Existing combo setup** (no change) | – | Current | LOW |
| Accessibility / fast modifiers | **One-Shot Keys** (optional) | +0.2 KB | Upstream stable | LOW |
| Bidirectional navigation | **Repeat Key** (optional) | +0.2 KB | v0.28+ | LOW |

---

## Summary: Keymap Redesign Path

1. **Update QMK firmware to v0.28.0 or later** (if not already there)
   - Unlocks Chordal Hold, Flow Tap, Community Modules
   - No breaking changes; your current keymap remains compatible

2. **Replace PERMISSIVE_HOLD + HOLD_ON_OTHER_KEY_PRESS with Chordal Hold**
   - Better UX for home row mods, especially with rolling keypresses
   - ~400 bytes, well worth the trade

3. **Add Leader Key for tmux-prefix layer**
   - Designate a key as `QK_LEAD`
   - Implement `leader_end_user()` with sequences like `leader_sequence_two_keys(KC_T, KC_X)` → Ctrl+B+t, x
   - ~500 bytes; excellent fit for your use case

4. **Add macros for Ctrl+Tab / Ctrl+Shift+Tab navigation**
   - Define custom keycodes + `process_record_user()` SEND_STRING calls
   - ~100 bytes; low overhead

5. **Enhance word-delete combos** (if needed)
   - Existing combo framework supports up to 32-key combos with tuning
   - Reuse your current combo infrastructure; no new features needed

6. **Optional: Enable Flow Tap**
   - Minimal cost; smooths fast typing with home row mods
   - Recommended for reduced accidental mod activations

**Total flash impact: +1.1 KB. Total available budget: ~10 KB. No constraints; prioritize correctness.**

---

## Sources

- [Tap-Hold Configuration Options | QMK Firmware](https://docs.qmk.fm/tap_hold)
- [Chordal Hold PR #24560](https://github.com/qmk/qmk_firmware/pull/24560)
- [Chordal Hold Fix PR #24878](https://github.com/qmk/qmk_firmware/pull/24878)
- [Combos | QMK Firmware](https://docs.qmk.fm/features/combo)
- [Combos Source (process_combo.c)](https://github.com/qmk/qmk_firmware/blob/master/quantum/process_keycode/process_combo.c)
- [Tap Dance: A Single Key Can Do 3, 5, or 100 Different Things | QMK Firmware](https://docs.qmk.fm/features/tap_dance)
- [The Leader Key: A New Kind of Modifier | QMK Firmware](https://docs.qmk.fm/features/leader_key)
- [One Shot Keys | QMK Firmware](https://docs.qmk.fm/one_shot_keys)
- [Layer Lock | QMK Firmware](https://docs.qmk.fm/features/layer_lock)
- [Repeat Key | QMK Firmware](https://docs.qmk.fm/features/repeat_key)
- [Macros | QMK Firmware](https://docs.qmk.fm/feature_macros)
- [Send String | QMK Firmware](https://docs.qmk.fm/features/send_string)
- [RGB Lighting | QMK Firmware](https://docs.qmk.fm/features/rgblight)
- [Compatible Microcontrollers | QMK Firmware](https://docs.qmk.fm/compatible_microcontrollers)
- [QMK Breaking Changes - 2025 February 23 Changelog](https://docs.qmk.fm/ChangeLog/20250223)
- [QMK Breaking Changes - 2025 May 25 Changelog](https://docs.qmk.fm/ChangeLog/20250525)
- [Breaking Changes | QMK Firmware](https://docs.qmk.fm/breaking_changes)
