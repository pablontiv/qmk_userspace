#pragma once

#include QMK_KEYBOARD_H

// Custom keycodes
enum custom_keycodes {
  KVM_PREV = SAFE_RANGE, // KVM switch: previous computer (Ctrl, Ctrl, Page Up)
  KVM_NEXT,              // KVM switch: next computer (Ctrl, Ctrl, Page Down)
};