#pragma once

#include QMK_KEYBOARD_H

// Function declarations from macros.c
bool process_record_user(uint16_t keycode, keyrecord_t *record);
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record);