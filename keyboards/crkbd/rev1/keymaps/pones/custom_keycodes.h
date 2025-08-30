#pragma once

#include QMK_KEYBOARD_H

// Custom keycodes
enum custom_keycodes {
  DV_ARR = SAFE_RANGE,  // Arrow function: =>
  DV_CMNT,              // VS Code: Toggle line comment (Ctrl+K, C)
  DV_UNCMT,             // VS Code: Toggle uncomment (Ctrl+K, U)
  DV_SELLN,             // Select entire line (Home, Home, Shift+End)
  DV_REFS,              // VS Code: Find all references (Ctrl+K, R)
  WI_MAX,               // Windows: Maximize window (Alt+Space, Shift+X)
  KVM_PREV,             // KVM switch: Previous computer (Ctrl, Ctrl, Page Up)
  KVM_NEXT,             // KVM switch: Next computer (Ctrl, Ctrl, Page Down)
};