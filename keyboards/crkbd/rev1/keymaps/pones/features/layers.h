#pragma once

// Layer order. _SYMBOL index is used by tap_dance.c; keep before _NAV.
enum layer_names {
  _QWERTY,   // Base QWERTY with home row mods
  _NUMBER,   // F-keys + numpad (unchanged)
  _SYMBOL,   // Symbols right (Enter hold)
  _NAV,      // Pure cursor navigation
  _MOVE,     // Tab hold: KVM, app tabs, herdr tabs/spaces
  _MOUSE,    // Backspace hold: mouse move (WASD) + buttons on thumbs
};
