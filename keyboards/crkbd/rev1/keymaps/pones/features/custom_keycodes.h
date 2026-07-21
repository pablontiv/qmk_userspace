#pragma once

#include QMK_KEYBOARD_H

// Custom keycodes
enum custom_keycodes {
  KVM_PREV = SAFE_RANGE, // KVM switch: previous computer (Ctrl, Ctrl, Page Up)
  KVM_NEXT,              // KVM switch: next computer (Ctrl, Ctrl, Page Down)
  HRD_NTAB,              // herdr: next tab (Ctrl+B n)
  HRD_PTAB,              // herdr: prev tab (Ctrl+B p)
  HRD_NSPC,              // herdr: next space (Ctrl+B .) — herdr must bind . to next_workspace
  HRD_PSPC,              // herdr: prev space (Ctrl+B ,) — herdr must bind , to previous_workspace
  HRD_NWSP,              // herdr: new space (Ctrl+B Shift+N)
  HRD_NWTB,              // herdr: new tab (Ctrl+B c)
  HRD_SPLH,              // herdr: split horizontal (Ctrl+B -)
};