#pragma once

#include QMK_KEYBOARD_H

// Tap dance definitions
enum tap_dances {
    TD_J_BRACES,    // { | {} | }
    TD_K_PARENS,    // ( | () | ("") | () => {} | )
    TD_SC_ANGLES,   // < | <> | <= | >
    TD_L_BRACKETS,  // [ | [] | [0] | ]
    TD_EQ_PLUS,     // = | == | += | +
    TD_SL_BACK,     // / | // | backslash
    TD_MN_UNDER,    // - | -- | => | _
    TD_AM_PIPE,     // & | && | || | |
    TD_EX_QUEST,    // ! | != | ?
};