#include QMK_KEYBOARD_H

enum layer_names {
  _QWERTY,
  _NUMBER,
  _SYMBOL,
  _SYMBOL2,
  _NAV,
  _FUNC,
  _DEV,
  _UTIL,
  _MEDIA,
};

// Fancy zones keycodes
#define FZ_EXT G(S(KC_UP)) // Expand window vertically
#define FZ_LFT G(KC_LEFT) // Move window to previous area
#define FZ_RGT G(KC_RIGHT) // Move window to next area
#define VD_NXT G(C(KC_LEFT)) // Move window to previous area
#define VD_PRV G(C(KC_RIGHT)) // Move window to next area

// Word navigation and selection keycodes
#define C_LFT C(KC_LEFT) // Move to previous word
#define C_RGT C(KC_RIGHT) // Move to next word
#define C_UP C(KC_HOME)
#define C_DWN C(KC_END)
#define CS_LFT C(S(KC_LEFT)) // Select previous word
#define CS_RGT C(S(KC_RIGHT)) // Select next word
#define CS_UP C(S(KC_UP))
#define CS_DWN C(S(KC_DOWN))
#define S_PGUP S(KC_PGUP)
#define S_PGDW S(KC_PGDN)
#define LN_UP A(KC_UP) // Move line up
#define LN_DN A(KC_DOWN) // Move line down

// Windows management keycodes
#define WD_CLO C(KC_F4) // Close document or tab
#define WI_CLO A(KC_F4) // Close window

// Shorthand macros and keycodes
#define SPC_TE LT(_UTIL,KC_SPC) // Space on tap Test layer on hold
#define DV_TRM C(KC_GRV) // VS Code terminal
#define DV_NTRM C(S(KC_GRV)) // VS Code new terminal
#define DV_CTRM C(KC_PGDN) // VS Code cycle terminal
#define DV_BLD C(S(KC_B)) // VS build
#define DV_BRKP KC_F9 // VS breakpoint
#define DV_SOUT S(KC_F11) // VS Step Out
#define DV_SIN KC_F11 // VS Step Into
#define DV_SOVR KC_F10 // VS Step Over
#define DV_IMPL C(KC_F12) // VS Go to Implementation
#define DV_NXER C(S(KC_F12)) // VS Next error
#define DV_NVFW C(S(KC_MINUS)) // VS Navigate forward
#define DV_NVBW C(KC_MINUS) // VS Navigate backwards
#define CS_P C(S(KC_P))
#define CS_V C(S(KC_V))
#define CS_F C(S(KC_F))
#define CS_S C(S(KC_S))

// Left-hand home row mods
#define HM_A LGUI_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LCTL_T(KC_D)
#define HM_F LSFT_T(KC_F)

// Right-hand home row mods
#define HM_J RSFT_T(KC_J)
#define HM_K RCTL_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_SCLN RGUI_T(KC_SCLN)

#define L_SPC LT(_NUMBER, KC_SPACE)
#define L_TAB LT(_SYMBOL, KC_TAB)
#define L_ESC LT(_SYMBOL2, KC_ESCAPE)
#define L_ENT LT(_DEV, KC_ENTER)
#define L_DEL LT(_MEDIA, KC_DEL)
#define L_BK LT(_FUNC, KC_BSPC)
#define L_Z LT(_NAV, KC_Z)
#define L_QUOT LT(_UTIL, KC_QUOT)

enum custom_keycodes {
  DV_ARR = SAFE_RANGE, // Print C# lambda arrow
  DV_CMNT, // Comment line
  DV_UNCMT, // Uncomment line
  DV_SELLN, // Select line
  DV_REFS,  // Find references
  WI_MAX,  // Maximize window
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX,    HM_A,    HM_S,    HM_D,    HM_F,    KC_G,   KC_H,    HM_J,    HM_K,    HM_L, HM_SCLN, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX,     L_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_N,    KC_M, KC_COMM,  KC_DOT,  L_QUOT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX,   L_ESC,   L_TAB,   L_SPC, XXXXXXX,XXXXXXX,   L_ENT,    L_BK,   L_DEL, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_NUMBER] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, C(KC_Z), C(KC_X), C(KC_C), C(KC_V),RCS(KC_V),XXXXXXX,    KC_7,    KC_8,    KC_9, KC_ASTR, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_EQL,    KC_4,    KC_5,    KC_6, KC_PLUS, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN2, KC_BTN1, XXXXXXX,XXXXXXX,    KC_1,    KC_2,    KC_3, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,KC_MINUS,  KC_DOT,    KC_0, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_SYMBOL] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, C(KC_Z), C(KC_X), C(KC_C), C(KC_V),RCS(KC_V),XXXXXXX, KC_SLSH, KC_LBRC, KC_RBRC, KC_BSLS, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_AMPR,KC_MINUS, KC_LPRN, KC_RPRN, KC_EXLM, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PIPE, KC_UNDS, KC_LCBR, KC_RCBR, KC_QUES, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,  KC_EQL,   KC_LT,   KC_GT, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_SYMBOL2] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, C(KC_Z), C(KC_X), C(KC_C), C(KC_V),RCS(KC_V),XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, XXXXXXX,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_GRV, KC_CIRC, KC_TILD,  DV_ARR, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_NAV] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, C(KC_Z), C(KC_X), C(KC_C), C(KC_V),RCS(KC_V),XXXXXXX, KC_HOME,   KC_UP,  KC_END, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_PGUP, KC_LEFT, KC_DOWN,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGDN,   C_LFT, XXXXXXX,   C_RGT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_DEV] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX,  DV_TRM, DV_NVBW, DV_NVFW, DV_CMNT,DV_UNCMT,RCS(KC_V), C(KC_V), C(KC_C), C(KC_X), C(KC_Z), XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, DV_BRKP, DV_SOUT,  DV_SIN, DV_SOVR,  DV_BLD, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, DV_NXER, XXXXXXX, DV_REFS, DV_IMPL,   KC_F5, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

    [_FUNC] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_PSCR,RCS(KC_V), C(KC_V), C(KC_C), C(KC_X), C(KC_Z), XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX,   KC_F5,   KC_F6,   KC_F7,   KC_F8, KC_PAUS, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX,   KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

     [_MEDIA] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, XXXXXXX, KC_WH_D, KC_MS_U, KC_WH_U, KC_MPRV,RCS(KC_V), C(KC_V), C(KC_C), C(KC_X), C(KC_Z), XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_VOLU, KC_MS_L, KC_MS_D, KC_MS_R, KC_MPLY, XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_VOLD, XXXXXXX, KC_BTN2, KC_BTN1, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  ),

   [_UTIL] = LAYOUT_ortho_4x12(
  //,----------------------------------------------------------------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_SPI, RGB_SPD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD,RGB_RMOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+----------------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, RGB_VAI, RGB_VAD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+--------+--------+--------+--------+-------+--------+--------+--------+--------+--------|
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {
      case DV_CMNT:
        SEND_STRING(SS_LCTL("kc"));
        break;
      case DV_UNCMT:
        SEND_STRING(SS_LCTL("ku"));
        break;
      case DV_REFS:
        SEND_STRING(SS_LCTL("kr"));
        break;
      case DV_SELLN:
        tap_code16(KC_HOME);
        tap_code16(KC_HOME);
        tap_code16(S(KC_END));
        break;
      case WI_MAX:
        tap_code16(A(KC_SPC));
        tap_code16(S(KC_X));
        break;
    }
  }

  return true;
};

