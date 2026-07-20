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

// Single-symbol tap/hold: tap = primary, hold = shifted secondary.
void td_ex_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    if (state->pressed) { tap_code16(KC_QUES); } // hold = ?
    else                { tap_code16(KC_EXLM); } // tap  = !
}

void td_mn_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    if (state->pressed) { tap_code16(KC_UNDS); } // hold = _
    else                { tap_code16(KC_MINS); } // tap  = -
}

void td_am_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    if (state->pressed) { tap_code16(KC_PIPE); } // hold = |
    else                { tap_code16(KC_AMPR); } // tap  = &
}

// Tap dance actions array is now defined in keymap.c
