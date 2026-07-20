#pragma once

#include QMK_KEYBOARD_H
#include "layers.h"

// Tap dance definitions
enum tap_dances {
    TD_J_BRACES,    // { | {} | }
    TD_K_PARENS,    // ( | () | ("") | () => {} | )
    TD_SC_ANGLES,   // < | <> | <= | >
    TD_L_BRACKETS,  // [ | [] | [0] | ]
};