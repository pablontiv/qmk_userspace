#include QMK_KEYBOARD_H
#include "macros.h"
#include "tap_dance.h"
#include "custom_keycodes.h"
#include "layers.h"

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_herdr(keycode, record)) {
    return false;
  }
  if (record->event.pressed) {
    switch (keycode) {
      case KVM_PREV:
        tap_code16(KC_RCTL);
        wait_ms(50);
        tap_code16(KC_RCTL);
        wait_ms(50);
        tap_code16(KC_PGUP);
        break;
      case KVM_NEXT:
        tap_code16(KC_RCTL);
        wait_ms(50);
        tap_code16(KC_RCTL);
        wait_ms(50);
        tap_code16(KC_PGDN);
        break;
    }
  }

  return true;
}

// Per-key tapping term optimization
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_J_BRACES:
        case TD_K_PARENS:
        case TD_L_BRACKETS:
        case TD_SC_ANGLES:  return 180;
        case TD_EX_QUEST:
        case TD_SL_BACK:
        case TD_MN_UNDER:
        case TD_AM_PIPE: return 120;
        case TD_TAB_CTRLTAB: return 140;
        default: return TAPPING_TERM;
    }
}
