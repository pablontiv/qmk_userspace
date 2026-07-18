#include QMK_KEYBOARD_H
#include "macros.h"
#include "tap_dance.h"
#include "custom_keycodes.h"


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
