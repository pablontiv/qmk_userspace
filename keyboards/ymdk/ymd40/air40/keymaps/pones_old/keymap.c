#include QMK_KEYBOARD_H

enum layer_names {
  _QWERTY,
  _NUMBER,
  _SYMBOL,
  _DEV,
  _NAV,
  _UTIL,
  _GAME
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

enum custom_keycodes {
  DV_ARR = SAFE_RANGE, // Print C# lambda arrow
  DV_CMNT, // Comment line
  DV_UNCMT, // Uncomment line
  DV_SELLN, // Select line
  DV_REFS,  // Find references
  WI_MAX,  // Maximize window
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  | Nav  |Number|    Space    |Symbol|      | Alt  | GUI  | Ctrl |
 * `-----------------------------------------------------------------------------------'
 */
  [_QWERTY] = LAYOUT_ortho_4x12(
    QK_GESC,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
    KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  SC_SENT,
    KC_LCTL,  KC_LGUI,  KC_LALT,  TT(3),    TT(1),    SPC_TE,   KC_SPC,   MO(2),    MO(4),    KC_LALT,  KC_LGUI,  KC_LCTL
  ),
/* Number
 * ,-----------------------------------------------------------------------------------.
 * | Qwer |  F1  |  F2  |  F3  |  F4  |      |      |   7  |   8  |   9  |   =  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  F5  |  F6  |  F7  |  F8  |      |   *  |   4  |   5  |   6  |   +  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F9  |  F10 |  F11 |  F12 |      |   /  |   1  |   2  |   3  |   -  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |   0  |   0  |   .  |      |      |
 * `-----------------------------------------------------------------------------------'
 */
  [_NUMBER] = LAYOUT_ortho_4x12(
    TO(0),    KC_F1,    KC_F2,    KC_F3,    KC_F4,    XXXXXXX,  XXXXXXX,  KC_7,     KC_8,     KC_9,     KC_EQUAL, KC_BSPC,
    XXXXXXX,  KC_F5,    KC_F6,    KC_F7,    KC_F8,    XXXXXXX,  XXXXXXX,  KC_4,     KC_5,     KC_6,     KC_PLUS,  XXXXXXX,
    XXXXXXX,  KC_F9,    KC_F10,   KC_F11,   KC_F12,   XXXXXXX,  KC_SLASH, KC_1,     KC_2,     KC_3,     KC_MINUS, KC_ENT,
    KC_WAKE,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  SPC_TE,   XXXXXXX,  KC_0,     KC_0,     KC_DOT,   XXXXXXX,  XXXXXXX
  ),
/* Symbol
 * ,-----------------------------------------------------------------------------------.
 * |      |   `  |   <  |   >  |      |      |      |  =>  |   [  |   ]  |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |   !  |   -  |   +  |   =  |   ~  |   &  |   @  |   (  |   )  |   #  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   ?  |   /  |   *  |      |   ^  |   |  |   $  |   {  |   }  |   %  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |    Space    |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
  [_SYMBOL] = LAYOUT_ortho_4x12(
    XXXXXXX,  KC_GRV,   KC_LT,    KC_GT,    XXXXXXX,  XXXXXXX,  XXXXXXX,  DV_ARR,   KC_LBRC,  KC_RBRC,  XXXXXXX,  KC_DEL,
    XXXXXXX,  KC_EXLM,  KC_MINUS, KC_PLUS,  KC_EQUAL, KC_TILDE, KC_AMPR,  KC_AT,    KC_LPRN,  KC_RPRN,  KC_HASH,  XXXXXXX,
    XXXXXXX,  KC_QUES,  KC_SLASH, KC_ASTR,  XXXXXXX,  KC_CIRC,  KC_PIPE,  KC_DLR,   KC_LCBR,  KC_RCBR,  KC_PERC,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  SPC_TE,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),
  [_DEV] = LAYOUT_ortho_4x12(
    TO(0),    DV_TRM,   DV_NTRM,  DV_CTRM,  XXXXXXX,  XXXXXXX,  XXXXXXX,  CS_V,     CS_P,     CS_F,     CS_S,     XXXXXXX,
    XXXXXXX,  DV_BRKP,  DV_SOUT,  DV_SIN,   DV_SOVR,  DV_BLD,   XXXXXXX,  DV_NVBW,  DV_NVFW,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  DV_NXER,  XXXXXXX,  DV_REFS,  DV_IMPL,  KC_F5,    XXXXXXX,  DV_CMNT,  DV_UNCMT, XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  SPC_TE,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),
  [_NAV] = LAYOUT_ortho_4x12(
    TO(0),    XXXXXXX,  S_PGUP,   S_PGDW,   XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_HOME,  KC_PGUP,  KC_PGDN,  KC_END,   XXXXXXX,
    XXXXXXX,  C_LFT,    C_UP,     C_DWN,    C_RGT,    XXXXXXX,  XXXXXXX,  KC_LEFT,  KC_UP,    KC_DOWN,  KC_RIGHT, XXXXXXX,
    KC_LSFT,  CS_LFT,   CS_UP,    CS_DWN,   CS_RGT,   DV_SELLN, XXXXXXX,  XXXXXXX,  LN_UP,    LN_DN,    XXXXXXX,  XXXXXXX,
    KC_LCTL,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  SPC_TE,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),
  [_UTIL] = LAYOUT_ortho_4x12(
    AS_RPT,   QK_BOOT,  XXXXXXX,  XXXXXXX,  WD_CLO,   WI_CLO,   XXXXXXX,  WI_MAX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  TO(6),
    AS_UP,    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  FZ_EXT,   FZ_LFT,   VD_NXT,   VD_PRV,   FZ_RGT,   XXXXXXX,
    AS_DOWN,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MPLY,  KC_MPRV,  KC_VOLU,  KC_VOLD,  KC_MNXT,  XXXXXXX,
    AS_TOGG,  XXXXXXX,  XXXXXXX,  KC_LGUI,  KC_LALT,  XXXXXXX,  XXXXXXX,  KC_LCTL,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),
  [_GAME] = LAYOUT_ortho_4x12(
    QK_GESC,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     TO(0),
    KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  SC_SENT,
    KC_LCTL,  KC_LGUI,  KC_LALT,  XXXXXXX,  XXXXXXX,  KC_SPC,   KC_SPC,   XXXXXXX,  XXXXXXX,  KC_LALT,  KC_LGUI,  KC_LCTL
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

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case DV_ARR:
      return true;
    default:
      return false;
  }
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch(keycode) {
    case KC_DOT:
      (shifted) ? tap_code16(KC_UNDS) : tap_code16(KC_DOT);
      break;
    case KC_SLSH:
      (shifted) ? tap_code16(KC_BSLS) : tap_code16(KC_SLSH);
      break;
    case KC_COMM:
      (shifted) ? tap_code16(KC_MINUS) : SEND_STRING(", ");
      break;
    case DV_ARR:
      (shifted) ? SEND_STRING("/>") : SEND_STRING("=>");
      break;
    case KC_GRV:
      (shifted) ? SEND_STRING("``` ") : tap_code16(KC_GRV);
      break;
    default:
      if (shifted) {
        add_weak_mods(MOD_BIT(KC_LSFT));
      }
      // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
      register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch(keycode) {
    case KC_DOT:
    case KC_SLSH:
    case KC_COMM:
    case DV_ARR:
    case KC_GRV:
      break;
    default:
      // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
      // The IS_RETRO check isn't really necessary here, always using
      // keycode & 0xFF would be fine.
      unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
  }
}

// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

//   switch(get_highest_layer(layer_state|default_layer_state)) {
//     case 1:
//       rgb_matrix_set_color(led_min, RGB_RED);
//       break;
//     case 2:
//       rgb_matrix_set_color(led_min, RGB_GREEN);
//       break;
//     case 3:
//       rgb_matrix_set_color(led_min, RGB_BLUE);
//       break;
//     case 4:
//       rgb_matrix_set_color(led_min, RGB_YELLOW);
//       break;
//     case 5:
//       rgb_matrix_set_color(led_min, RGB_PURPLE);
//       break;
//     case 6:
//       rgb_matrix_set_color(led_min, RGB_WHITE);
//       break;
//   }

//   return false;
// }
