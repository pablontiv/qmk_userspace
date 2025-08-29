#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// Home row modifiers (refer to keymap.c for primary definitions)
#define HM_A LGUI_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LCTL_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J RSFT_T(KC_J)
#define HM_K RCTL_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_SCLN RGUI_T(KC_SCLN)

// Combo definitions
const uint16_t PROGMEM combo_esc[] = {HM_J, HM_K, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {HM_D, HM_F, COMBO_END};
const uint16_t PROGMEM del_word_r[] = {HM_K, HM_L, COMBO_END};
const uint16_t PROGMEM del_word_l[] = {HM_S, HM_D, COMBO_END};
const uint16_t PROGMEM combo_enter[] = {KC_COMM, KC_DOT, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_esc, KC_ESC),
    COMBO(combo_tab, KC_TAB),
    COMBO(del_word_r, C(KC_BSPC)),
    COMBO(del_word_l, C(KC_DEL)),
    COMBO(combo_enter, KC_ENTER),
};