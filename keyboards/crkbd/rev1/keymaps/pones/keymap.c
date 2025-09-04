#include QMK_KEYBOARD_H
#include "features/tap_dance.h"
#include "features/custom_keycodes.h"
#include "features/macros.h"
#include "features/oled.h"

// VS Code shortcuts (only the ones actually used)
#define DV_TRM C(KC_GRV)        // Toggle integrated terminal
#define CS_P C(S(KC_P))         // Command palette
#define CS_V C(S(KC_V))         // Paste from clipboard history
#define CS_F C(S(KC_F))         // Find in files

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

// Layer names and their purposes
enum layer_names {
  _QWERTY,      // Base QWERTY layer with home row mods
  _NUMBER,      // Number pad and arithmetic operators
  _SYMBOL,      // Optimized symbols layer with tap dance functionality
  _NAV,         // Navigation keys (arrows, page up/down, word movement)
};


// Layer-tap definitions (tap for key, hold to access layer)
#define L_SPC LT(_NUMBER, KC_SPACE)     // Space: tap for space, hold for numbers layer
#define L_TAB LT(_SYMBOL, KC_TAB)   // Tab: tap for tab, hold for new symbols layer
#define L_ESC LT(_NAV, KC_ESCAPE)       // Escape: tap for escape, hold for navigation layer
#define L_ENT LT(_SYMBOL, KC_ENTER)        // Enter: tap for enter, hold for symbols layer
#define L_DEL KC_DEL                    // Delete: now just delete (layer access removed)
#define L_BK LT(_NAV, KC_BSPC)         // Backspace: tap for backspace, hold for navigation layer

// Tap dance aliases (max 7 characters)
#define TD_JBR TD(TD_J_BRACES)    // { | {} | }
#define TD_KPR TD(TD_K_PARENS)    // ( | () | ("") | () => {} | )
#define TD_SAN TD(TD_SC_ANGLES)   // < | <> | <= | >
#define TD_LBK TD(TD_L_BRACKETS)  // [ | [] | [0] | ]
#define TD_SLB TD(TD_SL_BACK)     // / | // | backslash
#define TD_MNU TD(TD_MN_UNDER)    // - | -- | => | _
#define TD_AMP TD(TD_AM_PIPE)     // & | && | || | |
#define TD_EXQ TD(TD_EX_QUEST)    // ! | != | ?

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
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, C(KC_B), XXXXXXX,DV_UNCMT, DV_CMNT, XXXXXXX,                       KC_GRV,  TD_AMP, KC_HASH,  TD_SLB, KC_CIRC, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_PSCR,  DV_TRM,    CS_F,    CS_V,    CS_P,                        KC_AT,  TD_JBR,  TD_KPR,  TD_SAN,  TD_LBK, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD,  TD_MNU,  KC_DLR, KC_PERC,  TD_EXQ, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

    [_NAV] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX,KVM_PREV,KVM_NEXT, XXXXXXX,                      XXXXXXX, KC_HOME,   KC_UP,  KC_END,  KC_INS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_PGUP, KC_LEFT, KC_DOWN,KC_RIGHT, KC_PGDN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, UG_SPDD, UG_SPDU, XXXXXXX, UG_NEXT, UG_TOGG,                      KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,   DV_SELLN, XXXXXXX, KC_CAPS
                                      //`--------------------------'  `--------------------------'
  ),
};

// Combo definitions (required in keymap.c for QMK introspection system)
const uint16_t PROGMEM combo_esc[] = {HM_J, HM_K, COMBO_END};        // J+K = Escape
const uint16_t PROGMEM combo_tab[] = {HM_D, HM_F, COMBO_END};        // D+F = Tab
const uint16_t PROGMEM del_word_r[] = {HM_K, HM_L, COMBO_END};       // K+L = Delete word right
const uint16_t PROGMEM del_word_l[] = {HM_S, HM_D, COMBO_END};       // S+D = Delete word left
const uint16_t PROGMEM combo_enter[] = {KC_COMM, KC_DOT, COMBO_END}; // ,+. = Enter

combo_t key_combos[] = {
    COMBO(combo_esc, KC_ESC),           // J+K combo produces Escape
    COMBO(combo_tab, KC_TAB),           // D+F combo produces Tab
    COMBO(del_word_r, C(KC_BSPC)),      // K+L combo deletes word to the right
    COMBO(del_word_l, C(KC_DEL)),       // S+D combo deletes word to the left
    COMBO(combo_enter, KC_ENTER),       // ,+. combo produces Enter
};

// Tap dance function declarations (defined in tap_dance.c)
void td_j_finished(tap_dance_state_t *state, void *user_data);
void td_j_reset(tap_dance_state_t *state, void *user_data);
void td_k_finished(tap_dance_state_t *state, void *user_data);
void td_k_reset(tap_dance_state_t *state, void *user_data);
void td_sc_finished(tap_dance_state_t *state, void *user_data);
void td_sc_reset(tap_dance_state_t *state, void *user_data);
void td_l_finished(tap_dance_state_t *state, void *user_data);
void td_l_reset(tap_dance_state_t *state, void *user_data);
void td_sl_finished(tap_dance_state_t *state, void *user_data);
void td_sl_reset(tap_dance_state_t *state, void *user_data);
void td_mn_finished(tap_dance_state_t *state, void *user_data);
void td_mn_reset(tap_dance_state_t *state, void *user_data);
void td_am_finished(tap_dance_state_t *state, void *user_data);
void td_am_reset(tap_dance_state_t *state, void *user_data);
void td_ex_finished(tap_dance_state_t *state, void *user_data);
void td_ex_reset(tap_dance_state_t *state, void *user_data);

// Tap dance actions (different actions based on number of taps)
tap_dance_action_t tap_dance_actions[] = {
    [TD_J_BRACES] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_j_finished, NULL),     // { | {} | }
    [TD_K_PARENS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_k_finished, NULL),     // ( | () | ("") | () => {} | )
    [TD_SC_ANGLES] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sc_finished, NULL),  // < | <> | <=
    [TD_L_BRACKETS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_l_finished, NULL),   // [ | [] | [0] | ]
    [TD_SL_BACK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sl_finished, NULL),    // / | // | backslash
    [TD_MN_UNDER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_mn_finished, NULL),   // - | -- | => | _
    [TD_AM_PIPE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_am_finished, NULL),    // & | && | || | |
    [TD_EX_QUEST] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ex_finished, NULL),   // ! | != | ?
};
