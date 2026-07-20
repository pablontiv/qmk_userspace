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

// Tap dance actions array is now defined in keymap.c
