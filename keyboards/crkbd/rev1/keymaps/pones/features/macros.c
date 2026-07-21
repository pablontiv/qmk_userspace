#include QMK_KEYBOARD_H
#include "print.h"
#include "macros.h"
#include "tap_dance.h"
#include "custom_keycodes.h"
#include "layers.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (debug_enable) {
    uprintf("KL:t%lu,k0x%04X,r%u,c%u,m0x%02X,%c\n",
            timer_read32(), keycode,
            record->event.key.row, record->event.key.col,
            get_mods() | get_oneshot_mods(),
            record->event.pressed ? 'd' : 'u');
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
      case HRD_NTAB: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "n"); break; // herdr next tab
      case HRD_PTAB: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "p"); break; // herdr prev tab
      case HRD_NSPC: SEND_STRING(SS_LCTL("b") SS_DELAY(10) "."); break; // herdr next space
      case HRD_PSPC: SEND_STRING(SS_LCTL("b") SS_DELAY(10) ","); break; // herdr prev space
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
        case TD_SC_ANGLES:
        case TD_EX_QUEST:
        case TD_MN_UNDER:
        case TD_AM_PIPE:
        case TD_SL_BACK:   return 180;
        default: return TAPPING_TERM;
    }
}
