#pragma once

// Layer order. _SYMBOL index is used by tap_dance.c; keep before _NAV.
enum layer_names {
  _QWERTY,   // Base QWERTY with home row mods
  _NUMBER,   // F-keys + numpad (unchanged)
  _SYMBOL,   // Symbols right, KVM + copy/paste cluster left
  _NAV,      // Pure cursor navigation
  _HERDR,    // herdr prefix layer (Ctrl+B + key)
  _MEDIA,    // RGB, media, base utility keys, BOOT/DEBUG
  _MOVE,     // Tab hold: KVM switches and Ctrl+Tab navigation
};
