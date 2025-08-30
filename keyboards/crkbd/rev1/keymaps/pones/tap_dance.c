#include QMK_KEYBOARD_H
#include "tap_dance.h"
#include "action_util.h"

// Tap dance functions
void td_j_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("}");  // Hold = }
            } else {
                register_code16(KC_LCBR);  // 1 tap = {
            }
            break;
        case 2:
            SEND_STRING("{}");
            tap_code(KC_LEFT);              // 2 taps = {}←
            break;
        case 3:
            SEND_STRING("{\n\t\n}");        // 3 taps = {\n\t\n}
            tap_code(KC_UP);
            tap_code(KC_END);
            break;
    }
}

void td_j_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_LCBR);
            }
            break;
    }
}

void td_k_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING(")");  // Hold = )
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
    }
}

void td_k_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_LPRN);
            }
            break;
    }
}

void td_sc_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING(">");     // Hold = >
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
            if (!state->pressed) {
                unregister_code16(KC_LT);
            }
            break;
    }
}

void td_l_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("]");   // Hold = ]
            } else {
                register_code16(KC_LBRC);   // 1 tap = [
            }
            break;
        case 2:
            SEND_STRING("[]");
            tap_code(KC_LEFT);              // 2 taps = []←
            break;
    }
}

void td_l_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_LBRC);
            }
            break;
    }
}

void td_eq_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("+");   // Hold = +
            } else {
                register_code16(KC_EQL);    // 1 tap = =
            }
            break;
        case 2:
            SEND_STRING("==");              // 2 taps = ==
            break;
        case 3:
            SEND_STRING("+=");              // 3 taps = +=
            break;
    }
}

void td_eq_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_EQL);
            }
            break;
    }
}

void td_sl_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("\\");   // Hold = backslash
            } else {
                register_code16(KC_SLSH);   // 1 tap = /
            }
            break;
        case 2:
            SEND_STRING("//");              // 2 taps = //
            break;
    }
}

void td_sl_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_SLSH);
            }
            break;
    }
}

void td_mn_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("_");   // Hold = _
            } else {
                register_code16(KC_MINS);   // 1 tap = -
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

void td_mn_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_MINS);
            }
            break;
    }
}

void td_am_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("|");   // Hold = |
            } else {
                register_code16(KC_AMPR);   // 1 tap = &
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

void td_am_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_AMPR);
            }
            break;
    }
}

void td_ex_finished(tap_dance_state_t *state, void *user_data) {
    clear_weak_mods();
    switch (state->count) {
        case 1:
            if (state->pressed) {
                SEND_STRING("?");   // Hold = ?
            } else {
                register_code16(KC_EXLM);   // 1 tap = !
            }
            break;
        case 2:
            SEND_STRING("!=");              // 2 taps = !=
            break;
    }
}

void td_ex_reset(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (!state->pressed) {
                unregister_code16(KC_EXLM);
            }
            break;
    }
}

// Tap dance actions array is now defined in keymap.c
