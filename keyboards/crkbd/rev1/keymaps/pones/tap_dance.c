#include QMK_KEYBOARD_H
#include "tap_dance.h"

// Tap dance functions
void td_j_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                register_code16(KC_RCBR);  // Hold = }
            } else {
                register_code16(KC_LCBR);  // 1 tap = {
            }
            break;
        case 2:
            SEND_STRING("{}");
            tap_code(KC_LEFT);              // 2 taps = {}←
            break;
        case 3:
            register_code16(KC_RCBR);       // 3 taps = }
            break;
    }
}

void td_j_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                unregister_code16(KC_RCBR);
            } else {
                unregister_code16(KC_LCBR);
            }
            break;
        case 3:
            unregister_code16(KC_RCBR);
            break;
    }
}

void td_k_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                register_code16(KC_RPRN);  // Hold = )
            } else {
                register_code16(KC_LPRN);  // 1 tap = (
            }
            break;
        case 2:
            SEND_STRING("()");
            tap_code(KC_LEFT);              // 2 taps = ()←
            break;
        case 3:
            SEND_STRING("(\"\")");
            tap_code(KC_LEFT);
            tap_code(KC_LEFT);              // 3 taps = ("")←←
            break;
        case 4:
            SEND_STRING("() => {}");
            tap_code(KC_LEFT);              // 4 taps = () => {}←
            break;
        case 5:
            register_code16(KC_RPRN);       // 5 taps = )
            break;
    }
}

void td_k_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                unregister_code16(KC_RPRN);
            } else {
                unregister_code16(KC_LPRN);
            }
            break;
        case 5:
            unregister_code16(KC_RPRN);
            break;
    }
}

void td_sc_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                register_code16(KC_GT);     // Hold = >
            } else {
                register_code16(KC_LT);     // 1 tap = <
            }
            break;
        case 2:
            SEND_STRING("<>");
            tap_code(KC_LEFT);              // 2 taps = <>←
            break;
        case 3:
            SEND_STRING("<=");              // 3 taps = <=
            break;
    }
}

void td_sc_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                unregister_code16(KC_GT);
            } else {
                unregister_code16(KC_LT);
            }
            break;
    }
}

void td_l_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                register_code16(KC_RBRC);   // Hold = ]
            } else {
                register_code16(KC_LBRC);   // 1 tap = [
            }
            break;
        case 2:
            SEND_STRING("[]");
            tap_code(KC_LEFT);              // 2 taps = []←
            break;
        case 3:
            SEND_STRING("[0]");
            tap_code(KC_LEFT);              // 3 taps = [0]←
            break;
        case 4:
            register_code16(KC_RBRC);       // 4 taps = ]
            break;
    }
}

void td_l_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                unregister_code16(KC_RBRC);
            } else {
                unregister_code16(KC_LBRC);
            }
            break;
        case 4:
            unregister_code16(KC_RBRC);
            break;
    }
}

void td_eq_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            register_code16(KC_EQL);        // 1 tap = =
            break;
        case 2:
            SEND_STRING("==");              // 2 taps = ==
            break;
        case 3:
            SEND_STRING("+=");              // 3 taps = +=
            break;
        case 4:
            register_code16(KC_PLUS);       // 4 taps = +
            break;
    }
}

void td_eq_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            unregister_code16(KC_EQL);
            break;
        case 4:
            unregister_code16(KC_PLUS);
            break;
    }
}

void td_sl_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            register_code16(KC_SLSH);       // 1 tap = /
            break;
        case 2:
            SEND_STRING("//");              // 2 taps = //
            break;
        case 3:
            register_code16(KC_BSLS);       // 3 taps = backslash
            break;
    }
}

void td_sl_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            unregister_code16(KC_SLSH);
            break;
        case 3:
            unregister_code16(KC_BSLS);
            break;
    }
}

void td_mn_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            register_code16(KC_MINS);       // 1 tap = -
            break;
        case 2:
            SEND_STRING("--");              // 2 taps = --
            break;
        case 3:
            SEND_STRING("=>");              // 3 taps = =>
            break;
        case 4:
            register_code16(KC_UNDS);       // 4 taps = _
            break;
    }
}

void td_mn_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            unregister_code16(KC_MINS);
            break;
        case 4:
            unregister_code16(KC_UNDS);
            break;
    }
}

void td_am_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            register_code16(KC_AMPR);       // 1 tap = &
            break;
        case 2:
            SEND_STRING("&&");              // 2 taps = &&
            break;
        case 3:
            SEND_STRING("||");              // 3 taps = ||
            break;
        case 4:
            register_code16(KC_PIPE);       // 4 taps = |
            break;
    }
}

void td_am_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            unregister_code16(KC_AMPR);
            break;
        case 4:
            unregister_code16(KC_PIPE);
            break;
    }
}

void td_ex_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            register_code16(KC_EXLM);       // 1 tap = !
            break;
        case 2:
            SEND_STRING("!=");              // 2 taps = !=
            break;
        case 3:
            register_code16(KC_QUES);       // 3 taps = ?
            break;
    }
}

void td_ex_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            unregister_code16(KC_EXLM);
            break;
        case 3:
            unregister_code16(KC_QUES);
            break;
    }
}

// Tap dance actions array is now defined in keymap.c