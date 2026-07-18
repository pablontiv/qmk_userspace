#include QMK_KEYBOARD_H
#include "tap_dance.h"
#include "action_util.h"

// Tap dance functions
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


void td_sl_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                tap_code16(KC_BSLS);   // Hold = backslash
            } else {
                tap_code16(KC_SLSH);   // 1 tap = /
            }
            break;
        case 2:
            SEND_STRING("//");              // 2 taps = //
            break;
    }
}

void td_mn_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                tap_code16(KC_UNDS);   // Hold = _
            } else {
                tap_code16(KC_MINS);   // 1 tap = -
            }
            break;
        case 2:
            SEND_STRING("--");              // 2 taps = --
            break;
        case 3:
            SEND_STRING("=>");              // 3 taps = =>
            break;
    }
}

void td_am_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                tap_code16(KC_PIPE);   // Hold = |
            } else {
                tap_code16(KC_AMPR);   // 1 tap = &
            }
            break;
        case 2:
            SEND_STRING("&&");              // 2 taps = &&
            break;
        case 3:
            SEND_STRING("||");              // 3 taps = ||
            break;
    }
}

void td_ex_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                tap_code16(KC_EXLM);   // Hold = !
            } else {
                tap_code16(KC_QUES);   // 1 tap = ?
            }
            break;
        case 2:
            SEND_STRING("!=");              // 2 taps = !=
            break;
    }
}

static bool tab_layer_activated = false;

void td_tab_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                layer_on(_SYMBOL);
                tab_layer_activated = true;
            } else {
                tap_code(KC_TAB);
            }
            break;
        case 2:
            tap_code16(C(KC_TAB));
            break;
        case 3:
            tap_code16(C(S(KC_TAB)));
            break;
    }
}

void td_tab_reset(tap_dance_state_t *state, void *user_data) {
    if (tab_layer_activated) {
        layer_off(_SYMBOL);
        tab_layer_activated = false;
    }
}

// Tap dance actions array is now defined in keymap.c
