#include QMK_KEYBOARD_H
#include "features/layers.h"
#include "features/tap_dance.h"
#include "features/custom_keycodes.h"
#include "features/macros.h"
#include "features/oled.h"

// Left-hand home row modifiers (GUI/Alt/Ctrl/Shift on A/S/D/F)
#define HM_A LGUI_T(KC_A)       // A key: tap for 'a', hold for GUI/Win key
#define HM_S LALT_T(KC_S)       // S key: tap for 's', hold for Alt
#define HM_D LCTL_T(KC_D)       // D key: tap for 'd', hold for Ctrl
#define HM_F LSFT_T(KC_F)       // F key: tap for 'f', hold for Shift

// Right-hand home row modifiers (Shift/Ctrl/Alt/GUI on J/K/L/;)
#define HM_J RSFT_T(KC_J)       // J key: tap for 'j', hold for Shift
#define HM_K RCTL_T(KC_K)       // K key: tap for 'k', hold for Ctrl
#define HM_L LALT_T(KC_L)       // L key: tap for 'l', hold for Alt
#define HM_SCLN RGUI_T(KC_SCLN) // ; key: tap for ';', hold for GUI/Win key

// Layer-tap definitions (tap for key, hold to access layer)
#define L_ESC LT(_NAV, KC_ESC)         // Escape: tap Esc, hold NAV — left thumb holds, right-hand arrows stay cross-handed
#define L_TAB LT(_SYMBOL, KC_TAB)      // Tab: tap for Tab, hold for SYMBOL layer
#define L_SPC LT(_NUMBER, KC_SPACE)    // Space: tap space, hold numbers
#define L_ENT LT(_MOVE, KC_ENTER)      // Enter: tap enter, hold MOVE layer (KVM, Ctrl+Tab) — right thumb keeps left-hand MOVE keys cross-handed
#define L_BK  LT(_MOUSE, KC_BSPC)      // Backspace: tap bspc, hold MOUSE layer (WASD move, thumb clicks)
#define L_DEL KC_DEL                    // Delete (plain; _MEDIA layer removed)

// Tap dance aliases (max 7 characters)
#define TD_JBR TD(TD_J_BRACES)    // { | 2x=} | hold={}
#define TD_KPR TD(TD_K_PARENS)    // ( | 2x=) | hold=()
#define TD_SAN TD(TD_SC_ANGLES)   // < | 2x=> | hold=<>
#define TD_LBK TD(TD_L_BRACKETS)  // [ | 2x=] | hold=[]
#define TD_EXQ TD(TD_EX_QUEST)    // ? | hold=!
#define TD_MNU TD(TD_MN_UNDER)    // - | hold=_
#define TD_AMP TD(TD_AM_PIPE)     // & | hold=|
#define TD_SLB TD(TD_SL_BACK)     // / | hold=backslash

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,    HM_A,    HM_S,    HM_D,    HM_F,    KC_G,                         KC_H,    HM_J,    HM_K,    HM_L, HM_SCLN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_QUOT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            L_ESC,   L_TAB,   L_SPC,      L_ENT,    L_BK,   L_DEL
                                      //`--------------------------'  `--------------------------'
  ),

    [_NUMBER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4, XXXXXXX,                      XXXXXXX,    KC_7,    KC_8,    KC_9, KC_ASTR, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   KC_F5,   KC_F6,   KC_F7,   KC_F8, XXXXXXX,                       KC_EQL,    KC_4,    KC_5,    KC_6, KC_PLUS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX,                      XXXXXXX,    KC_1,    KC_2,    KC_3, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,   KC_MINUS,  KC_DOT,    KC_0
                                      //`--------------------------'  `--------------------------'
  ),


    [_SYMBOL] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_GRV,  TD_AMP, KC_HASH,  TD_SLB, KC_CIRC, XXXXXXX,
      XXXXXXX, KC_PSCR, C(KC_C), C(KC_V),C(S(KC_V)), XXXXXXX,                     KC_AT,  TD_JBR,  TD_KPR,  TD_SAN,  TD_LBK, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD, TD_MNU, KC_DLR, KC_PERC,  TD_EXQ, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),

    [_MOVE] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, HRD_PSPC, HRD_PTAB, C(S(KC_TAB)), KVM_PREV,               KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, XXXXXXX,
      XXXXXXX, XXXXXXX, HRD_NSPC, HRD_NTAB, C(KC_TAB), KVM_NEXT,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),

    [_NAV] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_HOME,   KC_UP,  KC_END, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_PGUP, KC_LEFT, KC_DOWN,KC_RIGHT, KC_PGDN, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,A(KC_LEFT),XXXXXXX,A(KC_RGHT),XXXXXXX, XXXXXXX,
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
  ),

    [_MOUSE] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX, MS_UP,   XXXXXXX, MS_WHLU,                      KC_INS, XXXXXXX, KC_CAPS, DB_TOGG, QK_BOOT, XXXXXXX,
      XXXXXXX, XXXXXXX, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          XXXXXXX, MS_BTN2, MS_BTN1,    XXXXXXX, XXXXXXX, XXXXXXX
  ),
};

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

// Combo definitions (QMK introspection requires these in keymap.c)
const uint16_t PROGMEM combo_capsword[] = {HM_F, HM_J, COMBO_END};    // F+J

combo_t key_combos[] = {
    COMBO(combo_capsword, CW_TOGG),      // F+J  = Caps Word
};

// Tap dance function declarations (defined in tap_dance.c)
void td_j_finished(tap_dance_state_t *state, void *user_data);
void td_k_finished(tap_dance_state_t *state, void *user_data);
void td_sc_finished(tap_dance_state_t *state, void *user_data);
void td_l_finished(tap_dance_state_t *state, void *user_data);
void td_ex_finished(tap_dance_state_t *state, void *user_data);
void td_mn_finished(tap_dance_state_t *state, void *user_data);
void td_am_finished(tap_dance_state_t *state, void *user_data);
void td_sl_finished(tap_dance_state_t *state, void *user_data);

// Tap dance actions (different actions based on number of taps)
tap_dance_action_t tap_dance_actions[] = {
    [TD_J_BRACES] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_j_finished, NULL),     // { | {} | }
    [TD_K_PARENS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_k_finished, NULL),     // ( | () | ("") | () => {} | )
    [TD_SC_ANGLES] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sc_finished, NULL),  // < | <> | <=
    [TD_L_BRACKETS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_l_finished, NULL),   // [ | [] | [0] | ]
    [TD_EX_QUEST] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ex_finished, NULL),    // ? | hold=!
    [TD_MN_UNDER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_mn_finished, NULL),    // - | hold=_
    [TD_AM_PIPE]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_am_finished, NULL),    // & | hold=|
    [TD_SL_BACK]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sl_finished, NULL),    // / | hold=backslash
};
